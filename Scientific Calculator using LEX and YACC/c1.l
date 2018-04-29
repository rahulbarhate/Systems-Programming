%{
#include<math.h>
%}

DIGIT [0-9]+\.?|[0-9]*\.[0-9]+

%%

{DIGIT} {yylval=atof(yytext); return NUM;}
cos|COS {return COS;}
sin|SIN {return SIN;}
tan|TAN {return TAN;}
log|LOG {return LOG;}
\n|.	{return yytext[0];}
