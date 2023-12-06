#include "input.h"
/***************************************************************************************************
Command Instructions
***************************************************************************************************/
//To Compile: make
//To Run: ./project c17.isc 
/***************************************************************************************************
 Main Function
***************************************************************************************************/
int main(int argc,char **argv)
{
double time_spent = 0.0;
clock_t begin = clock();
FILE *Isc,*Pat,*Res;                  //File pointers used for .isc, .pattern, and .res files
int Npi,Npo,last_node_id,last_patt_id;                     //Tot no of PIs,Pos,Maxid,Tot no of patterns in.vec,.fau
GATE *Node;                           //Structure to store the ckt given in .isc file 
clock_t Start,End;                    //Clock variables to calculate the Cputime
double Cpu;                           //Total cpu time
int i,j, g, f, faliure_count =0;                              //Temporary variables
PATTERN *Pattern;
TWO_INT gf;
/****************PART 1.-Read the .isc file and store the information in Node structure***********/
Npi=Npo=last_node_id=0 ;                                //Intialize values of all variables
Isc=fopen(argv[1],"r");                        //File pointer to open .isc file 
Node=(GATE *) malloc(Mnod * sizeof(GATE));     //Dynamic memory allocation for Node structure
last_node_id=ReadIsc(Isc,Node);                        //Read .isc file and return index of last node
fclose(Isc);                                   //Close file pointer for .isc file
//PrintGats(Node,last_node_id);                        //Print the information of each active gate in Node structure after reading .isc file
//printf("1   Hello World\n");
CountPri(Node,last_node_id,&Npi,&Npo);                 //Count the No of Pis and Pos
//printf("\n\nNpi: %d Npo: %d\n",Npi,Npo);       //Print the no of primary inputs and outputs

/**********************************Read the pattern file*********************************************/

// Pat = fopen(argv[2],"r");
// Pattern = (PATTERN *)malloc(Mpt * sizeof(PATTERN));   ////////////////////////////
// last_patt_id = ReadPat(Pat, Pattern, Npi);
// fclose(Pat);
// //PrintPattern( last_patt_id,  Pattern,  Npi);
// /**********************************Logic Simulation*********************************************/
// Res = fopen(argv[3], "w");
// LogicSimulation(Node, last_node_id, Pattern,last_patt_id, Res );
// fclose(Res);
// //PrintGats(Node,last_node_id);
// printf("\n\nNpi: %d Npo: %d\n",Npi,Npo);
/****************************************mpiake***********************************************************/
// g = 156 ; f = 1;
// gf.first = g; gf.second =f;
// PODEM (Node, gf, last_node_id);
for (i = 11; i<=last_node_id; i++){
	if(Node[i].Type != 0){
		gf.first = i;

		gf.second = 0;
		printf("%d /%d:\t", gf.first, gf.second);
		if(!PODEM (Node, gf, last_node_id))
			faliure_count++;
		gf.second = 1;
		printf("%d /%d:\t", gf.first, gf.second);
		if(!PODEM (Node, gf, last_node_id))
			faliure_count++;
	}
}
printf("No of of Failures = %d\n", faliure_count);
int coverage;
coverage = (2*last_node_id - faliure_count)*100/(2*last_node_id);
printf("Coverage = %d %%\n", coverage);




/***************************************************************************************************/

ClearGat(Node,last_node_id);  free(Node);                                      //Clear memeory for all members of Node
clock_t end = clock();
 
// calculate elapsed time by finding difference (end - begin) and
// dividing the difference by CLOCKS_PER_SEC to convert to seconds
time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
printf("Time = %f\n", time_spent);
getchar();	
return 0;
}//end of main
/****************************************************************************************************************************/

