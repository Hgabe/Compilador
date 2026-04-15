#include "TabSimb.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


void Iniciar_tabela() {
    tabela_idef.tamTab = 0;
}


void Imprimir_tabela() {
    printf("\nTabela de Simbolos:\n");
    printf("___________________________________________________________________________________________________________________________________________________________________________\n\n");
    printf("| %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s |\n",
           "Indice", "Lexema", "ValorConst", "Escopo", "Categoria", "Array", "Tipo", "Zombie", "Eh_const", "Dim1", "Dim2","Passagem", "Endereco");
    printf("___________________________________________________________________________________________________________________________________________________________________________\n\n");

    for (int i = 0; i < tabela_idef.tamTab; i++) {
        printf("| %-10d | %-10s | %-10.2f | %-10d | %-10s | %-10s | %-10s | %-10s | %-10d | %-10d | %-10d | %-10s | %-10d |\n",
               i,
               tabela_idef.tabela_simb[i].lexema,
               tabela_idef.tabela_simb[i].valor,
               tabela_idef.tabela_simb[i].escopo,
               tabela_idef.tabela_simb[i].categoria,
               tabela_idef.tabela_simb[i].arrai,
               tabela_idef.tabela_simb[i].tipo,
               tabela_idef.tabela_simb[i].zombie,
               tabela_idef.tabela_simb[i].eh_const,
               tabela_idef.tabela_simb[i].dim1,
               tabela_idef.tabela_simb[i].dim2,
               tabela_idef.tabela_simb[i].passagem,
               tabela_idef.tabela_simb[i].endrel);
    }
    printf("___________________________________________________________________________________________________________________________________________________________________________\n\n");
}


int Buscar_tabela(const char lexema[]) {
    for (int i = 0; i < tabela_idef.tamTab; i++) {
        if (strcmp(tabela_idef.tabela_simb[i].lexema, lexema) == 0) {
            return i;
        }
    }
    return -1;
}

int Verif_se_const(const char lexema[]) {
    for (int i = 0; i < tabela_idef.tamTab; i++) {
        if (strcmp(tabela_idef.tabela_simb[i].lexema, lexema) == 0) {
            if (tabela_idef.tabela_simb[i].eh_const == 1) {
                return tabela_idef.tabela_simb[i].valor;
            } else {
                return -1;
            }
        }
    }
    return -1;
}

int Buscar_endereco_lexema(const char lexema[]) {
    int indice = Buscar_tabela(lexema);

    if (indice == -1) {
        return -1;
    }
    return tabela_idef.tabela_simb[indice].endrel;
}

int Buscar_valor_lexema(const char lexema[]) {
    for (int i = 0; i < tabela_idef.tamTab; i++) {
        if (strcmp(tabela_idef.tabela_simb[i].lexema, lexema) == 0) {
            return tabela_idef.tabela_simb[i].valor;
        }
    }
    return -1;
}


int Buscar_escopo(char lexema[], int escopo) {
    for (int i = 0; i < tabela_idef.tamTab; i++) {
        if (strcmp(tabela_idef.tabela_simb[i].lexema, lexema) == 0 && tabela_idef.tabela_simb[i].escopo == escopo) {
            if (strcmp(tabela_idef.tabela_simb[i].categoria, "PAR") == 0) {
                continue;
            }
            return i;
        }
    }
    return -1;
}

void att_z() {
    for (int i = 0; i < tabela_idef.tamTab; i++) {
        if (strcmp(tabela_idef.tabela_simb[i].zombie, "N/A") != 0) {
            strncpy(tabela_idef.tabela_simb[i].zombie, "TRUE", TAM_ZOMBIE - 1);
            tabela_idef.tabela_simb[i].zombie[TAM_ZOMBIE - 1] = '\0';
        }
    }
}

int Insercao_tabela(char lexema[], int escopo, float valor, char categoria[], char arrai[], char tipo[], char zombie[], bool eh_const, int dim1, int dim2, char passagem[], int endrel) {
    if (tabela_idef.tamTab >= TAM_MAX_TAB) {

        return -1;
    }

    int pos = Buscar_escopo(lexema, escopo);
    if (pos != -1) {
        if (strcmp(tabela_idef.tabela_simb[pos].categoria, "PRT") == 0 && strcmp(categoria, "PRC") == 0) {
            strncpy(tabela_idef.tabela_simb[pos].categoria, categoria, TAM_CATEGORIA - 1);
            tabela_idef.tabela_simb[pos].categoria[TAM_CATEGORIA - 1] = '\0';
            strncpy(tabela_idef.tabela_simb[pos].tipo, tipo, TAM_TIPO - 1);
            tabela_idef.tabela_simb[pos].tipo[TAM_TIPO - 1] = '\0';
            strncpy(tabela_idef.tabela_simb[pos].arrai, arrai, TAM_TIPO - 1);
            tabela_idef.tabela_simb[pos].arrai[TAM_TIPO - 1] = '\0';
            strncpy(tabela_idef.tabela_simb[pos].zombie, zombie, TAM_ZOMBIE - 1);
            tabela_idef.tabela_simb[pos].zombie[TAM_ZOMBIE - 1] = '\0';
            tabela_idef.tabela_simb[pos].valor = valor;
            tabela_idef.tabela_simb[pos].eh_const = eh_const;
            tabela_idef.tabela_simb[pos].dim1 = dim1;
            tabela_idef.tabela_simb[pos].dim2 = dim2;
            strncpy(tabela_idef.tabela_simb[pos].passagem, passagem, TAM_PASSAGEM - 1);
            tabela_idef.tabela_simb[pos].passagem[TAM_PASSAGEM - 1] = '\0';
            tabela_idef.tabela_simb[pos].endrel = endrel;
            return 0;
        }

        printf("Lexema '%s' ja declarado no escopo %d.\n", lexema, escopo);
        return -1;
    }

    TabSimb novoSimbolo;
    strncpy(novoSimbolo.lexema, lexema, TAM_MAX_LEXEMA - 1);
    novoSimbolo.lexema[TAM_MAX_LEXEMA - 1] = '\0';
    novoSimbolo.escopo = escopo;
    novoSimbolo.valor = valor;
    strncpy(novoSimbolo.categoria, categoria, TAM_CATEGORIA - 1);
    novoSimbolo.categoria[TAM_CATEGORIA - 1] = '\0';
    strncpy(novoSimbolo.tipo, tipo, TAM_TIPO - 1);
    novoSimbolo.tipo[TAM_TIPO - 1] = '\0';
    strncpy(novoSimbolo.arrai, arrai, TAM_TIPO - 1);
    novoSimbolo.arrai[TAM_TIPO - 1] = '\0';
    strncpy(novoSimbolo.zombie, zombie, TAM_ZOMBIE - 1);
    novoSimbolo.zombie[TAM_ZOMBIE - 1] = '\0';
    novoSimbolo.eh_const = eh_const;
    novoSimbolo.dim1 = dim1;
    novoSimbolo.dim2 = dim2;
    strncpy(novoSimbolo.passagem, passagem, TAM_PASSAGEM - 1);
    novoSimbolo.passagem[TAM_PASSAGEM - 1] = '\0';
    novoSimbolo.endrel = endrel;

    tabela_idef.tabela_simb[tabela_idef.tamTab++] = novoSimbolo;
    return 0;
}


int Remover_ultimo() {
    if (tabela_idef.tamTab > 0) {
        tabela_idef.tamTab--;
        return 0;
    } else {
        printf("[ERRO] Tabela de símbolos vazia.\n");
        return -1;
    }
}
