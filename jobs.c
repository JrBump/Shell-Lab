#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "jobs.h"

/* TODO: Nothing! */
/*       But you will call functions in this file. */
/* The functions in this file are used to manage the jobs list.  They are complete. */

extern int verbose;
int nextjid;
job_t jobs[MAXJOBS]; /* The job list */

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

//static function is local to this file
static int addjob(job_t *jobs, pid_t pid, int state, char *cmdline); 

/* initjobs - Initialize the job list */
void initjobs(job_t *jobs) {
    int i;
    nextjid = 1;
    for (i = 0; i < MAXJOBS; i++) clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(job_t *jobs) 
{
    int i, max=0;

    for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].jid > max)
        max = jobs[i].jid;
    return max;
}

/* add a background job to the jobs list */
int addBGjob(job_t *jobs, pid_t pid, char *cmdline) 
{
    return addjob(jobs, pid, BG, cmdline);
}

/* add a foreground job to the jobs list */
int addFGjob(job_t *jobs, pid_t pid, char *cmdline) 
{
    return addjob(jobs, pid, FG, cmdline);
}

/* addjob - Add a job to the job list */
int addjob(job_t *jobs, pid_t pid, int state, char *cmdline) 
{
    int i;
    
    if (pid < 1)
    return 0;

    for (i = 0; i < MAXJOBS; i++) 
    {
        if (jobs[i].pid == 0) 
        {
            jobs[i].pid = pid;
            jobs[i].state = state;
            jobs[i].jid = nextjid++;
            if (nextjid > MAXJOBS) nextjid = 1;
            strcpy(jobs[i].cmdline, cmdline);
              if(verbose)
            {
                printf("Added job [%d] %d %s\n", 
                       jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
            }
            return 1;
        }
    }
    printf("Tried to create too many jobs\n");
    return 0;
}


/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(job_t *jobs, pid_t pid) 
{
    int i;

    if (pid < 1) return 0;

    for (i = 0; i < MAXJOBS; i++) 
    {
        if (jobs[i].pid == pid) 
        {
            clearjob(&jobs[i]);
            nextjid = maxjid(jobs)+1;
            return 1;
        }
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].state == FG) return jobs[i].pid;
    return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
job_t *getjobpid(job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1) return NULL;
    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].pid == pid) return &jobs[i];
    return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
job_t *getjobjid(job_t *jobs, int jid) 
{
    int i;

    if (jid < 1) return NULL;
    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].jid == jid) return &jobs[i];
    return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) 
{
    int i;

    if (pid < 1) return 0;
    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].pid == pid) 
        {
            return jobs[i].jid;
        }
    return 0;
}

/* listjobs - Print the job list */
void listjobs(job_t *jobs) 
{
    int i;
    
    for (i = 0; i < MAXJOBS; i++) 
    {
        if (jobs[i].pid != 0) 
        {
            printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
            switch (jobs[i].state) 
            {
                case BG: 
                    printf("Running ");
                    break;
                case FG: 
                    printf("Foreground ");
                    break;
                case ST: 
                    printf("Stopped ");
                break;
            default:
                printf("listjobs: Internal error: job[%d].state=%d ", 
                   i, jobs[i].state);
            }
            printf("%s", jobs[i].cmdline);
        }
    }
}
