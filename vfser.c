
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include<time.h>

void voting(char* validNo);
void history(char* validNo);
void result();
char buff[10000],buff2[8];
char *validNo, *yesNo, *histo, *name3, *calc, *validNo3, *v1,*l1,*v2,*l2;

char line[100],line2[100],line3[100],line4[100],line5[100],line6[100],line7[100],line8[100],line9[100], choice[10],selection[1],vote[5];
int listenfd, connfd, clilen, input;
int flag=0;
struct sockaddr_in servaddr, cliaddr;


int main(int argc, char **argv) {

    time_t ticks;
	srand (time(NULL));

	if(argc!=2)
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
		int uv = read(connfd, selection, 1);
		
		if(strcmp(selection,"0")==0)
		{
			flag = 2;
		}
	
		if(strcmp(selection,"1")==0)
		{
			flag = 3;
		}
	
		printf("selection:a%sa\n",selection);


		//n = read(connfd, buff, 10000);

		if(n<0)
		{
			perror("read");
			exit(5);
		}

		int flagbit = 0;
//		printf("voternumber in buffer:%s\n",buff);

		if(flag ==2)
		{
			printf("flag = %d\n",flag);
			write(connfd,"ok",10);
			printf("after write and before read in if\n");

			int prob = read(connfd, buff2, 8);
			if(prob < 0)
			{
				printf("problem with reading voter number\n");
			}

			printf("validation number read hua");

			FILE *f1;
			f1 = fopen ("voternumber.txt", "a+");
		
			//fseek(f1, -100, SEEK_END);
		    	fprintf(f1,"%s,0\n",buff2);
			//exit(5);
			printf("vallNum:%s, done with procesing LA",buff2);
			fclose(f1);
		
		}
		else if( flag == 3)
		{
			write(connfd,"ok",5);


			n = read(connfd, buff, 10000);


			FILE *f2;
			f2 = fopen ("voternumber.txt", "rw+");

		
			while(fgets(line2, 100, f2) != NULL)
			{
				char *search = ",";
				validNo = strtok(line2, search);
				yesNo = strtok(NULL, search);
			
				printf("validation number: %s\n",validNo);				
				printf("Yes/No: %s\n",yesNo);
				printf("buffer:%s",buff);	


				if(strcmp(buff,validNo)==0)
				{
					flagbit = 1;
					break;
				}
			}	
			
			if(flagbit == 1)
			{
				int m= write(connfd,"valid",strlen("valid"));

				n = read(connfd, choice, 10);

				input = atoi(choice);

				if(input==4)
				exit(0);

				switch(input)
		     		{
		   			case 1:
					printf("Voting is Selected :)\n");
					voting(buff);
					break;
	
				    case 2:
					printf("My vote history is Selected :)\n");
					history(buff);
					break;
	
					case 3:
					printf("View the latest result is Selected :)\n");
					result();
					break;
	
					case 4:
					exit(0);
		
					default:
					break;
		     		}

			}
			else
			{
				int m= write(connfd,"Invalid",strlen("Invalid"));
			}
		}


		/*	int m = write(connfd,buff, strlen(buff));
		
		if(m<0){
		perror("write error");
		exit(5);
		} 
		*/

		bzero(buff, 10000);	
		printf("%s", buff);


	}


    close(connfd);
	close(listenfd);
}

void voting(char* validNo)
{
	char line3[100];
	char* validNo2, *hi;
	int flagbit2=0, abc;

	FILE *f3;
	f3 = fopen ("voternumber.txt", "r");
	

	printf("inside voting\n");
		
	while(fgets(line3, 100, f3) != NULL)
   	{
		printf("inside while\n");
		char *search = ",";
		validNo2 = strtok(line3, search);
		hi = strtok(NULL, search);

		printf("validation number: %s\n",validNo2);
		printf("voted or not: a%sa\n",hi);
	
		
		if(strcmp(validNo,validNo2)==0)
		{
			if(strcmp(hi,"0\n")==0)
			{
				flagbit2 == 1;
				break;
			}
		}
    		
	}
	fclose(f3);

	printf("flagbit2: %d", flagbit2);

	if(flagbit2 == 0)
	{
		int p= write(connfd,"voted",strlen("voted"));
	}
	else
	{
		int k= write(connfd,"notvoted",strlen("notvoted"));

		int n = read(connfd, vote, 10);


		printf("vote: %s",vote);

		FILE *f5;
		f5 = fopen ("result.txt", "r");
		int e=0;
		char l11[15],l12[15],te1[5],te2[5];

		while(fgets(line5, 100, f5) != NULL)
		{	
			if (e ==0)
				{
					v1 = strtok(line5,",");
					l1 = strtok(NULL," ");
					printf("e =0\n");
					printf("v1:%s\n",v1);
					printf("l1:%s\n",l1);

					int x = atoi(l1);
					if(strcmp(vote,"1")==0)
					{
						x++;
					}
//					l1= itoa(x);
					int bv = sprintf(te1,"%d",x);			

					strcpy(l11,v1);
					strcat(l11,",");
					strcat(l11,te1);
				}
			if (e==1)
				{
					v2 = strtok(line5,",");
					l2 = strtok(NULL," ");
					printf("e =1\n");
					printf("v2:%s\n",v2);
					printf("l2:%s\n",l2);
							
					int y = atoi(l2);
					if(strcmp(vote,"2")==0)
					{
   						y++;
					}
//					l2= itoa(y);
					int bw = sprintf(te2,"%d",y);					
					strcpy(l12,v2);
					strcat(l12,",");
					strcat(l12,te2);

				}
				e++;
		}
		//fclose(f5);


//					printf("outside v1:%s\n",v1);
//					printf("outside l1:%s\n",l1);
//					printf("outside v2:%s\n",v2);
//					printf("outside l2:%s\n",l2);

	
					printf("outside l11:%s\n",l11);
					printf("outside l12:%s\n",l12);


		FILE *f6;
		f6 = fopen ("temp1.txt","w+");
		if(f6 == NULL){printf("File error");}
//		if(strcmp(vote,"1")==0)
//		{
//			fprintf(f6,"%s,%d\n",v1,atoi(l1));
//			fprintf(f6,"%s,%s\n",v2,l2);
						
//		}
//		if(strcmp(vote,"2")==0)
//		{
			fprintf(f6,"%s\n",l11);
			fprintf(f6,"%s\n",l12);
//		}

		fclose(f6);
		fclose(f5);		
			
		int r1 = remove("result.txt");
		int r2 = rename("temp1.txt","result.txt");
			

		FILE *f4;
		f4 = fopen ("history.txt", "a");

		time_t curtime;
   		time(&curtime);
		fprintf(f4,"%s,%s",validNo,ctime(&curtime));

		printf("end tak pahucha\n");

		fclose(f4);
		

			
					FILE *f200;
					f200 = fopen("voternumber.txt", "rb");
					char line200[100];

					FILE *ftemp100;
					ftemp100 = fopen ("temp100.txt","w+");
					
					//flag =1 ;
					char *v200,*l200;


					while(fgets(line200, 100, f200) != NULL)
					{
				
						v200 = strtok(line200,",");
						l200 = strtok(NULL," ");

					if(strcmp(validNo,v200)==0)
							fprintf(ftemp100,"%s,1\n",v200);
						
					else
							fprintf(ftemp100,"%s,%s\n",v200,l200);


					}
					fclose(ftemp100);
					fclose(f200);

					remove("voternumber.txt");
					rename("temp100.txt","voternumber.txt");
								
								
		
		
		
		
		
	}
}



void history(char* validNo)
{

	char hist[100];
	FILE *f7;
	f7 = fopen("history.txt", "r+");
		
	while(fgets(line3, 100, f7) != NULL)
   	{
		char *search = ",";
		validNo3 = strtok(line3, search);
		histo = strtok(NULL, search);

		printf("validation number: %s\n",validNo3);
		printf("History: %s\n",histo);
	
		if(strcmp(validNo,validNo3)==0)
		{
			strcpy(hist,histo);
			printf("history:%s",hist);
			int o= write(connfd,hist,100);
			break;
		}		
	}
	fclose(f7);
}


void result()
{
	char resu[100];
	FILE *f8;
	f8 = fopen("result.txt", "r+");
		
	while(fgets(line8, 100, f8) != NULL)
   	{
		strcat(resu,line8);
	}
	int o= write(connfd,resu,100);	
	fclose(f8);

}
