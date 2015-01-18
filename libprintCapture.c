/*
 * lib.c
 *
 *  Created on: 2011-12-23
 *      Author: leo
 */

#include "libprintCapture.h"
/*Define the pipe used for communication between processes */
int pipefd[2];

/*This is the child thread function, used for writing the content in pipe to the file*/
void * thr_wf(void * arg) {
	char buf;
	struct ThreadParams * myThreadParam = (struct ThreadParams *) arg;
	int fd = myThreadParam->fd;
	close(pipefd[1]);
	while (read(pipefd[0], &buf, 1) > 0)
		write(fd, &buf, 1);
	close(fd);
	close(pipefd[0]);

	sleep(2);
	return ((void *) 0);
}

void redirect(char *f_name) {
	int fd3;
	pid_t pid;
	pthread_t tid;
	int err;

	fflush(stdout);
	/*Create the file for the redirected content */
	fd3 = open(f_name, O_RDWR | O_APPEND | O_CREAT, S_IRWXU);
	if (fd3 < 0) {
		fprintf(stderr, "open error\n");
		exit(-1);
	}
	/*Create pipes */
	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	close(STDOUT_FILENO);

	/*Create child process with fork */
	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
	} else if (pid == 0) {
		/* child process*/
		struct ThreadParams * myThreadParam = (struct ThreadParams *) malloc(
				sizeof(struct ThreadParams));
		close(STDERR_FILENO);/*used for avoiding the double write of the standard error */
		/*The sturct is used for passing arguments to the child process */

		myThreadParam->fd = fd3;
		/*Create a thread to read the pipe and save the output to the file */
		err = pthread_create(&tid, NULL, thr_wf, myThreadParam);
		if (err != 0)
			fprintf(stderr, "can not create thread\n");
		pthread_join(tid, NULL);
	} else {
		/*Parent process */
		close(pipefd[0]); /* Close unused read end */
		/*redirect the STDOUT to the pipe */
		if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
			fprintf(stderr, "err in dup2\n");
		}
		close(pipefd[1]);
	}

}

