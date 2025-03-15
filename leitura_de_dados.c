/**
 * @brief Módulo para a leitura de dados provenientes do utilizador.
 * 
 * @file leitura_de_dados.c
 * @author Pedro Vicente ist1109852
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**Valores "boleanos" para determinação se algo é verdade ou não.*/
#define FALSE 0
#define TRUE 1

typedef struct nodeArg{
    char* palavra;
    struct nodeArg *proximo, *anterior;
}NodeArg;


typedef struct{
    struct nodeArg *primeiroParametro, *ultimoParametro;
    int comprimento;
}ListArg;


/**
 * @brief Liberta toda a memória alocada para lista que contém os parâmetros
 * da linha de comandos introduzida pelo utilizador.
 * 
 * @param node o último node da lista
 */
void freeNodeArg(NodeArg* node){
    if (node!=NULL){
        free(node->palavra);
        freeNodeArg(node->anterior);
        free(node);
    }
}


/**
 * @brief toda a memória alocada para a lista e os parâmetros da linha
 * comandos.
 * 
 * @param lista a lista que tem o primeiro e último parâmetros da linha
 * de comandos introduzida
 */
void  freeList(ListArg *lista){

    freeNodeArg(lista->ultimoParametro);
    free(lista);

}


/**
 * @brief Alcoa memória para uma estrutura lista.
 * 
 * @return ListArg* 
 */
ListArg* criaListaArg(){

    ListArg * lista = (ListArg*)malloc(sizeof(ListArg));

    lista->comprimento = 0;
    lista->primeiroParametro=NULL;
    lista->ultimoParametro =NULL; 
    
    return lista;
}


/**
 * @brief Adiciona ao fim de uma dada lista um dado parametro obtido do
 * input do utilizador.
 * 
 * @param lista a lista de argumentos a que se pretende adcionar 
 * o parametro
 * @param palavra a palavra que se pretende adicionar a lista de 
 * argumentos
 */
void adicionaFim(ListArg* lista, char* palavra){

    /**Criação de um nove NodeArg que terá um novo parâmetro da
     * linha de comandos.
    */
    NodeArg* new = (NodeArg*)malloc(sizeof(NodeArg));
    new->palavra = (char*)malloc(sizeof(char)*(strlen(palavra)+1));
    strncpy(new->palavra, palavra, strlen(palavra)+1);
    new->proximo = NULL;
    new->anterior = lista->ultimoParametro;

    /**Adiciona o parâmetro ao fim da lista.*/
    if (lista->ultimoParametro == NULL){
        lista->ultimoParametro = new;
        lista->primeiroParametro = new;
    }
    else{
        lista->ultimoParametro->proximo = new;
        lista->ultimoParametro=new;
    }

    /**aumenta o comprimento da lista de argumentos*/
    lista->comprimento++;
}


/**
 * @brief Lê uma linha de input introduzida pelo utilizador e devolve-a.
 * 
 * @return char* 
 */
char* leLinha(){

    int c, comprimento = 0;
    char * input = (char*)malloc(sizeof(char)*BUFSIZ);
    char * pinput = input;

    while ((c = getchar())!= '\n'){
        *pinput++ = c;
        comprimento++;
    }

    *pinput++ = ' ';
    *pinput = '\0'; 
    input = (char*)realloc(input, sizeof(char)*(comprimento + 2));

    return input;
}


/**
 * @brief De um determinado input introduzido pelo utilizador 
 * adiciona a uma lista de argumentos os respetivos
 * parâmetros introduzidos.
 * 
 * @param listaArgumentos uma lista onde se vai colocar os
 * respetivos argumentos
 * @param input o vetor de caracteres introuzida pelo 
 * utilizador
 */
void adicionaArgumentos(ListArg* listaArgumentos, char*input){
    char *pinput = input, * temp = NULL, *ptemp = NULL;
    int dentro = FALSE, aspas = FALSE, comprimento = strlen(input) + 1, aux;

     do {
        if(*input == '"') aspas = !aspas;
        /*inciar a captura de um parâmetro*/
        else if (*input != ' ' && dentro == FALSE) {
            
            temp = (char*)malloc(sizeof(char) * comprimento);
            dentro = TRUE;
            ptemp = temp;
            *ptemp++ = *input;
        }
        /*captar a palavra introduzida*/
        else if ((*input != ' ' && dentro == TRUE && aspas == FALSE) ||
                (*input != '"' && aspas == TRUE)) 
            *ptemp++ = *input;

        /*finalizar o vetor de caracteres*/
        else if ((*input == ' ' && dentro == TRUE && aspas == FALSE) ||
                (*input == '"' && aspas == TRUE)) {
            aspas = dentro = FALSE;
            *ptemp = '\0';
            aux = strlen(temp) + 1;
            temp = (char*)realloc(temp, sizeof(char) * aux);
            adicionaFim(listaArgumentos, temp);
            free(temp);
        }
        input++, comprimento--;
    } while (*input != '\0');

    free(pinput);

}


/**
 * @brief Cria uma lista vazia, lê uma linha dada pelo utilizador e vai
 * adicionando no final da lista os respetivos parametros obtidos
 * (incrementado também o comprimento de parametros que tem a lista).
 * 
 * @return ListArg* 
 */
ListArg * obtemArgumentos() {

    /*criar a lista de argumentos e obter a linha 
    dada pelo utilizador*/
    ListArg * listaArgumentos = criaListaArg();
    char* input = leLinha();

    /*Adiconar os argumentos*/
    adicionaArgumentos(listaArgumentos, input);

    return listaArgumentos;
}


