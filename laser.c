#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include<time.h>
#include<netdb.h>


void socket_program (int port, int ssn_bit);

int main(int argc, char **argv) {
    int   listenfd, connfd, clilen,ssnbit;
    struct sockaddr_in servaddr, cliaddr;
    char buff[10000];
	char *name,*ftemp, *ssn, *ftemp2;
	char line[100],fname[40],fssn[25],fstatus[50];

    time_t ticks;
	srand (time(NULL));

	if(argc!=3)
	{
        printf("Please type in this format: ./telnetserv <portNumber>");
        exit(1);
    }


    /* Create a TCP socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

	/* Initialize server's address and well-known port */
	bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(atoi(argv[1]));   /* daytime server */

	/* Bind servers address and port to the socket */
    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));       
	/* Convert socket to a listening socket max 100 pending clients*/
	listen(listenfd, 100); 

for(;;)
{

   
	clilen = sizeof(cliaddr);
	connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

	int n ;
	n = read(connfd, buff, 10000);
	if(n<0)
	{
		perror("read");
		exit(5);
	}
	char *search = " ";

	name = strtok(buff, search);
	ssn = strtok(NULL, search);
	int flag=0;

	printf("Name: %s\n",name);
	printf("SSN: %s\n",ssn);
	
	FILE *fl;
	fl = fopen ("status.txt", "rt");
		
	while(fgets(line, 100, fl) != NULL)
   	{
		ftemp = strtok(line,",");
		int i =0;

		while(ftemp != NULL)
		{
			if(i == 0)
			{
				strcpy(fname,ftemp);		
			}
			else if(i==1)
			{
				strcpy(fssn,ftemp);
			}
			else
			{
				strcpy(fstatus,ftemp);		
			}

			ftemp = strtok(NULL,",");
			i++;
		}
		printf("%s\n",fname);
		printf("%s\n",fssn);
		printf("%s\n",fstatus);
    
		if (strcmp(fname,name)==0 && strcmp(fssn,ssn)==0)
		{
			printf("his status =%s\n",fstatus);
			if(strcmp(fstatus,"citizen\n")==0)
			{
				int mainflag = 0;
				FILE *f22;
				f22 = fopen ("verify.txt", "rb");
				char line22[100];
				char *v2,*l2;
				while(fgets(line22, 100, f22) != NULL)
				{
				
					v2 = strtok(line22,",");
					l2 = strtok(NULL," \n");
					if(strcmp(v2, ssn)==0)
					{
						if(strcmp("0",l2)==0)
						{
							mainflag = 1;
							break;
						}
						else
						{
							write(connfd,l2,50);
							break;
						}
					}
				}
				fclose(f22);
				

				if(mainflag == 1)
				{
			
					FILE *f2;
					f2 = fopen("verify.txt", "rb");
					char line2[100];

					printf("%d\n",mainflag);

					FILE *ftemp;
					ftemp = fopen ("temp.txt","w+");

					//flag =1 ;
					char *v1,*l1;


					ssnbit = rand() % 89999999 + 10000000;
					printf("%d\n",ssnbit);
					
					char ssnb[15];
					int ss = sprintf(ssnb,"%d",ssnbit);
					write(connfd,ssnb,15);	

					while(fgets(line2, 100, f2) != NULL)
					{
				
						v1 = strtok(line2,",");
						l1 = strtok(NULL," ");


						if(strncmp(fssn,v1,9)==0)
						{
							fprintf(ftemp,"%s,%d\n",v1,ssnbit);
						
						}
						else
						{
							fprintf(ftemp,"%s,%s",v1,l1);					
						}

					}
					fclose(ftemp);
					fclose(f2);



					if(flag == 0)
					{
						int ret = remove("verify.txt");
						int ret2 = rename("temp.txt","verify.txt");
					}
			printf("socket program k upar");
					socket_program(atoi(argv[2]),ssnbit);


				}

			}
			else
			{
				flag =1;
				int mm= write(connfd,"NO",strlen("NO"));
			}
		}
		
	}
	fclose(fl);
		
	printf("flag value=%d\n",flag);
	

		bzero(buff, 10000);	
		printf("%s", buff);

}


    close(connfd);
    close(listenfd);

//	socket_program(atoi(argv[1]),ssnbit);

}








void socket_program (int port, int ssn_bit)
{
	int  sockfd;
	char recvline[100] ,reply[10];
	struct sockaddr_in servaddr;

	/* Create a TCP socket */
	if((sockfd=socket(AF_INET,SOCK_STREAM, 0)) < 0){
	perror("socket"); exit(2);}
	    
	/* Connect to the server */
	struct hostent *he;
	struct in_addr **addr_list;

	
	if ((he = gethostbyname("bingsuns.binghamton.edu"))== NULL)
	{  // get the host info
		herror("gethostbyname");
		//return 2;
		exit(0);
	}


//	gethostbyname("bingsuns.binghamton.edu");


	//	printf("Official name is: %s\n", he->h_name);
	//	printf("    IP addresses: ");


	addr_list = (struct in_addr **)he->h_addr_list;	
	//	printf("%s \n", inet_ntoa(*addr_list[0]));
	/* Specify server's IP address and port */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port); /* daytime server port */ 
	if (inet_pton(AF_INET,inet_ntoa(*addr_list[0]) , &servaddr.sin_addr) <= 0)
	{
		perror("inet_pton"); exit(3);
	}
	if (connect(sockfd,  (struct sockaddr *) &servaddr,sizeof(servaddr)) < 0 )
	{
		perror("connect");
		exit(4);
	}

	printf("in socket code");

	int op= write(sockfd,"0",strlen("0"));
	printf("write 0 hua\n");

	int n = read(sockfd, reply, 10);
	printf("ok read hua\n");

	if(strcmp(reply,"ok")==0)
	{

		printf("if condition me gaya\n");

		char ssb[15];
		int sb = sprintf(ssb,"%d",ssn_bit);
		printf("ssb:%s, sb:%d, ssn_bit:%d\n ",ssb,sb,ssn_bit);

		int wr = write(sockfd,ssb,strlen(ssb));
		if(wr <0)
			printf("problem happened in write");	

	}
}
