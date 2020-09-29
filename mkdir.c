#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
extern int errno;
char *temp;

void implementP()
{
	const char del[2] = "/";
	char* token;
	token = strtok(temp, del);
   	while( token != NULL ) {
    	int status=mkdir(token,0777);
    	if(status==-1)
    		perror("mkdir: "); 
    	else{
    		int status = chdir(temp);
			if (status == -1)
				perror("cd: ");
    	}   
      token = strtok(NULL, del);
   }
   return;
}


int main(int noOfArguements, char *args[])
{
	int flagV=0;
	int flagP=0;
	int ini=1;
    for (int a = 1; a < noOfArguements; a++)
	{
	   	if (strcmp(args[a], "-p") == 0)
	        flagP = 1;
	    if (strcmp(args[a], "-v") == 0)
	        flagV = 1;
	}
	int temp5=1+flagV+flagP;
	if(noOfArguements==temp5)
		printf("missing operand\n");
	while(ini<noOfArguements)
	{		
	    if (strcmp(args[ini], "-p") == 0 || strcmp(args[ini], "-v") == 0)
	    {
	        ini++;
	        continue;
	    }
	    else if(flagP==0){
	    	int status=mkdir(args[ini],0777);
	    	if(status==-1)
	    		perror("mkdir: ");
	    	if(flagV)
	    		printf("created directory '%s'\n", args[ini]);
	    	ini++;
	    }
	    else{
	    	temp=args[ini];
	    	implementP();
	    	int len=strlen(temp);
	    	for(int abc=0;abc<len;abc++)
	    		temp[abc]='\0';
	    }
	}
	return 0;
}