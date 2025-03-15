/**
 * @brief Conjunto de funções e estruturas que permitem 
 * controlar e criar veiculos e registos associados a 
 * suas entradas e saídas de um determinado parque de 
 * estacionamento.
 * 
 * @file veiculos_registos.c
 * @author Pedro Vicente ist1109852
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datas_horas.h"

/**Valor dado a um acontecimento falso.*/
#define FALSE 0

/**Valor dado a um acontecimento verdadeiro.*/
#define TRUE 1

/**
 * Valor dado se um vetor de caracteres for só composto
 * por letras.
*/
#define LETRAS 2

/**
 * Valor dado se um vetor de caracteres for só composto
 * por números.
*/
#define NUMEROS 3

/**Número de pares que uma matrícula tem*/
#define NPARES 3

/**O hashvalue determinado para a hashTable*/
#define HASHVALUE 151
/**O valor primo auxiliar no cálculo da hashkey*/
#define PRIME1 211

/**Outro valor primo auxiliar no cálculo da hashkey*/
#define PRIME2 479

/**Número máximo de caracteres numa matrícula*/
#define MATRICULA 9

/**Número de caracteres que tem cada parte da matrícula*/
#define PAR 2

typedef struct nodeRegisto{
    char matricula[MATRICULA];
    DataCompleta* entrada;
    DataCompleta* saida;
    float faturado;
    struct nodeRegisto* proximoRegisto, *registoAnterior;
}NodeRegisto;

typedef struct listRegisto{
    NodeRegisto* primeiroRegisto;
    NodeRegisto* ultimoRegisto;
}ListRegisto;

typedef struct{
    char* parque;
    char matricula[MATRICULA];
    DataCompleta* entrada;
    NodeRegisto* registoVeiculo;
}Veiculo;

typedef struct nodeVeiculo{
    char chave[MATRICULA];
    Veiculo *veiculo;
    struct nodeVeiculo* proximaEntrada;

}NodeVeiculo;

typedef struct{
    int tamanho;  
    NodeVeiculo** table;  
}HashTable;


/**
 * @brief Aloca a memória necessária para a criação de um 
 * veículo.
 * 
 * @param matricula um vetor de caracteres com o formato de
 * uma matŕicula
 * @return Veiculo* 
 */
Veiculo* criaVeiculo(char* matricula){
    Veiculo* veiculo = (Veiculo*)malloc(sizeof(Veiculo));
    strcpy(veiculo->matricula, matricula);
    veiculo->entrada = NULL;
    veiculo->parque = NULL;
    veiculo->registoVeiculo = NULL;
    return veiculo;
}


/**
 * @brief Liberta toda a memória associada a um determinado 
 * veículo.
 * 
 * @param veiculo o Veículo a qual se pretende libertar a 
 * memória
 */
void freeVeiculo(Veiculo* veiculo){
    freeDataCompleta(veiculo->entrada);
    free(veiculo);
}


/**
 * @brief Aloca memória para um novo node da lista de 
 * registos.
 * 
 * @return NodeVeiculo* 
 */
NodeVeiculo* criaNodeVeiculo(){
    NodeVeiculo* new = (NodeVeiculo*)malloc(sizeof(NodeVeiculo));
    new->proximaEntrada = NULL;
    new->veiculo = NULL;

    return new;
}


/**
 * @brief Liberta toda a memória associada aos elementos da 
 * hastable e da estrutura em si.
 * 
 * @param hashtable a HashTable que se pretende libertar
 */
void freeHashTable(HashTable* hashtable) {

    /*libertar a memoria associada a todas as entradas da
    hashtable e lista*/
    for (int i = 0; i < hashtable->tamanho; i++) {
        NodeVeiculo* current = hashtable->table[i]; 
        while (current != NULL) {
            NodeVeiculo* temp = current; 
            current = current->proximaEntrada;
            free(temp->veiculo); 
            free(temp); 
        }
    }
    /*libertar a memoria associada a estrutura hashtable*/
    free(hashtable->table);
    free(hashtable); 
}


/**
 * @brief Cria o valor hash, ou seja, o local onde o 
 * suposto veículo se encontra.
 * 
 * @param matricula a matrícula de um dado carro
 * @param valor o hashvalue da HashTable
 * @return int 
 */
int hash(char* matricula, int valor){

    int chave, a = PRIME1, b = PRIME2;
    for(chave = 0; *matricula != '\0'; matricula++, a = a*b %(valor-1))
        chave = (a*chave + *matricula) % valor;
    return chave;

}


/**
 * @brief A função cria uma HashTable com um tamanho igual ao 
 * hashvalue.
 * 
 * @param tamanho o tamanho/hashvalue da HashTable
 * @return HashTable* 
 */
HashTable* criaHashtable(int tamanho) {
    HashTable* new = (HashTable*)malloc(sizeof(HashTable));
    new->tamanho = tamanho;
    new->table = (NodeVeiculo**)malloc(tamanho * sizeof(NodeVeiculo*));

    for (int i = 0; i < tamanho; i++) {
        new->table[i] = NULL;
    }
    return new;
}


/**
 * @brief Insere o veículo na entrada correspondente na 
 * HashTable.
 * 
 * @param hashtable a HashTable a que se pretende adicionar
 * @param chave a matrícula do veículo
 * @param veiculo o Veículo em si
 */
void insereVeiculo(HashTable* hashtable, char* chave, Veiculo* veiculo) {
    unsigned int indice = hash(chave, hashtable->tamanho);
    NodeVeiculo* new = criaNodeVeiculo();
    new->veiculo= veiculo;
    strcpy(new->chave, chave);
    new->proximaEntrada = hashtable->table[indice];
    hashtable->table[indice] = new;
}


/**
 * @brief Procura na HashTable um determinado carro com a 
 * matrícula igual a chave introduzida.
 * 
 * @param hashtable a HashTable em que se pretende procurar
 * @param chave a matrícula do veículo que se pretende encontrar
 * @return Veiculo* 
 */
Veiculo* procuraVeiculo(HashTable* hashtable, char* chave){
    unsigned int indice = hash(chave, hashtable->tamanho);
    NodeVeiculo* entrada = hashtable->table[indice];
    while (entrada != NULL){
        if (strcmp(entrada->chave, chave) == 0) {
            return entrada->veiculo; 
        }
        entrada = entrada->proximaEntrada;
    }
    
    return NULL;  
}


/**
 * @brief Liberta toda a memória associada aos elementos da 
 * lista de registos.
 * 
 * @param registo o primeiro NodeRegisto da lista
 */
void freeNodeRegisto(NodeRegisto* registo){
    if (registo!=NULL){
        freeNodeRegisto(registo->proximoRegisto);
        freeDataCompleta(registo->entrada);
        if (registo->saida != NULL){
            freeDataCompleta(registo->saida);
        }
        
        free(registo);
    }
}


/**
 * @brief Liberta toda memória associada à lista e aos 
 * seus elementos.
 * 
 * @param lista uma lista de Registos
 */
void  freeListRegisto(ListRegisto* lista){

    freeNodeRegisto(lista->primeiroRegisto);
    free(lista);

}


/**
 * @brief Aloca mamória para uma nova lista de Registos e
 * devolve-a vazia.
 * 
 * @return ListRegisto* 
 */
ListRegisto* cria_listaRegisto(){

    ListRegisto * lista = (ListRegisto*)malloc(sizeof(ListRegisto));

    lista->primeiroRegisto=NULL;
    lista->ultimoRegisto =NULL; 
    
    return lista;
}


/**
 * @brief Adciona um Registo de entrada à lista de Registos.
 * 
 * @param lista a lista de Registos de um determinado parque
 * @param matricula a matrícula de um veículo
 * @param entrada a DataCompleta associada à entrada do veículo
 * @return NodeRegisto* 
 */
NodeRegisto* adicionaEntrada(ListRegisto* lista, char matricula[], 
                    DataCompleta* entrada){
    
    /*criar um novo node*/
    NodeRegisto* novo = (NodeRegisto*)malloc(sizeof(NodeRegisto));

    /*criar um novo registo*/
    novo->entrada = entrada;
    strcpy(novo->matricula, matricula);
    novo->saida = NULL;
    novo->faturado = 0;
    novo->proximoRegisto = NULL;
    novo->registoAnterior= lista->ultimoRegisto;
    /*adicionar o registo ao fim da lista*/ 
    if (lista->primeiroRegisto == NULL){
        lista->ultimoRegisto = novo;
        lista->primeiroRegisto = novo;
    }
    else{
        lista->ultimoRegisto->proximoRegisto = novo;
        lista->ultimoRegisto = novo;
    }

    return novo;
}


/**
 * @brief Remove o elemento da lista libertar a memória associada 
 * ao mesmo.
 * 
 * @param listaRegisto lista de registos de um parque
 * @param registoVeiculo o elemento que se pretende remover de uma
 * dada posição
 */
void removeElemento(ListRegisto* listaRegisto,
                    NodeRegisto* registoVeiculo){
    
    if(registoVeiculo->registoAnterior == NULL &&
        registoVeiculo->proximoRegisto==NULL){
        listaRegisto->primeiroRegisto =NULL;
        listaRegisto->ultimoRegisto =NULL;
    }
            
    else if(registoVeiculo->registoAnterior== NULL){
        listaRegisto->primeiroRegisto = registoVeiculo->proximoRegisto;
        listaRegisto->primeiroRegisto->registoAnterior = NULL;
    }
            
    else if(registoVeiculo->proximoRegisto == NULL){
        listaRegisto->ultimoRegisto = registoVeiculo->registoAnterior;
        listaRegisto->ultimoRegisto->proximoRegisto = NULL;
    }

    if(registoVeiculo->registoAnterior != NULL &&
        registoVeiculo->proximoRegisto != NULL){

        registoVeiculo->registoAnterior->proximoRegisto =
        registoVeiculo->proximoRegisto;

        registoVeiculo->proximoRegisto->registoAnterior =
        registoVeiculo->registoAnterior;
    }
                    
}


/**
 * @brief Altera um registo de entrada obtido de um 
 * veiculo, de forma a corresponder a saída do veículo,
 * remove-o da posição atual e coloca-o no fim da 
 * lista de Registos.
 * 
 * @param listaRegisto a lista de registos de um dado parque
 * @param registoVeiculo o elemento da lista que corresponde
 * a entrada de um dado veículo
 * @param saida a DataCompleta associada a saída do mesmo
 * @param faturado o valor faturado entre essas datas
 */
void adicionaSaida(ListRegisto* listaRegisto, 
                    NodeRegisto* registoVeiculo, 
                    DataCompleta* saida, float faturado){
    
    registoVeiculo->saida = saida;
    registoVeiculo->faturado = faturado;

    /*quando o parque for encontrado alterar a estrutura da lista*/
    removeElemento(listaRegisto, registoVeiculo);
    
    registoVeiculo->proximoRegisto = NULL;
    registoVeiculo->registoAnterior= listaRegisto->ultimoRegisto;

    /*adicionar o registo ao fim da lista*/ 
    if (listaRegisto->primeiroRegisto == NULL){
        listaRegisto->ultimoRegisto = registoVeiculo;
        listaRegisto->primeiroRegisto = registoVeiculo;
    }
    else{
        listaRegisto->ultimoRegisto->proximoRegisto = registoVeiculo;
        listaRegisto->ultimoRegisto = registoVeiculo;
    }

 
}


/**
 * @brief Valida se um dado par é válido, ou seja,
 * se ambos são letras ou números.
 * Devolve LETRAS se ambos os caracteres forem 
 * letras, NUMEROS se ambos forem números,
 * FALSE caso contrário.
 * 
 * @param par um vetor de caracteres
 * @return int 
 */
int validaPar(char par[]){
    if (par[0] >= 'A' && par[0]<='Z' &&
        par[1] >= 'A' && par[1] <= 'Z')
        return LETRAS;
    if (par[0] >= '0' && par[0] <= '9' &&
        par[1] >= '0' && par[1] <= '9')
        return NUMEROS;
    
    return FALSE;

}


/**
 * @brief Verifica se uma dada matrícula é válida.
 * Devolve TRUE se for válida, FALSE caso contrário.
 * 
 * @param matricula um vetor de caracteres com 
 * formato de matrícula
 * @return int 
 */
int validaMatricula(char matricula[]){
    char pares[NPARES][PAR] = {{matricula[0], matricula[1]},
                        {matricula[3], matricula[4]},
                        {matricula[6], matricula[7]}}; 

    if (validaPar(pares[0]) == FALSE ||
        validaPar(pares[1]) == FALSE ||
        validaPar(pares[2]) == FALSE)
            return FALSE;
    
    if (validaPar(pares[0]) == LETRAS &&
        validaPar(pares[1]) == LETRAS &&
        validaPar(pares[2]) == LETRAS)
            return FALSE;
    
    if (validaPar(pares[0]) == NUMEROS &&
        validaPar(pares[1]) == NUMEROS &&
        validaPar(pares[2]) == NUMEROS)
            return FALSE;
    
    return TRUE;
}



