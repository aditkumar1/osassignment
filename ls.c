#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <string.h>

//to store file info
struct fileInfo {
   int blk_size;
   char permission[20];
   int links;
   char username[50];
   char grpname[50];
   int size;
   char date[20];
   char name[50];
};

int main(int argc, char **argv)
{
	DIR *dp;
	int total_blk=0;
  	struct dirent *ep;
    struct fileInfo fileInfo[100],temp;
    int i=0,j=0,k=0;
    struct stat fileStat;
    char date[20];
    char s[20];
    dp = opendir ("./");
	 if (dp != NULL){
	      while (ep = readdir (dp)){
	        if(stat(ep->d_name,&fileStat) < 0)    
        		return 1;
		    total_blk+=(fileStat.st_blocks/2);
		    fileInfo[i].blk_size=(fileStat.st_blocks/2);
			strcpy(s,( (S_ISDIR(fileStat.st_mode)) ? "d" : "-"));
			strcat(s, (fileStat.st_mode & S_IRUSR) ? "r" : "-");
			strcat(s, (fileStat.st_mode & S_IWUSR) ? "w" : "-");
			strcat(s, (fileStat.st_mode & S_IXUSR) ? "x" : "-");
			strcat(s, (fileStat.st_mode & S_IRGRP) ? "r" : "-");
			strcat(s, (fileStat.st_mode & S_IWGRP) ? "w" : "-");
			strcat(s, (fileStat.st_mode & S_IXGRP) ? "x" : "-");
			strcat(s, (fileStat.st_mode & S_IROTH) ? "r" : "-");
			strcat(s, (fileStat.st_mode & S_IWOTH) ? "w" : "-");
			strcat(s, (fileStat.st_mode & S_IXOTH) ? "x" : "-");
			strcpy(fileInfo[i].permission,s);
			fileInfo[i].links=fileStat.st_nlink;
			strcpy(fileInfo[i].username,getpwuid(fileStat.st_uid)->pw_name);
			strcpy(fileInfo[i].grpname,getgrgid(fileStat.st_gid)->gr_name);
			fileInfo[i].size=fileStat.st_size;
			strftime(date, 20, "%b %d %R", localtime(&(fileStat.st_atime)));;
			strcpy(fileInfo[i].date, date);
			strcpy(fileInfo[i].name, ep->d_name);
			i++;
		}  
	    (void) closedir (dp);
	  }
	  else{
	  	perror ("Couldn't open the directory");
	  	return 1;
	  }
	  //ordering according to name
	  for(k=1;k<i;k++){
	  	for(j=0;j<i-k;j++){
	  		if(strcmp(fileInfo[j+1].name,fileInfo[j].name)<0){
	  			temp=fileInfo[j];
	  			fileInfo[j]=fileInfo[j+1];
	  			fileInfo[j+1]=temp;
	  		}
	  	}

	  }

	  //final printing
	  printf("total %d\n",total_blk);
	  for(j=0;j<i;j++){
	  		printf("%-3d",fileInfo[j].blk_size);
			printf("%-11s",fileInfo[j].permission);
			printf("%-4d",fileInfo[j].links);
			printf("%-10s",fileInfo[j].username);
			printf("%-10s",fileInfo[j].grpname);
			printf("%-10d",fileInfo[j].size);
			printf("%-14s",fileInfo[j].date);
			printf("%-10s",fileInfo[j].name);
			printf("\n");	
	  }
    	
    return 0;
}
