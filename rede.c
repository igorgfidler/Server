
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
    char *rede = NULL;
    char *buffer_rede = NULL;
   
  if(args != NULL) {
  	
    rede = strstr(args, "interface=");
    tam = strlen("interface=");
	if(rede != NULL) {
		while(rede[tam+i] != '\0' && rede[tam+i] != '&') 
			i+=1;
		
		buffer_rede = (char*)malloc(sizeof(char)*i);
		strncpy(buffer_rede, rede+tam, i);	
    }
  }
  /* Write the start of the page.  */
  write (fd, page_start, strlen (page_start));
  /* Fork a child process.  */
  child_pid = fork ();
  if (child_pid == 0) {
	char** argv;  	

  	argv= malloc(sizeof(char*)*3);
	argv[0] = "ifconfig";
    if(buffer_rede!= NULL){
		argv[1] = malloc(sizeof(char)*strlen(buffer_rede));
		argv[2] = malloc(sizeof(char*));
		argv[1] = buffer_rede;
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