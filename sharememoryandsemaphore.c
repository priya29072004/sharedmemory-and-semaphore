#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>

void semaphore(int semid)
{
  struct sembuf s={0,-1,0};
  semop(semid,&s,1);
}
void signalsemaphore(int semid)
{
  struct sembuf s={0,1,0};
  semop(semid,&s,1);
}
int main()
{
  key_t key=ftok("shmfile",65);
  int shmid = shmget(key,100,0666 | IPC_CREAT);
  char *data=(char *)shmat(shmid,NULL,0);
  int semid = semget(key, 1, 0666 | IPC_CREAT);   
    semctl(semid, 0, SETVAL, 0);                    
    pid_t pid = fork();

    if (pid == 0) {
        
        semaphore(semid);
        printf("Child reads: %s\n", data);
        shmdt(data);
    } else {
        
        strcpy(data, "Hello Child!");
        printf("Parent writes: Hello Child!\n");
        signalsemaphore(semid); 

        wait(NULL); 
        shmdt(data);
        shmctl(shmid, IPC_RMID, NULL); 
        semctl(semid, 0, IPC_RMID);    
    }

    return 0;

}
//Output 
/*
Parent writes: Hello Child!
Child reads: Hello Child!
*/











