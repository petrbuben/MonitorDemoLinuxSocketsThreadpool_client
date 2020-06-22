#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#include <ctime>

int main(){//client

  const int msg_to_server_size = 300; //better than define
  const int msg_from_server_size = 1000; //better than define

  char msg_to_server[msg_to_server_size]="";
  char msg_from_server[msg_from_server_size]="";

  //memset(message,0,sizeof(message));
  //memset(buffer,0,sizeof(buffer));

  int clientSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  int i;

  printf ("client\n");

  //more clients - demo testing
  for(i=0;i<3;i++){
  //Create the socket.
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  if(clientSocket < 0){
     perror("client socket creation error");
     exit(-1);
  }

  //Configure settings of the server address
  //Address family is Internet
  serverAddr.sin_family = AF_INET;

  //Set port number, using htons function - must know about server
  serverAddr.sin_port = htons(7799);

  //Set IP address to localhost - must know about server
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);


  //5 temperature readings first here, done by monitor - demo testing
  char readings[16]="";

  for(int r=12; r<22; r+=2){
    sprintf(readings, "%s%d%c", readings, r*(i+1), ',');
  }
  readings[strlen(readings)-1] ='\0';

  //Connect the socket to the server using the address
  addr_size = sizeof (serverAddr);
  int c = connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  if (c < 0){
    perror("from client: failed to connect, exiting\n");
    exit(-1);
  }
  printf("\nconnected, sending to server.. \n");


  //file name to send  yyyy.mo.dd.hh.mi.dat
  time_t rawtime = time(nullptr);
  struct tm * ltm = localtime(&rawtime);
  sprintf(msg_to_server,"client%02d.%4d.%02d.%02d.%02d:%02d.dat", i, ltm->tm_year+1900, ltm->tm_mon+1,ltm->tm_mday,
  ltm->tm_hour,ltm->tm_min);

  //send file name to server
  if( write(clientSocket, msg_to_server, sizeof(msg_to_server) ) < 0)
  {
    printf("from client: Send failed\n");
    exit(-1);
  }

  //Read the message from the server into the buffer
  if(recv(clientSocket, msg_from_server, sizeof(msg_from_server), 0) < 0)
  {
    printf("from client: Receive failed\n");
  }

  //Print the received message
  printf("from server: %s\n", msg_from_server);


  //send readings to server
  if( write(clientSocket, readings, sizeof(readings) ) < 0)
  {
    printf("from client: Send failed\n");
    exit(-1);
  }

  //Read the readings message from the server
  if(recv(clientSocket, msg_from_server, sizeof(msg_from_server), 0) < 0)
  {
    printf("from client: Receive failed\n");
  }

  //Print the received readings message
  printf("from server: %s\n", msg_from_server);


  close(clientSocket);
 }//for

  return 0;

}
