/*
 * lib.h
 *
 *  Created on: 2011-12-23
 *      Author: leo
 */

#ifndef LIBPRINTCAPTURE_H_
#define LIBPRINTCAPTURE_H_

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/*The structure is used to pass parameters to the child thread.*/
struct ThreadParams {
	int fd;
};
/*The function is used to redirect the standard output to the file named by argument*/
extern void redirect(char *f_name);

#endif /* LIB_H_ */
