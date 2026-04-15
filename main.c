#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "analexproc.h"


FILE *fd;
TOKEN tk;
int contLinha;
bool mostrArvore;
char TABS[200] = "";

const char* getTokenName(enum TOKEN_CAT cat) {
    switch (cat) {
        case ID: return "ID";
        case COMENTARIO_LINHA: return "COMENTARIO_LINHA";
        case PAL_RESERV: return "PAL_RESERV";
        case SINAL: return "SINAL";
        case CONST_CHAR: return "CONST_CHAR";
        case CONST_FLOAT: return "CONST_FLOAT";
        case CONST_INT: return "CONST_INT";
        case ID_CONST: return "ID_CONST";
        case LITERAL: return "LITERAL";
        case OP_ARIT: switch (tk.codigo){
            case ATRIBUICAO: return "ATRIBUICAO";
            case ADICAO: return "ADICAO";
            case SUBTRACAO: return "SUBTRACAO";
            case MULTIPLICACAO: return "MULTIPLICACAO";
            case DIVISAO: return "DIVISAO";
        }
        case OP_LOGIC: return "OP_LOGIC";
        case OP_RELAC: return "OP_RELAC";
        case FIM_EXPR: return "FIM_EXPR";
        case FIM_PROG: return "FIM_PROG";
        case COMENTARIO: return "COMENTARIO";
        default: return "UNKNOWN";
    }
}

const char* getResName (enum PAL_RESERV res){
    switch (res){
        case CONST: return "const";
        case PR: return "pr";
        case GETOUT: return "getout";
        case GETINT: return "getint";
        case GETREAL: return "getreal";
        case GETCHAR: return "getchar";
        case GETSTR: return "getstr";
        case PUTINT: return "putint";
        case INIT: return "init";
        case ENDP: return "endp";
        case ENDV: return "endv";
        case ENDW: return "endw";
        case IF: return "if";
        case ELIF: return "elif";
        case ELSE: return "else";
        case ENDI: return "endi";
        case INTCON: return "intcon";
        case IDCONST: return "idconst";
        case CHAR: return "char";
        case INT: return "int";
        case REAL: return "real";
        case BOOL: return "bool";
        case DO: return "do";
        case IDPROC: return "idproc";
        case WHILE: return "while";
        case VAR: return "var";
        case FROM: return "from";
        case TO: return "to";
        case DT: return "dt";
        case BY: return "by";
        case PUTCHAR: return "putchar";
        case PUTREAL: return "putreal";
        case PUTSTR: return "putstr";
        case DEF: return "def";
        case PROT: return "prot";
        default: return "UNKNOWN";
    }
}

const char* getArit (enum OP_ARIT ari){
    switch (ari){
        case ATRIBUICAO: return "=";// =
        case ADICAO: return "+";     // +
        case SUBTRACAO: return "-";    // -
        case MULTIPLICACAO: return "*";  // *
        case DIVISAO: return "/";      // /
        default: return "UNKNOWN";
    }
}
const char* getRel(enum OP_RELAC rel){
    switch (rel){
        case IGUALDADE: return " == ";
        case DIFERENTE: return " != ";
        case MENOR_IGUAL: return " <= ";
        case MAIOR_IGUAL: return " >= ";
        case MENOR_QUE: return " < ";
        case MAIOR_QUE: return " > ";

    }

}
const char* getLogic(enum OP_LOGIC logic){
    switch (logic){
        case AND_LOGIC: return " && ";
        case OR_LOGIC: return " || ";
        case NOT_LOGIC: return " ! ";

    }

}
const char* getSinais(enum SINAIS sin){
    switch (sin){
        case ABRE_PAR: return " ( ";
        case FECHA_PAR: return " ) ";
        case ABRE_CHAVE: return " { ";
        case FECHA_CHAVE: return " } ";
        case ABRE_COL: return " [ ";
        case FECHA_COL: return " ] ";
        case VIRGULA: return " , ";
        case REFERENCIA: return " & ";


    }

}


void Testalex() {



    if ((fd = fopen("expressao.txt", "r")) == NULL) {
        fprintf(stderr, "Arquivo de entrada da expressao nao encontrado!\n");

    }



    while (1) {
    tk = AnaLex(fd);
    printf("\nLINHA %d: ", contLinha);
    if (tk.cat == FIM_PROG) break;
    switch(tk.cat){
    case ID: printf("Token encontrado: categoria = <<%s>>, lexema = <<%s>>\n", getTokenName(tk.cat), tk.lexema);
                break;
    case SINAL: printf("Token encontrado: categoria = <<%s>>,lexema =  <<%s>>\n", getTokenName(tk.cat), getSinais(tk.codigo));
                break;
    case PAL_RESERV: printf("Token encontrado: categoria = <<%s>>,lexema = <<%s>>\n ", getTokenName(tk.cat), getResName(tk.codigo));
                break;
    case LITERAL: printf("Token encontrado: categoria = <<%s>>, lexema = <<%s>>\n", getTokenName(tk.cat), tk.lexema);
                break;
    case COMENTARIO: printf("");
                break;
    case OP_ARIT: printf("Token encontrado: categoria = <<%s>>,lexema =  <<%s>>\n", getTokenName(tk.cat), getArit(tk.codigo));
                break;
    case OP_RELAC: printf("Token encontrado: categoria = <<%s>>,lexema =  <<%s>>\n", getTokenName(tk.cat), getRel(tk.codigo));
                break;
    case OP_LOGIC: printf("Token encontrado: categoria = <<%s>>,lexema =  <<%s>>\n", getTokenName(tk.cat), getLogic(tk.codigo));
                break;
    case CONST_FLOAT: printf("Token encontrado: categoria = <<%s>>,digito = <<%f>>\n", getTokenName(tk.cat),tk.valFloat);
                break;
    case CONST_INT: printf("Token encontrado: categoria = <<%s>>,digito = <<%d>>\n", getTokenName(tk.cat),tk.valInt);
                break;
    case CONST_CHAR:printf("Token encontrado: categoria = <<%s>>, lexema = <<%s>>\n", getTokenName(tk.cat), tk.lexema);
                break;
    case FIM_EXPR: printf("<FIM_EXPR, %d>\n", 0);
                           printf("\nLINHA %d: ", contLinha);
                           break;
    }
}
    fclose(fd);
}
void Testasint(){
    if ((fd = fopen("expressao.txt", "r")) == NULL) {
        fprintf(stderr, "Arquivo de entrada da expressao nao encontrado!\n");
        return 1;
    }
    while (1){
        tk = AnaLex(fd);


        if(tk.cat == FIM_PROG){
            printf("\n fim do arquivo fonte");
            break;
        }
        Prog();
        if (tk.cat==FIM_EXPR){
                printf("\nLINHA %d: Expressão sintaticamente correta!\n\n", contLinha - 1);
            }
    }
}


int main() {
    printf("\n\n    >>>>>>> Analise Lexica <<<<<<<\n");
    Testalex();

    mostrArvore = true;

    printf("\n\n    >>>>>>> Analise Sintatica <<<<<<<\n");
    Testasint();
}
