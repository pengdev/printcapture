/*
 * printcapture.c
 *
 *  Created on: 2011-12-22
 *      Author: leo
 */

#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "libprintCapture.h"

struct foo {
	int count;
	pthread_mutex_t mutex;
	char s[3];
};
struct foo * fp;
/*Initialize the structure foo */
struct foo * init_foo() {
	struct foo * fp;

	if ((fp = (struct foo *) malloc(sizeof(struct foo))) != NULL) {
		fp->count = 1;
		fp->s[0] = 'a';
		fp->s[1] = 'b';
		fp->s[2] = 'c';
		/*initialize the mutex*/
		if (pthread_mutex_init(&fp->mutex, NULL) != 0) {
			pthread_mutex_destroy(&fp->mutex);
			free(fp);
			return NULL;
		}
	}
	return fp;
}

/*
 * add reference to foo
 */

void attach_foo(struct foo * fp) {
	/* Mutex here is used here to synchronize the three threads */
	pthread_mutex_lock(&fp->mutex);
	fp->count++;
	printf("%d thread has attached '%c' in thread:%u\n", fp->count,
			fp->s[fp->count - 2], (unsigned int) pthread_self());
	sleep(1);
	pthread_mutex_unlock(&fp->mutex);
}
/*function for thread 1 */
void * func1(void * arg) {
	attach_foo(fp);
	return ((void *) 0);
}
/*function for thread 2 */
void * func2(void * arg) {
	attach_foo(fp);
	return ((void *) 0);
}
/*function for thread 3 */
void * func3(void *arg) {
	attach_foo(fp);
	return ((void *) 0);
}
/*This function is used to print the process id and thread id to the standard output */
void printids(const char *s) {
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int) pid,
			(unsigned int) tid, (unsigned int) tid);
}
/*Main function starts */
int main(int argc, char *argv[]) {
	int ret;
	pid_t pid;
	pthread_t tid1, tid2, tid3;
	/*This is the function used to redirect standard output to "capture name"*/
	redirect("capture name.capture");
	/*This is a standard error, should not be redirected to the file */
	fprintf(stderr, "This error message should not be redirected\n");
	fflush(stderr);
	/*Create the child process to test the printcapture function in child */
	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error");
	} else if (pid == 0) {
		/*Test the print capture in child process */
		printids("This is from child process:");
	} else {
		printids("This is from parent process:"); /* parent */
		printids("This is from main thread:");
		fp = init_foo();
		/*Create 3 threads to test the print capture in child threads, mutex is used tosynchronize these threads */
		printf("%d thread begin to attach foo\n", fp->count);

		ret = pthread_create(&tid1, NULL, func1, NULL);
		if (ret != 0) {
			fprintf(stderr, "%s\n", strerror(errno));
		}

		ret = pthread_create(&tid2, NULL, func2, NULL);

		if (ret != 0) {
			fprintf(stderr, "%s\n", strerror(errno));
		}
		ret = pthread_create(&tid3, NULL, func3, NULL);
		if (ret != 0) {
			fprintf(stderr, "%s\n", strerror(errno));
		}

		sleep(4);
		printf("%d thread has attached all foo\n", fp->count);
	}
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
	exit(0);
}
