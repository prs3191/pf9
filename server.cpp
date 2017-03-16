#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<iostream>
#include<stdlib.h>
#include<sstream>
#include<netdb.h>
#include <getopt.h>

using namespace std;
int main(int argc, char *argv[])    {

       int sockfd,newfd;     
       struct sockaddr_in myaddr;
       struct sockaddr_storage their_addr;
       struct addrinfo serv, *serv_details;
       unsigned int sin_size;
       int numbytes, status,c;
       size_t BUFSZ = 1024*1024*10;
       char *sendbuf = (char*) malloc(BUFSZ);
       char *recbuf = (char*) malloc(BUFSZ);

  
	char *ip;
	char *port_num;
       while (1)
	{
           static struct option long_options[] =
           {
		{"server-ip",required_argument,0,'i'},
		{"server-port",required_argument,0,'p'}
           };
         int option_index = 0;
         c = getopt_long (argc, argv, "i:p:",long_options, &option_index);
         /* Detect the end of the options. */
         if (c == -1)
           break;
          switch (c)
          {
            case 'i':
	     ip = (char *) malloc(strlen(optarg));
             strcpy(ip,optarg);
             break;
            case 'p':
	     port_num = (char *) malloc(strlen(optarg));
             strcpy(port_num,optarg);
             break;
            default:
              cout<<"Pl pass arguments..";
	      exit(1);
          }
       }
      if(sendbuf==NULL || recbuf == NULL || ip== NULL || port_num == NULL){
         cout<<"\nmemory not allocated..pl try again..";
	 exit(1);
       }
       cout<<"\nEntered ip:"<<ip;
       cout<<"\nEntered port number:"<<port_num;


       memset(&serv, 0 , sizeof(serv));
       serv.ai_family = AF_UNSPEC;
       serv.ai_socktype = SOCK_STREAM;
       status = getaddrinfo(ip,port_num,&serv,&serv_details);

       if(status!=0){
         cout<<"\nerror getting server address, status: "<<status;
	 exit(1);
       }   

       sockfd = socket(serv_details->ai_family, serv_details->ai_socktype, serv_details->ai_protocol);

       if(bind(sockfd, serv_details->ai_addr,serv_details->ai_addrlen ) < 0){
		cout<<"\nBinding error..\n";
		exit(1);
       } 
       else  {
	cout<<"\nBinding successful..\n";
       }
       
       listen(sockfd,1);

       sin_size=sizeof(struct sockaddr_storage);

       newfd=accept(sockfd,(struct sockaddr *)&their_addr,&sin_size);
       if(newfd < 0) 
       {
	cout<<"Error accepting connection..\n";
       }	
       else 
       { 
	cout<<"\nConnection accepted\n";
       }

       //recv from client
       numbytes = recv(newfd,recbuf,BUFSZ,MSG_WAITALL);
       if(numbytes<0){
         cout<<"\nrecv error";
       }
       else{
       	cout<<"\nin server received bytes:"<<numbytes<<"\n";
       }

       strcpy(sendbuf,recbuf);
       size_t len = BUFSZ;
       char *ptr = sendbuf;
       while(len>0){
           int i=0;

           i=send(newfd,ptr,len,0);
           if(i<0){
             cout<<"\nSend error\n";
             exit(1);
	   }
           len = len - i;
           ptr = ptr+i;
           
       }

       close(newfd);
       close(sockfd);
       free(sendbuf);
       free(recbuf);  
       free(ip);
       free(port_num);    


       return 0;
}


