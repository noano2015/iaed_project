#include "leitura_de_dados.h"
#include "veiculo_registos.h"
#define FALSE 0
#define TRUE 1
#define MAXP 20

typedef struct{

    char *nome;

    int capacidade,
    disponiveis,
    indice;
   
    ListRegisto* ListaRegistos;

    float valor15min;
    float valorApos1Hora; 
    float valorMaxDiario;

}Parque;

typedef struct nodeParque{
    Parque* parque;
    struct nodeParque* proximo, *anterior;

}NodeParque;

typedef struct{
    struct nodeParque *primeiroParque, *ultimoParque;
    int comprimento;
    int todosParques;
    HashTable* hashTable;
    DataCompleta* ultimaData;
}ListParque;

ListParque * criaListaParque();

void  freeListParque(ListParque * lista);

void escreveParque(Parque * parque);

void escreveTodos(ListParque *listaParque);

Parque* procuraParque(ListParque *listaParque, char* nomeParque);

void removeParque(ListParque* listaParque, char* nomeParque);

void adicionaParque(ListParque *listaParque, Parque* novoParque);

void obtemParque(ListParque *listaParque, ListArg* listaArgumentos);

void ordenaIndices(ListParque* listaParques);

void ordenaLetras(ListParque* listaParques);

float faturacao(Parque* parque, DataCompleta* dataInicial, 
                DataCompleta* dataFinal);

void faturadoParque(Parque* parque);
void faturadoDiario(Parque* parque, DataCompleta* dataBase);
