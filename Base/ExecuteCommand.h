#ifndef __EXECUTECMD_H__
#define __EXECUTECMD_H__
#include <stdio.h>
#include <string.h>


#define MAX_FILE_LEN  1024*10
char const  *allowed[] = {"ls","pwd","cd"};
bool is_allowed(const char* p)
{
	int ret = -1;
	if(p==NULL){
		return ret;
	}
	for(int i=0;i< (sizeof(allowed)/sizeof(char*)) ;i++)
	{
		char const *tmp =allowed[i];
		printf("i %d tmp %s p %s\n",i,tmp,p);
		if(strcmp(tmp,p)==0)
		{
			ret =0;
			break;
		}
	}
	return ret;
}
int  execute_command(const char *cmd, char *result)
{
	if(is_allowed(cmd) !=0 )
	{
		strcpy(result,"not allowed cmd");
		return -1;
	}
	char buf_ps[MAX_FILE_LEN];                          
	char ps[MAX_FILE_LEN]={0};
	FILE *ptr;
	strcpy(ps, cmd);
	if((ptr=popen(ps, "r"))!=NULL)              //打开管道
	{
		while(fgets(buf_ps, MAX_FILE_LEN, ptr)!=NULL)
		{
			strcat(result, buf_ps);
			if(strlen(result)>MAX_FILE_LEN)
				break;
		}
		pclose(ptr);                            //关闭管道
		ptr = NULL;
		return 0;
	}
	else
	{
		printf("popen %s error\n", ps);
		return -1;
	}
}
/*
   int main()
   {
   char result[MAX_FILE_LEN];
   executeCMD("ifconfig", result);
   printf("===================shell[\"ifconfig\"]====================\r\n");
   printf("%s\r\n", result);
   printf("==========================================================\r\n");
   return 0;
   }
 */

#endif
