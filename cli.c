#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char **argv) {
        int  sockfd, n; 
        char recvline[100], name[10000], ssn[10000], c[10000], vNum[15], c1[10000], okie[5];
	char val[15],voted[15],hist[100];
        struct sockaddr_in servaddr;
	char latRes[100];

	if(argc!=4){
		printf("Please type in this format: ./telnetcli bingsuns.binghamton.edu <portNumber>");
		exit(1); 
	}


        /* Create a TCP socket */
	if((sockfd=socket(AF_INET,SOCK_STREAM, 0)) < 0){
			perror("socket"); exit(2);}
	    

	/* Connect to the server */
	struct hostent *he;
    	struct in_addr **addr_list;
	
	if ((he = gethostbyname(argv[1])) == NULL) {  // get the host info
  	      herror("gethostbyname");
        	return 2;
    	}
//	printf("Official name is: %s\n", he->h_name);
//	printf("    IP addresses: ");
    	addr_list = (struct in_addr **)he->h_addr_list;	
//	printf("%s \n", inet_ntoa(*addr_list[0]));


        /* Specify server's IP address and port */
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(atoi(argv[2])); /* daytime server port */ 

        if (inet_pton(AF_INET,inet_ntoa(*addr_list[0]) , &servaddr.sin_addr) <= 0) {
			 perror("inet_pton"); exit(3);
        }

       if (connect(sockfd,  (struct sockaddr *) &servaddr,sizeof(servaddr)) < 0 ) {
                 perror("connect");
		exit(4);
	}



	for(;;)
	{
		if(atoi(argv[3]) == 0)
		{


		printf("\n Please Enter your name: >");
		gets(name);

		printf("\n Please enter your Social Security Number:");
		gets(ssn);

		char* old = strcat(name," ");
		char* new = strcat(old, ssn);

		printf("%s\n",new);


		n= write(sockfd, new, 10000);
		
		if (n<0)
		{
			perror("write");
			exit(6);
		} 

		int m = read(sockfd, c, 10000);
		
		
		if(m<0)
		{
			perror("read");
			exit(7);
		}
//		c[m] = '\0';

		printf("%s\n", c);
		
		if(strcmp("NO", c)==0)
		{
			printf("Sorry, you are not eligible to vote.\n");
			exit(10);
		}
		else
		{
			printf("Your validation number = %s\n",c);
			exit(0);
		}			

		exit(0);	
		}//if over


		if(atoi(argv[3]) == 1)
		{
			n= write(sockfd,"1",1);

			read(sockfd,okie,5);

			if(strcmp(okie,"ok")==0)
			{
				printf("\n Enter validation number: >");
				gets(vNum);
			}
			n = write(sockfd, vNum, 15);

			int mi = read(sockfd, c1, 10000);
			
			if(strcmp("Invalid", c1)==0)
			{
				printf("You have entered invalid validation number\n");
				exit(10);
			}
			
			if(strcmp("valid", c1)==0)
			{
			
				while(1)
				{
					int input;
					
					printf("\n\n------------------------------------------------------------------------------------");
					printf("\n Make selection ! :");
					printf("\n 1. Vote");
					printf("\n 2. My vote History");
					printf("\n 3. View the latest results");
					printf("\n 4. Exit");
					printf("\n Enter Your Choice:\t");
					scanf("%d",&input);

					char choice[10];
					int aaa= sprintf(choice, "%d",input);					
			
					write(sockfd,choice,aaa);

					switch(input)
					{
					case 1:
						
						read(sockfd, voted, 15);						
						if(strcmp("voted",voted )==0)
						{
							printf("You have already voted. so you cant vote again");
						}
						else if(strcmp("notvoted",voted )==0)
						{
							int voteNum;
							
							printf("\n\n------------------------------------------------------------------------------------");
							printf("\n Enter number 1 or 2 for vote :");
							printf("\n 1. Bob");
							printf("\n 2. John");
							printf("\n Enter Your Choice:");
							scanf("%d",&voteNum);

							char cho[10];
							int aab= sprintf(cho, "%d",voteNum);					

							n= write(sockfd, cho, aab);

							printf("after write\n");
						}
						break;
							
					case 2:
							
						read(sockfd, hist, 100);
						printf("%s\n",hist);						
						break;
							
					case 3:
						read(sockfd, latRes, 100);
						printf("%s\n", latRes);
						break;
					case 4:
						exit(0);
					default:
						break;								
					}
					
				//	write(sockfd, input, 10);
					}
			}
		}


	
	}
	close(sockfd);	
} 

