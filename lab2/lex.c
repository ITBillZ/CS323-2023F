#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

enum TOKEN {
    TYPE,
    INT,
    ID,
    IF,
    ELSE,
    WHILE,
    RET,
    SEMI,
    ASSIGN,
    LT,
    LE,
    GT,
    GE,
    NE,
    EQ,
    PLUS,
    MINUS,
    LP,
    RP,
    LC,
    RC,
    ERROR
};

const char *tokenStrings[] = {
    "TYPE", "INT", "ID", "IF", "ELSE", "WHILE", "RET", "SEMI", "ASSIGN", "LT",
    "LE",   "GT",  "GE", "NE", "EQ",   "PLUS", "MINUS", "LP",  "RP",   "LC", "RC", "ERROR"};


void retract(char **p, int num) { *p -= num; }

// TODO 把回退方法写成retract！
int judge_reserve(char **p) {
    char c = **p;
    int state = 0;
    int num = 0;
    while (1) {
        switch (state) {
        case 0:
            if (c == 'i')
                state = 1;
            else if (c == 'e')
                state = 5;
            else if (c == 'w')
                state = 9;
            else if (c == 'r')
                state = 14;
            else
                state = -1;
            break;

        case 1:
            if (c == 'n')
                state = 2;
            else if (c == 'f')
                state = 4;
            else
                state = -1; // fail 不是关键字
            break;
        case 2:
            if (c == 't')
                state = 3;
            else
                state = -1;
            break;

        case 3: // TYPE
            return TYPE;

        case 4: // IF
            return IF;

        case 5:
            if (c == 'l')
                state = 6;
            else
                state = -1;
            break;

        case 6:
            if (c == 's')
                state = 7;
            else
                state = -1;
            break;

        case 7:
            if (c == 'e')
                state = 8;
            else
                state = -1;
            break;

        case 8: // ELSE
            return ELSE;

        case 9:
            if (c == 'h')
                state = 10;
            else
                state = -1;
            break;

        case 10:
            if (c == 'i')
                state = 11;
            else
                state = -1;
            break;

        case 11:
            if (c == 'l')
                state = 12;
            else
                state = -1;
            break;

        case 12:
            if (c == 'e')
                state = 13;
            else
                state = -1;
            break;

        case 13: // WHILE
            return WHILE;

        case 14:
            if (c == 'e')
                state = 15;
            else
                state = -1;
            break;

        case 15:
            if (c == 't')
                state = 16;
            else
                state = -1;
            break;

        case 16:
            if (c == 'u')
                state = 17;
            else
                state = -1;
            break;

        case 17:
            if (c == 'r')
                state = 18;
            else
                state = -1;
            break;

        case 18:
            if (c == 'n')
                state = 19;
            else
                state = -1;
            break;

        case 19: // RET
            return RET;

        case -1: // fail to match
            retract(p, num);
            return -1;
        }
        *p += 1; // 向后移动
        num += 1;
        c = **p;
    }
}

int judge_sign(char **p) {
    char c = **p;
    int state = 0;
    while (1) {
        switch (state) {
        case 0:
            if (c == '<')
                state = 1;
            else if (c == '>')
                state = 4;
            else if (c == '!')
                state = 7;
            else if (c == '=')
                state = 9;

            // single char sign
            else if (c == ';')
                return SEMI;
            else if (c == '+')
                return PLUS;
            else if (c == '-') {
                // 看看下一个是不是数字
                if (!isdigit(*((*p)+1))) {
                    // 不是数字，那这个 - 就是减号
                    return MINUS;
                } else {
                    // 是数字，那就回退，放到number里面去判断
                    // retract(p, 1);
                    return -1;
                }
            }
            else if (c == '(')
                return LP;
            else if (c == ')')
                return RP;
            else if (c == '{')
                return LC;
            else if (c == '}')
                return RC;

            // TODO
            else
                return -1;
            break;

        case 1:
            if (c == '=')
                state = 2;
            else
                state = 3;
            break;

        case 2: // LE
            return LE;

        case 3: // LT
            retract(p, 1);
            return LT;

        case 4:
            if (c == '=')
                state = 5;
            else
                state = 6;
            break;

        case 5:
            return GE;

        case 6:
            retract(p, 1);
            return GT;

        case 7:
            if (c == '=')
                state = 8;
            break;

        case 8:
            return NE;

        case 9:
            if (c == '=')
                state = 10;
            else
                state = 11;
            break;

        case 10: // ==
            return EQ;

        case 11: // =
            retract(p, 1);
            return ASSIGN;
        }
        *p += 1;
        c = **p;
    }
}

int judge_blank(char** p) {
    char c = **p;
    if (isspace(c)) return 1;
    else return -1;
}

int judge_number(char** p) {
    char c = **p;
    if (c == '-' || isdigit(c)) {
        while (1) {
            *p += 1;
            c = **p;
            if (isdigit(c)) {
                continue;
            } else if (isalpha(c)) {
                return ERROR;
            } else {
                retract(p, 1);
                return INT;
            }
        }
    }
    return -1;
}

int judge_identifier(char** p) {
    char c = **p;
    if (isalpha(c) || c == '$' || c == '_') {
        while (1) {
            *p += 1;
            c = **p;
            if (c == '$' || c == '_' || isalnum(c)) {
                continue;
            } else {
                // retract(p, 1);
                return ID;
            }
        }
    } 
    return -1;
}

int lexical_analyzer(char **p) {
    // 顺序：空白 -> 保留字 -> 符号
    int token;
    if ((token = judge_blank(p)) != -1) {
        *p += 1;

    } else if ((token = judge_reserve(p)) != -1) {
        *p += 1;
        printf("%s ", tokenStrings[token]);

    } else if ((token = judge_sign(p)) != -1) {
        *p += 1; // 匹配成功了就向下跳一个字符
        printf("%s ", tokenStrings[token]);

    } else {
        
        token = judge_number(p);
        if (token == ERROR) {
            printf("ERROR ");
            return ERROR;

        } else if (token != -1) {
            *p += 1;
            printf("INT ");
        } else {
            token = judge_identifier(p);
            if (token != -1) {
                printf("ID ");
            }
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    FILE *file_pointer;
    long file_size;
    char *file_content;

    // 打开文件
    char* filename = argv[1];
    file_pointer = fopen(filename, "r");
    if (file_pointer == NULL) {
        printf("无法打开文件\n");
        return 1;
    }

    // 获取文件大小
    fseek(file_pointer, 0, SEEK_END);
    file_size = ftell(file_pointer);
    fseek(file_pointer, 0, SEEK_SET);

    // 分配内存来存储文件内容
    file_content = (char *)malloc(file_size + 1); // +1 用于存储字符串结束符

    if (file_content == NULL) {
        printf("内存分配失败\n");
        fclose(file_pointer);
        return 1;
    }

    // 读取文件内容到变量中
    fread(file_content, 1, file_size, file_pointer);

    // 添加字符串结束符
    file_content[file_size] = '\0';

    // 关闭文件
    fclose(file_pointer);

    char *start = file_content;
    while (*file_content) {
        int res = lexical_analyzer(&file_content);
        if (res == ERROR) break;
    }
    printf("\n");

    // 释放分配的内存
    free(start);

    return 0;
}
