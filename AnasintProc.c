#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "AnalexProc.h"
#include "AnasintProc.h"
#include "func.h"
#include "TabSimb.h"
TabIdef tabela_idef;
int escopoAtual = 0;
char zombie[10] = "N/A";
int dim1 =0 ,dim2 =0;
int end;
int tokenop;
int c;
int iniprcont = 1;
int lab =1;
char pas[10] = "N/A";
int endrelpar = -3;
int endrel = 0;
int endrelloc = 0;
int am =1;
float valor;
float valo;
int indar =1;
char arrai[10] = "N/A";
char categ[10] = "";
char tipo[10] = "";
bool eh_const = false;

FILE *file;

void Prog(){
    file = fopen("expressao2.txt", "w");


    if (file == NULL) {
        erro("erro na geracao de codigo");
        exit(1);
    }


    fprintf(file, "INIP\n");
    ////fclose(file);

    Iniciar_tabela();
    while ((tk.cat == PAL_RESERV && (tk.codigo == CONST))||(tk.cat == PAL_RESERV && (tk.codigo == CHAR || tk.codigo == INT||tk.codigo == REAL||tk.codigo == BOOL))) {

        DeclListVar();
    }
    while((tk.cat == PAL_RESERV &&(tk.codigo == PROT))||(tk.cat == PAL_RESERV && (tk.codigo == DEF))){
        DeclDefProc();
    }


	////fclose(file);

}
void DeclListVar(){
    am = 1;
    //FILE *file = fopen("expressao2.txt", "a");


    if (tk.codigo == CONST){
        eh_const = true;

        tk = AnaLex(fd);
    }
    else{
       eh_const = false;
    }

    Tipo();
    tk = AnaLex(fd);
    DeclVar();
    while (tk.codigo == VIRGULA){
        am++;
        if (escopoAtual == 1){


        }


        tk = AnaLex(fd);

        DeclVar();
    }
    fprintf(file, "AMEM %d\n", am);
    am = 0;
    //fclose(file);
}
void Tipo(){


    if (tk.cat == PAL_RESERV && (tk.codigo == CHAR || tk.codigo == INT||tk.codigo == REAL||tk.codigo == BOOL)){
       if(tk.codigo == CHAR ){

            strcpy(tipo, "char");

       }
       else if(tk.codigo == INT ){

            strcpy(tipo, "int");
       }
       else if(tk.codigo == REAL ){

            strcpy(tipo, "real");
       }
       else if(tk.codigo == BOOL ){

            strcpy(tipo, "bool");
       }


    }
    else{
        erro("CHAR, INT, REAL OU BOOL ESPERADO");
    }

}
void DeclVar(){
    //FILE *file = fopen("expressao2.txt", "a");
    char lexema[TAM_MAX_LEXEMA];
    strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
    lexema[TAM_MAX_LEXEMA - 1] = '\0';

    if (escopoAtual == 0){
        strcpy(categ, "VG");

        if (Buscar_escopo(lexema, escopoAtual) != -1) {
            erro("Identificador ja foi declarado como global");
        }
    }
    if (escopoAtual == 1){
        strcpy(categ, "VL");

        escopoAtual --;

        if (Buscar_escopo(lexema, escopoAtual) != -1 ) {
            erro("Identificador ja foi declarado como global");
        }
        escopoAtual ++;
    }


    if (tk.cat != ID){
        erro("id esperado");
    }
    if (tk.cat == ID){

        tk = AnaLex(fd);
        if (tk.cat == OP_ARIT && tk.codigo == ATRIBUICAO){

            tk = AnaLex(fd);
            if (tk.cat == CONST_INT||tk.cat == CONST_CHAR||tk.cat == CONST_FLOAT){

                if (tk.cat == CONST_INT) {


                    if (eh_const == 1){
                       valor = tk.valInt;
                    }else{
                        valor =0;
                    }
                    valo = tk.valInt;
                    fprintf(file, "PUSH%2.f\n", valo);
                    if (strcmp(tipo, "bool") == 0) {
                        if ((valor != 0)&& (valor != 1)){
                            erro("booleanos devem receber 0 ou 1 (verdadeiro ou falso)");
                        }
                    }

                    if (escopoAtual == 0){
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrel) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }else{
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrelloc) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }
                }
                else if (tk.cat == CONST_FLOAT) {

                    if (eh_const == 1){
                       valor = tk.valFloat;
                    }else{
                        valor =0;//como eu não entendi muito bem se valor const era pra ser de fato só em constantes pode alterar caso seja pra tudo pondo tk.valfloat
                    }
                    valo = tk.valFloat;
                    fprintf(file, "PUSH%2.f\n", valo);
                    if (escopoAtual == 0){
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrel) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }else{
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrelloc) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }


                }
                else if (tk.cat == CONST_CHAR) {

                    valor = tk.codigo;

                    if (escopoAtual == 0){
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrel) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }else{
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrelloc) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }
                }
                if (escopoAtual == 1){
                    fprintf(file, "STOR 1,%d\n", endrelloc);
                    endrelloc ++;
                    printf("%d",endrelloc);
                } else{
                    fprintf(file, "STOR 0,%d\n", endrel);
                    endrel ++;
                    printf("%d",endrel);
                }
                tk = AnaLex(fd);
            }
            else if (tk.codigo == ABRE_CHAVE){

                tk = AnaLex(fd);
                if (tk.cat == CONST_INT||tk.cat == CONST_CHAR||tk.cat == CONST_FLOAT){
                    if (tk.cat == CONST_INT) {


                    }
                    else if (tk.cat == CONST_FLOAT) {


                    }
                    else if (tk.cat == CONST_CHAR) {


                    }
                    tk = AnaLex(fd);
                }
                else {
                    erro("const char, const int ou const float devem ter pelo menos uma ocorrencia apos as chaves");
                }
                while (tk.codigo == VIRGULA){

                    tk = AnaLex(fd);
                    if (tk.cat == CONST_INT||tk.cat == CONST_CHAR||tk.cat == CONST_FLOAT){
                        if (tk.cat == CONST_INT) {


                        }
                        else if (tk.cat == CONST_FLOAT) {


                        }
                        else if (tk.cat == CONST_CHAR) {


                        }
                        tk = AnaLex(fd);
                    }
                    else {
                        erro("const char, const int ou const float devem ter pelo menos uma ocorrencia apos a virgula");
                    }
                }
                if (tk.codigo != FECHA_CHAVE){
                    erro("abre chaves nao finalizado");
                }
                else{

                    tk = AnaLex(fd);
                }
            }
            else{
                erro("apos o id apenas = ou ={");
            }
        }
        else if (tk.codigo == ABRE_COL){
            while (tk.codigo == ABRE_COL) {



                tk = AnaLex(fd);
                if (indar ==1){
                    dim1 = tk.valInt;
                    strcpy(arrai, "VET");
                }
                if (indar >= 2){
                    dim2 = tk.valInt;
                    strcpy(arrai, "MAT");
                }
                if (tk.cat != CONST_INT && tk.cat != ID){
                    erro("inteiro ou constante pre declarada esperados");
                }
                else if (tk.cat == CONST_INT){

                        tk = AnaLex(fd);
                }
                else if (tk.cat == ID){
                        if (indar ==1){
                            dim1 = Buscar_valor_lexema(tk.lexema);
                            int ver = Verif_se_const(tk.lexema);
                            if (ver == -1){
                                printf("< %s > nao foi declarada como constante\n", tk.lexema);
                                erro("");
                            }
                        }
                        if (indar >= 2){
                            dim2 = Buscar_valor_lexema(tk.lexema);
                            int ver =Verif_se_const(tk.lexema);
                            if (ver == -1){
                                printf("< %s > nao foi declarada como constante\n", tk.lexema);
                                erro("");
                            }
                        }
                        char lexema[TAM_MAX_LEXEMA];
                        strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
                        lexema[TAM_MAX_LEXEMA - 1] = '\0';

                        if (Buscar_escopo(lexema, escopoAtual) != -1) {

                            tk = AnaLex(fd);
                        }else{
                            escopoAtual--;
                            if(Buscar_escopo(lexema, escopoAtual) != -1){

                                tk = AnaLex(fd);
                            }
                            else{
                                erro("idconst nao foi previamente declarado");
                            }
                            escopoAtual++;

                        }
                }
                if (tk.codigo != FECHA_COL){
                    erro("fecha colchete esperado");
                }
                else {

                    tk = AnaLex(fd);
                    indar++;
                }
            }
            valor = 0;
            if (escopoAtual == 0){
                if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrel) == -1) {
                    erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                }
            }else{
                if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrelloc) == -1) {
                    erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                }
            }
            if (escopoAtual == 1){
                endrelloc ++;
                printf("%d",endrelloc);
            } else{
                endrel ++;
                printf("%d",endrel);
            }
            dim1 = 0;
            dim2 = 0;
            indar = 1;
            strcpy(arrai, "N/A");
            if (tk.cat == OP_ARIT && tk.codigo == ATRIBUICAO){

                tk = AnaLex(fd);

                    if (tk.codigo == ABRE_CHAVE){

                        tk = AnaLex(fd);
                        if (tk.cat == CONST_INT||tk.cat == CONST_CHAR||tk.cat == CONST_FLOAT){
                            if (tk.cat == CONST_INT) {


                            }
                            else if (tk.cat == CONST_FLOAT) {


                            }
                            else if (tk.cat == CONST_CHAR) {


                            }
                            tk = AnaLex(fd);
                        }
                        else {
                            erro("const char, const int ou const float devem ter pelo menos uma ocorrencia apos as chaves");
                        }
                        while (tk.codigo == VIRGULA){

                            tk = AnaLex(fd);
                            if (tk.cat == CONST_INT||tk.cat == CONST_CHAR||tk.cat == CONST_FLOAT){
                                if (tk.cat == CONST_INT) {


                                }
                                else if (tk.cat == CONST_FLOAT) {


                                }
                                else if (tk.cat == CONST_CHAR) {


                                }
                                tk = AnaLex(fd);
                            }
                            else {
                                erro("const char, const int ou const float devem ter pelo menos uma ocorrencia apos a virgula");
                            }
                        }
                        if (tk.codigo != FECHA_CHAVE){
                            erro("abre chaves nao finalizado");
                        }
                        else{

                            tk = AnaLex(fd);
                        }
                    }
            }
        }else{
            valor = 0;
            fprintf(file, "PUSH 0\n");
            if (escopoAtual == 0){
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrel) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }else{
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrelloc) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }
            if (escopoAtual == 1){
                    fprintf(file, "STOR 1,%d\n", endrelloc);
                    endrelloc ++;
                    printf("%d",endrelloc);
                } else{
                    fprintf(file, "STOR 0,%d\n", endrel);
                    endrel ++;
                    printf("%d",endrel);
                }

        }


    }



}



void PosOuNaoRefer(){
    Tipo();


    tk = AnaLex(fd);
    while (tk.codigo == ABRE_COL){

        tk = AnaLex(fd);
        if (tk.codigo == FECHA_COL){

            tk = AnaLex(fd);
        }else{erro("] esperado");}
    }
}

void DeclDefProc(){
    //FILE *file = fopen("expressao2.txt", "a");
    fprintf(file, "\n\nLABEL L%d\n", lab);
    lab++;
    fprintf(file, "INIPR %d\n", iniprcont);
    //fclose(file);

    escopoAtual++;




    if (tk.codigo == PROT){

        ;

        tk = AnaLex(fd);
        if (tk.cat == ID){
            char lexema[TAM_MAX_LEXEMA];
            strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
            lexema[TAM_MAX_LEXEMA - 1] = '\0';
            int ver = Verif_se_const(tk.lexema);

            valor = 0;
            escopoAtual--;
            strcpy(categ, "PRT");
            strcpy(tipo, "N/A");
            eh_const = 0;

            if (escopoAtual == 0){
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrel) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }else{
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrelloc) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }

            escopoAtual++;

            tk = AnaLex(fd);
            if (tk.codigo == ABRE_PAR){

                    tk = AnaLex(fd);
                    if (tk.codigo == REFERENCIA){

                            tk = AnaLex(fd);
                            PosOuNaoRefer();
                            while (tk.codigo== VIRGULA){

                                tk = AnaLex(fd);
                                if (tk.codigo == REFERENCIA){

                                    tk = AnaLex(fd);
                                    PosOuNaoRefer();
                                }
                                else{
                                    PosOuNaoRefer();
                                }


                            }
                            if (tk.cat == SINAL && tk.codigo == FECHA_PAR){


                                tk = AnaLex(fd);
                            }else {
                                erro("fecha parenteses esperado");
                            }
                    }
                    else{
                        PosOuNaoRefer();
                        while (tk.codigo== VIRGULA){


                                    tk = AnaLex(fd);
                                    if (tk.codigo == REFERENCIA){

                                        tk = AnaLex(fd);
                                        PosOuNaoRefer();
                                        if (tk.codigo != VIRGULA){
                                            if (tk.cat == SINAL && tk.codigo == FECHA_PAR){


                                            tk = AnaLex(fd);
                                            }
                                        }
                                    }
                                    else{
                                        PosOuNaoRefer();
                                        if (tk.cat == SINAL && tk.codigo == FECHA_PAR){


                                            tk = AnaLex(fd);
                                        }
                                    }
                        }
                        if (tk.cat == SINAL && tk.codigo == FECHA_PAR){


                            tk = AnaLex(fd);
                        }
                    }
            }
            else if (tk.cat != SINAL && tk.codigo != FECHA_PAR){
                erro("abre parenteses esperado");
            }
        }
        else if (tk.codigo == INIT){
            erro("init nao pode ser usado como id");
        }
    }
    //agora ta pronto o prot
    else if (tk.codigo == DEF){



        tk = AnaLex(fd);

        if (tk.codigo == INIT){
            char lexema[TAM_MAX_LEXEMA];
            strncpy(lexema, "init", TAM_MAX_LEXEMA - 1);
            lexema[TAM_MAX_LEXEMA - 1] = '\0';
            valor = 0;
            escopoAtual--;
            strcpy(categ, "PRC");
            strcpy(tipo, "N/A");
            eh_const = 0;
            if (escopoAtual == 0){
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrel) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }else{
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrelloc) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }
            escopoAtual++;

            tk = AnaLex(fd);

            while ((tk.cat == PAL_RESERV && (tk.codigo == CONST))||(tk.cat == PAL_RESERV && (tk.codigo == CHAR || tk.codigo == INT||tk.codigo == REAL||tk.codigo == BOOL))) {
                DeclListVar();
            }
            while (tk.cat == ID||tk.codigo == DO||tk.codigo == WHILE||tk.codigo == VAR||tk.codigo == IF||tk.codigo == GETOUT ||tk.codigo == GETINT||tk.codigo == GETREAL||tk.codigo == GETCHAR||tk.codigo == GETSTR||tk.codigo == PUTINT||tk.codigo == PUTREAL||tk.codigo == PUTCHAR||tk.codigo == PUTSTR){

                Cmd();
                if(tk.codigo == ENDI||tk.codigo == ENDV||tk.codigo == ENDW){
                    AnaLex(fd);
                }

            }

            if (tk.codigo == ENDP){

                Imprimir_tabela();
                tk = AnaLex(fd);



            }else{
                erro("endp necessario");
            }

        }
        else if (tk.cat == ID){
            char lexema[TAM_MAX_LEXEMA];
            strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
            lexema[TAM_MAX_LEXEMA - 1] = '\0';
            valor = 0;
            escopoAtual--;
            strcpy(categ, "PRC");
            strcpy(tipo, "N/A");
            eh_const = 0;
            if (escopoAtual == 0){
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrel) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }else{
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrelloc) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }
                    }
            escopoAtual++;

            tk = AnaLex(fd);

            if (tk.codigo == ABRE_PAR){


                PosInitou();
                while(tk.codigo== VIRGULA){

                    c = c + 1;

                    if (c == 2){
                        erro("apos a virgula deve vir & ou tipo");
                    }
                    PosInitou();
                    if (c == 2){
                        erro("apos a virgula deve vir & ou tipo");
                    }
                }
                if (tk.codigo == FECHA_PAR){

                    tk = AnaLex(fd);
                }else{erro("fecha parenteses esperado");}
            }else{erro("abre parenteses esperado");}
            while ((tk.cat == PAL_RESERV && (tk.codigo == CONST))||(tk.cat == PAL_RESERV && (tk.codigo == CHAR || tk.codigo == INT||tk.codigo == REAL||tk.codigo == BOOL))) {
                DeclListVar();
            }
            while (tk.cat == ID||tk.codigo == DO||tk.codigo == WHILE||tk.codigo == VAR||tk.codigo == IF||tk.codigo == GETOUT ||tk.codigo == GETINT||tk.codigo == GETREAL||tk.codigo == GETCHAR||tk.codigo == GETSTR||tk.codigo == PUTINT||tk.codigo == PUTREAL||tk.codigo == PUTCHAR||tk.codigo == PUTSTR){
                Cmd();
                if(tk.codigo == ENDI||tk.codigo == ENDV||tk.codigo == ENDW){
                    AnaLex(fd);
                }
            }

            if (tk.codigo == ENDP){


                Imprimir_tabela();
                tk = AnaLex(fd);

            }else{
                erro("endp necessario2");
            }
        }att_z();

    }

    int Remover_por_escopo(int escopo) {
        int removidos = 0; // Contador de símbolos removidos

        for (int i = 0; i < tabela_idef.tamTab; ) {
            if (tabela_idef.tabela_simb[i].escopo == escopo) {

                for (int j = i; j < tabela_idef.tamTab - 1; j++) {
                    tabela_idef.tabela_simb[j] = tabela_idef.tabela_simb[j + 1];
                }
                tabela_idef.tamTab--;
                removidos++;

            } else {
                i++;
            }
        }

        return removidos; // Retorna o número de símbolos removidos

    }

    endrelloc = 0;
    int removidos = Remover_por_escopo(1);
    printf("\nLinhas removidas do escopo 1: %d\n", removidos);
    Imprimir_tabela();
    escopoAtual--;

}

void PosInitou(){
            //FILE *file = fopen("expressao2.txt", "a");
            strcpy(zombie, "FAlSE");
            escopoAtual = 0;

            tk = AnaLex(fd);
            if(tk.codigo == REFERENCIA){
                strcpy(pas, "REF");
                c = 0;

                tk = AnaLex(fd);
                Tipo();

                tk = AnaLex(fd);
                if (tk.cat == ID){
                        char lexema[TAM_MAX_LEXEMA];
                        strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
                        lexema[TAM_MAX_LEXEMA - 1] = '\0';



                    tk = AnaLex(fd);
                    if (tk.codigo == FECHA_PAR){


                    }
                    while (tk.codigo == ABRE_COL) {



                        tk = AnaLex(fd);
                        if (indar ==1){
                            dim1 = tk.valInt;
                            strcpy(arrai, "VET");
                        }
                        if (indar >= 2){
                            dim2 = tk.valInt;
                            strcpy(arrai, "MAT");
                        }
                        if (tk.cat != CONST_INT && tk.cat != ID){
                            erro("inteiro ou constante pre declarada esperados");
                        }
                        else if (tk.cat == CONST_INT){

                                tk = AnaLex(fd);
                        }
                        else if (tk.cat == ID){
                                if (indar ==1){
                                    dim1 = Buscar_valor_lexema(tk.lexema);
                                    int ver = Verif_se_const(tk.lexema);
                                    if (ver == -1){
                                        printf("< %s > nao foi declarada como constante\n", tk.lexema);
                                        erro("");
                                    }
                                }
                                if (indar >= 2){
                                    dim2 = Buscar_valor_lexema(tk.lexema);
                                    int ver =Verif_se_const(tk.lexema);
                                    if (ver == -1){
                                        printf("< %s > nao foi declarada como constante\n", tk.lexema);
                                        erro("");
                                    }
                                }
                                char lexema[TAM_MAX_LEXEMA];
                                strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
                                lexema[TAM_MAX_LEXEMA - 1] = '\0';

                                if (Buscar_escopo(lexema, escopoAtual) != -1) {

                                    tk = AnaLex(fd);
                                }else{
                                    escopoAtual--;
                                    if(Buscar_escopo(lexema, escopoAtual) != -1){

                                        tk = AnaLex(fd);
                                    }
                                    else{
                                        erro("idconst nao foi previamente declarado");
                                    }
                                    escopoAtual++;

                                }
                        }
                        if (tk.codigo != FECHA_COL){
                            erro("fecha colchete esperado");
                        }
                        else {

                            tk = AnaLex(fd);
                            indar++;
                        }
                    }
                    valor = 0;

                        strcpy(categ, "PAR");
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrelpar) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }


                        endrelpar --;
                        printf("%d",endrelloc);

                    dim1 = 0;
                    dim2 = 0;
                    indar = 1;
                    strcpy(arrai, "N/A");
                }
                else{
                    erro("declare um id");
                }
            }
            else if(tk.cat == PAL_RESERV && (tk.codigo == CHAR || tk.codigo == INT||tk.codigo == REAL||tk.codigo == BOOL)){
                c = 0;
                Tipo();
                tk = AnaLex(fd);
                if (tk.cat == ID){
                    strcpy(pas, "VAL");

                    char lexema[TAM_MAX_LEXEMA];
                    strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
                    lexema[TAM_MAX_LEXEMA - 1] = '\0';

                    if (escopoAtual == 1){
                        strcpy(categ, "PAR");
                        escopoAtual --;
                        if (Buscar_escopo(lexema, escopoAtual) != -1) {
                            erro("Identificador ja foi declarado como global");
                        }
                        escopoAtual ++;
                    }
                    tk = AnaLex(fd);
                    if (tk.codigo == FECHA_PAR){


                    }
                    while(tk.codigo == ABRE_COL){

                        tk = AnaLex(fd);
                        if (indar ==1){
                            dim1 = tk.valInt;
                            strcpy(arrai, "VET");
                        }
                        if (indar >= 2){
                            dim2 = tk.valInt;
                            strcpy(arrai, "MAT");
                        }
                        if (tk.cat != CONST_INT && tk.cat != ID){
                            erro("inteiro ou constante pre declarada esperados");
                        }
                        else if (tk.cat == CONST_INT){

                                tk = AnaLex(fd);
                        }
                        else if (tk.cat == ID){
                                if (indar ==1){
                                    dim1 = Buscar_valor_lexema(tk.lexema);
                                    int ver = Verif_se_const(tk.lexema);
                                    if (ver == -1){
                                        printf("< %s > nao foi declarada como constante\n", tk.lexema);
                                        erro("");
                                    }
                                }
                                if (indar >= 2){
                                    dim2 = Buscar_valor_lexema(tk.lexema);
                                    int ver =Verif_se_const(tk.lexema);
                                    if (ver == -1){
                                        printf("< %s > nao foi declarada como constante\n", tk.lexema);
                                        erro("");
                                    }
                                }
                                char lexema[TAM_MAX_LEXEMA];
                                strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
                                lexema[TAM_MAX_LEXEMA - 1] = '\0';

                                if (Buscar_escopo(lexema, escopoAtual) != -1) {

                                    tk = AnaLex(fd);
                                }else{
                                    escopoAtual--;
                                    if(Buscar_escopo(lexema, escopoAtual) != -1){

                                        tk = AnaLex(fd);
                                    }
                                    else{
                                        erro("idconst nao foi previamente declarado");
                                    }
                                    escopoAtual++;

                                }
                        }
                        if (tk.codigo != FECHA_COL){
                            erro("fecha colchete esperado");
                        }
                        else {

                            tk = AnaLex(fd);
                            indar++;
                        }
                    }
                    valor = 0;
                    strcpy(categ, "PAR");
                        if (Insercao_tabela(lexema, escopoAtual, valor, categ,arrai,tipo, zombie,eh_const,dim1,dim2,pas,endrelpar) == -1) {
                            erro("Não foi possível inserir o identificador na tabela de símbolos.\n");
                        }


                    endrelpar --;
                    printf("%d",endrelloc);
                    dim1 = 0;
                    dim2 = 0;
                    indar = 1;
                    strcpy(arrai, "N/A");

                }
                else{
                    erro("declare um id");
                }
            }
    strcpy(pas, "N/A");
    strcpy(zombie, "N/A");
    escopoAtual = 1;

}
void Atrib() {


	if (tk.cat == ID) {

        tk = AnaLex(fd);
        while (tk.codigo == ABRE_COL){

            tk = AnaLex(fd);
            Expr();
            if (tk.codigo == FECHA_COL){

                tk = AnaLex(fd);
            }
        }
        if (tk.codigo == ATRIBUICAO){

            tk = AnaLex(fd);
            Expr();
        }
        else{
            erro("atribuicao esperada");
        }
	}
	else{
        erro("ID esperado");
	}


}

void ExprSimp(){


    if (tk.codigo == ADICAO && tk.cat == OP_ARIT){

        tk = AnaLex(fd);
        Termo();
        if (tk.codigo != FECHA_PAR){
            while((tk.codigo == ADICAO||tk.codigo == SUBTRACAO||tk.codigo == OR_LOGIC)&& tk.cat == OP_ARIT){
                if (tk.codigo == ADICAO){

                    tk = AnaLex(fd);
                    Termo();
                }
                else if (tk.codigo == SUBTRACAO){

                    tk = AnaLex(fd);
                    Termo();
                }
                else if (tk.codigo == OR_LOGIC){

                    tk = AnaLex(fd);
                    Termo();
                }
            }
        }
    }
    else if (tk.codigo == SUBTRACAO && tk.cat == OP_ARIT){

        tk = AnaLex(fd);
        Termo();

        if (tk.codigo != FECHA_PAR){
            while(tk.codigo == ADICAO||tk.codigo == SUBTRACAO||tk.codigo == OR_LOGIC ){
                if (tk.codigo == ADICAO){

                    tk = AnaLex(fd);
                    Termo();
                }
                else if (tk.codigo == SUBTRACAO){

                    tk = AnaLex(fd);
                    Termo();
                }
                else if (tk.codigo == OR_LOGIC){

                    tk = AnaLex(fd);
                    Termo();
                }
            }
        }
    }
    else{
        Termo();
        while(tk.cat== OP_ARIT &&(tk.codigo == ADICAO||tk.codigo == SUBTRACAO||tk.codigo == OR_LOGIC) ){
            if (tk.cat== OP_ARIT && tk.codigo == ADICAO){

                tk = AnaLex(fd);
                Termo();
            }
            else if (tk.codigo == SUBTRACAO){

                tk = AnaLex(fd);
                Termo();
            }
            else if (tk.codigo == OR_LOGIC){

                tk = AnaLex(fd);
                Termo();
            }
        }
    }

}
void Termo(){


	Fator();

    while((tk.codigo==MULTIPLICACAO||tk.codigo==DIVISAO)||tk.codigo==AND_LOGIC){
        if (tk.codigo==MULTIPLICACAO){

            tk = AnaLex(fd);
            Termo();
        }
        else if (tk.codigo==DIVISAO && tk.cat == OP_ARIT){

            tk = AnaLex(fd);
            Termo();
        }
        else if (tk.codigo==AND_LOGIC){

            tk = AnaLex(fd);
            Termo();
        }
    }

}
void Cmd(){



    if (tk.cat == ID){

        Atrib();

    }
    else if (tk.codigo==DO){
                tk = AnaLex(fd);

                if (tk.cat == ID){

                    tk = AnaLex(fd);
                    if (tk.codigo == ABRE_PAR){

                        tk = AnaLex(fd);
                        if(tk.codigo == ADICAO||tk.codigo == SUBTRACAO||tk.cat == ID){
                            Expr();
                            while(tk.codigo == VIRGULA){

                                tk = AnaLex(fd);
                                Expr();
                            }
                        }
                        if (tk.codigo == FECHA_PAR){

                            tk = AnaLex(fd);

                        }else{erro("insira fecha parenteses1");}
                    }else{erro("insira abre parenteses");}
                }else{erro("insira id");}
        }
        else if (tk.codigo == WHILE){
                    //FILE *file = fopen("expressao2.txt", "a");
                    fprintf(file, "\n\nLABEL %d\n", lab);
                    lab++;
                    //fclose(file);

                    tk = AnaLex(fd);

                    if (tk.codigo == ABRE_PAR){
                        tk = AnaLex(fd);

                        Expr();
                        if (tk.codigo == FECHA_PAR){
                            tk = AnaLex(fd);

                        }else if (tk.codigo != FECHA_PAR){erro("insira fecha parenteses2");}
                    }else{erro("insira abre parenteses");}
                    while (tk.cat == ID||tk.codigo == DO||tk.codigo == WHILE||tk.codigo == VAR||tk.codigo == IF||tk.codigo == GETOUT ||tk.codigo == GETINT||tk.codigo == GETREAL||tk.codigo == GETCHAR||tk.codigo == GETSTR||tk.codigo == PUTINT||tk.codigo == PUTREAL||tk.codigo == PUTCHAR||tk.codigo == PUTSTR){
                        Cmd();

                    }
                    if(tk.codigo == ENDW){
                        tk = AnaLex(fd);


                    }else if(tk.codigo != ENDW){erro("insira endw");}
        }
        else if (tk.codigo == VAR){
                  AnaLex(fd);

                  if (tk.cat == ID){

                        tk = AnaLex(fd);
                        if(tk.codigo == FROM){

                            tk = AnaLex(fd);
                            Expr();

                            if (tk.codigo == TO){

                                tk = AnaLex(fd);
                                Expr();
                                if(tk.codigo == BY){

                                    tk = AnaLex(fd);
                                    if(tk.cat == CONST_INT){

                                        tk = AnaLex(fd);
                                        while (tk.cat == ID||tk.codigo == DO||tk.codigo == WHILE||tk.codigo == VAR||tk.codigo == IF||tk.codigo == GETOUT ||tk.codigo == GETINT||tk.codigo == GETREAL||tk.codigo == GETCHAR||tk.codigo == GETSTR||tk.codigo == PUTINT||tk.codigo == PUTREAL||tk.codigo == PUTCHAR||tk.codigo == PUTSTR){
                                                Cmd();
                                                if(tk.codigo == ENDI||tk.codigo == ENDW){
                                                    AnaLex(fd);
                                                }
                                        }
                                        if (tk.codigo == ENDV){

                                            tk = AnaLex(fd);
                                        }else{erro("endv esperado");}
                                    }
                                    else if(tk.cat == ID){
                                        char lexema[TAM_MAX_LEXEMA];
                                        strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
                                        lexema[TAM_MAX_LEXEMA - 1] = '\0';

                                        if (Buscar_escopo(lexema, escopoAtual) != -1) {

                                            tk = AnaLex(fd);
                                        }else{
                                            erro("idconst nao foi previamente declarado");
                                        }
                                        while (tk.cat == ID||tk.codigo == DO||tk.codigo == WHILE||tk.codigo == VAR||tk.codigo == IF||tk.codigo == GETOUT ||tk.codigo == GETINT||tk.codigo == GETREAL||tk.codigo == GETCHAR||tk.codigo == GETSTR||tk.codigo == PUTINT||tk.codigo == PUTREAL||tk.codigo == PUTCHAR||tk.codigo == PUTSTR){
                                                Cmd();

                                        }
                                        if (tk.codigo == ENDV){

                                            tk = AnaLex(fd);

                                        }else{erro("endv esperado");}
                                    }else{erro("após by deve vir Const Int ou ID");}
                                }else{
                                    if (tk.codigo == ENDV){

                                            tk = AnaLex(fd);
                                        }else{erro("endv esperado");}
                                }
                            }
                            else if (tk.codigo == DT){

                                tk = AnaLex(fd);
                                Expr();
                                if(tk.codigo == BY){

                                    tk = AnaLex(fd);
                                    if(tk.cat == CONST_INT){

                                        tk = AnaLex(fd);
                                        while (tk.cat == ID||tk.codigo == DO||tk.codigo == WHILE||tk.codigo == VAR||tk.codigo == IF||tk.codigo == GETOUT ||tk.codigo == GETINT||tk.codigo == GETREAL||tk.codigo == GETCHAR||tk.codigo == GETSTR||tk.codigo == PUTINT||tk.codigo == PUTREAL||tk.codigo == PUTCHAR||tk.codigo == PUTSTR){
                                                Cmd();

                                        }
                                        if (tk.codigo == ENDV){

                                            AnaLex(fd);
                                        }else{erro("endv esperado");}
                                    }
                                    else if(tk.cat == ID){
                                        char lexema[TAM_MAX_LEXEMA];
                                        strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
                                        lexema[TAM_MAX_LEXEMA - 1] = '\0';

                                        if (Buscar_escopo(lexema, escopoAtual) != -1) {

                                            tk = AnaLex(fd);
                                        }else{
                                            erro("idconst nao foi previamente declarado");
                                        }
                                        while (tk.cat == ID||tk.codigo == DO||tk.codigo == WHILE||tk.codigo == VAR||tk.codigo == IF||tk.codigo == GETOUT ||tk.codigo == GETINT||tk.codigo == GETREAL||tk.codigo == GETCHAR||tk.codigo == GETSTR||tk.codigo == PUTINT||tk.codigo == PUTREAL||tk.codigo == PUTCHAR||tk.codigo == PUTSTR){
                                                Cmd();

                                        }
                                        if (tk.codigo == ENDV){

                                            AnaLex(fd);
                                        }else{erro("endv esperado");}
                                    }else{erro("após by deve vir Const Int ou ID");}
                                }
                            }else{erro("dt ou to esperados");}
                        }else {erro("from esperado");}
                  }else {erro("id esperado");}
        }
        else if (tk.cat == PAL_RESERV && tk.codigo == IF) {
            //FILE *file = fopen("expressao2.txt", "a");
            fprintf(file, "\n\nLABELif %d\n", lab);
            lab++;

            AnaLex(fd);

            if (tk.codigo == ABRE_PAR) {
                AnaLex(fd);


                Expr();
                if (tk.codigo == FECHA_PAR) {
                    AnaLex(fd);

                } else {
                    erro("fecha parenteses esperado");
                }
                while (tk.cat == ID||tk.codigo == DO||tk.codigo == WHILE||tk.codigo == VAR||tk.codigo == IF||tk.codigo == GETOUT ||tk.codigo == GETINT||tk.codigo == GETREAL||tk.codigo == GETCHAR||tk.codigo == GETSTR||tk.codigo == PUTINT||tk.codigo == PUTREAL||tk.codigo == PUTCHAR||tk.codigo == PUTSTR) {
                    Cmd();
                }

                while (tk.codigo == ELIF) {
                    fprintf(file, "\n\nLABEL %d\n", lab);
                    lab++;

                    AnaLex(fd);

                    if (tk.codigo == ABRE_PAR) {
                        AnaLex(fd);


                        Expr();
                        if (tk.codigo == FECHA_PAR) {
                            AnaLex(fd);

                        } else {
                            erro("fecha parenteses esperado");
                        }
                        while (tk.cat == ID||tk.codigo == DO||tk.codigo == WHILE||tk.codigo == VAR||tk.codigo == IF||tk.codigo == GETOUT ||tk.codigo == GETINT||tk.codigo == GETREAL||tk.codigo == GETCHAR||tk.codigo == GETSTR||tk.codigo == PUTINT||tk.codigo == PUTREAL||tk.codigo == PUTCHAR||tk.codigo == PUTSTR) {
                            Cmd();
                        }
                    } else {
                        erro("abre parenteses esperado");
                    }
                }

                if (tk.codigo == ELSE) {
                    fprintf(file, "\n\nLABELelse %d\n", lab);

                    lab++;

                    AnaLex(fd);

                    while (tk.cat == ID||tk.codigo == DO||tk.codigo == WHILE||tk.codigo == VAR||tk.codigo == IF||tk.codigo == GETOUT ||tk.codigo == GETINT||tk.codigo == GETREAL||tk.codigo == GETCHAR||tk.codigo == GETSTR||tk.codigo == PUTINT||tk.codigo == PUTREAL||tk.codigo == PUTCHAR||tk.codigo == PUTSTR) {
                        Cmd();
                    }
                }

                if (tk.codigo == ENDI) {

                    AnaLex(fd);
                } else {
                    erro("endi esperado");
                }
            } else {
                erro("abre parenteses esperado");
            }
            //fclose(file);
        }



        else if (tk.codigo == GETOUT){

                     AnaLex(fd);

        }
        else if (tk.codigo == GETINT){
                     AnaLex(fd);

                     if (tk.cat == ID){

                        tk = AnaLex(fd);
                     }else {erro("espera id");}
        }
        else if (tk.codigo == GETREAL){
                     AnaLex(fd);

                     if (tk.cat == ID){

                        AnaLex(fd);
                     }else {erro("espera id");}
        }
        else if (tk.codigo == GETCHAR){
                     AnaLex(fd);

                     if (tk.cat == ID){

                        AnaLex(fd);
                     }else {erro("espera id");}
        }
        else if (tk.codigo == GETSTR){
                     AnaLex(fd);
                     ;
                     if (tk.cat == ID){

                        AnaLex(fd);
                     }else {erro("espera id");}
        }
        else if (tk.codigo == PUTINT){
                     AnaLex(fd);

                     if (tk.cat == ID){

                        AnaLex(fd);
                     }else if (tk.cat == CONST_INT){

                        AnaLex(fd);
                     }else {erro("espera id ou const int");}
        }
        else if (tk.codigo == PUTREAL){
                     AnaLex(fd);

                     if (tk.cat == ID){

                        AnaLex(fd);
                     }else if (tk.cat == CONST_FLOAT){

                        AnaLex(fd);
                     }else {erro("espera id ou const float");}
        }
        else if (tk.codigo == PUTCHAR){
                     AnaLex(fd);

                     if (tk.cat == ID){

                        AnaLex(fd);
                     }else if (tk.cat == CONST_CHAR){

                        AnaLex(fd);
                     }else {erro("espera id ou charcon");}
        }
        else if (tk.codigo == PUTSTR){
                     AnaLex(fd);

                     if (tk.cat == ID){

                        AnaLex(fd);
                     }else if (tk.cat == LITERAL){

                        AnaLex(fd);
                     }else {erro("espera id ou literal");}
        }

}

void Expr() {


    tokenop = 0;
    ExprSimp();
    if (tk.cat==OP_RELAC){

        OpRel();
        ExprSimp();
        //FILE *file = fopen("expressao2.txt", "a");
        if (tokenop==1){

            fprintf(file, "SUB\n");
            fprintf(file, "GOTRUE L\n");
        }
        if (tokenop==2){


        }
        if (tokenop==3){
            //<=
            fputs("SUB\n",file);
            fprintf(file, "GOTRUE L%d\n",lab);//CASO FALSO
            fprintf(file, "PUSH 1\n");
            lab++;
            fprintf(file, "GOTO L%d\n",lab);
            lab--;
            fprintf(file, "LABEL L%d\n",lab);
            fprintf(file, "PUSH 0");
            lab++;


        }
        if (tokenop==4){


        }
        if (tokenop==5){


        }
        if (tokenop==6){


        }
    }

}
void OpRel(){

    if (tk.cat == OP_RELAC){
        if (tk.codigo == IGUALDADE){

            tk = AnaLex(fd);
            tokenop=1;
        }
        if (tk.codigo == DIFERENTE){

            tk = AnaLex(fd);
            tokenop=2;
        }
        if (tk.codigo ==MENOR_IGUAL ){

            tk = AnaLex(fd);
            tokenop=3;
        }
        if (tk.codigo == MAIOR_IGUAL){

            tk = AnaLex(fd);
            tokenop=4;
        }
        if (tk.codigo == MENOR_QUE){

            tk = AnaLex(fd);
            tokenop=5;
        }
        if (tk.codigo == MAIOR_QUE){

            tk = AnaLex(fd);
            tokenop=6;
        }
    }else{erro("Operador relacional esperado");}

}
void Fator() {
    //FILE *file = fopen("expressao2.txt", "a");

    if (tk.cat==ID){
        end = Buscar_endereco_lexema(tk.lexema);

        fprintf(file, "LOAD %d\n", end);


        tk = AnaLex(fd);
        while (tk.codigo == ABRE_COL){

            tk = AnaLex(fd);
            Expr();

            if(tk.codigo == FECHA_COL){

                tk = AnaLex(fd);
            }else if(tk.codigo != FECHA_COL) {erro("fecha colchete esperado");}
        }

    }
    else if (tk.cat == CONST_INT){

        fprintf(file, "PUSH %d\n",tk.valInt);
        tk = AnaLex(fd);


    }
    else if (tk.cat == CONST_FLOAT){

        tk = AnaLex(fd);

    }
    else if (tk.cat == CONST_CHAR){

        tk = AnaLex(fd);

    }
    else if (tk.codigo == ABRE_PAR){

        tk = AnaLex(fd);

        Expr();
        if (tk.codigo == FECHA_PAR){

            tk = AnaLex(fd);
        }
    }
    else if(tk.codigo == NOT_LOGIC){

        tk = AnaLex(fd);
        Fator();
    }

}

