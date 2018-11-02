
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>

static struct termios initial_settings, new_settings;
static int peek_character = -1;

void init_keyboard()
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}

void close_keyboard()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}
//
//int kbhit()
//{
//unsigned char ch;
//int nread;
//
//    if (peek_character != -1) return 1;
//    new_settings.c_cc[VMIN]=0;
//    tcsetattr(0, TCSANOW, &new_settings);
//    nread = read(0,&ch,1);
//    new_settings.c_cc[VMIN]=1;
//    tcsetattr(0, TCSANOW, &new_settings);
//    if(nread == 1)
//    {
//        peek_character = ch;
//        return 1;
//    }
//    return 0;
//}

int readch()
{
char ch;

    if(peek_character != -1)
    {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0,&ch,1);
    return ch;
}

//pthread_mutex_t mutex;
pthread_mutex_t baseMutex;
pthread_cond_t workCond;
pthread_cond_t baseCond;

int search ()
{
	sleep(3);
	return 5;
}
void transport()
{
	sleep(3);
}
int unload()
{
	sleep(1);
	return 5;
}

static int resources = 4096;
static int barracks = 0;
static int baseFull = 0;
static int baseRes = 200;
static int buildQueue = 0;
//static int threadsToLaunch = 0;
int launchedThreads = 0;
int warriorQueue = 0;
int warriorCount = 0;

struct Barracks
{
	int id;
	pthread_t pthrd;
	pthread_mutex_t mutex;
	pthread_cond_t warCond;
	int training;

};


struct Barracks barArr[5];
void* createWarrior (void *ptr)
{
	while(1)
	{

		if(warriorQueue > 0)
		{
			warriorQueue--;
			baseRes -= 25;
			if (baseRes - 25 < 0)
			{
				continue;
			}
			printf("TRAINING\n");

			sleep(15);
			warriorCount++;
			printf("WARRIOR TRAINED\n");

		}

	}
	pthread_exit(0);
}



//pthread_t t;

pthread_t warThreads[5];
void build(i)
{
	buildQueue--;
	if(baseRes - 100 < 0){
		printf("Not enough resources\n");
		return;
	}

	baseRes -= 100 ;
	printf("Worker %d is constructing new building (Barracks)\n",i);
	sleep(20);
	barracks++;
	printf("Worker %d completed new building (Barracks)\n",i);

	int index = barracks-1;

	pthread_mutex_init(&barArr[barracks-1].mutex, NULL);
	pthread_cond_init(&barArr[barracks-1].warCond, NULL);

	barArr[index].training = 0;
	pthread_create(&warThreads[index], NULL, createWarrior, (void*)index);

}




void* gather(void *ptr)
{
	while (resources > 5)
	{
		printf("worker %d is searching\n",(int)ptr);
		resources -= search();

//		printf("resources: %d\n",resources);

		printf("worker %d is transporting\n",(int)ptr);
		transport();

		pthread_mutex_lock(&baseMutex);
		if(baseFull == 1)
		{
			pthread_cond_wait(&baseCond, &baseMutex);
		}
		baseFull = 1;
		baseRes += unload();
		printf("Worker %d unloaded resources to Base station\n",(int)ptr);

		printf("base resources: %d\n",baseRes);
		printf("WARIORQUEUE: %d\n",warriorQueue);
		printf("BARRRARRARAKAS: %d\n",barracks);
		printf("Warriors: %d\n",warriorCount);

		baseFull = 0;
		pthread_cond_signal(&baseCond);
		pthread_mutex_unlock(&baseMutex);


		if(buildQueue > 0 && barracks <= 5)
		{
			build((int)ptr);
		}

	}

	 printf("%d\n",resources);
	 printf("%d\n",baseRes);
	pthread_exit(0);
}

int main(void) {

		pthread_t w1, w2,w3,w4,w5;



		pthread_mutex_init(&baseMutex, NULL);

	    pthread_cond_init(&baseCond, NULL);
	    pthread_cond_init(&workCond, NULL);

	    pthread_create(&w1, NULL, gather, (void*)1);
	    pthread_create(&w2, NULL, gather, (void*)2);
	    pthread_create(&w3, NULL, gather, (void*)3);
	    pthread_create(&w4, NULL, gather, (void*)4);
	    pthread_create(&w5, NULL, gather, (void*)5);

//	    pthread_create(&t, NULL, createWarrior, NULL);
//	    int index = 0;
	    while(resources > 0)
	    {

//	    	if(threadsToLaunch > 0)
//	    	{
//	    		threadsToLaunch--;
////	    		pthread_create(&barArr[index].pthrd, NULL, createWarrior, NULL);
//	    		pthread_create(&t, NULL, createWarrior, NULL);
//	    		launchedThreads++;
//	    		index++;
//	    	}
	    	char ch;
    		ch = readch();
    		if(ch == 'b')
     		{
     			if(baseRes >= 100)
     			{
     				buildQueue++;
     			}
     			else
     			{
     				printf("Not enough resources\n");
     			}
		   	}
    		else if(ch == 'w')
    		{
    			if(barracks > 0 && baseRes - (warriorQueue*25) >= 25)
    			{
    				warriorQueue++;
    			}
    			else if(baseRes - (warriorQueue*25) < 25)
    			{
    				printf("Not enough resources\n");
    			}
    			else
    			{
    				printf("Operation not supported.\n");
    			}

    		}
     		else
     		{
     			if	(ch == '\n') continue;
     			printf("INVALID\n");
     		}
	    }
//	    pthread_join(&c,NULL);


	    pthread_join(&w1, NULL);
	    pthread_join(&w2, NULL);
	    pthread_join(&w3, NULL);
	    pthread_join(&w4, NULL);
	    pthread_join(&w5, NULL);
//
	    int i;
	    for(i = 0;i<launchedThreads;i++)
	    {
	    	pthread_join(&warThreads[i],NULL);

	    }
	    printf("%d\n",resources);

//	    pthread_mutex_destroy(&mutex);
	    pthread_mutex_destroy(&baseMutex);
	    pthread_cond_destroy(&workCond);
	    pthread_cond_destroy(&baseCond);

	    //TODO:DESTROY barracks mutex and conditions

//		printf("%d",barArr[4].id);

//	    printf("%d\n",resources);

	return EXIT_SUCCESS;
}
