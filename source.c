//--------------------------------------------
// NAME: Veselin Kirov
// ЕА4 GROUP: 1
// EA4 ID: xxxx
// PROBLEM: #4 Real time strategy prototype
// FILE NAME: rts.c
// FILE PURPOSE:
// Файлът съдържа цялата функционалност свързана с прототипа, на стратегия в реално време.
//(събиране на ресур,обучаванае на работници,строене на казарми,обучаване на войници).
//
//---------------------------------------------
// Please press enter after threads exit.
//--------------------------------------------------------
//       	Функционалност на приложението
// Поддържа до 5 казарми и 20 работника.
// Работниците започват търсенето на ресурс заедно,
// работят и транспортират едновременно, но трябва
// да се изчакват за разтоварване.При ниски нива на
// ресурс(<50) започват да се изчакват при събиране.
// При подаване на команда за обучаване на войник,
// ако в този момент няма свободна казарма,
// но има достатъчно ресурс за обучението ,
// заявката се поставя на опашка и изчаква освобождаване на казарма.
// При успешно обучение на 20 войника или изчерпване на ресурса
// на картата, нишките спират работата си, но е нужно
// да се натисне ENTER за излизане от цикъла за
// четене на входни данни и получаване на крайният резултат
//---------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

const int maxAllowedResource = 4095;


static int initialResources = 4096;
static int barracksCount = 0;
static int isBaseFull = 0;
static int availableRecources = 0; 		// ресурс в базата.
static int totalCollectedResources = 0;
static int buildQueue = 0; 					// опашка за строене на казарми.
//(не се ползва точно като опашка защото ако няма ресурс или свободни работници заявката се чисти)

static int warriorQueue = 0; 				// опашка за обучение на войници.
static int warriorCount = 0;

// Следят тренирането на войниците и тяхната бройка.
static int isWorkerTraining = 0;
static int workerCount = 5;
static int workerTrainingQueue = 0;

pthread_mutex_t baseMutex;
pthread_mutex_t workMutex;
pthread_cond_t baseCond;
pthread_cond_t workCond;

pthread_t workerThreads[20];  // до 20 работника.
pthread_t barracksThreads[5]; // до 5 казарми.
//--------------------------------------------
// FUNCTION: search
// Simulates resource searching time and gathers 5 resource.
// Helps prevent gathering negative resources.
// PARAMETERS: N/A
//----------------------------------------------
void search ()
{
	if(totalCollectedResources == maxAllowedResource || initialResources == 1)
				return;
	sleep(3);
	initialResources -= 5;
}

//--------------------------------------------
// FUNCTION: transport
// Simulates transport time. (probably useless)
// PARAMETERS: N/A
//----------------------------------------------
void transport()
{
	sleep(3);
}

//--------------------------------------------
// FUNCTION: unload
// Simulates resource unloading time.
// Adds the gathered resource value to the command center amount and to the total.
// Prevents unloading more than the max amount of resource.
// PARAMETERS: totalCollectedResources, maxAllowedResource,
// availableResources
//----------------------------------------------
void unload()
{
	if(totalCollectedResources == maxAllowedResource)
				return ;
	totalCollectedResources += 5;
	sleep(1);
	availableRecources += 5;
}

//--------------------------------------------
// FUNCTION: barracks
// Barracks deal with training warriors.
// After thread creation a barracks runs until
// map resources are depleted or 20 warriors are trained.
// A barracks does extra validation to help synchronization.
//
// PARAMETERS: N/A
//----------------------------------------------
void* barracks (void *ptr)
{
	while(warriorCount < 20 && initialResources > 1)
	{

		if(warriorQueue > 0)
		{
			warriorQueue--;

			if (availableRecources - 25 < 0)
			{
				continue;
			}
			availableRecources -= 25;
			printf("Warrior is being trained\n");

			sleep(2);
			if(warriorCount == 20)
			{
				printf("You have max number of warriors.\n");
				printf("Resource refunded.\n");
				availableRecources += 25;
				continue;
			}
			warriorCount++;
			printf("warrior is ready for duty\n");

		}

	}
	pthread_exit(0);
}

//--------------------------------------------
// FUNCTION: build
// Does extra validation then consumes the resources , does
// construction (20s) and launches a barracks thread.
// map resources are depleted or 20 warriors are trained.
// PARAMETERS: N/A
//----------------------------------------------
void build(int i)
{
	buildQueue--;
	if(availableRecources - 100 < 0){
		printf("Not enough resources\n");
		return;
	}

	availableRecources -= 100 ;
	printf("Worker %d is constructing new building (Barracks)\n",i);
	sleep(20);
	if(barracksCount == 5)
	{
		printf("Can't finish build. You have maximum barracks.\n");
		printf("Resource refunded.\n");
		availableRecources += 100 ;
		return;
	}
	barracksCount++;
	printf("Worker %d completed new building (Barracks)\n",i);

	int index = barracksCount-1;

	int err = pthread_create(&barracksThreads[index], NULL, barracks, (void*)(long)index);
	if(err != 0)
	{
		fprintf(stderr, "Error creating barracks thread: error: %d\n", err);
	}
}

//--------------------------------------------
// FUNCTION: worker
// Workers deal with all the functionality
// associated with them - gathering resource,
// transporting it and unloading it.
// They also handle building of barracks through
// build function. Only one worker can unload to
// base station at a time. Also when resources drop
// enough workers gather one at a time.
// PARAMETERS: N/A
//----------------------------------------------
void* worker(void *ptr)
{
	while (initialResources >= 6)
	{
		if(warriorCount >= 20)
		{
			break;
		}
		if(initialResources <= 50)
		{
			pthread_mutex_lock(&workMutex);
		}

		printf("Worker %ld is searching\n",(long)ptr);
		search();
		if(initialResources <= 50)
		{
			pthread_mutex_unlock(&workMutex);
		}


		printf("Worker %ld is transporting\n",(long)ptr);
		transport();

		pthread_mutex_lock(&baseMutex);
		if(isBaseFull == 1)
		{
			pthread_cond_wait(&baseCond, &baseMutex);
		}
		isBaseFull = 1;
		unload();
		printf("Worker %ld unloaded resources to Base station\n",(long)ptr);
		isBaseFull = 0;
		pthread_cond_signal(&baseCond);
		pthread_mutex_unlock(&baseMutex);

		if(buildQueue > 0)
		{
			if(barracksCount <= 4)
			{
				build((long)ptr);
			}
			else
			{
				printf("You have too many barracks.\n");
				buildQueue = 0;
			}
		}
	}
	pthread_exit(0);
}
//--------------------------------------------
// FUNCTION: workerTrainer
// Waits for worker train command, and after simulating
// training, it launches a new worker thread.
// PARAMETERS: N/A
//----------------------------------------------
void * workerTrainer(void *ptr)
{
	while(warriorCount < 20 && initialResources > 1)
	{
		if(workerTrainingQueue > 0 && availableRecources-25 >= 0)
		{
			availableRecources -= 25;
			printf("Worker is being trained.\n");
			sleep(8);
			workerCount++;
			int err = pthread_create(&workerThreads[workerCount-1], NULL, worker, (void*)(long)workerCount);
			if (err != 0)
		    {
	  	        fprintf(stderr, "Error creating worker thread: error: %d\n", err);
	        }
			printf("Worker is ready for work.\n");
			workerTrainingQueue = 0;
			isWorkerTraining = 0;
		}

	}
	pthread_exit(0);
}

//--------------------------------------------
// FUNCTION: commandCenter
// Reads input and does validation of the commands
// before handing them to the workers
// PARAMETERS: N/A
//----------------------------------------------
void * commandCenter(void *ptr)
{
	while(initialResources > 1 && warriorCount < 20)
    {
		if((workerCount+warriorCount) >= 100)
		{
			printf("You have filled the unit capacity.\n");
			printf("Terminating...\n");
		}
    	char ch ;
    	ch = getchar();

	    if(ch == 'b')
 		{
 			if(availableRecources >= 100)
 			{
 				buildQueue++;
 			}
 			else
 			{
 				printf("Not enough resources.\n");
 			}
	   	}
		else if(ch == 'w')
		{
			if(barracksCount > 0 && availableRecources - (warriorQueue*25) >= 25)
			{
				warriorQueue++;
			}
			else if(availableRecources - (warriorQueue*25) < 25)
			{
				printf("Not enough resources.\n");
			}
			else
			{
				printf("Operation not supported.\n");
			}
		}
		else if(ch == 'c')
		{
			if(workerCount < 20)
			{
				if(isWorkerTraining != 0)
				{
					printf("Base busy.\n");
					continue;
				}

				if(availableRecources >= 25)
				{
					workerTrainingQueue = 1;
					isWorkerTraining = 1;
				}
				else
				{
					printf("Not enough resources.\n");
				}

			}
			else
			{
				printf("You have maximum number of workers.\n");
			}
		}
		else
 		{
 			if	(ch == '\n') continue;
 			printf("Invalid command please enter again.\n");
 		}

    }
	pthread_exit(0);
}

//--------------------------------------------
// FUNCTION: main
// Takes care of creating, launching and joining
// the threads, mutex, and condVar
// PARAMETERS: N/A
//----------------------------------------------
int main(void) {
		pthread_t cmdCenter;
		pthread_t workTraining;

		// Initialization of mutex and condVar
		int bMuErr = pthread_mutex_init(&baseMutex, NULL);
		if(bMuErr != 0)
		{
		   	fprintf(stderr, "Error initializing baseMutex: error: %d\n", bMuErr);
		}
		int wMuErr = pthread_mutex_init(&workMutex, NULL);
		if(wMuErr != 0)
		{
	    	fprintf(stderr, "Error initializing workMutex: error: %d\n", wMuErr);
	    }
	    int cndErr = pthread_cond_init(&baseCond, NULL);
	    if(cndErr != 0)
	    {
	       	fprintf(stderr, "Error initializing unload condition: error: %d\n", cndErr);
	    }

	    // Launching of training thread.
	    int trainErr = pthread_create(&workTraining, NULL, workerTrainer, NULL);
	    if(trainErr != 0)
	    {
	    	fprintf(stderr, "Error creating worker trainer thread: error: %d\n", trainErr);
	    }

	    // Launching of command thread.
	    int cmdErr = pthread_create(&cmdCenter, NULL, commandCenter, NULL);
		if(cmdErr != 0)
    	{
    		fprintf(stderr, "Error creating command thread: error: %d\n", cmdErr);
    	}

		// Launching the 5 initial workers.
	    for(int i = 0;i < workerCount;i++)
	    {
	    	int err = pthread_create(&workerThreads[i], NULL, worker, (void*)(long)i+1);
	    	if (err != 0)
	        {
	   	        fprintf(stderr, "Error creating worker thread: error: %d\n", err);
	        }
	    }

	    // Waiting and joining the command thread.
	    int cmdErr2 = pthread_join(cmdCenter, NULL);
		if(cmdErr2 != 0)
    	{
    		fprintf(stderr, "Error joining: command thread: %d\n", cmdErr2);
    	}

		// Waiting and joining the worker threads.
	    for	(int y = 0;y < workerCount;y++)
	    {
	    	int err = pthread_join(workerThreads[y], NULL);
	    	if(err != 0)
	    	{
	    		fprintf(stderr, "Error joining worker thread: error: %d\n", err);
	    	}
	    }

	    // Waiting and joining the trainer thread.
	    int trainErr2 = pthread_join(workTraining, NULL);
	    if(trainErr2 != 0)
	    {
	    	fprintf(stderr, "Error joining worker trainer thread: error: %d\n", trainErr2);
	    }

	    // Waiting and joining the barracks threads.
	    for(int i = 0;i < barracksCount;i++)
	    {
	    	int err = pthread_join(barracksThreads[i],NULL);
	    	if(err != 0)
	       	{
	    		fprintf(stderr, "Error joining barracks thread: error: %d\n", err);
	    	}

	    }

	    // Final results.
	    printf("Initial quantity of resources on the map: %d\n",4096);
	    printf("Remaining resources on the map: %d\n",initialResources);
	    printf("Collected resources: %d\n",totalCollectedResources);


	    // Destruction of mutex and condVar
	    int dErr1 = pthread_mutex_destroy(&baseMutex);
		if(dErr1 != 0)
		{
		   	fprintf(stderr, "Error destroying baseMutex: error: %d\n", dErr1);
		}
	    int dErr2 = pthread_mutex_destroy(&workMutex);
		if(dErr2 != 0)
		{
		   	fprintf(stderr, "Error destroying workMutex: error: %d\n", dErr2);
		}
	    int dErr3 = pthread_cond_destroy(&baseCond);
		if(dErr3 != 0)
		{
		   	fprintf(stderr, "Error destroying condition: error: %d\n", dErr3);
		}

	return EXIT_SUCCESS;
}

