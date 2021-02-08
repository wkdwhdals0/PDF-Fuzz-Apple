#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

#include "simplerun.h"

Debugger *runner;

//single execution.
//get program status code when that terminated by some reasons.
void RunTarget(int argc, char **argv, unsigned int pid, uint32_t timeout) {
    DebuggerStatus status;

    if(runner->IsTargetAlive()) {
        status = runner->Continue(timeout);
    } 
    else {
        runner->Kill();
        if(argc) {
            status = runner->Run(argc, argv, timeout);
        }
    }
    
    switch (status) {
        case DEBUGGER_CRASHED:
        printf("Process crashed!!!!\n");
        system("/bin/cp /Users/singi/TinyInst/test.aif $RANDOM.aif");
        runner->Kill();
        break;
    case DEBUGGER_HANGED:
        printf("Process hanged\n");
        runner->Kill();
        break;
    case DEBUGGER_PROCESS_EXIT:
        if (runner->IsTargetFunctionDefined()) {
            printf("Process exit during target function\n");
        } else {
            ;//printf("Process finished normally\n");
        }
        break;
        
    case DEBUGGER_TARGET_END:
        if (runner->IsTargetFunctionDefined()) {
            printf("Target function returned normally\n");
            //cur_iteration++;
        } else {
            printf("Unexpected status received from the debugger\n");
        }
        break;
        
    default:
        printf("Unexpected status received from the debugger\n");
        break;
  }

}

//run program with no cov info,
//just catch.
int main(int argc, char *argv[])
{
    printf("Build Sucees!\n");
    printf("BOB Fuzzer Start\n");
    runner = new Debugger();
    runner->Init(argc, argv);
    
    int target_opt_ind = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--") == 0) {
            target_opt_ind = i + 1;
            break;
        }
    }

  int target_argc = (target_opt_ind) ? argc - target_opt_ind : 0;
  char **target_argv = (target_opt_ind) ? argv + target_opt_ind : NULL;

  if (!target_argc) {
      printf("Usage: %s -- <target command line>\n", argv[0]);
      return 0;
  }

  for(;;)
    RunTarget(target_argc, target_argv, 0, 0xFFFFFFFF);

  return 0;
}