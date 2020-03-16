
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include "server.h"

/* HTML source for the start of the page we generate.  */

static char* page_start =
  "<html>\n"
  " <body>\n"
  "  <pre>\n";

/* HTML source for the end of the page we generate.  */

static char* page_end =
  "  </pre>\n"
  " </body>\n"
  "</html>\n";

void module_generate (int fd, char* args)
{
  pid_t child_pid;
  int rval;
  int i=0, tam;
  char *buffer_portas = NULL;
  char* portas = NULL;
  
  if(args != NULL) {
    
    portas = strstr(args, "tipo=");
    tam = strlen("tipo=");
    if(portas != NULL) {
      while(portas[tam+i] != '\0' && portas[tam+i] != '&') 
        i+=1;
    
      buffer_portas = (char*)malloc(sizeof(char)*i);
      strncpy(buffer_portas, portas+tam, i);  
    }

  /* Write the start of the page.  */
  write (fd, page_start, strlen (page_start));
  /* Fork a child process.  */
  child_pid = fork ();
  if (child_pid == 0) {
  char** argv;
  
  argv = malloc(sizeof(char*) * 3);
  argv[0] = "netstat";
  if(buffer_portas != NULL) {

    argv[1] = malloc(sizeof(char) * tam+4);
    sprintf(argv[1], "-p%s", buffer_portas);
    argv[2] = malloc(sizeof(char*));
    argv[2] = NULL;
  }
  else {
    argv[1] = malloc(sizeof(char*));
    argv[1] = NULL;
  }
  rval = dup2 (fd, STDOUT_FILENO);
  dup2(fd, STDERR_FILENO);
  execvp (argv[0], argv);
  }
    else if (child_pid > 0) {
    /* This is the parent process.  Wait for the child process to
       finish.  */
      rval = waitpid (child_pid, NULL, 0);
    if (rval == -1)
      system_error ("waitpid");
    }
    else 
      /* The call to fork failed.  */
      system_error ("fork");
  /* Write the end of the page.  */
  write (fd, page_end, strlen (page_end));
  }
}
