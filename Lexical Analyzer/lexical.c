#include<stdio.h>
#include<string.h>
#include<stdlib.h>

FILE *ifp,*ofp;
char buffer[80];
int flag=0;
int strflag=0;

typedef struct literal
{
	char name[10];
}literal;

literal lit[50];
int litcnt=0;

int addlit(char temp[])
{
	int i;

	for(i=0;i<litcnt;i++)
	{
		if(!strcmp(temp,lit[i].name))			// if literal does not exist already ?
			return i;		
	}
	
	strcpy(lit[litcnt].name,temp);
	litcnt++;
	return (litcnt-1);
}

typedef struct identifier
{
	char name[10];
}identifier;

identifier id[50];
int idcnt=0;

int addid(char temp[])
{
	int i;

	for(i=0;i<idcnt;i++)
	{
		if(!strcmp(temp,id[i].name))
		return i;		
	}
	
	strcpy(id[idcnt].name,temp);
	idcnt++;
	return (idcnt-1);
}

int isdelimiter(char ch)
{
    if (ch == ' ') return 1;
    if (ch == '+') return 2;
    if (ch == '-') return 3;
    if (ch == '*') return 4;
    if (ch == '/') return 5;
    if (ch == ',') return 6;
    if (ch == ';') return 7;
    if (ch == '>') return 8;
    if (ch == '<') return 9;
    if (ch == '=') return 10;
    if (ch == '(') return 11; 
    if (ch == ')') return 12;
    if (ch == '[') return 13;
    if (ch == ']') return 14;
    if (ch == '{') return 15;
    if (ch == '?') return 16;
    if (ch == ':') return 17;
    if (ch == '}') return 18;
    
    return (0);
}
 
int isarithoperator(char ch)
{
    if (ch == '+') return 2;
    if (ch == '-') return 3;
    if (ch == '*') return 4;
    if (ch == '/') return 5;
    
    return (0);
}

int iscomparithoperator(char ch, char ch1)
{
    if (ch == '+' && ch1 == '+') return 45;
    if (ch == '-' && ch1 == '-') return 46;

    return (0);
}

int iscompoundop(char ch, char ch1)
{
    if (ch == '+' && ch1 == '=') return 45;
    if (ch == '-' && ch1 == '=') return 46;
    
    return (0);
}

int isreloperator(char ch)
{
    if (ch == '>') return 8;
    if (ch == '<') return 9;
       
    return (0);
}

int isassignoperator(char ch)
{
    if (ch == '=')
        return 10;

    return (0);
}

int iscomprelop(char ch,char ch1)
{
    if (ch == '=' && ch1 == '=') return 42;
    if (ch == '<' && ch1 == '=') return 43;
    if (ch == '>' && ch1 == '=') return 44;
    
    return (0);
}

int valididentifier(char* str)
{
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' || 
        str[0] == '6' || str[0] == '7' || str[0] == '8' || 
        str[0] == '9' || str[0] == '\t'|| str[0] == '\n' || 
        str[0] == ' ' || isdelimiter(str[0]) == 1)
        return (0);
    return (1);
}
 
int iskeyword(char* str)
{
    if (!strcmp(str, "if")) return 19;
    if (!strcmp(str, "else")) return 20;
    if (!strcmp(str, "while")) return 21;
    if (!strcmp(str, "do")) return 22;
    if (!strcmp(str, "break")) return 23;
    if (!strcmp(str, "continue")) return 24; 
    if (!strcmp(str, "int")) return 25;
    if (!strcmp(str, "double")) return 26;
    if (!strcmp(str, "float")) return 27;
    if (!strcmp(str, "return")) return 28;
    if (!strcmp(str, "char")) return 29;
    if (!strcmp(str, "case")) return 30;
    if (!strcmp(str, "char")) return 31;
    if (!strcmp(str, "sizeof")) return 32;
    if (!strcmp(str, "long")) return 33;
    if (!strcmp(str, "short")) return 34;
    if (!strcmp(str, "typedef")) return 35;
    if (!strcmp(str, "switch")) return 36;
    if (!strcmp(str, "unsigned")) return 37;
    if (!strcmp(str, "void")) return 38;
    if (!strcmp(str, "static")) return 39;
    if (!strcmp(str, "struct")) return 40;
    if (!strcmp(str, "goto")) return 41;
    
       
    return (0);
}

char* substring(char* str,int left, int right)
{
	int i;
	char* substr = (char*)malloc(sizeof(char)* (right - left + 2));

	for(i=left;i<=right;i++)
	{
		substr[i-left]=str[i];
	}
	
 	substr[right-left+1] = '\0';
 	return (substr);
}

void parse(char *str)
{
ofp = fopen("parsed.txt","a");

int left=0,right=0, len = strlen(str);

while(right<=len && left<=right)
{
	if(str[right]=='/' && str[right+1]=='/')
	{
		break;
	}
	 
	if(str[right]=='/' && str[right+1]=='*')
	{
		flag=1;
		right+=2;
		left+=2;
	}
	
	if(str[right]=='*' && str[right+1]=='/')
	{
		flag=0;
		right+=2;
		left+=2;
	}
	
	if(str[right]=='"')
	{
		if(strflag==0)
			strflag=1;
		
		else if(strflag==1)
			strflag=0;
	}
	
	if(flag==1 && isdelimiter(str[right]))
	{
		right++;
		left++;
	}
	
	if(!isdelimiter(str[right]) || strflag==1) 				//moves right pointer until delimiter
		right++;
		
	if(isdelimiter(str[right]) && left==right && flag==0) 				//handles delimiters and operators
	{
		if(isassignoperator(str[right]))
		{
			if(isassignoperator(str[right+1]))
			{
				int x = iscomprelop(str[right],str[right+1]);
				fprintf(ofp,"TRM	%c%c	%d\n",str[right],str[right+1],x);
				right++;
			}

			else
			{
				int x = isassignoperator(str[right]);
				fprintf(ofp,"TRM	%c	%d\n",str[right],x);
			}
		}

		else if(isreloperator(str[right]))
		{
			if(isassignoperator(str[right+1]))
			{
				int x = iscomprelop(str[right],str[right+1]);
				fprintf(ofp,"TRM	%c%c	%d\n",str[right],str[right+1],x);
				right++;
			}

			else
			{
				int x = isreloperator(str[right]);
				fprintf(ofp,"TRM	%c	%d\n",str[right],x);
			}
		}

		else if(isarithoperator(str[right]))
		{
			if((str[right]=='+'&&str[right+1]=='+')||(str[right]=='-'&&str[right+1]=='-'))
			{
				int x = iscomparithoperator(str[right],str[right+1]);
				fprintf(ofp,"TRM	%c%c	%d\n",str[right],str[right+1],x);
				right++;
			}

			else if(isassignoperator(str[right+1]))
			{
				int x = iscompoundop(str[right],str[right+1]);
				fprintf(ofp,"TRM	%c%c	%d\n",str[right],str[right+1],x);
				right++;
			}

			else
			{
				int x = isarithoperator(str[right]);
				fprintf(ofp,"TRM	%c	%d\n",str[right],x);
			}
		}

		else if(str[right]!=' '&&str[right]!='\n'&&str[right]!='\t')
		{
			int x = isdelimiter(str[right]);
			fprintf(ofp,"TRM	%c	%d\n",str[right],x);
		}

		right++;
		left=right;
	}
	
	if(isdelimiter(str[right]) && left!=right && flag==0) 				//handles keywords and identifiers
	{
		char* temp = substring(str,left,right-1);
		
		if(iskeyword(temp))
		{
			int x = iskeyword(temp);
			fprintf(ofp,"TRM	%s	%d\n",temp,x);
		}
		else if(temp[0]=='"'&& temp[strlen(temp)-1]=='"')
		{
			int x = addlit(temp);
			fprintf(ofp,"LIT	%s	%d\n",temp,x);
		}
		else if(valididentifier(temp))
		{
			int x = addid(temp);
			fprintf(ofp,"ID	%s	%d\n",temp,x);
		}
		else if(temp[0]!='\n' && temp[0]!='\t' && temp[0]!=' ')
		{
			int x = addlit(temp);
			fprintf(ofp,"LIT	%s	%d\n",temp,x);
		}
			
		left=right;
	}	
	
}


fclose(ofp);

}

void showlittab()
{
printf("~~~~~~~~LIT TAB~~~~~~~~~\n");
	int i;
	for(i=0;i<litcnt;i++)
	{
		printf("%d %s \n",i,lit[i].name);
	}
}

void showidtab()
{
printf("~~~~~~~~ID TAB~~~~~~~~~\n");
	int i;
	for(i=0;i<idcnt;i++)
	{
		printf("%d %s \n",i,id[i].name);
	}
}


int main(int argc,char* argv[])
{

ifp = fopen(argv[1],"r");

while(fgets(buffer,80,ifp))
{
parse(buffer);
}

showlittab();
showidtab();

fclose(ifp);

return 0;
}

