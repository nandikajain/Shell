#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
extern int errno;
//handle diff positions of flag
int main(int noOfArguements, char *args[])
{
	int flaga=0;
	int flagA=0;
	DIR *dir;
    for (int a = 0; a < noOfArguements; a++)
	{
	   	if (strcmp(args[a], "-A") == 0)
	        flagA = 1;
	    if (strcmp(args[a], "-a") == 0)
	        flaga = 1;
	}
	int temp=flaga+flagA+1;
	if(noOfArguements==temp)
		dir= opendir(".");
	else
		dir=opendir(args[1]);
	if (dir == NULL)
	{
		perror("ls");
		return 0;
	}
	struct dirent *fileHere;
	fileHere = readdir(dir);
	if(fileHere==NULL)
		perror("ls");
	while (fileHere != NULL)
	{
		char tmp[1024];
		if (fileHere->d_name[0] == '.')
		{
			if (flaga)
				printf("%s ", fileHere->d_name);
			else if (flagA && strcmp(fileHere->d_name, ".") && strcmp(fileHere->d_name, ".."))
				printf("%s ", fileHere->d_name);
		}
		else
			printf("%s ", fileHere->d_name);
		fileHere=readdir(dir);
	}
	printf("\n");
	return 0;
}