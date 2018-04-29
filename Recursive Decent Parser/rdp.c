#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char input[10];
int i,error;
void E();
void T();
void EPrime();
void TPrime();
void F();

int main()
{
	i=0; error=0;
	printf("Enter Arithmetic Expression :\n");
	gets(input);
	puts(input);
	E();
	if(strlen(input)==i && error==0)
		printf("\tAccepted !!\n");
	else
		printf("\t## Rejected\n");
	return 0;
}

void E()
{
	T();
	EPrime();
}

void T()
{
	F();
	TPrime();
}

void EPrime()
{
	if(input[i]=='+' || input[i]=='-')
	{
		i++;
		T();
		EPrime();
	}
}

void TPrime()
{
	if(input[i]=='*' || input[i]=='/')
	{
		i++;
		F();
		TPrime();
	}
}

void F()
{
	if(isalpha(input[i]))
		i++;
	else if (input[i]=='(')
	{
		i++;
		E();
		if(input[i]==')')
			i++;
		else
			error=1;
	}
	else
		error=1;
}
