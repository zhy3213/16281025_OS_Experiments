#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/stat.h>
#include<fcntl.h>
char buf[10];
sem_t *empty=NULL;
sem_t *full=NULL;
void *input()
{
	
	for(int i=0;i<10;i++)
	{
		sem_wait(empty);
		buf[i]=getchar();
		sem_post(full);
		if(i==9)
		{
			i=-1;
		}
	}	
	return NULL;
}

void *output()
{
	for(int i=0;i<10;i++)
	{
		sem_wait(full);
		printf("输出：%c\n",buf[i]);
		sem_post(empty);
		sleep(1);
		if(i==9)
		{
			i=-1;
		}
	}	
	return NULL;
}

int main(int argc,char *argv[])
{
	empty=sem_open("empty_",O_CREAT,0666,10);
	full=sem_open("full_",O_CREAT,0666,0);
	pthread_t p1,p2;
	pthread_create(&p1,NULL,input,NULL);
	pthread_create(&p2,NULL,output,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	sem_close(empty);
	sem_close(full);
	sem_unlink("empty_");
	sem_unlink("full_");
	return 0;
}
