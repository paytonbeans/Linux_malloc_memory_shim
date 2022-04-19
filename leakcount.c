//Name: Payton Beans
//Date: 2/8/21
// Leakcounter



# include <stddef.h>
# include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc,char** argv)
{
//fprintf(stderr, "Begin leakcount\n");
//Fork the program in args

pid_t p;

if ((p=fork())==0)
{
  //execlp("LD_PRELOAD=./malloc_lib.so ","LD_PRELOAD=./malloc_lib.so ",argv[1],NULL);
  char *const envs[] = {"LD_PRELOAD=./malloc_lib.so",NULL};
  char *const arglist[] = {argv[1],NULL};
  execvpe(argv[1], arglist, envs);
  return 0;
}

//wait for program to finish
wait(NULL);

// Print results
//fprintf(stderr, "End leakcount\n");


}
