#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
int ticketCount=1000;
sem_t *mutex=NULL;
void *sell()
{
	int num,temp;
	num=1000;
	for(int i=0;i<num;i++)
	{
		sem_wait(mutex); 
		temp=ticketCount;
		pthread_yield();
		temp=temp-1;
		ticketCount-=1;
		pthread_yield();
		ticketCount=temp;
		sem_post(mutex); 
	}
	return NULL;
}

void *refund()
{
	int num,temp;
	num=1000;
	for(int i=0;i<num;i++)
	{
		sem_wait(mutex); 
		temp=ticketCount;
		pthread_yield();
		temp=temp+1;
		ticketCount+=1;
		pthread_yield();
		ticketCount=temp;
		pthread_yield();
		sem_post(mutex); 
	}
	return NULL;
}
int main(int argc,char *argv[])
{
	mutex=sem_open("mutex",O_CREAT,0666,1); 
	printf("初始票数为：%d\n",ticketCount);
	pthread_t p1,p2;
	pthread_create(&p1,NULL,sell,NULL);
	pthread_create(&p2,NULL,refund,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	printf("最终票数为：%d\n",ticketCount);
	sem_close(mutex); 
	sem_unlink("mutex"); 
	return 0;
}
