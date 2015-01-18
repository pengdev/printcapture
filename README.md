### INFORMATION:          

This program offers a function named redirect(char *F_name) to capture the standard output stream and print the content to a file, whose file name can be defined in the old program.
The child processes and threads spawned by the programs are also supported.
However, STDERR message is not redirected and should be left untouched.

### BULID & RUN & CLEAN:  

In the home directory, type "make" to build.
$make

Usage of the program:
$./printCapture


The cleaning work is also simply type "make clean" in the terminal.
$make clean
Cleaning work will remove all generated files, including the captured files.

### RESULT OF THE PROGRAM:

The result is that the stderr message is printed in the console while other stdout messages including the stdout in child processes and child threads are writen into the file named by the old program, which, in this program is capture name.capture. 




