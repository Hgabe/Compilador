#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "AnalexProc.h"

#define TAM_LEXEMA 50
#define TAM_LITERAL 50
#define TAM_NUM 20

int contLinha = 1;

void errorLex(int contLinha, char caracter)
{
    char c_str[2];
    c_str[0] = caracter;
    c_str[1] = '\0';
    printf("\nCaracter '%s' invalido na linha: %d\n", c_str, contLinha);
    exit(1);
}

TOKEN AnaLex(FILE *fd)
{
    int estado = 0;

    int tamL = 0;
    int tamD = 0;
    char lexema[TAM_LEXEMA] = "";
    char digitos[TAM_NUM] = "";

    char c;


    tk.processado = false;



    while(1)
    {
        c = fgetc(fd);

        switch(estado)
        {
        case 0:
            if(c == ' ' || c == '\t')
            {
                estado = 0;
            }
            else if (c == '\'')
            {
                estado = 9;

            }
            else if(c == '\"')
            {
                estado = 15;

            }
            else if(c == '/')
            {
                estado = 3;
            }
            else if(c == '=')
            {
                estado = 25;
            }
            else if((c == '_') || isalpha(c))
            {
                estado = 1;
                lexema[tamL++] = c;
                lexema[tamL] = '\0';
            }
            else if(isdigit(c))
            {
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
                estado =  5;

            }
            else if (c == '+')
            {
                estado = 37;
                tk.cat = OP_ARIT;
                tk.codigo = ADICAO;
                return tk;
            }
            else if (c == '-')
            {
                estado = 38;
                tk.cat = OP_ARIT;
                tk.codigo = SUBTRACAO;
                return tk;
            }
            else if (c == '*')
            {
                estado = 39;
                tk.cat = OP_ARIT;
                tk.codigo = MULTIPLICACAO;
                return tk;
            }
            else if (c == '!')
            {
                estado = 28;
                tk.cat = OP_RELAC;
            }
            else if (c == '|')
            {
                estado = 23;
                tk.cat = OP_LOGIC;
            }
            else if (c == '&')
            {
                estado = 20;
                tk.cat = OP_LOGIC;
            }
            else if (c == '>')
            {
                estado = 34;
                tk.cat = OP_RELAC;
            }
            else if (c == '<')
            {
                estado = 31;
                tk.cat = OP_RELAC;
            }
            else if (c == '(')
            {
                estado = 43;
                tk.cat = SINAL;
                tk.codigo = ABRE_PAR;
                return tk;
            }
            else if (c == ')')
            {
                estado = 42;
                tk.cat = SINAL;
                tk.codigo = FECHA_PAR;
                return tk;
            }
            else if (c == ',')
            {
                estado = 18;
                tk.cat = SINAL;
                tk.codigo = VIRGULA;
                return tk;
            }
            else if (c == '[')
            {
                estado = 44;
                tk.cat = SINAL;
                tk.codigo = ABRE_COL;
                return tk;
            }
            else if (c == ']')
            {
                estado = 41;
                tk.cat = SINAL;
                tk.codigo = FECHA_COL;
                return tk;
            }
            else if (c == '\n')
            {
                contLinha ++;
                estado = 0;
            }
            else if (c == EOF)
            {
                tk.cat = FIM_PROG;
                contLinha = 1;
                return tk;
            }
            else if (c == '{')
            {
                estado = 49;
                tk.cat = SINAL;
                tk.codigo = ABRE_CHAVE;
                return tk;
            }
            else if (c == '}')
            {
                estado = 50;
                tk.cat = SINAL;
                tk.codigo = FECHA_CHAVE;
                return tk;
            }
            else
            {
                errorLex(contLinha, c);
            }
            break;

        case 1:
            if (c == '_' || isalpha(c) || isdigit(c))
            {
                estado = 1;
                lexema[tamL++] = c;
                lexema[tamL] = '\0';
            }
            else
            {
                estado = 4;

                ungetc(c, fd);

                strcpy(tk.lexema, lexema);

                if (strcmp("const", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = CONST;
                }
                else if (strcmp("pr", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = PR;
                }
                else if (strcmp("init", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = INIT;
                }
                else if (strcmp("endp", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = ENDP;
                }
                else if (strcmp("intcon", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = INTCON;
                }
                else if (strcmp("idconst", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = IDCONST;
                }
                else if (strcmp("char", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = CHAR;
                }
                else if (strcmp("int", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = INT;
                }
                else if (strcmp("real", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = REAL;
                }
                else if (strcmp("bool", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = BOOL;
                }
                else if (strcmp("endp", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = ENDP;
                }
                else if (strcmp("do", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = DO;
                }
                else if (strcmp("idproc", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = IDPROC;
                }
                else if (strcmp("while", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = WHILE;
                }
                else if (strcmp("var", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = VAR;
                }
                else if (strcmp("from", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = FROM;
                }
                else if (strcmp("to", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = TO;
                }
                else if (strcmp("dt", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = DT;
                }
                else if (strcmp("by", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = BY;
                }
                else if (strcmp("endv", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = ENDV;
                }
                else if (strcmp("endw", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = ENDW;
                }
                else if (strcmp("if", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = IF;
                }
                else if (strcmp("elif", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = ELIF;
                }
                else if (strcmp("else", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = ELSE;
                }
                else if (strcmp("endi", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = ENDI;
                }
                else if (strcmp("getout", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = GETOUT;
                }
                else if (strcmp("getint", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = GETINT;
                }
                else if (strcmp("getreal", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = GETREAL;
                }
                else if (strcmp("getchar", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = getchar;
                }
                else if (strcmp("getstr", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = GETSTR;
                }
                else if (strcmp("putint", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = PUTINT;
                }
                else if (strcmp("putreal", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = PUTREAL;
                }
                else if (strcmp("putchar", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = PUTCHAR;
                }
                else if (strcmp("putstr", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = PUTSTR;
                }
                else if (strcmp("def", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = DEF;
                }
                else if (strcmp("prot", lexema) == 0)
                {
                    tk.cat = PAL_RESERV;
                    tk.codigo = PROT;
                }
                else
                {
                    tk.cat = ID;
                    strcpy(tk.lexema, lexema);
                }

                return tk;
            }
            break;

            case 5:
        if (isdigit(c)) {
            estado = 5;
            digitos[tamD++] = c;
            digitos[tamD] = '\0';
        } else if (c == '.') {

            estado = 7;
            digitos[tamD++] = c;
            digitos[tamD] = '\0';
        } else {
            estado = 6;
            ungetc(c, fd);
            tk.cat = CONST_INT;
            tk.valInt = atoi(digitos);
            return tk;
        }
        break;

    case 7:

        if (isdigit(c)) {

            digitos[tamD++] = c;
            digitos[tamD] = '\0';
            estado = 8;
        } else {
            errorLex(contLinha, c);
        }
        break;

    case 8:

        if (isdigit(c)) {

            estado = 8;

            digitos[tamD++] = c;
            digitos[tamD] = '\0';
        } else {
            ungetc(c, fd);
            tk.cat = CONST_FLOAT;
            tk.valFloat = atof(digitos);

            return tk;
        }
        break;

        case 9:
            if (c == '\\') {
                estado = 12;
            }
            else if (c == '\'') {
                if (tamL == 0) {
                    errorLex(contLinha, c);
                }
                tk.cat = CONST_CHAR;
                strcpy(tk.lexema, lexema);
                return tk;
            }
            else if (isprint(c)) {
                estado = 11;
                if (tamL < TAM_LEXEMA - 1) {
                    lexema[tamL++] = c;


                } else {
                    errorLex(contLinha, c);
                }
            }
            else if (c == '\n') {
                errorLex(contLinha, c);
            }
            else {
                errorLex(contLinha, c);
            }
            break;
        case 10:
            if (c == '\'')
            {
                estado = 0;

                tk.cat = CONST_CHAR;

                strcpy(tk.lexema, lexema);

                return tk;
            }
            else
            {
                errorLex(contLinha, c);
            }
            break;



        case 12:
            if (c == 'n') {
                 if (strlen("enter") < TAM_LEXEMA) {
            strcpy(lexema, "enter");
            tamL = strlen(lexema);
            estado = 13;
                 }
            }

            else if (c == '0') {
                if (strlen("null") < TAM_LEXEMA) {
            strcpy(lexema, "null");
            tamL = strlen(lexema);
            estado = 14;
                 }
            }

            else {
                errorLex(contLinha, c);
            }
            break;
        case 13:
            if (c == '\'') {

                tk.cat = CONST_CHAR;
                strcpy(tk.lexema, lexema);
                estado = 0;
                return tk;
            }
            break;
        case 14:
            if (c == '\'') {

                tk.cat = CONST_CHAR;
                strcpy(tk.lexema, lexema);
                estado = 0;
                return tk;
            }
            break;
        case 15:
            if (c == '\"') {
                if (tamL == 0) {
                    errorLex(contLinha, c);
                }

                estado = 0;

                tk.cat = LITERAL;
                strcpy(tk.lexema, lexema);

                return tk;
            }
            else if (isprint(c)) {
                if (tamL < TAM_LEXEMA - 1) {
                    lexema[tamL++] = c;
                    lexema[tamL] = '\0';
                }
                else {
                    errorLex(contLinha, c);
                }
            }
            else if (c == '\n') {
                errorLex(contLinha, c);
            }
            break;
        case 3:
            if (c == '/')
            {
                estado = 30;


            }
            else
            {
                estado = 40;
                ungetc(c, fd);

                tk.cat = OP_ARIT;
                tk.codigo = DIVISAO;

                return tk;
            }
            break;
        case 19:
            if (c == '\n')
            {
                estado = 0;
            }
            else
            {

                lexema[tamL++] = c;
            }
            break;
        case 25:
            if (c == '=')
            {
                estado = 27;

                tk.cat = OP_RELAC;
                tk.codigo = IGUALDADE;

                return tk;
            }
            else
            {
                estado = 26;

                ungetc(c, fd);

                tk.cat = OP_ARIT;
                tk.codigo = ATRIBUICAO;


                return tk;
            }
            break;
        case 28:
            if (c == '=')
            {
                estado = 45;
                tk.cat = OP_RELAC;
                tk.codigo = DIFERENTE;

                return tk;
            }
            else
            {
                estado = 0;
                ungetc(c, fd);
                tk.cat = OP_LOGIC;
                tk.codigo = NOT_LOGIC;

                return tk;
            }
            break;
        case 31:
            if (c == '=')
            {
                estado = 32;

                tk.codigo = MENOR_IGUAL;

                return tk;
            }
            else
            {
                estado = 0;
                ungetc(c, fd);

                tk.codigo = MENOR_QUE;

                return tk;
            }
            break;
        case 34:
            if (c == '=')
            {
                estado = 35;

                tk.codigo = MAIOR_IGUAL;

                return tk;
            }
            else
            {
                estado = 0;
                ungetc(c, fd);

                tk.codigo = MAIOR_QUE;

                return tk;
            }
            break;
        case 20:
            if (c == '&')
            {
                estado = 21;

                tk.cat = OP_LOGIC;

                tk.codigo = AND_LOGIC;

                return tk;
            }
            else
            {
                estado = 22;

                tk.cat = SINAL;

                ungetc(c, fd);

                tk.codigo = REFERENCIA;

                return tk;
            }
            break;
        case 23:
            if (c == '|')
            {
                estado = 24;

                tk.cat = OP_LOGIC;

                tk.codigo = OR_LOGIC;

                return tk;
            }
            else
            {
                errorLex(contLinha, c);
            }
            break;
        case 30:
            if (c == '\n') {

                lexema[tamL] = '\0';
                estado = 0;
                contLinha++;
                tk.cat = COMENTARIO;
                tk.codigo = COMENTARIO_LINHA;
                return tk;
            }
        case 11:
            if (c == '\'') {

                tk.cat = CONST_CHAR;
                strcpy(tk.lexema, lexema);

                return tk;
            }
            else if (isprint(c)) {
                if (tamL < TAM_LEXEMA - 1) {
                    lexema[tamL++] = c;

                    lexema[tamL] = '\0';
                }
                else {
                    errorLex(contLinha, c);
                }
            }
            else if (c == '\n') {
                errorLex(contLinha, c);
            }
            break;


        }
    }
}


