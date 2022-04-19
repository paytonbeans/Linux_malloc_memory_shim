//Name: Payton Beans
//Date: 2/8/21
//Sctracer



# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/reg.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/ptrace.h>


#ifdef ORIG_EAX
#define EAX_OR_RAX ORIG_EAX
#else
#define EAX_OR_RAX ORIG_RAX
#endif


int main (int argc, char *argv[]){
  pid_t pid;

  // Fork that bad boy
  if ((pid=fork())==0)
  {
    //ptrace
    ptrace(PT_TRACE_ME);
    kill(getpid(), SIGSTOP);
    return execvp(argv[1], argv+1);
  }
  else{
    int state;
    int call_num;
    int calls[400] = {0};
    int total = 0;

    ptrace(PTRACE_SETOPTIONS, pid, NULL, PTRACE_O_TRACESYSGOOD);

    for( ; ; ){
      // Stop at sys calls
      ptrace(PTRACE_SYSCALL, pid, NULL, 0);
      waitpid(pid, &state, 0);
      if(WIFEXITED(state)){
        break;
      }

      //Record sys calls
      call_num = ptrace(PTRACE_PEEKUSER, pid, sizeof(long)*EAX_OR_RAX);
      if(call_num > total){
        total = call_num;
      }
      calls[call_num]++;


      ptrace(PTRACE_SYSCALL, pid, NULL, 0);
      waitpid(pid, &state, 0);
      if(WIFEXITED(state)){
        break;
      }
    }

    //Print calls
    FILE *outfile;
    outfile= fopen(argv[2], "w");
    for(int i = 0; i < total + 1; i++){
      if(calls[i] > 0){
        fprintf(outfile, "%d\t%d\n",i,calls[i]);
      }
    }
    fclose(outfile);

  }

  return 0;
}
