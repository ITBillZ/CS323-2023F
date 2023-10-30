%option yylineno
%{
    #include "syntax.tab.h"
    #include "stdlib.h"
%}
%%
[0-9]+ { yylval = atoi(yytext); return INT; }
"+" { return ADD; }
"-" { return SUB; }
"*" { return MUL; }
"/" { return DIV; }
[ \r\t] {}
\n {}
. { fprintf(stderr, "unknown symbol: %s\n", yytext); exit(1); }
