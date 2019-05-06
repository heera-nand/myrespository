#include<linux/delay.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/sched.h>
#include<linux/time.h>
#include<linux/timer.h>
#include<linux/semaphore.h>
#include<linux/kernel.h>


static struct semaphore Smoker[3];
static struct semaphore tempSem[3];

static struct task_struct *Stid[3];
static struct task_struct *Atid;

int i=0;
int AllDone=0;
int Members[3]={0};
char our_thread[8] = "thread1";

int Tobacco_smoker(void)
{	
	
	while(AllDone == 0)
	{	
		down(&Smoker[0]);
		down(&tempSem[0]);	


		up(&Smoker[2]);
		up(&Smoker[1]);


		printk("Tobacco holder is waiting for other two constraints\n");
		msleep(1);
		down(&Smoker[1]);
		down(&Smoker[2]);
		printk("Tabacco Holder is smoking\n");
		msleep(1);
		printk("Tobacco holder has finished smoking\n");
		
		// sem_post(&Smoker[1]);
		// sem_post(&Smoker[2]);
		up(&Smoker[0]);

	}
	return 0;
}


int Matches_smoker(void)
{	
	
	while(AllDone == 0)
	{	
		down(&Smoker[1]);	
		down(&tempSem[1]);	

		
		up(&Smoker[0]);
		up(&Smoker[2]);

		printk("Matches holder is waiting for other two constraints\n");
		msleep(1);
		
		down(&Smoker[0]);
		down(&Smoker[2]);

		printk("Matches Holder is smoking\n");
		msleep(1);
		printk("Matches holder has finished smoking\n");
	
		
		// sem_post(&Smoker[0]);
		// sem_post(&Smoker[2]);
		up(&Smoker[1]);
	}

	return 0;
}



int Paper_smoker(void)
{	
	

	while(AllDone == 0)
	{
		down(&Smoker[2]);	
		down(&tempSem[2]);	

		up(&Smoker[0]);
		up(&Smoker[1]);
	
		printk("Paper holder is waiting for other two constraints\n");
		msleep(1);
		
		down(&Smoker[0]);
		down(&Smoker[1]);
		
		printk("Paper Holder is smoking\n");
		msleep(1);
		printk("Paper holder has finished smoking\n");

		
		up(&Smoker[2]);
	
	}

	return 0;
}





int Agent(void)
{	

	msleep(1);
	
	for(int i=0;;i++)	
	{	
		// int x=get_random_bytes()%3;
		int x = i%3;
		printk("Random: %d\n", x);
		for(int j=0;j<3;j++)
		{		
				
			if(j==x)
				up(&tempSem[j]);	
			
		
		}
		msleep(3);

		
	}

	AllDone = 1;

	return 0;
}



asmlinkage long sys_chainsmoker(void)
{


		for (int i = 0; i < 3; ++i)
			sema_init(&Smoker[i],1);
	

		for (int i = 0; i < 3; ++i)
			sema_init(&tempSem[i],0);
	
		Atid = kthread_create(Agent,NULL,our_thread);
		if((Agent))
		{
			wake_up_process(Atid);
			
		}
	

	Stid[0] = kthread_create(Tobbacoo_smoker,NULL,our_thread);
		if((Stid[0]))
		{
			wake_up_process(Stid[0]);
			
		}

	Stid[1] = kthread_create(Matches_smoker,NULL,our_thread);
		if((Stid[1]))
		{
			wake_up_process(Stid[1]);
			
			
		}
		
	Stid[2] = kthread_create(Paper_smoker,NULL,our_thread);
		if((Std[2]))
		{
			wake_up_process(Stid[2]);
			printk(KERN_INFO "in if");
			
		}
			


	
	msleep(5);


	for(i=0; i<3; i++)
		{
			if (Stid[i]) {
				kthread_stop(Stid[i]);
				printk(KERN_INFO "Smoker %d killed\n", i+1);		
			}
		}

		kthread_stop(Atid);
		
	return 0;
}

		
