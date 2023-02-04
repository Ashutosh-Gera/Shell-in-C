#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

// use the global variable errno that will be set with an error code by mkdir()
// if an error occurs
extern int errno;

int main(int argc, char* argv[])
{
  if (mkdir(argv[1], S_IRWXU | S_IRWXG | S_IRWXO ) == -1)
  {
    // we could use perror() to output the error if it occurs, the error 
    // message will be preprended with "Error" because we have supplied this 
    // as an argument
    perror("Error");
    
    //
    if (errno == EEXIST)
    {

      // mkdir("test2", 0777);
    }
   
    // returning 1 is a signal to the shell/terminal that something has gone 
    // wrong in the execution of our program, in contrast to returning 0 
    // as below...
    return 1;
  }

  return 0;
}