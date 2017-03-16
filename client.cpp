#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<iostream>
#include<stdlib.h>
#include<netdb.h>
#include<sys/stat.h> 
#include<fcntl.h>
#include<time.h>
#include<sys/time.h>
#include<getopt.h>
#include<chrono>
using namespace std;

int main(int argc, char * argv[])    {

        int sockfd,numbytes,status,c;
        struct addrinfo client, *client_details;    
        size_t BUFSZ = 1024*1024*10;
	char *sendbuf = (char*) malloc(BUFSZ);
        char *recbuf = (char*) malloc(BUFSZ);   
	char *ip = nullptr;
	char *port_num = nullptr;
       while (1)
	{
	   
           static struct option long_options[] =
           {
		{"server-ip",required_argument,0,'i'},
		{"server-port",required_argument,0,'p'}
           };
         int option_index = 0;
         c = getopt_long (argc, argv, "i:p:",
                       long_options, &option_index);
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
              cout<<"Invalid arguments..";
	      exit(1);
          }
       }

      if(sendbuf==NULL || recbuf == NULL || ip== NULL || port_num == NULL){
         cout<<"\nmemory not allocated..pl try again..";
	 exit(1);
       }

       cout<<"\nEntered ip:"<<ip;
       cout<<"\nEntered port number:"<<port_num;

       memset(&client, 0 , sizeof(client));
       client.ai_family = AF_UNSPEC;
       client.ai_socktype = SOCK_STREAM;
       status = getaddrinfo(ip,port_num,&client,&client_details);
       if(status!=0){
         cout<<"error getting server address, status: "<<status;
	 exit(1);
       }    
       sockfd = socket(client_details->ai_family, client_details->ai_socktype, client_details->ai_protocol);   
        
       if(connect(sockfd,client_details->ai_addr,client_details->ai_addrlen)< 0){
		cout<<"\nConnect error..\n";
		exit(1);
       }
       else
       {
	 cout<<"\nConnect successful..\n";
       }
	
       // Populate a buufer with random value
       int temp_fd = open("/dev/random",O_RDONLY);
       read(temp_fd,sendbuf,BUFSZ);
       sendbuf[sizeof(sendbuf)-1]='\0';

       size_t len = BUFSZ;
       char *ptr = sendbuf;
      std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
      cout << "Starting upload test \n";
       while(len>0){
           int i=0;

           i=send(sockfd,ptr,len,0);
           if(i<0){
             cout<<"\nSend error\n";
             exit(1);
	   }
           len = len - i;
           ptr = ptr+i;
           
       }

       std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
       auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
       double up_speed = ((BUFSZ*8)/diff)*1000;
       cout<<"\nupload speed = "<<up_speed<<" bits/sec";
      
      cout << "\nstarting download test\n"; 
      start = std::chrono::steady_clock::now();
      numbytes = recv(sockfd,recbuf,BUFSZ,MSG_WAITALL);
      end = std::chrono::steady_clock::now();
       if(numbytes<0){
         cout<<"\nrecv error";
       }
       else{
        diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	double down_speed = ((BUFSZ*8)/diff)*1000;
        cout<<"\ndownload speed = "<<down_speed<<" bits/sec"<<"\n";
      
       }


       close(sockfd);
       free(sendbuf);
       free(recbuf);
       free(ip);
       free(port_num);   
       return 0;

}
