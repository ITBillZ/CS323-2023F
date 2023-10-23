%{
    #define YYSTYPE char *
    #include "lex.yy.c"
    #include <ctype.h>
    #include <string.h>

    int yyerror(char* s);
    void verify_IPv4(char* s);
    void verify_IPv6(char* s);
%}

%token X
%token DOT
%token COLON

%%
IP: IPv4  {printf("IPv4\n");}
    | IPv6 {printf("IPv6\n");}
    ;
IPv4: T4 DOT T4 DOT T4 DOT T4
    ;
IPv6: T6 COLON T6 COLON T6 COLON T6 COLON T6 COLON T6 COLON T6 COLON T6
    ;
T4: X {verify_IPv4($1); $$ = $1;} 
    ;
T6: X {verify_IPv6($1); $$ = $1;}
    ;
%%

int yyerror(char* s) {
    fprintf(stderr, "%s\n", "Invalid");
    exit(1);
}

void verify_IPv4(char* s) {
    // 是否是数字
    if (!isdigit(*s)) {
        yyerror(NULL);
    }

    if (strlen(s) >= 2 && *s == '0') {
        yyerror(NULL);
    }

    int term = atoi(s);
    if (!(term >= 0 && term <= 255)) {
        yyerror(NULL);
    }
}

void verify_IPv6(char* s) {
    int len = strlen(s);
    if (!(len >= 1 && len <= 4)) {
        yyerror(NULL);
    }

    char* ptr = s;
    char c;
    while (c = *(ptr++)) {
        int is_digit = (c >= '0' && c <= '9');
        int is_hex_letter = (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
        if (!(is_digit || is_hex_letter)) {
            yyerror(NULL);
        }
    }
}

int main() {
    yyparse();
}
