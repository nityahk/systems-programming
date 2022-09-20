/* lab1.c
 * Nitya Harikumar
 * nhariku
 * ECE 2220, Fall 2022
 * MP1
 *
 * Purpose: The purpose of this project is to use 
 * conditionals and loops to decrypt messages about 
 * "agents" status from user input.
 *
 *
 * Assumptions: None
 * 
 *  
 *
 * Bugs:  None
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//notice there is no semi-colon at the end of a #define
#define MAXLINE 100
#define MINNUM 10
#define MAXNUM 100
#define MINSIZE 6
#define MAXSIZE 12
#define MINKEY 1
#define MAXKEY 5


int main()
{
    char line[MAXLINE];
    int msgsize;
    int keyvalue; 
    int iter;
    int id;
    int msg = 0;
    
    fgets(line, MAXLINE, stdin);//Utilizing fgets and sscanf to get user input
    sscanf(line, "%d", &msgsize);
    printf("\nMsgsize is set to %d.\n", msgsize);
    fgets(line,MAXLINE,stdin);
    sscanf(line, "%d", &keyvalue);
    printf("\nKeyValue is set to %d.\n", keyvalue);

    while (msg!=-2){ 
	    //Loop goes to the end of file
	    iter++;

	    int signals=0;
	    int corrFlag = 0;

	    //Loop to divide by msgsize
	    for (int i=0; i<msgsize;i++){
		    fgets(line,MAXLINE,stdin); // Starts reading messages
		    sscanf(line,"%d",&msg); //msg is replaced with each iteration

		    if (iter==1 && i==msgsize-1)
			    id = msg; //id is the last num in original msg
		    if (msg == -2){
			    printf("\n>> Agent Center is not safe now. Go find a safe place. Program exited.\n\n\n");
			    break; //exits out of entire program
		    }
		    else if (msg<=MINNUM || msg>=MAXNUM){
			    corrFlag = 1; //Flag allows for the msg to be read, but no signal processing
		    }

		    int msgMinKey = msg-keyvalue; 
		    int checkPrime =1; //Prime until proven not

		    // Loop to check for prime numbers
		    for (int j=2; j<msgMinKey/2;j++){
			    if (msgMinKey%j==0){
				    checkPrime=0;
				    break;
			    }
			    else
				    checkPrime =1;
		    }
		    if (checkPrime==1)
			    signals++;
	    }
	    if (msg == -2)
		    break;

	    //outputs  based on number of signals in given msg segment
	    if(corrFlag ==0){
		    if (signals ==0)
			    printf(">> Msg:  No Content.    \n");
		    else if (signals==1)
			    printf(">> Msg:  I'm safe, all good.\n");
		    else if (signals==2) 
			    printf(">> Msg:  Mission Success. Agent ID: %d.\n", id);
		    else if(signals==3)
			    printf(">> Msg:  Don't contact me.\n");
		    else if (signals==4)
			    printf(">> Msg:  Mission Failed. Agent ID: %d.\n", id);
		    else
			    printf(">> Msg:  Don't contact me.\n");
	    }
	    else
		    printf(">> Corrupted Message.\n");
    }
  
    exit(0);
}


