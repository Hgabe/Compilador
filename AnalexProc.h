#ifndef ANALEXPROC_H
#define ANALEXPROC_H

#include <stdio.h>
#include <stdbool.h>
#define TAM_MAX_LEXEMA 31
bool processado;
enum TOKEN_CAT
{
    ID = 1,      // 1
    PAL_RESERV,  // 2
    SINAL,       // 3
    CONST_CHAR,  // 4
    CONST_INT,   // 5
    ID_CONST,    // 6
    LITERAL,     // 7
    OP_ARIT,     // 8
    OP_LOGIC,    // 9
    OP_RELAC,    // 10
    CONST_FLOAT, // 11
    FIM_EXPR,    // 12
    FIM_PROG,    // 13
    COMENTARIO,  //14
    COMENTARIO_LINHA,   //15
};

enum PAL_RESERV
{
    CONST = 1,
    PR,
    GETOUT,
    GETINT,
    GETREAL,
    GETCHAR,
    GETSTR,
    PUTINT,
    INIT,
    ENDP,
    ENDV,
    ENDW,
    IF,
    ELIF,
    ELSE,
    ENDI,
    INTCON,
    IDCONST,
    CHAR,
    INT,
    REAL,
    BOOL,
    DO,
    IDPROC,
    WHILE,
    VAR,
    FROM,
    TO,
    DT,
    BY,
    PUTCHAR,
    PUTREAL,
    PUTSTR,
    DEF,
    PROT
};

enum SINAIS
{
    ABRE_PAR = 1, // 1
    FECHA_PAR,    // 2
    ABRE_CHAVE,   // 3
    FECHA_CHAVE,  // 4
    ABRE_COL,     // 5
    FECHA_COL,    // 6
    VIRGULA,      // 7
    REFERENCIA
};

enum OP_ARIT
{
    ATRIBUICAO = 1, // =
    ADICAO,         // +
    SUBTRACAO,      // -
    MULTIPLICACAO,  // *
    DIVISAO         // /
};

enum OP_RELAC //X
{
    IGUALDADE = 1, // ==
    DIFERENTE,     // !=
    MENOR_IGUAL,   // <=
    MAIOR_IGUAL,   // >=
    MENOR_QUE,     // <
    MAIOR_QUE,     // >

};

enum OP_LOGIC //X
{
    AND_LOGIC = 1, // &&
    OR_LOGIC,      // ||
    NOT_LOGIC      // !
};


typedef struct
{
    enum TOKEN_CAT cat;
    bool processado;
    union
    {
        int codigo;
        int indice;
        char lexema[TAM_MAX_LEXEMA];
        int valInt;
        float valFloat;
        char caractere;

    };

} TOKEN;

extern TOKEN tk;
extern FILE *fd;
TOKEN AnaLex(FILE *);

#endif // ANALEXPROC_H
