#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sqlite3.h>
 
char buf[200];
char user[50];
char key[50];
char method[10];

void check_input(void);

int callback(void *p, int argc, char **argv,char **argvv)
{
	*(int *)p = 1;
	return 0;
}

int main(int argc, char *argv[])
{
	int fd;
	sqlite3 *db;
	char *err = 0;
	int ret = 0;
	int login_flag = 0;
	char value[1024];

	printf("Content-Type: text/html\n\n");

	ret = sqlite3_open("./suibian.db",&db);
	if(ret != SQLITE_OK)
	{
		fputs(sqlite3_errmsg(db),stdout);
		fputs("\n",stdout);
		exit(1);
	}
	//printf("ret for open = %d\n",ret);
  
	check_input();

	if((fd = strcmp((const char *)method,"login")) == 0)
	{
		sprintf(value,"select * from usr_msg1 where name='%s' and password='%s'",user,key);
		sqlite3_exec(db,value,callback,&login_flag,&err);
		if(login_flag)
			printf("<meta http-equiv=\"refresh\" content=\"0;url=http://10.1.14.45/sucess.html\"");
		else
			printf("<meta http-equiv=\"refresh\" content=\"0;url=http://10.1.14.45/index.html\"");
	}
	else
	{
		sprintf(value,"select * from usr_msg1 where name='%s'",user);
		sqlite3_exec(db,value,callback,&login_flag,&err);
		if(!login_flag)  	//the register name has not exist yet
		{
			sprintf(value,"insert into usr_msg1(name, password) values('%s','%s');",user,key);
			ret = sqlite3_exec(db,value,NULL,NULL,&err);
			printf("<meta http-equiv=\"refresh\" content=\"0;url=http://10.1.14.45/regis.html\"");
		
		}
		else 
			printf("<meta http-equiv=\"refresh\" content=\"0;url=http://10.1.14.45/index.html\"");
	}

	sqlite3_close(db);
	return 0;
}

void check_input(void)
{
	char *trval = NULL;
	char *head = NULL;
	int i = 0;


	scanf("%s", buf);

	trval = buf;
	while(*(trval++) != '=')
		;	

	head = trval;
	while(*head != '&')
	{
		user[i++] = *(head ++);
	}
	user[i] = '\0';
	//printf("user:%s len = %d\n",user, i);

	while(*(trval++) != '=')
		;

	head = trval;
	i = 0;
	while(*head != '&')
	{
		key[i++] = *(head ++);
	}
	key[i] = '\0';
	//printf("password:%s len = %d\n",key,i);

	while(*(trval++) != '=')
		;

	head = trval;
	i = 0;
	while(*head != '\0')
	{
		method[i ++] = *(head ++);
	}
	method[i] = '\0';
	//printf("method:%s len = %d\n",method, i);
}
 

