                  #include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <stdbool.h>
#define SIZE 11
#define BUF 256
#define length 512
 
 struct pdu{
         char type;
         unsigned int usage;
         char data[100];
         char user[10];
         unsigned int port_num;
         unsigned int count;
         
 }  content_pdu, request_pdu;
 
  struct pdu contents[SIZE]={0};
  
          
  
 int main(int argc, char *argv[])
{
	struct  sockaddr_in peer ;	/* the from address of a peer	*/
	char	buf[100];		/* "input" buffer; any size > 0	*/
	char    *pts;
	int	sock;			/* server socket		*/
	int	peer_len;			/* from-address length		*/
	struct  sockaddr_in sin; /* an Internet endpoint address         */
        int     s, type, new_s;        /* socket descriptor and socket type    */
	int 	port=3000;
        char usernames[SIZE];    
        int counter=0;     
         
         int i,j;  
         bool  match=false;                                                     
int c_names,c_data,c_ports;
	switch(argc){
		case 1: 
			break;
		case 2:
			port = atoi(argv[1]);
			break;
		default:
			fprintf(stderr, "Usage: %s [port]\n", argv[0]);
			exit(1);
	}

        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_port = htons(port);
                                                                                                 
    /* Allocate a socket */
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0)
		fprintf(stderr, "can't creat socket\n");
                                                                                
    /* Bind the socket */
        if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		fprintf(stderr, "can't bind to %d port\n",port);
        listen(s, 5);	
	peer_len = sizeof(peer );
        
	while (1) {
		
		peer_len=sizeof(peer);
		
	//if(recvfrom(s,buf,sizeof(buf),0,(struct sockaddr *)&sin, sizeof(sin)) < 0)
	    // fprintf(stderr,"recvfrom error\n");
		//Receive PDU from peers 
		
 if(recvfrom(s,(struct pdu*)&request_pdu,sizeof(request_pdu),0,(struct sockaddr *)&peer,&peer_len)>0){
 
 //Switch statement to determine which case to execute
 switch (request_pdu.type){
 
 case 'R':                  //register content
 if (strcmp(content_pdu.user,request_pdu.user)==0){
  content_pdu.type='E';
   strcpy(content_pdu.data,"This name is taken");
   (void) sendto(s, (struct pdu *)&content_pdu, sizeof(content_pdu), 0, (struct sockaddr *)&peer,peer_len);
 
 } 
 else {
   content_pdu.type='A';
   
 strcpy(content_pdu.data,"Content is registered successfully");
 
 (void) sendto(s, (struct pdu *)&content_pdu, sizeof(content_pdu), 0, (struct sockaddr *)&peer,peer_len);
   for(i=0; i<SIZE; i++){
   if  (strcmp(contents[i].user,request_pdu.user)==0){
   counter=i;
   } } 
 
   strcpy(contents[counter].user,request_pdu.user);
   
   strcpy(contents[counter].data,request_pdu.data);
     
   contents[counter].port_num=request_pdu.port_num;
   
 counter+=1;
 }
 
 break;
 

     case 'S': // search for content and associated server
     
  for(i=0;i<SIZE;i++){
  if (strcmp(contents[i].data,request_pdu.data)==0 ){
  
   content_pdu.type='S';
   content_pdu.port_num=contents[i].port_num;
   if(contents[i].usage>=contents[i+1].usage){
   (void) sendto(s, (struct pdu *)&content_pdu, sizeof(content_pdu), 0, (struct sockaddr *)&peer,peer_len);
   contents[i].usage+=1;
    match=true;
 
  }else if (contents[i].usage<contents[i+1].usage) { //if the usage of the current peer is lower than the next peer
  continue; } } 
  }
 if(match=false) {
   content_pdu.type='E';
 strcpy(content_pdu.data,"Can't find this content");
 (void) sendto(s, (struct pdu *)&content_pdu, sizeof(content_pdu), 0, (struct sockaddr *)&peer,peer_len);
 
 
 }
     break;
     
     case 'T': //content deregistration
     
      for(i=0;i<=SIZE;i++){
  if (strcmp(contents[i].user,request_pdu.user)==0 ){
  //for (j=i;j<size;j++){
  strcpy(contents[i].user,contents[i+1].user);
  strcpy(contents[i].data,contents[i+1].data);
  contents[i].port_num=contents[i+1].port_num;
  //}
  //size-=1;
   
  } } 
  content_pdu.type='A';

   (void) sendto(s, (struct pdu *)&content_pdu, sizeof(content_pdu), 0, (struct sockaddr *)&peer,peer_len);
     break; 
     
     case 'O': //List of online registered contents
     printf("Registered contents:");
     for (i=0;i<SIZE;i++){
     strcpy(content_pdu.user,contents[i].user);
     strcpy(content_pdu.data,contents[i].data);
     strcat(content_pdu.data, "  |");
     strcat(content_pdu.data, content_pdu.user);
     strcat(content_pdu.data, " |");
     strcat(content_pdu.data, contents[i+1].data);
     strcat(content_pdu.data, " |");
     strcat(content_pdu.data, contents[i+1].user);
   
     (void) sendto(s, (struct pdu *)&content_pdu, sizeof(content_pdu), 0, (struct sockaddr *)&peer,peer_len);
     }
     break;
     
     case 'Q' : //quitting
     for(i=0;i<SIZE;i++){
  if (strcmp(contents[i].user,request_pdu.user)==0 ){
  for (j=i;j<SIZE;j++){
  
  strcpy(contents[j].data,contents[i].data);
  contents[j].count++;
  content_pdu.count = contents[j].count;
  
  }
  (void) sendto(s, (struct pdu *)&content_pdu, sizeof(content_pdu), 0, (struct sockaddr *)&peer,peer_len);
  
  } } 
     break;
     
	}
}
} }
