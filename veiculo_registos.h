#define MATRICULA 9
#define HASHVALUE 151
#include "datas_horas.h"


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
    char* parque;
    DataCompleta* entrada;
    struct nodeVeiculo* proximaEntrada;

}NodeVeiculo;

typedef struct{
    int tamanho;  
    NodeVeiculo** table;  
}HashTable;


Veiculo* criaVeiculo(char* matricula);

void  freeHashTable(HashTable* hashtable);

HashTable* criaHashtable(int tamanho);

void insereVeiculo(HashTable* hashtable, const char* chave,
                    Veiculo* veiculo);

Veiculo* procuraVeiculo(HashTable* hashtable, const char* chave);

int validaMatricula(char matricula[]);

NodeRegisto* adicionaEntrada(ListRegisto* lista, char matricula[], 
                    DataCompleta* entrada);

void adicionaSaida(ListRegisto* listaRegisto, 
                    NodeRegisto* registoVeiculo, 
                    DataCompleta* saida, float faturado);

ListRegisto* cria_listaRegisto();

void  freeListRegisto(ListRegisto* lista);



