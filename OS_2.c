#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
extern int errno;
char *getcwd(char *buf, size_t size);
char *line;
char **args;
int noOfArguements;
size_t inputSize = 1000;
const char *Delimiter = " \t\r\n\a";
char **history;
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
	if (noOfArguements == 1)
	{
		printf("\n");
	}
	else
	{

		int st = 1;
		int isdashN = 0;
		int isdashE = 0;
		if (strcmp(args[st], "-n") == 0)
		{
			isdashN = 1;
			st++;
		}
		if (strcmp(args[st], "-e") == 0)
		{
			isdashE = 1;
			st++;
		}
		if (strcmp(args[st], "-n") == 0)
		{
			isdashN = 1;
			st++;
		}
		if (strcmp(args[st], "-n") == 0)
		{
			isdashE = 1;
			st++;
		}
		char dest[250];
		strcpy(dest, "");
		if (isdashE == 0)
		{
			int start = 1;
			if (isdashN == 1)
			{
				start++;
			}
			for (int i = start; i < noOfArguements; i++)
			{
				if (i == start && args[i][0] == '"')
				{
					args[i] = args[i] + 1;
					if (args[i][(strlen(args[i])) - 1] == '"')
					{
						args[i][(strlen(args[i])) - 1] = '\0';
					}
					strcat(dest, args[i]);
					strcat(dest, " ");
				}
				else if (i == (noOfArguements - 1) && args[i][(strlen(args[i])) - 1] == '"')
				{
					args[i][(strlen(args[i])) - 1] = '\0';
					strcat(dest, args[i]);
					strcat(dest, " ");
				}
				else
				{
					strcat(dest, args[i]);
					strcat(dest, " ");
				}
			}
			if (isdashN == 1)
			{
				printf("%s", dest);
			}
			else
			{
				printf("%s \n", dest);
			}
		}
		if (isdashE == 1)
		{
			int start = 2;

			if (isdashN == 1)
			{
				start++;
			}
			for (int i = start; i < noOfArguements; i++)
			{
				if (i == start && args[i][0] == '"')
				{
					args[i] = args[i] + 1;
					if (args[i][(strlen(args[i])) - 1] == '"')
					{
						args[i][(strlen(args[i])) - 1] = '\0';
					}
					echo_checkbackslash(dest, args[i]);
					strcat(dest, " ");
				}
				else if (i == (noOfArguements - 1) && args[i][(strlen(args[i])) - 1] == '"')
				{
					args[i][(strlen(args[i])) - 1] = '\0';
					echo_checkbackslash(dest, args[i]);
					strcat(dest, " ");
				}
				else
				{
					echo_checkbackslash(dest, args[i]);
					strcat(dest, " ");
				}
			}
			if (isdashN == 1)
			{
				printf("%s", dest);
			}
			else
			{
				printf("%s \n", dest);
			}
		}
	}
}

void viewHistory()
{
	int fd3 = open("./history.txt", O_RDWR, 0);
	if (noOfArguements == 1)
	{
		char lineTemp2[1000];
		int idx = 0;
		int count = 1;
		for (int a = 0; a < 1000; a++)
			lineTemp2[a] = '\0';
		while (1)
		{
			int temp;
			idx = 0;
			while ((temp = read(fd3, &lineTemp2[idx], sizeof(char))) > 0)
			{
				if (lineTemp2[idx] == '\n')
				{
					break;
				}
				idx++;
			}
			if (temp <= 0)
			{
				break;
			}
			if (idx > 0)
			{
				printf("%d %s", count, lineTemp2);
				count++;
			}
			int len = strlen(lineTemp2);
			for (int a = 0; a < len; a++)
			{
				lineTemp2[a] = '\0';
			}
		}
	}
	else if (strcmp(args[1], "-c") == 0)
	{
		close(fd);
		fd = open("./history.txt", O_RDWR | O_CREAT | O_APPEND | O_TRUNC, S_IRWXO | S_IRWXG | S_IRWXU);
	}
	else if (strcmp(args[1], "-d") == 0)
	{
		int offset = atoi(args[2]);
		if(offset>total_count)
		{
			printf("%s\n","history position out of range" );
		}
		else{
			char lineTemp2[1000];
		    int temp;
		    int temp5=0;
		    int idx = 0;
		    while(temp5<(offset-1)){
		    	idx=0;
				while ((temp = read(fd3, &lineTemp2[idx], sizeof(char))) > 0)
				{
					if (lineTemp2[idx] == '\n')
					{
						break;
					}
					idx++;
				}
				temp5++;
				int len = strlen(lineTemp2);
				for (int a = 0; a < len; a++)
				{
					lineTemp2[a] = '\0';
				}
				// printf("Here \n" );
			}
			idx=0;

		    while ((temp5 = read(fd3, &lineTemp2[idx], sizeof(char))) > 0)
		    {
		    	printf("Here 2\n" );
		    	if(lineTemp2[0]== '\n' || lineTemp2[0]=='\0')
		    	{
		  //   						int len = strlen(lineTemp2);
				// for (int a = 0; a < len; a++)
				// {
				// 	lineTemp2[a] = '\0';
				// }
		    		continue;

		    	}
		        if (lineTemp2[idx] == '\n' || lineTemp2[idx] == '\0')
		        {
		        	printf("%s\n","Breaking" );
		            break;
		        }
		      	printf("%c\n", lineTemp2[idx]);
		        lseek(fd3, -1, SEEK_CUR);
		        write(fd3, "\n", sizeof(char) * strlen("\n"));

		    }

		}

	}
	else
	{
		if (noOfArguements > 2)
		{
			printf("%s\n", "too many arguements");
		}
		else
		{
			int value = atoi(args[1]);
			int temp = 0;
			if (value < 0)
			{
				printf("%s\n", "invalid option");
			}
			else if (value > total_count)
			{
				char lineTemp2[350];
				int count = 1;
				int idx = 0;
				while (1)
				{
					int temp;
					idx = 0;
					while ((temp = read(fd3, &lineTemp2[idx], sizeof(char))) > 0)
					{
						//printf("NOT HERE\n" );
						if (lineTemp2[idx] == '\n')
						{
							break;
						}
						idx++;
					}
					if (temp <= 0)
					{
						break;
					}
					printf("%d %s", count, lineTemp2);
					count++;
					int len = strlen(lineTemp2);
					for (int a = 0; a < len; a++)
					{
						lineTemp2[a] = '\0';
					}
				}
			}
			else
			{
				char lineTemp2[350];
				int idx = 0;
				int initial = 1;
				int count = total_count - value + 1;
				while (initial < (total_count - value + 1))
				{
					int temp;
					idx = 0;
					while ((temp = read(fd3, &lineTemp2[idx], sizeof(char))) > 0)
					{
						if (lineTemp2[idx] == '\n')
						{
							break;
						}
						idx++;
					}
					initial++;
					int len = strlen(lineTemp2);
					for (int a = 0; a < len; a++)
					{
						lineTemp2[a] = '\0';
					}
				}

				while (1)
				{
					int temp;
					idx = 0;
					while ((temp = read(fd3, &lineTemp2[idx], sizeof(char))) > 0)
					{
						if (lineTemp2[idx] == '\n')
						{
							break;
						}
						idx++;
					}
					if (temp <= 0)
					{
						break;
					}
					printf("%d %s", count, lineTemp2);
					count++;
					int len = strlen(lineTemp2);
					for (int a = 0; a < len; a++)
					{
						lineTemp2[a] = '\0';
					}
				}
			}
		}
	}
	close(fd3);
}

void pwd()
{
	if (noOfArguements == 1)
	{
		char tmp[1000];
		if (getcwd(tmp, sizeof(tmp)) == NULL)
		{
			printf("Error no : %d\n", errno);
		}
		else
		{
			getcwd(tmp, sizeof(tmp));
			printf("%s\n", tmp);
		}
	}
	else
	{
		if (strcmp(args[1], "-L") == 0)
		{
			if (getenv("PWD") == NULL)
			{
				printf("%s\n", "Specified name cannot be found in the environment of the calling process");
			}
			else
			{
				printf("%s\n", getenv("PWD"));
			}
		}
		else
		{
			if (strcmp(args[1], "-P") && noOfArguements > 1)
			{
				printf("%s\n", "too many arguements");
			}
			else
			{
				char tmp2[1000];
				if (getcwd(tmp2, sizeof(tmp2)) == NULL)
				{
					printf("Error no : %d\n", errno);
				}
				else
				{
					getcwd(tmp2, sizeof(tmp2));
					printf("%s\n", tmp2);
				}
			}
		}
	}
}

void callExit()
{
	exit(0);
}

int main()
{

	args = (char **)malloc(sizeof(char *) * inputSize);
	fd = open("./history.txt", O_RDWR | O_CREAT | O_APPEND, S_IRWXO | S_IRWXG | S_IRWXU);
	if (fd == -1)
	{
		printf("Error no %d\n", errno);
		perror("Error : ");
	}

	while (1)
	{
		printf(">");
		getline(&line, &inputSize, stdin);
		write(fd, line, strlen(line));
		char *temp = strtok(line, Delimiter);
		noOfArguements = 0;
		while (temp != NULL)
		{
			args[noOfArguements] = temp;
			noOfArguements++;
			temp = strtok(NULL, Delimiter);
		}
		if (strcmp(args[0], "cd") == 0)
		{
			cd();
		}
		else if (strcmp(args[0], "echo") == 0)
		{
			echo();
		}
		else if (strcmp(args[0], "history") == 0)
		{
			int fd2 = open("./history.txt", O_RDONLY, 0);
			total_count = 0;
			char lineTemp[300];
			int idx = 0;
			while (1)
			{
				int temp;
				idx = 0;
				while ((temp = read(fd2, &lineTemp[idx], sizeof(char))) > 0)
				{
					if (lineTemp[idx] == '\n')
					{
						break;
					}
					idx++;
				}

				if (temp <= 0)
				{
					break;
				}
				total_count++;
				int len = strlen(lineTemp);
				for (int a = 0; a < len; a++)
				{
					lineTemp[a] = '\0';
				}
			}
			close(fd2);

			viewHistory();
		}
		else if (strcmp(args[0], "pwd") == 0)
		{
			pwd();
		}
		else if (strcmp(args[0], "exit") == 0)
		{
			if (close(fd) < 0)
			{
				perror("Error : ");
			}
			callExit();
		}
	}
	return 0;
}
