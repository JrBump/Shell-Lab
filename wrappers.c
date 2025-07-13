#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "wrappers.h"

//Implement the missing wrappers (for fork, exec, sigprocmask, etc.)
//and call them in your tsh code.
//
//Note that exec does not return a value (void function). If an error
//occurs, it will simply output <command>: Command not found, 
//where <command> is the value of argv[0] and then call exit. (See code in figure 8.24.)
//The other wrappers will be in the style of the Fork wrapper in section 8.3. 
//
//The headers for all of the wrappers (including the missing ones) are
//in wrappers.h
//
//Note you will not need a wrapper for waitpid since you have to check
//the return value of that in your code anyway.

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}

/*  Add the missing wrappers for: fork, exec, sigprocmask, sigemptyset,
 *  sigfillset, and kill
 */

 /*
 * Fork - Wrapper that takes nothing creates a pid to then fork
 * returns pid_t
 */
 pid_t Fork(void)
 {
     pid_t pid;

     if ((pid = fork()) < 0)
     {
        unix_error("Fork error has occured.");
     }
     return pid;
 }

 /*
 * Exec - Wrapper that will execute what is provided in the params and return nothing else
 */
void Exec(char *filename, char **argv, char **envp)
{
    if (execve(filename, argv, envp) < 0) {
        printf("%s: Command not found\n", filename);
        exit(1);
    }
}

 /*
 * Sigprocmask - Used to 
 * returns an int
 */
 int Sigprocmask(int option, sigset_t *newmask, sigset_t *prevmask)
 {
    int retValue = sigprocmask(option, newmask, prevmask);

    if (retValue == 0)
    {
        return 0;
    }else
    {
        unix_error("A Sigprocmask error occurred");
        return -1;
    }
 }

 /*
 * Sigemptyset - Used to remove a number of signals from a set
 * returns an int
 */
 int Sigemptyset(sigset_t *mask)
 {
    int retValue = sigemptyset(mask);

    if (retValue == 0)
    {
        return 0;
    }
    else
    {
        unix_error("A Sigemptyset error occurred");
        return -1;
    }
 }


 /*
 * Sigaddset - Used to add a new signal to a existing set
 * returns an int
 */
 int Sigaddset(sigset_t *mask, int option)
 {
    int retValue = sigaddset(mask, option);

    if (retValue == 0)
    {
        return 0;
    }
    else
    {
        unix_error("A Sigaddset error occurred");
        return -1;
    }
 }


 /*
 * Sigfillset - Used to fill a set of Signals given
 * returns an int
 */
 int Sigfillset(sigset_t *mask)
 {
    int retValue = sigfillset(mask);

    if (retValue == 0)
    {
        return 0;
    }
    else
    {
        unix_error("A Sigfillset error occurred");
        return -1;
    }
 }


 /*
 * 
 * Kill - Used to end a job
 *
 * returns an int
 */
 int Kill(pid_t pid, int signal)
 {
    int retValue = kill(pid,signal);

    if (retValue == 0)
    {
        return 0;
    }
    else
    {
        unix_error("Kill error has occurred");
        return -1;
    }
 }







