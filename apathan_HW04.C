#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_PROC 32767
#define RESOLUTION 1

typedef struct _queue queue;

struct _queue {
    int size;      
    void **buffer; 
    int start;      
    int end;        
    int count;      
};

typedef void (*eptr)(void *arg, void *data);

queue *q_init(int n);
int q_insrt(queue *q, void *item);
void *q_dlt(queue *q);
void q_ech(queue *q, eptr func, void *data);
void q_dsty(queue *q);
void sh_jbs_helper(void *elem, void *data);
void st_hist_hlp(void *elem, void *data);

typedef struct Jobs Job;

struct Jobs {
    int jobId;
    char **argv;
    int status;
    pid_t pid;
    unsigned long start;
    unsigned long stop;
};

queue *jobs = 0;
int presentJobId = 1; 

enum JOB_STATUS
{
    WAITING = 0,
    RUNNING = 1,
    COMPLETED = 2
};

const char *JOB_STATUS_str[] = {"WAITING", "RUNNING", "COMPLETED"};

#define JOB_STATUS_STRING(js) JOB_STATUS_str[js]

void st(char **argv);
void sh_jbs();
void st_hist();

int main(int argc, char **argv)
{
    int count = 2;

    jobs = q_init(MAX_PROC);

    while (1)
    {
        printf("Enter command (submit, showjobs, submithistory, exit): ");
        char command[256];
        fgets(command, sizeof(command), stdin);

        if (strcmp(command, "submit\n") == 0)
        {
            st(argv);
        }
        else if (strcmp(command, "showjobs\n") == 0)
        {
            sh_jbs();
        }
        else if (strcmp(command, "submithistory\n") == 0)
        {
            st_hist();
        }
        else if (strcmp(command, "exit\n") == 0)
        {
            break;
        }
        else
        {
            printf("Invalid command\n");
        }
    }

    q_dsty(jobs);
    return 0;
}

queue *q_init(int n)
{
    queue *q = (queue *)malloc(sizeof(queue));
    q->size = n;
    q->buffer = malloc(sizeof(int) * n);
    q->start = 0;
    q->end = 0;
    q->count = 0;

    return q;
}

int q_insrt(queue *q, void *item)
{
    if ((q == NULL) || (q->count == q->size))
        return -1;

    q->buffer[q->end % q->size] = item;
    q->end = (q->end + 1) % q->size;
    q->count++;

    return q->count;
}

void *q_dlt(queue *q)
{
    if ((q == NULL) || (q->count == 0))
        return 0;

    void *x = q->buffer + q->start;
    q->start = (q->start + 1) % q->size;
    q->count--;

    return x;
}

void q_ech(queue *q, eptr func, void *data)
{
    if (q != NULL && q->count != 0)
    {
        for (int i = 0; i < q->count; i++)
            func(q->buffer + ((q->start + i) % q->size), data);
    }
}

void q_dsty(queue *q)
{
    free(q->buffer);
    free(q);
}

void st(char **argv)
{
Job *j;

    if ((j = malloc(sizeof(Job)))) {
        memset(j, 0, sizeof(Job));
        j->argv = argv;
        j->status = WAITING;
        j->jobId = presentJobId++;

        pid_t pid = fork();

        if (pid == 0) {
            execvp(argv[1], argv + 1);
			fprintf(stderr, "Error in execvp for command: %s\n", argv[1]);
            perror("Error in execvp");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            j->pid = pid;
            j->start = time(NULL);

            printf("Job ID: %d\n", j->jobId);
        } else {
            perror("Error in fork");
            exit(EXIT_FAILURE);
        }
    }

    q_insrt(jobs, j);
}

void sh_jbs()
{
    printf("Job ID\tStatus\n");
    q_ech(jobs, sh_jbs_helper, 0);
}

void sh_jbs_helper(void *elem, void *data)
{
    Job *j = elem;
    printf("%d\t%s\n", j->jobId, JOB_STATUS_STRING(j->status));
}

void st_hist()
{
    printf("Job ID\tStart Time\t\tEnd Time\t\tStatus\n");
    q_ech(jobs, st_hist_hlp, 0);
}

void st_hist_hlp(void *elem, void *data)
{
    Job *j = elem;
    printf("%d\t%lu\t%lu\t%s\n", j->jobId, j->start, j->stop, JOB_STATUS_STRING(j->status));
}
