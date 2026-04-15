#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

enum MOVIM
{
    AVANCA,
    MANTEM,
    RETROCEDE
};

extern bool mostrArvore;
extern char TABS[200];
extern int contLinha;
void erro(char msg[]);
void errorLex(int contLinha, char caracter);
void errorSint(int contLinha, char caracter[]);
void PrintNodo(char info[], int movim);
void PrintNodoInt(int val, int movim);
void PrintNodoFloat(float val, int movim);

#endif
