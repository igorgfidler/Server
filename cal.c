/***********************************************************************
* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
* Copyright (C) 2001 by New Riders Publishing                          *
* See COPYRIGHT for license information.                               *
***********************************************************************/

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

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
  int i=4;
    char *mes = NULL;
    char *ano = NULL;
    char *buffer_mes = NULL;
    char *buffer_ano = NULL;
   
  if(args != NULL) {
  	
    ano = strstr(args, "ano");
	if(ano != NULL) {
		while(ano[i] != '\0' && ano[i] != '&') 
		i+=1;
		
		i-=4;
		buffer_ano = (char*)malloc(sizeof(char)*i);
		strncpy(buffer_ano, ano+4, i);
    }
   	i = 4;
	mes = strstr(args, "mes");
	if(mes != NULL) {
		while(mes[i] != '\0' && mes[i] != '&') 
			i+=1;

		
		i-=4;
		buffer_mes = (char*)malloc(sizeof(char)*i);
		strncpy(buffer_mes, mes+4, i);
    }
  }
  /* Write the start of the page.  */
  write (fd, page_start, strlen (page_start));
  /* Fork a child process.  */
  child_pid = fork ();
  if (child_pid == 0) {
  	time_t t = time(NULL);
    struct tm ti = *localtime(&t);
    int tam = snprintf(NULL, 0, "%d", ti.tm_year);
    char ano_sistema[tam];
    sprintf(ano_sistema, "%d", ti.tm_year+1900);
  	char** argv = malloc(sizeof(char*));

  	argv= malloc(sizeof(char*)*4);
	argv[0] = "cal";
    if(buffer_ano!= NULL && buffer_mes != NULL){
		argv[1] = malloc(sizeof(char)*strlen(buffer_mes));
		argv[2] = malloc(sizeof(char)*strlen(buffer_ano));
		argv[3] = malloc(sizeof(char*));
		sprintf(argv[1], "-m%s", buffer_mes);
		argv[2] = buffer_ano;
		argv[3] = NULL;
	}
    else if(buffer_ano != NULL) {
      	argv[1] = malloc(sizeof(char)*strlen(buffer_ano));
		argv[1] = buffer_ano;
		argv[2] = malloc(sizeof(char*));
		argv[2] = NULL;
	}
    else if(buffer_mes != NULL) {
    	argv[1] = malloc(sizeof(char)*strlen(buffer_mes) +4);
		sprintf(argv[1], "-m%s", buffer_mes);
		argv[2] = malloc(sizeof(char*));
		argv[2] = NULL; 
	}
	else {
		argv[1] = malloc(sizeof(char*));
		argv[1] = NULL;
	}

	rval = dup2 (fd, STDOUT_FILENO);
	dup2 (fd, STDERR_FILENO);
	execvp (argv[0], argv);
   /* fprintf(stderr, "%s\n%s\n", mes, ano);
    /* Duplicate stdout and stderr to send data to the client socket.  
    rval = dup2 (fd, STDOUT_FILENO);
    if (rval == -1)
      system_error ("dup2");
    rval = dup2 (fd, STDERR_FILENO);
    if (rval == -1)
      system_error ("dup2");
    /* Run df to show the free space on mounted file systems.  */
    //execvp (argv[0], argv);
    /* A call to execv does not return unless an error occurred.  
    system_error Z("execv");*/
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
