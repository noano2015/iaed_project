typedef struct nodeArg{
    char* palavra;
    struct nodeArg *proximo, *anterior;
}NodeArg;

typedef struct{
    struct nodeArg *primeiroParametro, *ultimoParametro;
    int comprimento;
}ListArg;

void  freeList(ListArg *lista);

ListArg* criaListaArg();

ListArg * obtemArgumentos();