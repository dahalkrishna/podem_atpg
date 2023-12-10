#include "input.h"

// USER DEFINED CONSTANTS

//state of the PODEM algorithm
int state = neutral; 
clock_t podem_start = 0, podem_run = 0;
LIST *Dfrontier = NULL;

// Lookup tables for the logical gates
//					 0	1	X	D	Db
int ANDG [5][5] ={	{0,	0,	0,	0,	0 },		//0
					{0,	1,	X,	D,	Db},		//1
					{0,	X,	X,	X,	X },		//X
					{0,	D,	X,	D,	0 },		//D
					{0,	Db,	X,	0,	Db} };		//Db
					
//					 0	1	X	D	Db
int ORG [5][5] = {	{0,	1,	X,	D,	Db},		//0
					{1,	1,	1,	1,	1 },		//1
					{X,	1,	X,	X,	X },		//X
					{D, 1,	X,	D,	1 },		//D
					{Db,1,	X,	1,	Db} };		//Db
//					 0	1	X	D	Db					
int NOTG [5]   = {   1, 0,  X,  Db, D   };


//Read the pattern file and count the total number of patterns
int ReadPat(FILE *Pat, PATTERN *Pattern, int Npi){
int id = 0, i, last_patt_id;
char content[Mlin];
while (fgets(content, Mlin, Pat)){
	for(i=0; i<Npi; i++){
		if(content[i]=='1')
			Pattern[id].pattern[i] = 1;  
		else if(content[i]=='0')
			Pattern[id].pattern[i] = 0;
		else if(content[i]=='x' || content[i]=='X')
			Pattern[id].pattern[i] = X;
	}
	id++;
}
last_patt_id = id;
return last_patt_id;
}

void PrintPattern(int last_patt_id,PATTERN *Pattern, int Npi){
	int i =0, j = 0;
	for (i=0;i<last_patt_id;i++){
		printf("Pattern %d: ", i);
		for (j=0; j< Npi; j++)
			printf("%d", Pattern[i].pattern[j]);
		printf("\n");
	}
}


void LogicSimulation(GATE *Node, int last_node_id, PATTERN *Pattern, int last_patt_id, FILE *Res){
	int i, pat_id, p, fin_id;
	LIST *temp;
	if (last_patt_id >100)
		last_patt_id = 100;
	//For each pattern go through the entire circuit
	for (pat_id = 0; pat_id<last_patt_id; pat_id++){	// loop to go through all the pattern
		p = 0;
		for (i=1; i<=last_node_id; i++){		 // loop to go through all the nodes
			if (Node[i].Type != 0){
				//if the node is INPT assign the pattern to the value of input node
				if (Node[i].Type == INPT){
					Node[i].Val = Pattern[pat_id].pattern[p];
					p++;
				}
				
				else if (Node[i].Type == FROM || Node[i].Type == BUFF){
					temp = Node[i].Fin;
					Node[i].Val = Node[temp->Id].Val;
				}
				
				else if (Node[i].Type == NOT){
					temp = Node[i].Fin;
					Node[i].Val = NOTG[Node[temp->Id].Val];
				}
				
				else if (Node[i].Type == AND){
					temp = Node[i].Fin;
					fin_id = temp->Id;
					Node[i].Val = Node[fin_id].Val;
					temp = temp->Next;
					while (temp != NULL){
						fin_id = temp->Id;
						Node[i].Val = ANDG[Node[i].Val][Node[fin_id].Val];
						temp = temp->Next;
					}
				}
				
				else if (Node[i].Type == OR){
					temp = Node[i].Fin;
					fin_id = temp->Id;
					Node[i].Val = Node[fin_id].Val;
					temp = temp->Next;
					while (temp != NULL){
						fin_id = temp->Id;
						Node[i].Val = ORG[Node[i].Val][Node[fin_id].Val];
						temp = temp->Next;
					}
				}
				
				else if (Node[i].Type == NAND){
					temp = Node[i].Fin;
					fin_id = temp->Id;
					Node[i].Val = Node[fin_id].Val;
					temp = temp->Next;
					while (temp != NULL){
						fin_id = temp->Id;
						Node[i].Val = ANDG[Node[i].Val][Node[fin_id].Val];
						temp = temp->Next;
					}
					Node[i].Val = NOTG[Node[i].Val];
				}
				
				else if (Node[i].Type == NOR){
					temp = Node[i].Fin;
					fin_id = temp->Id;
					Node[i].Val = Node[fin_id].Val;
					temp = temp->Next;
					while (temp != NULL){
						fin_id = temp->Id;
						Node[i].Val = ORG[Node[i].Val][Node[fin_id].Val];
						temp = temp->Next;
					}
					Node[i].Val = NOTG[Node[i].Val];
				} 
			}
			
			if(Node[i].Type != 0 && Node[i].Nfo == 0){
				if (Node[i].Val == 0)
					fputc('0', Res);
				else if (Node[i].Val == 1)
					fputc('1', Res);
				else if (Node[i].Val == X)
					fputc('x', Res);
				else 
					fputc('e', Res);
			}
		}
		fputc('\n', Res);
	}
}

void Print_PI(GATE *Node, int last_node_id){
	// int i;
	// for (i =1; i<= last_node_id; i++){
	// 	if (Node[i].Type == INPT){
	// 			if ((Node[i].Val) == 0)
	// 				printf("0");
	// 			else if ((Node[i].Val) == 1)
	// 				printf("1");
	// 			else 
	// 				printf("X");
	// 	}
	// }
	// printf("\n");
	//printf("Success\n");
}


int PODEM (GATE *Node, TWO_INT gf, int last_node_id, FILE *Res){
	int result, flag;
	state = neutral;
	initGatesToX(Node, last_node_id);
	fprintf(Res,"%d /%d\t",gf.first, gf.second);
	podem_start = clock();
	result = PODEM_Recursion(Node, last_node_id, gf);
	if (result == success){
		// Print_PI(Node, last_node_id);
		fprintf(Res,"Success\n");
		flag = success;
		}
	else if (result == failure){
		flag = failure;
		fprintf(Res,"Failure\n");
	}
	else if (result == timeout){
		flag = timeout;
		fprintf(Res,"Timeout\n");
	}
	FreeList(&Dfrontier);
	return flag;
}

int PODEM_Recursion(GATE *Node, int last_node_id, TWO_INT gf){
	double time_spent = 0.0;
	podem_run = clock();
	time_spent += (float)(podem_run - podem_start) / CLOCKS_PER_SEC;
	if ( time_spent > 0.2){
		state = timeout;
	}
	int i, val= gf.second;
	TWO_INT iu, gv;
	int result = neutral;
	if (state == success || state == timeout || state == failure)
	{
		return state; 
	}
	gv = getObjective(Node, gf, val);
	iu = backtrace(Node, gv);
	//printf("f_site %d f_val %d g %d,v %d pi %d u %d\n",gf.first, Node[gf.first].Val,gv.first, gv.second, iu.first, iu.second);
	result = logicSimulate_imply(iu, Node, gf, last_node_id);
	if (result == neutral){
		result = PODEM_Recursion(Node, last_node_id, gf);
	}
	if (result == success || result == timeout)
	{
		state = result;
		return state; 
	}
	iu.second = NOTG[iu.second];
	result = logicSimulate_imply(iu, Node, gf, last_node_id);
	if (result == neutral){
		result = PODEM_Recursion(Node, last_node_id, gf);
	}
	if (result == success || result == timeout){
		state = result;
		return state; 
	}
	iu.second = X;
	initGatesToX(Node, last_node_id);
	return failure;
}


TWO_INT getObjective(GATE *Node, TWO_INT gf, int val){
	TWO_INT gv;
	LIST *temp;
	int fault_excitation = 0;
	gv.first = gf.first;
	if (Node[gv.first].Val == D || Node[gv.first].Val == Db){
		fault_excitation = 1;
	}
	if (!fault_excitation){
		gv.second = NOTG[val];
		return gv;
	}
	int i, d;
	if(Dfrontier == NULL){
		printf("Dfrontier is null\n");
		exit(0);
	}
	d = Dfrontier->Id;
	temp = Node[d].Fin;
	while (temp != NULL){
		if (Node[temp->Id].Val == X){
			gv.first = temp->Id;
			break;
		}
		temp = temp->Next;
	}
	gv.second = getNonControlling(Node[d].Type);
	//printf("Exiting getObjective recursion = %d\n", recursion_level);
	return gv;
}


int getNonControlling(int Type){
	if(Type == OR || Type == NOR)
		return 0;
	else if(Type == NAND || Type == AND)
		return 1;
	else if (Type ==XOR || Type == XNOR)
		return 0;
	else{ 
		printf("Error in getNonControlling Value as Type = %d is undefined", Type);
		exit(0);
	}
}


TWO_INT backtrace(GATE *Node, TWO_INT gv){ 		//g input of a gate in D frontier whose value is X	//v = noncontrolling value of d
	int i, num_inversion = 0, var, flag = 0;
	LIST *temp;
	TWO_INT iv;
	i = gv.first;
	if (Node[i].Type == NAND || Node[i].Type == NOR || Node[i].Type == NOT)
				num_inversion++;
	while (Node[i].Type != INPT){
		temp = Node[i].Fin;
		while (temp != NULL){
			if (Node[temp->Id].Val == X && flag == 0){
				flag = 1;
				i = temp->Id;
				if (Node[i].Type == NAND || Node[i].Type == NOR || Node[i].Type == NOT){
					num_inversion++;
				}
			}
			temp = temp->Next;
		}
		flag = 0;
	}
	if (num_inversion % 2 != 0)
		gv.second = NOTG[gv.second];
	
	iv.first = i;
	iv.second = gv.second;
	return iv;
}

void initGatesToX(GATE *Node, int last_node_id){
	int i;
	for (i =0; i<=last_node_id; i++){
		if(Node[i].Type != 0)
			Node[i].Val = X;
	}
}

int logicSimulate_imply(TWO_INT iu, GATE *Node, TWO_INT gf, int last_node_id){
	//printf("Inside logicSimulate_imply recursion = %d\n", recursion_level);
	LIST *temp;
	int id, val, fin_id, is_Dfrontier, i, local_state = neutral;
	id = iu.first; val = iu.second;
	Node[iu.first].Val = iu.second;
	FreeList(&Dfrontier);
	for (i=1; i<=last_node_id; i++){		 // loop to go through all the nodes
		if (Node[i].Type != 0){
			if (Node[i].Type == FROM || Node[i].Type == BUFF){
				temp = Node[i].Fin;
				Node[i].Val = Node[temp->Id].Val;
			}
							
			else if (Node[i].Type == NOT){
				temp = Node[i].Fin;
				Node[i].Val = NOTG[Node[temp->Id].Val];
			}
			
			else if (Node[i].Type == AND){
				temp = Node[i].Fin;
				fin_id = temp->Id;
				Node[i].Val = Node[fin_id].Val;
				temp = temp->Next;
				while (temp != NULL){
					fin_id = temp->Id;
					Node[i].Val = ANDG[Node[i].Val][Node[fin_id].Val];
					temp = temp->Next;
				}
			}
			
			else if (Node[i].Type == OR){
				temp = Node[i].Fin;
				fin_id = temp->Id;
				Node[i].Val = Node[fin_id].Val;
				temp = temp->Next;
				while (temp != NULL){
					fin_id = temp->Id;
					Node[i].Val = ORG[Node[i].Val][Node[fin_id].Val];
					temp = temp->Next;
				}
			}
			
			else if (Node[i].Type == NAND){
				temp = Node[i].Fin;
				fin_id = temp->Id;
				Node[i].Val = Node[fin_id].Val;
				temp = temp->Next;
				while (temp != NULL){
					fin_id = temp->Id;
					Node[i].Val = ANDG[Node[i].Val][Node[fin_id].Val];
					temp = temp->Next;
				}
				Node[i].Val = NOTG[Node[i].Val];
			}
			
			else if (Node[i].Type == NOR){
				temp = Node[i].Fin;
				fin_id = temp->Id;
				Node[i].Val = Node[fin_id].Val;
				temp = temp->Next;
				while (temp != NULL){
					fin_id = temp->Id;
					Node[i].Val = ORG[Node[i].Val][Node[fin_id].Val];
					temp = temp->Next;
				}
				Node[i].Val = NOTG[Node[i].Val];
			}

			//fault activation
			if(Node[gf.first].Val == 0 && gf.second == 1)
				Node[gf.first].Val = Db;
			else if(Node[gf.first].Val == 1 && gf.second == 0)
				Node[gf.first].Val = D;
							
			if (Node[i].Type !=0 && Node[i].Nfo ==0){
				if(Node[i].Val == D || Node[i].Val == Db){
				return success;
				}
			}
			//Creating Dfrontier
			is_Dfrontier = 0;
			is_Dfrontier = CheckDfrontier(Node, i);
			
			if (is_Dfrontier){
				InsertEle(&Dfrontier, i);
			}
			//printf("Fault site %d  val = %d\n",gf.first, Node[gf.first].Val);
			if (Node[gf.first].Val == 1 || Node[gf.first].Val == 0){
				local_state = failure;
				return failure;
			}
			
			//printf("Working gate = %d\n", i);
			//PrintGats(Node, i); getchar();
		}
		
	}
	if(Dfrontier == NULL && (Node[gf.first].Val == D || Node[gf.first].Val == Db)){ 
		local_state = failure; 
		//printf("Dfrontier is null\n");
	}
	return local_state;	
}

int CheckDfrontier(GATE *Node, int id){
	LIST *temp;
	if(Node[id].Val == X){
		temp = Node[id].Fin;
		while(temp != NULL){
			if (Node[temp->Id].Val == D || Node[temp->Id].Val == Db)
				return 1;
			temp = temp->Next;
		}
	}
	else
		return 0;
}






/**/











/***************************************************************************************************/
