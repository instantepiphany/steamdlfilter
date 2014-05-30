#include <sys/socket.h>
#include <sys/types.h>
#include <libconfig.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

ssize_t send(int sockfd, const void *buf, size_t len, int flags) {

  const char * cbuf = (const char *) buf; 

  ssize_t (*original_send) (int, const void*, size_t, int) = dlsym(RTLD_NEXT, "send");

  config_t config;
  config_init(&config);
  const char * chome = getenv("HOME");
  char prelog[100];
  char precfg[100];
  char * log = strcpy(prelog, chome);
  char * cfg = strcpy(precfg, chome);
  const char * configpath = strcat(cfg,"/.config/steamdlfilter/config");
  const char * logpath = strcat(log, "/.steamdlfilter.log");


  config_read_file(&config, configpath);

  int filterenabled;
  const char * enablepath = "filterenabled";

  config_lookup_bool(&config, enablepath, &filterenabled);


  if (strstr(cbuf, "/depot/") && filterenabled == 1) {

	  FILE *logfp;
	  char *logmode = "a+";

	  const char * unmeteredserver;
	  const char * serverpath = "unmeteredserver";

	  config_lookup_string(&config, serverpath, &unmeteredserver);

	  int logging;
	  const char * logconfigpath = "logging";

	  config_lookup_bool(&config, logconfigpath, &logging);


	  if (strstr(cbuf, unmeteredserver)) {	  
		  if (logging == 1) {
		  	logfp = fopen(logpath, logmode);
                  	fprintf(logfp, "=======ALLOWED=======\n%s\n", cbuf);
                  	fclose(logfp);
		  }
                  return original_send(sockfd, buf, len, flags);
    }
	  else {
                  if (logging == 1) {
			logfp = fopen(logpath, logmode);
			fprintf(logfp, "=======BLOCKED=======\n%s\n", cbuf);
			fclose(logfp);
		  }
                  return -1;
    }
	  

  }
  else 
      return original_send(sockfd, buf, len, flags);
  config_destroy(&config);

}
