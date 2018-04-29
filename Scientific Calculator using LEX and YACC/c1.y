%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<math.h>	

	#define YYSTYPE double
%}

%token NUM
%token SIN COS TAN LOG

%left '+' '-'
%left '*' '/'

%%

S:	  S E '\n'	{printf("\nAnswer: %g\nEnter: ",$2);}
	| S '\n'	
	| 
	| error '\n'	{yyerror("\nWrong input!\nTry again\n");yyerrok;}

E:	  E '+' T	{$$ = $1 + $3;}
	| E '-' T	{$$ = $1 - $3;}
	| T
	;

T:	  T '*' F	{$$ = $1 * $3;} 
	| T '/' F	{$$ = $1 / $3;}
	| F
	;

F:	  NUM
	| '(' E ')'	{$$ = $2;} 
	| COS '('E')'	{$$ = cos($3);}
	| TAN '('E')'	{$$ = tan($3);}
	| LOG '('E')'	{$$ = log($3);}
	| SIN '('E')'	{$$ = sin($3);}
	;
%%

#include "lex.yy.c"

int main()
{
	printf("\nEnter expression:\n");

	yyparse();
}
