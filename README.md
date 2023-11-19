This program serves as a simple job scheduler that allows users to submit jobs, view the status of currently running and completed jobs, and check the submission history.

The user interacts with the program through a command-line interface where they can enter commands such as "submit," "showjobs," "submithistory," and "exit" to perform various tasks.

The explanation of Functions is as follows -

1) queue *q_init(int n);
This function initializes a queue with a specified maximum size. And it takes n [Maximum size of the queue] as parameters.
Finally it returns a pointer to the initialized queue.

2) int q_insrt(queue *q, void *item);
This function inserts an item into the queue. And it takes q [Pointer to the queue] and item [Item to be inserted] as parameters.
Finally it returns the count of elements in the queue after insertion.

3) void *q_dlt(queue *q);
This function deletes an item from the queue. And it takes q [Pointer to the queue] as Parameters.
Also it returns the deleted item.

3) void q_ech(queue *q, eptr func, void *data);
This function applies a function to each element in the queue. And parameters are -
q [Pointer to the queue]
func [Function to be applied]
data [Additional data passed to the function]

4) void q_dsty(queue *q);
This function destroys the queue and frees allocated memory and takes q[Pointer to the queue] as parameters.

5) void st(char **argv);
This function submits a new job for execution.and its parameter is argv: Command-line arguments representing the job.

Undestanding the logic of this function:
This function allocates memory for a new job.Forks a new process to execute the specified command.
In the child process, executes the command using execvp.In the parent process, records the job details, such as job ID, process ID, and start time.
Prints the job ID to the standard output.

6) void sh_jbs();
This function displays the status of currently running and waiting jobs. Basically it prints the job ID and status of each job in the queue.

7) void st_hist();
This function displays the submission history of completed jobs. So this function prints the job ID, start time, end time, and status of each completed job in the queue.

In the program I have used Fork because-
The fork system call is used to create a new process, allowing the program to execute multiple tasks concurrently. In this case, fork is used to create a child process that can execute a specified command independently of the parent process. This is crucial for a job scheduler, as it enables the execution of multiple jobs simultaneously.
