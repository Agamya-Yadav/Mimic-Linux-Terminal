#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<signal.h>

#define MAXCOM 1000
#define MAXLIST 100

#define clear() printf("\033[H\033[J")

char* historyList[10];
pid_t bgProcesses[1000];
int no_of_bg_process=0, historyLines=0, parts=0;

void jobs(void);
void Kill(char*);

void printPrompt(void);
char* readCommandLine();
char** parseCommand(char*);
int isBuiltInCommand(char**);
void executeBuiltInCommand(char**);
void record_command_history(char*);
int isBackgroundJob(char**);
void Record_in_list_of_background_jobs(pid_t);
void executeCommand(char**);

int main(int argc, char **argv)
{
   while(1)
    {
        int childPid;
        char*  cmdLine;
        printPrompt(); // This function will print the prompt
        cmdLine=readCommandLine(); // To read the command line given by the user.
        if(strlen(cmdLine) == 0)continue;
        
        if(cmdLine[0]=='!')
        {
            cmdLine=cmdLine+1;
            int ind=atoi(cmdLine);
            if(ind==-1)ind=historyLines;
            cmdLine=historyList[ind-1];
            printf("%s\n",cmdLine);
        }

        
        char **cmd=parseCommand(cmdLine); // Understand and parse the command

        record_command_history(cmdLine); // Similar feature is availaible in GNU

        // Do the job now
        if(isBuiltInCommand(cmd))
        {
            executeBuiltInCommand(cmd);
        }
        else
        {
            childPid=fork();

            if(childPid==0)
            {
               executeCommand(cmd); //calls execvp
            }
            else
            {
                if(isBackgroundJob(cmd))
                {
                    Record_in_list_of_background_jobs(childPid);
                }
                else 
                {
                    
                   waitpid(childPid, NULL, 0);
                }
            }
        }

    }
}


void printPrompt(void)
{
    char* username = getlogin();
    char hostname[1000];
    gethostname(hostname,sizeof(hostname));
    char cwd[1000];
    if(getcwd(cwd,sizeof(cwd))!=NULL)
    printf(">>>%s@%s:~%s$ ",username,hostname,cwd);
    sleep(1);
    
}

char* readCommandLine()
{
    char *buf;

    buf = readline(" ");
    return buf;
}

char** parseCommand(char* cmdLine)
{
    char* temp = (char*)malloc(sizeof(cmdLine));
    strcpy(temp,cmdLine);
    char** cmd = (char**)malloc(MAXLIST*sizeof(char*));
    int i;
    parts=0;
    for(i=0;i<MAXLIST;i++)
    {
        cmd[i]=strsep(&temp," ");

        if(cmd[i] == NULL)break;
        if(strlen(cmd[i]) == 0)i--;
    }
    parts=i;
    return cmd;
}


int isBuiltInCommand(char ** cmd)
{
    if(strcmp(cmd[0],"jobs")==0 || strcmp(cmd[0],"cd")==0 || strcmp(cmd[0],"history")==0 || strcmp(cmd[0],"exit")==0 || strcmp(cmd[0],"kill")==0)return 1;
    else return 0;
}

void executeBuiltInCommand(char ** cmd)
{
    if(strcmp(cmd[0],"jobs")==0)
    {
        jobs();
    }
    else if(strcmp(cmd[0],"cd")==0)
    {
        int success=chdir(cmd[1]);
    }
    else if(strcmp(cmd[0],"history")==0)
    {
        for(int i=1;i<=historyLines;i++)
        {
            printf("%d. %s\n", i, historyList[i-1]);
        }
    }
    else if(strcmp(cmd[0],"kill")==0)
    {
        if(parts==2)Kill(cmd[1]);
        else printf("Incorrect number of arguments\n");
    }
    else  // exit
    {
        if(no_of_bg_process)
        {
            printf("Background Process Still Running. Cannot Exit.\n");
            jobs();
        }
        else exit(0);
    }
}

void record_command_history(char* cmdLine)
{
    if(historyLines==10)
    {
        int i=0;
        while(i<9)historyList[i]=historyList[i+1], i++;
        historyList[9]=cmdLine;
    }
    else
    {
        historyList[historyLines]=cmdLine;
        historyLines++;
    }
}


int isBackgroundJob(char** cmd)
{
    if(strcmp(cmd[parts-1],"&")==0)return 1;
    else return 0;
}
void Record_in_list_of_background_jobs(pid_t pid)
{
    bgProcesses[no_of_bg_process]=pid;
    no_of_bg_process++;
}

void jobs(void)
{
    printf("pid\n");
    int i=0;
    while(i<no_of_bg_process)
    {
        printf("[%d] %d \n",i+1, bgProcesses[i]);
        i++;
    }
}


void Kill(char* cmd)
{
    int  size = sizeof(cmd)/sizeof(char);

    int i=0;
    while(i<size-1)
    {
        cmd[i]=cmd[i+1];
        i++;
    }
    
    int ind = atoi(cmd);

    if(no_of_bg_process>ind || no_of_bg_process<=0)
    {
        printf("No such background process is running\n");
        return;
    }

    ind--;
    no_of_bg_process--;
    
    kill(bgProcesses[ind],SIGKILL);
    printf("Background Process with pid = %d is killed\n",bgProcesses[ind]);
    while(ind>no_of_bg_process)
    {
        bgProcesses[ind]=bgProcesses[ind+1];
        ind++;
    }
}

void executeCommand(char** cmd)
{
    if(execvp(*cmd,cmd)<0)
    {
        printf("ERROR: exec failed\n");
        exit(1);
    }
}
