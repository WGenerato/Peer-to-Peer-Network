
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>


#define SERVER_TCP_PORT 3000	/* well-known port */
#define BUFLEN		256	/* buffer length */

struct pdu {
char type;
unsigned int len;
char data[100];
char user[10];
unsigned int count;
unsigned int port_num;
}info_peer_pdu, t_pdu, r_pdu, name, content_pdu, request_pdu;

int main(int argc, char **argv)
{
	int 	n, i, bytes_to_read, length;
	int 	sd, port, sd1, new_sd, p_sock;
	struct	hostent		*hp;
	struct	sockaddr_in server, client;
	char    *host = "localhost";
 //	int     port = 3000;
	struct hostent  *phe;   /* pointer to host information entry    */
        struct sockaddr_in sin; /* an Internet endpoint address         */
        int     s, type;     /* socket descriptor and socket type    */
	//char	*host
	char    *bp, rbuf[BUFLEN], sbuf[BUFLEN],input,wbuf[BUFLEN];
	char file_name[256], username[10];
	FILE * fp;
	FILE * fs;	


	switch (argc) {
        case 1:
                break;
        case 2:
                host = argv[1];
        case 3:
                host = argv[1];
                port = atoi(argv[2]);
                break;
        default:
                fprintf(stderr, "usage: UDPtime [host [port]]\n");
                exit(1);
        }
	

	//switch(argc){
	//case 2:
	//	host = argv[1];
	//	port = SERVER_TCP_PORT;
	//	break;
//	case 3:
//		host = argv[1];
//		port = atoi(argv[2]);
//		break;
//	default:
//		fprintf(stderr, "Usage: %s host [port]\n", argv[0]);
//		exit(1);
//	}

	/* Create a stream socket	*/	
//	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
//		fprintf(stderr, "Can't creat a socket\n");
//		exit(1);
//	}
//

	memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);


    /* Map host name to IP address, allowing for dotted decimal */
        if ( phe = gethostbyname(host) ){
                memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
        }
        else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
                fprintf(stderr, "Can't get host entry \n");


    /* Allocate a socket */
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0)
                fprintf(stderr, "Can't create socket \n");
	/* Connect the socket */
        if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
                fprintf(stderr, "Can't connect to %s %s \n", host, "Time");

	struct sockaddr_in reg_addr; //setting up dynamic TCP ports 
	sd = socket(AF_INET, SOCK_STREAM, 0); 
	reg_addr.sin_family = AF_INET; 
	reg_addr.sin_port = htons(0); 
	reg_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	bind(sd, (struct sockaddr *)&reg_addr, sizeof(reg_addr));
	//listen(sd,5);
        
	bzero((char *)&server, sizeof(struct sockaddr_in));
        server.sin_family = AF_INET;
	if (hp = gethostbyname(host))
        bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);
        else if ( inet_aton(host, (struct in_addr *) &server.sin_addr) ){
        fprintf(stderr, "Can't get server's address\n");
        exit(1);
    }


       int alen = sizeof(struct sockaddr_in);
       getsockname(sd,(struct sockaddr*)&reg_addr, &alen);
	
       
       fd_set rfds, afds;
	//sd1 = socket(AF_INET, SOCK_STREAM, 0);
	FD_ZERO(&afds);
	FD_SET(sd,&afds);
	FD_SET(sd,&rfds);
	FD_SET(0, &afds);
	FD_SET(0, &rfds);

//	memcpy(&rfds, &afds, sizeof(rfds));
	//select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
 
	printf("Enter your username. (maximum 10 characters) \n");
	n=read(0,&username, 10);	



while(1){
	

	select(FD_SETSIZE, &rfds, NULL, NULL, NULL);

	if(FD_ISSET(sd, &rfds)){
	new_sd = accept(sd, (struct sockaddr*)&client, &alen);
	n = read(new_sd, &info_peer_pdu, BUFLEN);
	if(strcmp(info_peer_pdu.data, "movie1")==0){
	fs = fopen("/home/anujan/Peer1/movie1.txt", "r" );
	n = fread(content_pdu.data,1,256,fs);
	if(n>0){
	write(new_sd,&content_pdu, BUFLEN);
			}
		}
	close(new_sd);
	}

if(FD_ISSET(0, &rfds)){

         printf("\nEnter D to Download content \n");
         printf("Enter R to Register Content To Index \n");
         printf("Enter T For Content Deregistration \n");
	 printf("Enter O-List all the On-line Content \n");
         printf("Enter Q to Quit \n");
 
         scanf("%c", &input);


if(input == 'R'){
	printf("\nEnter the name of the file (exclude the extension) \n");
	n = read(0, &name.data, BUFLEN);
	strcpy(name.user, username);
	name.type = 'R';
	printf("\nContent Name:%s \n", name.data);
	
	name.port_num = reg_addr.sin_port;
	write(s, &name, BUFLEN);	
	n = read(s, &r_pdu, BUFLEN);
	if(r_pdu.type == 'A'){
	printf("\nThis content has been registered \n");
	}
	
}

if(input == 'D'){
	printf("\nEnter the name of the file without the extension: \n ");
	n = read(0, t_pdu.data, 256);
	strcpy(t_pdu.user, username);
	t_pdu.type = 'S';
	strcpy(file_name, t_pdu.data);
	strcat(file_name, ".txt");

	write(s, &t_pdu, BUFLEN);

	n = read(s, &r_pdu, BUFLEN);

	if(r_pdu.type == 'S'){
        server.sin_port = htons(r_pdu.port_num);
	sd1 = socket(AF_INET, SOCK_STREAM, 0);
	printf("sd1 is: %d", sd = socket(AF_INET, SOCK_STREAM, 0));
	connect(sd1, (struct sockaddr*)&server, sizeof(server)); 
	request_pdu.type = 'D';
	strcpy(request_pdu.data, t_pdu.data);
	write(sd1, &request_pdu, BUFLEN);
	n = read(sd1, &content_pdu, BUFLEN);
	if(content_pdu.type == 'C'){
	fp = fopen(file_name, "w+");
	fprintf(fp, "%s",content_pdu.data);
	printf("\nThe file is saved on local device\n");
	fclose(fp);
	strcpy(name.user, username); //start registering the content after download
	name.type = 'R';
	strcpy(name.data, t_pdu.data);
	printf("Content Name:%s \n", name.data);
	name.port_num = reg_addr.sin_port;
	write(s, &name, BUFLEN);
	n = read(s, &r_pdu, BUFLEN);
	if(r_pdu.type == 'A'){
	printf("\nThis content has been registered \n");
	}

	}
	}
     }

if(input == 'O'){
	printf("\nRecieving listing of online content\n");
	t_pdu.type = 'O';
	write(s, &t_pdu, BUFLEN);
	n = read(s, &r_pdu, BUFLEN);

	printf("\nAvailable Content: \n %s", r_pdu.data);
}

if(input == 'T'){
	printf("Enter the name of the file to deregister: \n");
	n = read(0, &t_pdu.data, BUFLEN);
	strcpy(t_pdu.user, username);
	t_pdu.type = 'T';
	write(s, &t_pdu, BUFLEN);
	n = read(s, &r_pdu, BUFLEN);

	if(r_pdu.type == 'A'){
		printf("The content was successfully deregistered");
	}
	else{
		printf("Content was not deregistered");
	}
}

if(input == 'Q'){
	t_pdu.type = 'Q';
	strcpy(t_pdu.user, username);
	write(s, &t_pdu, BUFLEN);
	n = read(s, &r_pdu, BUFLEN);
	int i;
	for(i=0;i<r_pdu.count+1; i++){
	strcpy(t_pdu.user, username);
        t_pdu.type = 'T';
        write(s, &t_pdu, BUFLEN);
	}
	close(sd);
        return(0);
}



}
}
}

