/**
 * @brief Conjunto de funções que permitem criar e remover 
 * parques de listas assim como ordenar listas de parques 
 * e obter a faturação de um determinado parque.
 * 
 * @file parques.c
 * @author Pedro Vicente ist1109852
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitura_de_dados.h"
#include "veiculo_registos.h"

/**Valor atribuído caso um acontecimento é falso.*/
#define FALSE 0

/**Valor atribuído caso um acontecimento é verdadeiro*/
#define TRUE 1

/**Número máximo de parques que podem estar registados no sistema.*/
#define MAXP 20

/**Número de minutos min antes de passar a pagar o valor após uma hora 
 * de estar estacionado.
*/
#define MAXMIN 45

/**Número de minutos em um quarto de hora.*/
#define QUARTO 15

/**Número de minutos em uma hora.*/
#define HORA 60

/**Número de horas em um dia.*/
#define DIA 24


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
    DataCompleta* ultimaData;
    HashTable* hashTable;
}ListParque;


/**
 * Aloca memória para uma listParque e devolve-a vazia.
 * 
 * @return uma lista de parques vazia
*/
ListParque * criaListaParque(){

    ListParque*  lista = (ListParque*)malloc(sizeof(ListParque));
    lista->hashTable = NULL;
    lista->primeiroParque=NULL;
    lista->ultimoParque =NULL; 
    lista->comprimento = 0;
    lista->todosParques = 0;
    lista->ultimaData = NULL;
    
    return lista;
}


/**
 * @brief Aloca memória para um parque e devolve-o 
 * vazio.
 * 
 * @return Parque* 
 */
Parque* criaParque(){
    Parque *parque = (Parque*)malloc(sizeof(Parque));
    parque->nome = NULL;
    parque->capacidade = 0;
    parque->disponiveis = 0;
    parque->ListaRegistos = NULL;
    parque->valor15min = 0;
    parque->valorApos1Hora = 0;
    parque->valorMaxDiario = 0;
    return parque;
}


/**
 * @brief Liberta toda a memória associada a um 
 * parque.
 * 
 * @param parque o parque que se pretende libertar
 * a sua memória
 */
void freeParque(Parque* parque){
    freeListRegisto(parque->ListaRegistos);
    free(parque->nome);
    free(parque);
}


/**
 * @brief Liberta toda a memória associada aos elementos 
 * de uma dada lista de parques.
 * 
 * @param node o primeiro Parque da respetiva lista
 */
void freeNodeParque(NodeParque* node){

    NodeParque* aux = NULL;
    while (node!=NULL){
        aux = node;
        node = node->proximo;
        freeParque(aux->parque);
        free(aux);   
    }
}


/**
 * @brief A função liberta toda memória associada a uma dada
 * lista de parques.
 * 
 * @param lista 
 */
void  freeListParque(ListParque * lista){
    
    freeNodeParque(lista->primeiroParque);
    free(lista);

}


/**
 * @brief Escreve o nome do parque, a sua capacidade e o 
 * número de lugares disponiveis.
 * 
 * @param parque
 */
void escreveParque(Parque * parque){
    printf("%s %d %d\n", parque->nome, parque->capacidade,
            parque->disponiveis);
}


/**
 * @brief Recebe uma lista de parques e escreve o nome do 
 * parque, a sua capacidade e o número de lugares 
 * disponiveis até chegar ao fim da mesma.
 * 
 * @param listaParque 
 */
void escreveTodos(ListParque *listaParque){
    NodeParque* primeiroParque = listaParque->primeiroParque;
    for(; primeiroParque!= NULL;
        primeiroParque = primeiroParque->proximo)
            escreveParque(primeiroParque->parque);

}


/**
 * @brief Procura em uma determinada lista por um dado parque.
 * Devolve o parque se existir na lista, NULL caso contrário.
 * 
 * @param listaParque 
 * @param nomeParque 
 * @return Parque* 
 */
Parque* procuraParque(ListParque *listaParque, char* nomeParque){
    NodeParque* primeiroParque;
    int comprimentoNome = strlen(nomeParque) + 1;
    for(primeiroParque = listaParque->primeiroParque; 
        primeiroParque != NULL; primeiroParque= primeiroParque->proximo){
        if(strncmp(primeiroParque->parque->nome, nomeParque,
            comprimentoNome)== 0)
            return primeiroParque->parque;
    }
    return NULL;

}


/**
 * @brief Adicona ao fim da lista um determinado parque.
 * 
 * @param listaParque 
 * @param novoParque 
 */
void adicionaParque(ListParque *listaParque, Parque* novoParque){

    NodeParque* novoLinkParque = (NodeParque*)malloc(sizeof(NodeParque));
    novoLinkParque->parque = novoParque;
    novoLinkParque->proximo = NULL;
    novoLinkParque->anterior = listaParque->ultimoParque;

    /**Caso seja uma lista vazia.*/
    if (listaParque->ultimoParque == NULL){
        listaParque->ultimoParque = novoLinkParque;
        listaParque->primeiroParque = novoLinkParque;
    }
    /**Caso Contrário*/
    else{
        listaParque->ultimoParque->proximo = novoLinkParque;
        listaParque->ultimoParque = novoLinkParque;
    }

    /*incrementa o tamanho da lista e de todos os parques já 
    existentes.*/
    listaParque->comprimento++;
    listaParque->todosParques++;
}


/**
 * @brief Remove um determinado parque da lista de parques, 
 * não alterando a ordem da lista e libertando a memórida desse
 * mesmo elemento da lista.
 * 
 * @param listaParque 
 * @param nomeParque 
 */
void removeParque(ListParque* listaParque, char* nomeParque){

    NodeParque* aux; 

    /*percorrer a lista até encontrar o parque desejado.*/
    for(aux = listaParque->primeiroParque; aux != NULL; 
        aux = aux->proximo){

        /*quando o parque for encontrado, alterar a estrutura da lista.*/
        if(strcmp(aux->parque->nome, nomeParque) == 0){
            if(aux->anterior == NULL && aux->proximo==NULL){
                listaParque->primeiroParque =NULL;
                listaParque->ultimoParque =NULL;
            }
            else if(aux->anterior== NULL){
                listaParque->primeiroParque = aux->proximo;
                listaParque->primeiroParque->anterior = NULL;
            }
            else if(aux->proximo == NULL){
                listaParque->ultimoParque = aux->anterior;
                listaParque->ultimoParque->proximo = NULL;
            }

            if(aux->anterior != NULL && aux->proximo != NULL){
                aux->anterior->proximo = aux->proximo;
                aux->proximo->anterior = aux->anterior;
            }
            /*Diminuir o comprimento da lista e libertar a 
            memória associada a esse parque.*/
            listaParque->comprimento--;
            freeParque(aux->parque);
            free(aux);
            break;
        }
    }

}


/**
 * @brief Ordena os parques de uma lista de parques pelo 
 * seu nome (código ASCII).
 * 
 * @param listaParques 
 */
void ordenaLetras(ListParque* listaParques){
    

    for (NodeParque* primeiroParque = listaParques->primeiroParque; 
        primeiroParque!= NULL; primeiroParque= primeiroParque->proximo){

        NodeParque* min = primeiroParque;

        for (NodeParque* segundoParque = primeiroParque->proximo;
        segundoParque != NULL; segundoParque = segundoParque->proximo){

            int cmp = strcmp(segundoParque->parque->nome, min->parque->nome);
            if (cmp < 0 || (cmp == 0 && strcmp(segundoParque->parque->nome,
                            min->parque->nome) < 0))
                min = segundoParque;
        }
        
        if (min != primeiroParque) {
            Parque* temp = primeiroParque->parque;
            primeiroParque->parque = min->parque;
            min->parque = temp;
        }
    }

}


/**
 * @brief Ordena os parques de uma dada lista de 
 * parques pela a sua data de criação.
 * 
 * @param listaParques 
 */
void ordenaIndices(ListParque* listaParques){
    

    for (NodeParque* primeiroParque = listaParques->primeiroParque; 
        primeiroParque!= NULL; primeiroParque= primeiroParque->proximo){

        NodeParque* min = primeiroParque;

        for (NodeParque* segundoParque = primeiroParque->proximo;
        segundoParque != NULL; segundoParque = segundoParque->proximo){

            if (segundoParque->parque->indice < min->parque->indice)
                        min = segundoParque;
        }
        
        if (min != primeiroParque) {
            Parque* temp = primeiroParque->parque;
            primeiroParque->parque = min->parque;
            min->parque = temp;
        }
    }

}



/**
 * Valida se um dado parque é válido, de forma a que possa a 
 * ser adicionado a lista de parques pretendida.
 * 
 * @param listaParque a lista de parques a que se pretende
 * adicionar
 * @param parque o parque que se pretende adicionar
 * @return  o parque se o parque for válido ou NULL caso contrário
 * (libertando toda a memória associada ao parque)
*/

/**
 * @brief Valida se um dado parque é válido, de forma a 
 * que possa a ser adicionado a lista de parques pretendida.
 * Devolve o parque se o parque for válido ou NULL caso 
 * contrário (libertando toda a memória associada ao parque).
 * @param listaParque 
 * @param parque 
 * @return Parque* 
 */
Parque* validaParque(ListParque* listaParque, Parque* parque){

    int erro = FALSE;

    /*Verifica se já existe um parque o nome do parque que
    se pretende introduzir.*/
    if (procuraParque(listaParque, parque->nome)!=NULL){
        printf("%s: parking already exists.\n", parque->nome);
        erro = TRUE;
    }
    /*Veifica se a capacidade do parque é válida.*/
    if(erro == FALSE && parque->capacidade <= 0){
        printf("%d: invalid capacity.\n", parque->capacidade);
        erro = TRUE;
    }
    /*Verifica se o tarifário do parque são válidos e 
    se estão por ordem crescente.*/
    if(erro == FALSE && (parque->valor15min <= 0 ||
        parque-> valorApos1Hora <= 0 ||
        parque->valorMaxDiario <= 0 ||
        parque->valor15min > parque->valorMaxDiario ||
        parque->valorApos1Hora > parque->valorMaxDiario ||
        parque->valor15min > parque->valorApos1Hora)){
            printf("invalid cost.\n");
            erro = TRUE;
        }
    /*Verifica se existe espaço para colocar o parque na lista.*/
    if(erro == FALSE && listaParque->comprimento == MAXP){
        printf("too many parks.\n");
        erro = TRUE;
    }
    /*Caso se encontre um erro, liberta-se a memória associada
    ao mesmo e retona NULL*/
    if(erro == TRUE){
        freeParque(parque);
        return NULL;
    }
    return parque;
}


/**
 * @brief Dos parâmetros que foram introduzidos pelo 
 * utilizador na linha de comandos atribui a um parque 
 * essas mesmas características.
 * 
 * @param listaArgumentos 
 * @return Parque* 
 */
Parque* atribuirParque(ListArg* listaArgumentos){

    Parque* parque = criaParque();
    int parametros = 0, comprimento = 0, numAux = 1;
    /*de um dado input, fazer a atribuição de cada argumento a
    cada um dos atributos do parque*/
    for(NodeArg* auxArg = listaArgumentos->primeiroParametro->proximo;
        auxArg != NULL; auxArg= auxArg->proximo, parametros++){
        switch (parametros){
            case 0:
                comprimento = strlen(auxArg->palavra) +1;
                parque->nome = (char*)malloc(sizeof(char)*comprimento);
                strncpy(parque->nome, auxArg->palavra, comprimento);  
                break;
            case 1:
                numAux = atoi(auxArg->palavra);
                parque->capacidade = numAux;
                parque->disponiveis = numAux;           
                break;
            case 2:
                parque->valor15min= atof(auxArg->palavra);
                break;
            case 3:
                parque->valorApos1Hora = atof(auxArg->palavra);
                break;
            case 4:
                parque->valorMaxDiario = atof(auxArg->palavra);
                break;
        }
    }

    return parque;
}


/**
 * @brief De um conjunto de parametros de uma linha de 
 * comandos introduzida pelo utilizador, obtem um 
 * parque com as características introduzidas e se for
 * vaĺido adiciona à lista de parques pretendida.
 * 
 * @param listaParque 
 * @param listaArgumentos 
 */
void obtemParque(ListParque *listaParque, ListArg* listaArgumentos){

    /*atribuição dos respetivos atributos ao parque*/
    Parque* parque = atribuirParque(listaArgumentos);
    parque->indice = listaParque->todosParques;

    /*Cria uma lista de Registos vazia.*/
    parque->ListaRegistos = cria_listaRegisto();
    /* verificar se um dado parque e valido*/
    parque= validaParque(listaParque, parque);
    
    /*Adcionar o parque em caso que o parque seja válido.*/
    if(parque != NULL)
        adicionaParque(listaParque, parque);
}


/**
 * @brief Calcula o valor da faturação de um determinado 
 * parque entre duas datas, em que a primeira data é 
 * mais antiga que a segunda data.
 * 
 * @param parque 
 * @param dataInicial a data correpondente ao momento 
 * em que o veículo entrou
 * @param dataFinal a data correspondente ao momento 
 * em que o veículo saiu
 * @return float 
 */
float faturacao(Parque* parque, DataCompleta* dataInicial,
                DataCompleta* dataFinal){

    int dias = numDias(dataInicial, dataFinal);
    float horas = dataFinal->relogio->horas- dataInicial->relogio->horas;
    float minutos = dataFinal->relogio->minutos- dataInicial->relogio->minutos;
    int tempo = 0;
    float faturado = 0;

    /*Caso o número de minutos seja negativo, adiciona-se o número
    de minutos em um dia*/
    minutos = horas*HORA + minutos;
    if (minutos < 0){
        minutos += DIA*HORA;
        dias--;
    }

    /*Obter o valor faturado nos primeiros 45 minutos.*/
    for(; tempo < HORA && minutos > 0; tempo += QUARTO, minutos-= QUARTO)
        faturado += parque->valor15min;

    /*Obter o valor faturado no tempo restante*/
    for(; minutos > 0; minutos-=QUARTO)
        faturado += parque->valorApos1Hora;
    
    /*Caso o valor faturado seja superior ao máximo diário,
    então o valor faturado é igual ao valor máximo diário 
    vezes o número de dias mais 1.*/
    if (faturado >= parque->valorMaxDiario)
        faturado = (dias +1) * parque->valorMaxDiario;
    else
        faturado += dias * parque->valorMaxDiario;
    

    return faturado;
}


/**
 * @brief Escreve a faturação diária de um determinado 
 * parque em todos os dias até ao último registo.
 * 
 * @param parque 
 */
void faturadoParque(Parque* parque){

    DataCompleta* dataBase = NULL;
    float contador = 0;
    for(NodeRegisto* auxRegisto = parque->ListaRegistos->primeiroRegisto;
            auxRegisto!= NULL; auxRegisto = auxRegisto->proximoRegisto){

            if (dataBase == NULL || auxRegisto->saida == NULL ||
                (auxRegisto->saida != NULL &&
                datasIguais(dataBase->data, 
                            auxRegisto->saida->data) == TRUE)){
                    
                    contador += auxRegisto->faturado;
            }else{

                printf("%02d-%02d-%04d %0.2f\n", dataBase->data->dia, 
                dataBase->data->mes, dataBase->data->ano, contador);
                dataBase = auxRegisto->saida;
                contador = auxRegisto->faturado;
            }
            if (auxRegisto != NULL && auxRegisto->saida != NULL)
                dataBase = auxRegisto->saida;

        }
        
        printf("%02d-%02d-%04d %0.2f\n", dataBase->data->dia, 
                dataBase->data->mes, dataBase->data->ano, contador);    
}


/**
 * @brief Escreve todos os momentos de um determinado 
 * dia em que o parque faturou, indicando a matrícula
 * e o relógio na hora de saída.
 * 
 * @param parque 
 * @param dataBase 
 */
void faturadoDiario(Parque* parque, DataCompleta* dataBase){
    
    
    for(NodeRegisto* auxRegisto = parque->ListaRegistos->primeiroRegisto;
        auxRegisto!= NULL; auxRegisto = auxRegisto->proximoRegisto){
        if (auxRegisto->saida != NULL &&
            datasIguais(dataBase->data, 
                        auxRegisto->saida->data) == TRUE){

            printf("%s %02d:%02d %0.2f\n", auxRegisto->matricula,
                                auxRegisto->saida->relogio->horas,
                                auxRegisto->saida->relogio->minutos,
                                auxRegisto->faturado);

        }
    }
    freeDataCompleta(dataBase);
}
