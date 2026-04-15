#ifndef TABSIMB_H_INCLUDED
#define TABSIMB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "AnalexProc.h"
#include <stdbool.h>
#define TAM_PASSAGEM 5
#define TAM_MAX_TAB 100
#define TAM_CATEGORIA 4
#define TAM_ZOMBIE 8
#define TAM_TIPO 10
#define TAM_MAX_LEXEMA 50

typedef struct
{
    char lexema[TAM_MAX_LEXEMA];
    float valor;
    int escopo;
    char categoria[TAM_CATEGORIA];
    char arrai[TAM_TIPO];
    char tipo[TAM_TIPO];
    char zombie[TAM_ZOMBIE];
    bool eh_const;
    int dim1;
    int dim2;
    char passagem[TAM_PASSAGEM];
    int endrel;

} TabSimb;


typedef struct
{
    int tamTab;
    TabSimb tabela_simb[TAM_MAX_TAB];
} TabIdef;


extern TabIdef tabela_idef;
extern TOKEN tk;
extern int escopoAtual;


void Iniciar_tabela();
int Buscar_valor_lexema(const char lexema[]);
void Imprimir_tabela();
int Verif_se_const(const char lexema[]);
int Buscar_tabela(const char lexema[]);
int Buscar_escopo(char lexema[], int escopo);
int Insercao_tabela(char lexema[], int escopo, float valor, char categoria[], char arrai[], char tipo[], char zombie[], bool eh_const, int dim1, int dim2, char passagem[], int endrel);
int Remover_ultimo();
int Buscar_endereco_lexema(const char lexema[]);
void att_z();
#endif
