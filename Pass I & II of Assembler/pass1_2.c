#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define max 20
#define ad_max 5
#define is_max 11
#define reg_max 4

struct sym_table
{
	char sym[15];
	int addr;
}s[max];

struct lit_table
{
	char lit[5];
	int addr;
}l[max];

int pool_table[max],sym_count=0,lit_count=0,pool_count=0,token_count=0,lc=-1,i=-1,j,k;
char tok1[max],tok2[max],tok3[max],tok4[max],tok5[max],buffer[80],temp[max];
FILE *fp1,*fp2,*fp;

char reg[4][10] = {"AREG","BREG","CREG","DREG"};
char ad[5][10] = {"START","END","ORIGIN","EQU","LTORG"};
char is[11][10] = {"STOP","ADD","SUB","MULT","MOVER","MOVEM","BC","COMP","READ","PRINT"};

int reg_search(char tok[])
{
	int i;

	for(i=0;i<reg_max;i++)
	{
		if(strcmp(reg[i],tok)==0)
			return i;
	}
	return -1;
}

int ad_search(char tok[])
{
	int i;

	for(i=0;i<ad_max;i++)
	{
		if(strcmp(ad[i],tok)==0)
			return i;
	}
	return -1;
}

int is_search(char tok[])
{
	int i;

	for(i=0;i<is_max;i++)
	{
		if(strcmp(is[i],tok)==0)
			return i;
	}
	return -1;
}

int sym_search(char tok[])
{
	int i;
	for(i=0;i<sym_count;i++)
	{
		if(strcmp(s[i].sym,tok)==0)
			return i;
	}
	return -1;
}

int lit_search(char tok[])
{
	int i;
	for(i=pool_table[pool_count];i<lit_count;i++)
	{
		if(strcmp(l[i].lit,tok)==0)
			return i;
	}
	return -1;
}

void display_sym_table()
{
	int i;
	printf("Symbol\tAddress\n");

	for (i = 0; i < sym_count; i++)
	{
		printf("%s\t%d\n",s[i].sym,s[i].addr );
	}
}

void display_lit_table()
{
	int i;
	printf("Count\tLiteral\tAddress\n");
	for (i = 0; i < lit_count; i++)
	{
		printf("%d\t%s\t%d\n",i,l[i].lit,l[i].addr );
	}
}

void display_pool_table()
{
	int i;
	printf("Pool Index\tPool Base\n");
	for (i = 0; i < pool_count; i++)
	{
		printf("%d\t%d\n",i,pool_table[i] );
	}
}

void print_file(char fn[])
{
	FILE *fp = fopen(fn,"r");

	if(fp==NULL) 
	{
		printf("Cant open file!\n");
		exit(0);
	}

	while(fgets(buffer,180,fp))
	{
		printf("%s",buffer);
	}
	fclose(fp);
}


void onepass()
{
	fp1 = fopen("asm1.txt","r");
	if(fp1==NULL)
	{
		printf("Cant open file 1!\n");
		exit(0);
	}

	fp2 = fopen("output.txt","w");
	if(fp2==NULL)
	{
		printf("Cant open file 2!\n");
		exit(0);
	}

	while(fgets(buffer,80,fp1))
	{
		//printf("line read : %s\n",buffer);
		
		strcpy(tok1," ");
		strcpy(tok2," ");
		strcpy(tok3," ");
		strcpy(tok4," ");

		token_count = sscanf(buffer,"%s %s %s %s",tok1,tok2,tok3,tok4);

		//printf("token found: %s, %s, %s, %s,\n\n",tok1,tok2,tok3,tok4);

		switch(token_count)
		{
			case 1: //START,STOP,LTORG,END

					//STOP is an IS
					// Rest START, LTORG and END are AD
					
					i = is_search(tok1);
					if(i==0) 				//STOP is at location 0 in the IS array
					{
						fprintf(fp2,"%d (IS,%02d)\n",lc,i);
						break;
					}

					i = ad_search(tok1);
					if(i==0)				//START
					{
						fprintf(fp2, "%d (AD,%02d)\n",lc,i);
						lc = -1;
						break;
					}

					if(i==1 || i==4) 		//LTORG,END
					{
						fprintf(fp2,"%d (AD,%02d)\n",lc,i);

						if(k!=lit_count)		// assigning addresses to the literals if LTORG and END is encountered
						{
							for (k = pool_table[pool_count]; k < lit_count; k++)
							{
								l[k].addr = lc++;
							}

							pool_table[++pool_count] = lit_count;
							lc--;
						}
					}
					
					if(i==-1)
					{
						printf("\nUndefined symbol %s @ lc:%d\n",tok1,lc);
					}

					else 
						i=-1;
					
					break;

			case 2: //START,ORIGIN,PRINT,READ

					i = ad_search(tok1);
					if(i==0 || i==2) 			//START,ORIGIN
					{
						lc = atoi(tok2)-1;
						fprintf(fp2, "%d (AD,%02d) (C,%02d)\n",lc,i,lc+1);
						break;
					}

					i = is_search(tok1);
					if(i==8 || i==9) 			//READ,PRINT
					{
						j = sym_search(tok2);
						if(j==-1)
						{
							fprintf(fp2, "%d (IS,%02d) (S,%02d)\n",lc,i,sym_count);
							strcpy(s[sym_count++].sym,tok2);
							break;
						}

						else
						{
							fprintf(fp2,"%d (IS,%02d) (S,%02d)\n",lc,i,j);
						}
					}
					
					if(i==-1)
					{
						printf("\nundefined symbol %s @ lc:%d\n",tok1,lc);
					}

					else 
						i=-1;
					
					break;

			case 3: //ADD,DIV
					i = is_search(tok1);

					if(i>=1 && i<=9)
					{
						if(tok3[0]=='=')						// searches for literal
						{
							j = lit_search(tok3);
							
							if(j==-1)							// indicates that literal is not there
							{
								sprintf(temp,"(L,%02d)",lit_count);
								strcpy(l[lit_count++].lit,tok3);
							}

							else 								// literal is there - store in a temp array
							{
								sprintf(temp,"(L,%02d)",j);
							}
						}
						
						else
						{
							j = sym_search(tok3);

							if(j==-1)								// indicates that symbol is not there
							{
								sprintf(temp,"(S,%02d)",sym_count);
								strcpy(s[sym_count++].sym,tok3);
							}

							else
							{
								sprintf(temp,"(S,%02d)",j);
							}
						}

						fprintf(fp2, "%d (IS,%02d) %d %s\n",lc,i,j,temp);
						break;
					}

					//DC
					if(strcmp(tok2,"DC")==0)
					{
						j = sym_search(tok1);
						
						if(j==-1)
						{
							strcpy(s[sym_count].sym,tok1);
							s[sym_count++].addr = lc;
						}
						
						else
						{
							s[j].addr = lc;
						}

						fprintf(fp2, "%d (DL,00) (C,%02d)\n",lc,atoi(tok3));
						break;
					}


					//DS
					if(strcmp(tok2,"DS")==0)
					{
						j = sym_search(tok1);
						
						if(j==-1)
						{
							strcpy(s[sym_count].sym,tok1);
							s[sym_count++].addr=lc;
						}
						
						else 
						{
							s[j].addr=lc;
						}

						lc = lc+atoi(tok3);							// adds the memory locations 

						fprintf(fp2,"%d (DL,01) (C,%02d)\n",lc,atoi(tok3));
						break;
					}

					//EQU
					j = ad_search(tok2);
					if(j==3)
					{
						i = sym_search(tok1);
						k = sym_search(tok3);
						
						if(i==-1)
						{
							strcpy(s[sym_count].sym,tok1);
							s[sym_count++].addr = s[k].addr;
						}
						
						else
						{
							s[i].addr = s[k].addr;
						}
						
						fprintf(fp2,"%d (AD,%02d)\n",lc,j);
						lc--;
						break;
					}
					
					if(i==-1)
					{
						printf("\n Undefined symbol %s @ lc:%d\n",tok2,lc);
					}
					else i=-1;
					
					break;
					
			case 4: j = sym_search(tok1); //label
					
					if(j==-1)
					{
						strcpy(s[sym_count].sym,tok1);
						s[sym_count++].addr = lc;
					}

					else
					{
						s[j].addr = lc;
					}

					i = is_search(tok2);

					if(i>=1 && i<=9)
					{
						if(tok4[0]=='=')
						{
							j = lit_search(tok4);
							
							if(j==-1)
							{
								sprintf(temp,"(L,%02d)",lit_count);
								strcpy(l[lit_count++].lit,tok4);
							}

							else 
							{
								sprintf(temp,"(L,%02d)",j);
							}
						}
						
						else
						{
							j = sym_search(tok4);
							
							if(j==-1)
							{
								sprintf(temp,"(S,%02d)",sym_count);
								strcpy(s[sym_count++].sym,tok4);
							}

							else
							{
								sprintf(temp,"(S,%02d)",j);
							}
						}

						fprintf(fp2, "%d (IS,%02d) %d %s\n",lc,i,j,temp);
						break;
					}

					if(i==-1)
					{
						printf("\nUndefined symbol %s @ lc:%d\n",tok2,lc);
					}
					else i=-1;
					break;
					
		}
		lc++;
	}
	fclose(fp1);
	fclose(fp2);
	
	int x;

	for(x=0;x<sym_count;x++)
	{
		if(s[x].addr==0)
			printf("\nSymbol %s used but not declared!\n",s[x].sym);
	}
	
}

void pass2()
{
	char opc[3];
	char index[5];
	int indexi;
	int i=0,j=0;

	fp1 = fopen("output.txt","r");
	if(fp1==NULL)
	{
		printf("Cant open IC file!\n");
		exit(0);
	}
		
	fp2 = fopen("pass2.txt","w");
	if(fp2==NULL)
	{
		printf("Cant open pass2 file!\n");
		exit(0);
	}
	
	while(fgets(buffer,80,fp1))
	{
		//printf("line read : %s\n",buffer);			
		strcpy(tok1," ");
		strcpy(tok2," ");
		strcpy(tok3," ");
		strcpy(tok4," ");
		
		token_count = sscanf(buffer,"%s %s %s %s",tok1,tok2,tok3,tok4);

		//printf("token found: %s, %s, %s, %s,\n\n",tok1,tok2,tok3,tok4);
		
		switch(token_count)
		{
			case 2: // START,STOP

					opc[0]=tok2[4];
					opc[1]=tok2[5];
					opc[2]='\0';
					
					fprintf(fp2,"+%s %s _ _ \n",tok1,opc);
					break;
					
			case 3: opc[0]=tok2[4];
					opc[1]=tok2[5];
					opc[2]='\0';
					
					for(i=3;tok3[i]!=')';i++)
					{
						index[j]=tok3[i];				// (C,02) : storing 02 in index
						j++;
					}

					index[j]='\0';
					j=0;
					
					indexi = atoi(index) ;				// converting string 02 to integer
					
					if(tok3[1]=='S')
					{
						fprintf(fp2,"+%s %s _ %d\n",tok1,opc,s[indexi].addr);
					}
					
					if(tok3[1]=='L')
					{
						fprintf(fp2,"+%s %s _ %d\n",tok1,opc,l[indexi].addr);
					}
					
					if(tok3[1]=='C')
					{
						fprintf(fp2,"+%s %s _ %d\n",tok1,opc,indexi);
					}
					break;
					
			case 4: opc[0]=tok2[4];
					opc[1]=tok2[5];
					opc[2]='\0';
					
					for(i=3;tok4[i]!=')';i++)
					{
						index[j]=tok4[i];
						j++;
					}
					
					index[j]='\0';
					j=0;
					
					indexi = atoi(index);
					
					if(tok4[1]=='S')
					{
						fprintf(fp2,"+%s %s %s %d\n",tok1,opc,tok3,s[indexi].addr);
					}
					
					if(tok4[1]=='L')
					{
						fprintf(fp2,"+%s %s %s %d\n",tok1,opc,tok3,l[indexi].addr);
					}
					
					if(tok4[1]=='C')
					{
						fprintf(fp2,"+%s %s %s %d\n",tok1,opc,tok3,indexi);
					}
					break;
		}
	}
	fclose(fp1);
	fclose(fp2);
}

int main(int argc, char const *argv[])
{
	print_file("asm1.txt");
	printf("\n");
	onepass();
	printf("\n");
	display_sym_table();
	printf("\n");
	display_lit_table();
	printf("\n");
	display_pool_table();
	printf("\n");
	print_file("output.txt");
	printf("\n");
	pass2();
	printf("\n");
	print_file("pass2.txt");
	
	return 0;
}
