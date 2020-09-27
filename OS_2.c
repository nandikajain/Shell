#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<math.h>
extern int errno;
char *getcwd(char *buf, size_t size);
char* line;
char** args;
int noOfArguements;
size_t inputSize=1000;
const char* Delimiter = " \t\r\n\a";
char** history;
int total_count;
int fd;

void cd()
{

}

void echo_checkbackslash(char *dest, char *argument)
{
	if (argument[0] == '\\')
	{
		if (argument[1] == 'a')
			strcat(dest, "\a");
		else if (argument[1] == 'b')
			strcat(dest, "\b");
		else if (argument[1] == '0')
			strcat(dest, "\0");
		else if (argument[1] == 'e')
			strcat(dest, "\e");
		else if (argument[1] == 'f')
			strcat(dest, "\f");
		else if (argument[1] == 'n')
			strcat(dest, "\n");
		else if (argument[1] == 'r')
			strcat(dest, "\r");
		else if (argument[1] == 't')
			strcat(dest, "\t");
		else if (argument[1] == 'v')
			strcat(dest, "\v");
		else if (argument[1] == '\\')
			strcat(dest, "\\");
		else if (argument[1] == '\"')
			strcat(dest, "\"");		
		else if (argument[1] == '\'')
			strcat(dest, "\'");	
		else if (argument[1] == '\?')
			strcat(dest, "\?");
		else
		{
			strcat(dest, argument);
			strcat(dest, " ");
		}
	}
	else
	{
		strcat(dest, argument);
		strcat(dest, " ");
	}
}


void echo()
{
	if(noOfArguements==1)
	{
		printf("\n");
	}
	else{

		int st = 1;
		int isdashN=0;
		int isdashE=0;
		if( strcmp(args[st],"-n")==0)
		{
			isdashN=1;
			st++;
		} 
		if( strcmp(args[st],"-e")==0)
		{
			isdashE=1;
			st++;
		} 	
		if( strcmp(args[st],"-n")==0)
		{
			isdashN=1;
			st++;
		} 	
		if( strcmp(args[st],"-n")==0)
		{
			isdashE=1;
			st++;
		} 
		char dest[250];
		strcpy(dest, "");
		if(isdashE==0)
		{
			int start=1;
			if (isdashN==1)
			{
				start++;

			}
			for(int i=start;i<noOfArguements;i++)
		{
			if(i==start && args[i][0]=='"')
			{
				args[i]=args[i]+1;
				if(args[i][(strlen(args[i]))-1]=='"')
				{
					args[i][(strlen(args[i]))-1] = '\0';
				}
				strcat(dest, args[i]);
				strcat(dest," ");	
			}
			else if(i==(noOfArguements-1) && args[i][(strlen(args[i]))-1]=='"')
			{
				args[i][(strlen(args[i]))-1] = '\0';
				strcat(dest, args[i]);
				strcat(dest," ");
			}
			else{
				strcat(dest, args[i]);
				strcat(dest," ");
			}
		}
		if(isdashN==1)
		{
			printf("%s",dest );
		}
		else{
			printf("%s \n",dest );
		}
		}
		if(isdashE==1)
		{ int start=2;

			if(isdashN==1)
			{
				start++;
			}
			for(int i=start;i<noOfArguements;i++)
		{
			if(i==start && args[i][0]=='"')
			{
				args[i]=args[i]+1;
				if(args[i][(strlen(args[i]))-1]=='"')
				{
					args[i][(strlen(args[i]))-1] = '\0';
				}
				echo_checkbackslash(dest, args[i]);
				strcat(dest, " ");	
			}
			else if(i==(noOfArguements-1) && args[i][(strlen(args[i]))-1]=='"')
			{
				args[i][(strlen(args[i]))-1] = '\0';
				echo_checkbackslash(dest, args[i]);
				strcat(dest, " ");	

			}
			else{
				echo_checkbackslash(dest, args[i]);
				strcat(dest, " ");	

			}
		}
		if(isdashN==1)
		{
			printf("%s",dest );
		}
		else{
			printf("%s \n",dest );
		}
		}}
}
void viewHistory()
{
	if(noOfArguements>2)
	{
		printf("%s\n","too many arguements" );
	}
	else if(noOfArguements==1)
	{
		for(int i=0;i<total_count;i++)
		{
			printf("%d %s",i+1, history[i] );
		}
	}
	else{
		int value= atoi(args[1]);

		if(value<0)
		{	printf("%s\n", "invalid option" );}
		else if(value>total_count)
		{
			for(int i=0;i<total_count;i++)
			{	printf("%d %s",i+1, history[i] );}
		}
		else{
			for(int i=total_count-value; i<total_count;i++)
			{	printf("%d %s",i+1, history[i] );}
		}
	}
}

//error with pwd, how to fix??
void pwd()
{
	if(noOfArguements==1)
	{
		char tmp[1000];
		if(getcwd(tmp, sizeof(tmp))==NULL)
		{
			printf("Error no : %d\n",errno );
		}
		else{
			getcwd(tmp, sizeof(tmp));
			printf("%s\n", tmp);		
		}
	}
	else{
		if(strcmp(args[1], "-L")==0)
		{
			if(getenv("PWD")==NULL)
			{
				printf("%s\n","Specified name cannot be found in the environment of the calling process" );
			}
			else{
				printf("%s\n",getenv("PWD") );		
			}
		}
		else{
			if(strcmp(args[1], "-P") && noOfArguements>1)
			{
				printf("%s\n","too many arguements" );
			}
			else{
				char tmp[1000];
				if(getcwd(tmp, sizeof(tmp))==NULL)
				{
					printf("Error no : %d\n",errno );
				}
				else{
					getcwd(tmp, sizeof(tmp));
					printf("%s\n", tmp);		
				}
		}
		}}
}

void callExit()
{
	exit(0);
}

int main(){

	args = (char **)malloc(sizeof(char *) * inputSize);
	// fd=open("history.txt", O_RDWR | O_CREAT );
	// if(fd==-1)
	// {
	// 	printf("Error no %d\n",errno );
	// 	perror("Error : ");
	// }

	while(1)
	{
		printf(">");
		getline(&line,&inputSize,stdin);
		// write(fd,line, strlen(line));
		char* temp=strtok(line, Delimiter);
		noOfArguements=0;
		// printf("%ld\n", strlen(args[0]));
		while(temp!=NULL)
		{
			args[noOfArguements]=temp;
			noOfArguements++;
			temp=strtok(NULL, Delimiter);
		}
		if(strcmp(args[0], "cd")==0)
		{
			cd();
		}
		else if(strcmp(args[0], "echo")==0)
		{
			echo();
		}
		else if(strcmp(args[0], "history")==0)
		{
			viewHistory();
		}
		else if(strcmp(args[0], "pwd")==0)
		{
			pwd();
		}
		else if(strcmp(args[0], "exit")==0)
		{
			// if (close(fd) < 0)  
   // 			{ 
   //     			perror("Error : "); 
 
   //  		}  
			callExit();
		}

	}
	return 0;

}