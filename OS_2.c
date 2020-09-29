#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
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
struct stat st;
char *SHELL_PATH;

void cd()
{
	int flagL = 0;
	int position = 0;
	for (int a = 0; a < noOfArguements; a++)
	{
		if (strcmp(args[a], "-L") == 0)
		{
			flagL = 1;
			position = a;
		}
	}
	if (position == 2)
		printf("too many arguements\n");
	else if (flagL == 1 && noOfArguements > 3)
		printf("too many arguements\n");
	else if (flagL == 0 && noOfArguements > 2)
		printf("too many arguements\n");
	else
	{
		if (noOfArguements == 1 || args[1][0] == '~')
		{
			char temp10[1000];
			strcpy(temp10, SHELL_PATH);
			strcat(temp10, args[1] + 1);
			int status = chdir(temp10);
			if (status == -1)
				printf("failed to change to the shell path\n");
			return;
		}
		int status = chdir(args[flagL + 1]);
		if (status == -1)
			printf("No file or directory\n");
	}
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
		printf("\n");
	else
	{
		int isdashN = 0, isdashE = 0, st = 1;
		for (int a = 0; a < noOfArguements; a++)
		{
			if (strcmp(args[a], "-e") == 0)
				isdashE = 1, st++;
			if (strcmp(args[a], "-n") == 0)
				isdashN = 1, st++;
		}
		char dest[250];
		strcpy(dest, "");
		int start = isdashE + isdashN + 1;
		for (int i = start; i < noOfArguements; i++)
		{
			if (i == start && args[i][0] == '"')
			{
				args[i] = args[i] + 1;
				if (args[i][(strlen(args[i])) - 1] == '"')
					args[i][(strlen(args[i])) - 1] = '\0';
				if (isdashE == 1)
				{
					echo_checkbackslash(dest, args[i]);
					strcat(dest, " ");
				}
				else
				{
					strcat(dest, args[i]);
					strcat(dest, " ");
				}
			}
			else if (i == (noOfArguements - 1) && args[i][(strlen(args[i])) - 1] == '"')
			{
				args[i][(strlen(args[i])) - 1] = '\0';
				if (isdashE == 1)
				{
					echo_checkbackslash(dest, args[i]);
					strcat(dest, " ");
				}
				else
				{
					strcat(dest, args[i]);
					strcat(dest, " ");
				}
			}
			else
			{
				if (isdashE == 1)
				{
					echo_checkbackslash(dest, args[i]);
					strcat(dest, " ");
				}
				else
				{
					strcat(dest, args[i]);
					strcat(dest, " ");
				}
			}
		}
		if (isdashN == 1)
			printf("%s", dest);
		else
			printf("%s \n", dest);
	}
}

void viewHistory()
{
	int fd3 = open("./history.txt", O_RDWR, 0);
	if (noOfArguements >= 2 && strcmp(args[1], "-c") == 0)
	{
		close(fd);
		fd = open("./history.txt", O_RDWR | O_CREAT | O_APPEND | O_TRUNC, S_IRWXO | S_IRWXG | S_IRWXU);
	}
	else if (noOfArguements >= 2 && strcmp(args[1], "-d") == 0)
	{
		int offset = atoi(args[2]);
		if (offset > total_count)
			printf("%s\n", "history position out of range");
		else
		{
			char lineTemp2[1000];
			int temp;
			int temp5 = 0;
			int idx = 0;
			while (temp5 < (offset - 1))
			{
				idx = 0;
				while ((temp = read(fd3, &lineTemp2[idx], sizeof(char))) > 0)
				{
					if (lineTemp2[idx] == '\n')
						break;
					idx++;
				}
				temp5++;
				int len = strlen(lineTemp2);
				for (int a = 0; a < len; a++)
					lineTemp2[a] = '\0';
			}
			idx = 0;
			while ((temp5 = read(fd3, &lineTemp2[idx], sizeof(char))) > 0)
			{
				lseek(fd3, -1, SEEK_CUR);
				write(fd3, "\b", sizeof(char) * strlen("\b"));
				if (lineTemp2[idx] == '\n' || lineTemp2[idx] == '\0')
					break;
			}
		}
	}
	else
	{
		if (noOfArguements > 2)
			printf("%s\n", "too many arguements");
		else
		{
			int value = total_count;
			if (noOfArguements == 2)
				value = atoi(args[1]);
			int temp = 0;
			if (value < 0)
				printf("%s\n", "invalid option");
			else
			{
				int idx = 0;
				int initial = 1;
				int count = 1;
				while (initial < (total_count - value + 1))
				{
					char lineTemp2[350] = {'\0'};
					int temp;
					idx = 0;
					char checkBackspace;
					while ((temp = read(fd3, &checkBackspace, sizeof(char))) > 0)
					{
						if (checkBackspace != '\b')
							lineTemp2[idx] = checkBackspace;
						else
							continue;
						if (lineTemp2[idx] == '\n')
							break;
						idx++;
					}
					initial++;
					count++;
				}
				while (1)
				{
					char lineTemp2[350] = {'\0'};
					int temp;
					idx = 0;
					char checkBackspace;
					while ((temp = read(fd3, &checkBackspace, sizeof(char))) > 0)
					{
						if (checkBackspace != '\b')
							lineTemp2[idx] = checkBackspace;
						else
							continue;
						if (lineTemp2[idx] == '\n')
							break;
						idx++;
					}
					if (temp <= 0)
						break;
					printf("%d %s", count, lineTemp2);
					count++;
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
			printf("Error no : %d\n", errno);
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
				printf("%s\n", "Specified name cannot be found in the environment of the calling process");
			else
				printf("%s\n", getenv("PWD"));
		}
		else
		{
			if (strcmp(args[1], "-P") && noOfArguements > 1)
				printf("%s\n", "too many arguements");
			else
			{
				char tmp2[1000];
				if (getcwd(tmp2, sizeof(tmp2)) == NULL)
					printf("Error no : %d\n", errno);
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
	char pathS[1000];
	SHELL_PATH = getcwd(pathS, 1000);
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
		args[noOfArguements] = NULL;
		if (strcmp(args[0], "cd") == 0)
			cd();
		else if (strcmp(args[0], "echo") == 0)
			echo();
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
						break;
					idx++;
				}

				if (temp <= 0)
					break;
				total_count++;
				int len = strlen(lineTemp);
				for (int a = 0; a < len; a++)
					lineTemp[a] = '\0';
			}
			close(fd2);
			viewHistory();
		}
		else if (strcmp(args[0], "pwd") == 0)
			pwd();
		else if (strcmp(args[0], "exit") == 0)
		{
			if (close(fd) < 0)
				perror("Error : ");
			callExit();
		}
		else
		{
			pid_t pid = fork();
			if (pid < 0)
				printf("Could not create a child process\n");
			else if (pid == 0)
			{
				char tmp[1024];
				strcpy(tmp, SHELL_PATH);
				if (strcmp(args[0], "ls") == 0)
					strcat(tmp, "/ls");
				else if (strcmp(args[0], "cat") == 0)
					strcat(tmp, "/cat");
				else if (strcmp(args[0], "date") == 0)
					strcat(tmp, "/date");
				else if (strcmp(args[0], "rm") == 0)
					strcat(tmp, "/rm");
				else if (strcmp(args[0], "mkdir") == 0)
					strcat(tmp, "/mkdir");
				else
				{
					printf("Command not found\n");
					callExit();
				}
				if (execv(tmp, args) < 0)
					perror("execv");
				callExit();
			}
			else if (pid > 0)
				waitpid(pid, NULL, WUNTRACED);
		}
	}
	return 0;
}