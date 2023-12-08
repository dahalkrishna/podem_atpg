#include "input.h"
/***************************************************************************************************
Insert an element "X" at end of LIST "Cur", if "X" is not already in "Cur". 
If the "Cur" is Null, it creates a single element List using "X"
***************************************************************************************************/
void InsertEle(LIST **Cur,int Ele)
{
LIST *tl=NULL;
LIST *nl=NULL;

if ((tl=(LIST *) malloc(sizeof(LIST)))==NULL){ 
  printf("LIST: Out of memory\n");  
  exit(1);  }   
else{
  tl->Id=Ele;  tl->Next=NULL;
  if(*Cur==NULL){  
    *Cur=tl; 
    return;  }
  nl=*Cur; 
  while(nl!=NULL){
    if(nl->Id==Ele){       break;       }
    if(nl->Next==NULL){  nl->Next=tl; } 
    nl=nl->Next; }  
 }
return;
}//end of InsertEle
/***************************************************************************************************
Delete an element "X" from LIST "Cur",
***************************************************************************************************/	
void DeleteEle(LIST **Cur,int Ele)
{
LIST *fir = (*Cur);
LIST *lst = NULL;

if (fir==NULL) return;  
while((fir->Id!=Ele)&&(fir!= NULL)){
  lst=fir;
  fir=fir->Next; }   
if(fir!=NULL){
   if(lst==NULL){  (*Cur)=(*Cur)->Next;       }
   else{           lst->Next = fir->Next; }
   free(fir); }
else{ 
   return; }
return;
}//end of DeleteEle
/***************************************************************************************************************************
Return 1 if the element "x" is present in LIST "Cur"; Otherwise return  0  
*****************************************************************************************************************************/
int FindEle(LIST *Cur,int Ele)
{
LIST *temp=NULL;
	
temp=Cur;
while(temp!=NULL){
  if(temp->Id==Ele) return 1;
  temp=temp->Next; }
return 0;
}//end of FindEle
/***************************************************************************************************
Print the elements in LIST "Cur"          
***************************************************************************************************/
void PrintList(LIST *Cur)
{
LIST *tmp=Cur;
printf("aaaaa");
while(tmp!=NULL){   
  printf("%d  ", tmp->Id);
  tmp = tmp->Next; } 
return;
}//end of PrintList
/***************************************************************************************************
Count the total number  of elements in LIST "Cur"          
***************************************************************************************************/
int CountList(LIST *Cur)
{
LIST *tmp=Cur;
int size=0;

while(tmp!=NULL){   
  size++;
  tmp = tmp->Next; } 
return size;
}//end of PrintList
/***************************************************************************************************
Free all elements in  LIST "Cur"  
***************************************************************************************************/
void FreeList(LIST **Cur)
{
LIST *tmp=NULL;

if(*Cur==NULL){   return;  }
tmp=(*Cur);
while((*Cur) != NULL){
  tmp=tmp->Next;
  free(*Cur); 
  (*Cur)=tmp; }   
(*Cur)=NULL;
return;
}//end of FreeList 
/***************************************************************************************************
Initialize the paricular member of GATE structure
***************************************************************************************************/        
void InitiGat(GATE *Node,int Num)
{
Node[Num].Name=(char *) malloc(Mnam *sizeof(char));           //Dynamic memory allocation for an array
bzero(Node[Num].Name,Mnam);                                   //Clearing the string
Node[Num].Type=Node[Num].Nfi=Node[Num].Nfo=Node[Num].Mark=0;
Node[Num].Val=6;  
Node[Num].Fin=Node[Num].Fot=NULL;
//Node[Num].Rpath=Node[Num].Fpath=NULL;   
return;
}//end of InitiGat
/***************************************************************************************************
Print all contents(attribute) of all active member of GATE structure(DdNodes are not printed)
***************************************************************************************************/
void PrintGats(GATE *Node,int Tgat)
{
int i;

printf("\nId\tName\tType\t#In\t#Out\tMark\tValue\t\tFanin\t\tFanout");
for(i=1;i<=Tgat;i++){
  if(Node[i].Type!=0){
    printf("\n%d\t%s\t%d\t%d\t%d\t%d\t%d\t\t",i,Node[i].Name,Node[i].Type,Node[i].Nfi,Node[i].Nfo,Node[i].Mark,Node[i].Val);
    PrintList(Node[i].Fin);  printf("\t\t");
    PrintList(Node[i].Fot); }}
printf("\n");
return;
}//end of PrintGats
/***************************************************************************************************
Free the memory of all contents of all members of GATE structure(DdNodes are already cleared)
***************************************************************************************************/
void ClearGat(GATE *Node,int Tgat)
{
int i;

for(i=1;i<=Tgat;i++){ 
  free(Node[i].Name);
  Node[i].Type=Node[i].Nfi=Node[i].Nfo=Node[i].Mark=Node[i].Val=0;
  FreeList(&Node[i].Fin);  FreeList(&Node[i].Fot); }
return;
}//end of ClearGat
/***************************************************************************************************
Count the Total Number of Primary inputs and outputs
***************************************************************************************************/
void CountPri(GATE *Node,int Tgat,int *Npi,int *Npo)
{
int i,j,k;

i=j=k=0;
for(i=1;i<=Tgat;i++){ 
  if(Node[i].Type!=0){
    if(Node[i].Nfi==0){      j++; }
    if(Node[i].Nfo==0){      k++; } }}
*Npi=j; *Npo=k;
return;
}//end of CountPri
/***************************************************************************************************
Convert (char *) type read to (int)     
***************************************************************************************************/
int AssignType(char *Gtyp)
{
if      ((strcmp(Gtyp,"inpt")==0) || (strcmp(Gtyp,"INPT")==0))       return 1;
else if ((strcmp(Gtyp,"from")==0) || (strcmp(Gtyp,"FROM")==0))       return 2;
else if ((strcmp(Gtyp,"buff")==0) || (strcmp(Gtyp,"BUFF")==0))       return 3;
else if ((strcmp(Gtyp,"not")==0)  || (strcmp(Gtyp,"NOT")==0))        return 4;
else if ((strcmp(Gtyp,"and")==0)  || (strcmp(Gtyp,"AND")==0))        return 5;
else if ((strcmp(Gtyp,"nand")==0) || (strcmp(Gtyp,"NAND")==0))       return 6;
else if ((strcmp(Gtyp,"or")==0)   || (strcmp(Gtyp,"OR")==0))         return 7;
else if ((strcmp(Gtyp,"nor")==0)  || (strcmp(Gtyp,"NOR")==0))        return 8;
else if ((strcmp(Gtyp,"xor")==0)  || (strcmp(Gtyp,"XOR")==0))        return 9;
else if ((strcmp(Gtyp,"xnor")==0) || (strcmp(Gtyp,"XNOR")==0))       return 10;
else                          			                     return 0;
}//end of AssignType
/***************************************************************************************************
 Function to read the Bench Mark(.isc files)
***************************************************************************************************/
int ReadIsc(FILE *Isc,GATE *Node)
{
  int id, g_type, fin, fout, i, max_node_id;
  LIST *temp;
//INSERT CODE HERE
char content[Mlin],gate_name[Mnam], gate_type[Mnam],fin_of_from_gt[Mnam], *ptr;
for(i = 0; i< Mnod; i++)
	InitiGat(Node,i);
while (fgets(content, Mlin, Isc))
{
	if (content[0] =='*')
		continue;
  	else
	{
  		sscanf(content,"%d %s %s",&id ,gate_name,gate_type);
		g_type = AssignType(gate_type);
		strcpy(Node[id].Name,gate_name);
		if (g_type ==0)
		{
			printf("ERROR: Invalid node type");
			exit(0);
		}
		//the *.isc file has three different types of format based on INPT, FROM and logical gates. 
		else if (g_type == INPT)	//1 is for inpt, see in AssignType function
		{
			sscanf(content, "%d %s %s %d %d",&id ,gate_name,gate_type, &fout , &fin);
			Node[id].Nfi = fin; Node[id].Nfo = fout;
			Node[id].Type = g_type;
		}
		else if (g_type == FROM) //2 is for from node
		{
			sscanf(content, "%d %s %s %s", &id ,gate_name,gate_type, fin_of_from_gt);
			Node[id].Nfi = 1;
			Node[id].Nfo = 1;
			Node[id].Type = g_type;
			
			for (i = 1; i < id; i++)
			{
				if(Node[i].Type !=0)
				{
					if (!strcmp(Node[i].Name , fin_of_from_gt))	//strcmp returns 0 if the two strings are equal.the conditions means if the string are equal then break the loop
						break; //i now has the node id of the gate that is fanin to the FROM gate
				}
			}
			InsertEle(&Node[i].Fot,id);
			InsertEle(&Node[id].Fin,i);
		}
		else if (g_type != 0)
		{
			i = 0;
			sscanf(content, "%d %s %s %d %d", &id ,gate_name,gate_type,&fout, &fin );
			Node[id].Nfi = fin; Node[id].Nfo = fout;
			Node[id].Type = g_type;
			fgets(content, Mlin, Isc);
			ptr = content;
			for (i =0;i<Node[id].Nfi; i++)
			{
				//skip the whitespace and loop to skip all the spaces after the integer
				while(*ptr == ' ')
					ptr++;
				//printf("%d\n", atoi(ptr));
				InsertEle(&Node[id].Fin, atoi(ptr));	//insert node 'atoi(ptr)' in the fanin list of node 'id'
				InsertEle(&Node[atoi(ptr)].Fot, id);	//insert node 'id' in the fanout list of node 'atoi(ptr)'
				while(*ptr !=' ')						//increase the pointer address from "integer" to next white space
					ptr++;						
			}
		}
	}
}
//Return the Maximum node of the Isc file
//since *.isc is topologically sorted, the last id is max_node_id
max_node_id = id;
return max_node_id;
}//end of ReadIsc 
/****************************************************************************************************************************/
