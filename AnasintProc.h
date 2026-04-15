#ifndef ANASINTPROC_H
#define ANASINTPROC_H

#include "AnalexProc.h"
#include <stdbool.h>
#include <stdio.h>

// Variáveis globais
extern TOKEN tk;
extern FILE *fd;
extern int contLinha;
extern int escopoAtual;

extern bool mostraArvore;
extern char TABS[200];

// Funções do analisador sintático
void Prog();
void DeclListVar();
void DeclDefProc();
void DeclVar();
void Tipo();
void Cmd();
void Expr();
void ExprSimp();
void Termo();
void Fator();
void OpRel();
void Atrib();



#endif // ANASINT_H
