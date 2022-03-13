<div align="center">
  <h1>Mimic-Linux-Terminal-functionality</h1>
</div>

---

## Overview
A bash like program which takes user input as command and executes them using builtin (implemented in code) commands or execvp().It is a kind of simulation of fuctionality of linux terminal.

<br />


## Commands supported
The following commands are implemented from scratch,
1. "jobs"
2. "cd"
3. "history"
4. "kill"
5. "exit"

Other commands are executed using execvp().


<br />


## Setting up and running the project locally
1. Clone this repository and go into the repo folder
```
cd Mimic-Linux-Terminal-functionality
```
2. Compile the *code.c* file using,
```
gcc code.c -o code 
```
3. Run the program using,
```
./code
```
This will open a linux-terminal like program.

<br />

## Description of Commands

- **jobs**
1. "jobs" will provide a numbered list of processes currently executing in the background.
2. To execute it type ```jobs``` as command and press enter.

- **cd**
1. "cd" will change the directory.
2. To execute it type ```cd``` followed by the relative address of directory you want to change in.
3. ``` cd ..``` will change directory to parent directory.

- **history**
1. "history" will provide a numbered list of last 10 commands executed.
2. To execute it type ```history``` and press enter.
3. To execute the command numbered **i** in the list of history, type ```!i``` and press enter. **Note:** No space between **!** and **i**.
4. To execute the latest command which was executed type ```!-1``` and press enter. **Note:** No space between **!** and **-1**.

- **kill**
1. "kill" will terminate the given process in running in background.
2. To kill the background process numbered **i** in the list given by **jobs** command, type ```kill %i``` and press enter. <br>**Note:** No space between **%** and **i**.

- **exit**
1. "exit" will end the linux-terminal like program.
2. To execute it type ```exit``` and press enter.
3. "exit" will work if and only if commands there are no background processes running.<br> If there is some process in background then user will be notified to kill it before exit.

## Background Process
A process will go to background if ```&``` flag is given at last of command seperated by space.<br>
Example: ```./a.out &```.

## Other commands
Other commands that are supported by linux-terminal are executed using execvp().
To execute them follow the same rule as used in linux.

**Note:** It is assumed that commands are given in coreect format.

