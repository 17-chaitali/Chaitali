#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Semaphore variables
sem_t x, y;
pthread_t tid;
pthread_t writerthreads[50];
pthread_t readerthreads[50];
int readercount = 0;

// Reader Function
void* reader(void* param)
{
    // Lock the semaphore
    sem_wait(&x);
    readercount++;
 
    if (readercount == 1)
        sem_wait(&y);
 
    // Unlock the semaphore
    sem_post(&x);
 
    printf("\n%d reader is inside",
           readercount);
 
    sleep(5);
 
    // Lock the semaphore
    sem_wait(&x);
    readercount--;
 
    if (readercount == 0) {
        sem_post(&y);
    }
 
    // Lock the semaphore
    sem_post(&x);
 
    printf("\n%d Reader is leaving",
           readercount + 1);
    pthread_exit(NULL);
}
 
// Writer Function
void* writer(void* param)
{
    printf("\nHello from C1");
 
    // Lock the semaphore
    sem_wait(&y);
 
    printf("\nHello from C2");
 
    // Unlock the semaphore
    sem_post(&y);
 
    printf("\nHello from C3");
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: %s <port_no>\n", argv[0]);
		exit(1);
	}

	int sfd, cfd, port_no;
	port_no = strtoul(argv[1], NULL, 10);

		/*
	Create your Socket do error checking
	Remember socket returns a socket descriptor
	SOCK_STREAM --->TCP
	or
	SOCK_DGRAM  --->UDP
	AF_INET ------->protocol/address family

	*/

// Initialize variables
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
 
    socklen_t addr_size;
    sem_init(&x, 0, 1);
    sem_init(&y, 0, 1);

	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		 perror("socket");
		 exit(2);
	}

	struct sockaddr_in saddr = {0};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port_no);
	saddr.sin_addr.s_addr = INADDR_ANY;// Accept any ip address
	
	//1. Bind is used for assigning port
	if (bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) 
	{
		perror("bind");
		close(sfd);
		exit(3);
	} 

	//2. waits for incoming connection
	if (listen(sfd, 5) < 0) 
	{
		perror("listen");
		close(sfd);
		exit(4);
	} 

	//3. Accepts the incoming connection

	struct sockaddr_in caddr = {0};
	socklen_t len = sizeof(caddr);
	if ((cfd = accept(sfd, (struct sockaddr *)&caddr, &len)) < 0) 
	{
		perror("accept");
		exit(5);
	} 
	
    // Array for thread
    pthread_t tid[60];
 
    int i = 0;

    while (1) {
        addr_size = sizeof(serverStorage);
 
        // Extract the first
        // connection in the queue
        newSocket = accept(serverSocket,
                           (struct sockaddr*)&serverStorage,
                           &addr_size);
        // ...........int choice = 0;
        // recv(newSocket,
        //      &choice, sizeof(choice), 0);............
 
        if (i >= 50) {
            // Update i
            i = 0;
 
            while (i < 50) {
                // Suspend execution of
                // the calling thread
                // until the target
                // thread terminates
             
                pthread_join(writerthreads[i++],
                             NULL);
                pthread_join(readerthreads[i++],
                             NULL);
            }
 
            // Update i
            i = 0;
        }
    }

// To make a program like ECHO
	char buf[20] = {0};
	int ret = 0;
	if ((ret = recv(cfd, buf, sizeof(buf), 0)) < 0)
	{
		perror("recv");
		close(cfd);
		close(sfd);
		exit(6);
	}

	if (send(cfd, buf, ret, 0) < 0)
	{
		perror("send");
		close(cfd);
		close(sfd);
		exit(7);
	}

	close(cfd);
	close(sfd);

	return 0;
}
