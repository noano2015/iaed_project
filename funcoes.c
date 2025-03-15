/**
 * @brief Conjunto de funções que permitem a execução de comandos
 * introduzida pelo utilizador para gestão de um parque
 * de estacionamento.
 * 
 * @file funcoes.c
 * @author Pedro Vicente ist1109852
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parques.h"


/**
 * @brief Permite introduzir um parque caso sejam 
 * introduzidos parâmetros suficientes pelo utilizador.
 * Escreve todos os parques, ou seja, o seu nome, a sua 
 * capacidade e o número de lugares disponíveis, caso o
 * utilizador tenha apenas escrito "p".
 * 
 * @param listaParque uma lista de parques que contém 
 * todos os parques do sistema
 * @param listaArg uma lista de argumentos que contém
 * a informação dada pelo utilizador
 */
void funcaoP(ListParque *listaParque, ListArg* listaArg){
    
    NodeArg* head = listaArg->primeiroParametro;

    if ( listaArg->comprimento == 1 && strcmp(head->palavra, "p") == 0){
        ordenaIndices(listaParque);
        escreveTodos(listaParque);
    }
    else
        obtemParque(listaParque, listaArg);
}


/**
 * @brief Deteta se os parâmetros introduzidos pelo
 * utilizador são válidos para a entrada do veículo 
 * no parque pretendido.
 * Devolve TRUE se for detetado um erro, FALSE 
 * caso contrário.
 * 
 * @param listaParque uma lista com todos os parques
 * no sistema
 * @param matricula um vetor de caracteres como o 
 * formato de matrícula do veículo que se pretende
 * introuzir
 * @param data o dia em que o carro entrou
 * @param parque o parque onde entrou
 * @return int 
 */
int erroEntrada(ListParque* listaParque, char* matricula, 
                    DataCompleta* data, Parque* parque){
    
    /*verifica se existem lugares disponiveis para o veiculo entrar*/
    if (parque->disponiveis <= 0){
        printf("%s: parking is full.\n", parque->nome);
        return TRUE;
    }
    /*averigua se a matricula e valida*/
    if (validaMatricula(matricula) == FALSE){
        printf("%s: invalid licence plate.\n", matricula);
        return TRUE;
    }
    /*verifica se o veiculo existe ou se está em outro parque*/
    Veiculo* veiculo = procuraVeiculo(listaParque->hashTable, 
                                        matricula);
        
    if (veiculo != NULL && veiculo->parque != NULL &&
        procuraParque(listaParque, veiculo->parque) != NULL){
        printf("%s: invalid vehicle entry.\n", matricula);
        return TRUE;
    }
        
    /*Averigua se a data é mais recente que a última do sistema*/
    if (listaParque->ultimaData != NULL &&
        comparaDatas(listaParque->ultimaData, data)==FALSE){
            printf("invalid date.\n");
            return TRUE;
    }
    return FALSE;

}


/**
 * @brief Introduz mais um registo a lista de Registos do parque,
 * assim como o veiculo com os devidos atributos no parque.
 * 
 * @param listaParque uma lista com todos os parques no sistema
 * @param veiculo o veiculo com a matricula que se pretende introduzir
 * no sistema
 * @param matricula um vetor de caracteres como o formato de matrícula
 * do veículo que se pretende introuzir
 * @param data o dia em que o carro entrou
 * @param parque o parque onde entrou
 */
void entrada(ListParque* listaParque, Veiculo* veiculo, 
            char* matricula, DataCompleta* data, Parque* parque){
    if (veiculo == NULL){
            veiculo = criaVeiculo(matricula);
            insereVeiculo(listaParque->hashTable, matricula, veiculo);
        }
        veiculo->registoVeiculo = adicionaEntrada(parque->ListaRegistos,
                                                        matricula, data);

        veiculo->parque = parque->nome;
        veiculo->entrada = data;

        if (listaParque->ultimaData != NULL)
            freeDataCompleta(listaParque->ultimaData);
            
        listaParque->ultimaData = copiaData(data);
        parque->disponiveis--;
        printf("%s %d\n", parque->nome, parque->disponiveis);
}


/**
 * @brief Insere um veículo no sistema e no respetivo parque
 * a partir de um dado input fornecido pelo utilizador.
 * 
 * @param listaParque lista com todos os parques do sistema
 * @param listaArg lista com os argumentos necessária para
 * introduzir o veículo num dado parque de estacionamento
 */
void funcaoE(ListParque* listaParque, ListArg* listaArg){

    int erro = FALSE;
    NodeArg* argumento = listaArg->primeiroParametro->proximo;
    Parque* parque = procuraParque(listaParque,argumento->palavra);
    char* matricula =argumento->proximo->palavra;
    DataCompleta* data = obtemDataCompleta(argumento->proximo
                                    ->proximo->palavra,
                                    argumento->proximo->proximo
                                    ->proximo->palavra);
    /*verifica a existencia do parque*/
    if (parque == NULL){
        printf("%s: no such parking.\n", argumento->palavra);
        erro = TRUE;
    }
    /*Verificação de todos os outros casos*/
    else
        erro = erroEntrada(listaParque, matricula, data,parque);
    
    /*libertar a memória associada a data uma vez que 
    esta não vei ser utilizada*/
    if(erro == TRUE)
        freeDataCompleta(data);
    
    Veiculo* veiculo = procuraVeiculo(listaParque->hashTable, matricula);
    /*insere o veiculo caso nao exista erro*/
    if(erro == FALSE)
        entrada(listaParque, veiculo,matricula, data, parque);
            
}


/**
 * @brief Deteta se os parâmetros introduzidos pelo utilizador 
 * são válidos para a saída do veículo no parque pretendido.
 * Devolve TRUE se for detetado um erro, FALSE caso contrário.
 * 
 * @param listaParque uma lista com todos os parques no sistema
 * @param matricula um vetor de caracteres como o formato de matrícula
 * do veículo que se pretende remover
 * @param data o dia em que o veículo saíu
 * @param parque o parque de onde saíu
 * @return int 
 */
int erroSaida(ListParque* listaParque, char* matricula, 
                    DataCompleta* data, Parque* parque){

    /*averigua se a matricula e valida*/
    if (validaMatricula(matricula) == FALSE){
        printf("%s: invalid licence plate.\n", matricula);
        return TRUE;
    }
    /*Verificar se o veiculo existe ou se está em outro parque
    de estacionamento que existá*/
    Veiculo* veiculo = procuraVeiculo(listaParque->hashTable, 
                                        matricula);
    if (veiculo == NULL || (veiculo != NULL && 
        (veiculo->parque == NULL || 
        strcmp(parque->nome, veiculo->parque) != 0))){

            printf("%s: invalid vehicle exit.\n", matricula);
            return TRUE;
    }

    if (listaParque->ultimaData != NULL &&
            comparaDatas(listaParque->ultimaData, data)==FALSE){

            printf("invalid date.\n");
            return TRUE;
    }
      
    return FALSE;
}


/**
 * @brief Adiciona um registo de saída ao parque pretendido 
 * e remove o carro desse mesmo parque de estacionamento.
 * 
 * @param listaParque uma lista com todos os parques no 
 * sistema
 * @param veiculo o veículo que se pretende remover do 
 * parque
 * @param data o momento em que saiu do parque
 * @param parque o parque do qual se pretende sair
 */
void saida(ListParque* listaParque, Veiculo* veiculo,
            DataCompleta* data, Parque* parque){
    
    float faturado;
    faturado = faturacao(parque, veiculo->entrada, data);
    printf("%s ", veiculo->matricula);
    escreveData(veiculo->entrada);
    printf(" ");
    escreveData(data);
    printf(" ");
    printf("%0.2f\n", faturado);

    if (listaParque->ultimaData != NULL)
    freeDataCompleta(listaParque->ultimaData);
            
    listaParque->ultimaData = copiaData(data);

    /*adicionar o registo das entradas*/
    adicionaSaida(parque->ListaRegistos, veiculo->registoVeiculo,
                            data, faturado);

    parque->disponiveis++;
    veiculo->parque = NULL;
    veiculo->entrada = NULL; 
    veiculo->registoVeiculo = NULL;

}


/**
 * @brief Retira um Veículo com determinada matrícula 
 * existente num parque do sistema sair desse mesmo 
 * parque.
 * 
 * @param listaParque uma lista com todos os parques do 
 * sistema
 * @param listaArg uma lista com os argumentos para remover
 * o veículo do parque
 */
void funcaoS(ListParque* listaParque, ListArg* listaArg){

    int erro = FALSE;
    NodeArg* argumento = listaArg->primeiroParametro->proximo;
    Parque* parque = procuraParque(listaParque,argumento->palavra);
    char* matricula =argumento->proximo->palavra;
    DataCompleta* data = obtemDataCompleta(argumento->proximo
                                    ->proximo->palavra,
                                    argumento->proximo->proximo
                                    ->proximo->palavra);
    /*verifica a existencia do parque*/
    if (parque == NULL){
        printf("%s: no such parking.\n", argumento->palavra);
        erro = TRUE;
    }
    else
        erro = erroSaida(listaParque, matricula, data, parque);
        
    if(erro == TRUE)
            freeDataCompleta(data);

    Veiculo* veiculo = procuraVeiculo(listaParque->hashTable, matricula);
    /*remover o elemento pretendido*/
    if (erro ==FALSE)
        saida(listaParque, veiculo, data, parque);
    
}


/**
 * @brief Lista todos os registos de entrada e de saída de
 * um veículo com uma dada matrícula em todos os parques
 * presentes no sistema.
 * 
 * @param listaParques uma lista com todos os parques do sistema
 * @param argumentos uma lista com os argumentos que especificam
 * o veículo que se pertende ver os respetivos registos
 */
void funcaoV(ListParque* listaParques, ListArg*argumentos){

    char* matricula = argumentos->primeiroParametro->proximo->palavra;
    ordenaLetras(listaParques);
    NodeParque* aux = listaParques->primeiroParque;
    int contador = 0;

    /*verifica se a matricula e valida*/
    if(validaMatricula(matricula) == FALSE)
        printf("%s: invalid licence plate.\n", matricula);

    /*percorre os parques da lista de parques e escreve todos os registos
    de entradas e saidas do veiculo com a matricula introduzida*/
    else{
        for(; aux != NULL; aux = aux->proximo){

            NodeRegisto* RegistoAux = aux->parque->
                                    ListaRegistos->primeiroRegisto;

            /*percorre os registos de cada parque e escreve aqueles que
            contenham a matricula introduzida*/
            for (; RegistoAux != NULL; 
                RegistoAux = RegistoAux->proximoRegisto){

                if (strcmp(RegistoAux->matricula, matricula) == 0){
                    printf("%s ", aux->parque->nome);
                    escreveData(RegistoAux->entrada);
                    if (RegistoAux->saida != NULL){
                        printf(" ");
                        escreveData(RegistoAux->saida);
                    }
                    printf("\n");
                    contador++;
                }  
            } 
        }
        /*se nao forem encontrados registos do veiculo em causa, entao
        emite a respetiva mensagem de erro*/
        if(contador == 0)
            printf("%s: no entries found in any parking.\n", matricula);
    }
}


/**
 * @brief Escreve a faturação diária caso seja apenas
 * apresentado o paque como argumento ou escreve 
 * a faturação ao longo de um dia completo em cada
 * momento em que houve uma saída desse parque.
 * 
 * @param listaParque uma lista com todos os parques
 * presentes no sistema
 * @param argumentos uma lista de argumentos introduzidas
 * pelo utilizador para execução do comando
 */
void funcaoF(ListParque* listaParque, ListArg* argumentos){

    char* nomeParque = argumentos->primeiroParametro->proximo->palavra;
    Parque* parque = procuraParque(listaParque, nomeParque);
    DataCompleta* dataBase = NULL;
    
    /*emitir a mensagem de erro caso o parque nao exista*/
    if(parque == NULL)
        printf("%s: no such parking.\n", nomeParque);

    /* se o numero de argumentos e dois, entao escreve o valor faturado
    em cada dia*/
    else if (argumentos->comprimento == 2)
        faturadoParque(parque);

    /* Caso contrario escreve o valor faturado em cada altura desse
    mesmo dia*/
    else{

        dataBase = obtemDataSimples(argumentos->primeiroParametro->
                                    proximo->proximo->palavra);

        if (listaParque->ultimaData != NULL &&
            comparaDatas(dataBase, listaParque->ultimaData) ==FALSE)
            printf("invalid date.\n");
        else
            faturadoDiario(parque, dataBase);
    }
}


/**
 * @brief Remove um parque da lista de parques presente no
 * sistema e liberta toda a memória associada a esse
 * parque, incluindo a lista de registos desse parque.
 * 
 * @param listaParques uma lista com todos os parques 
 * no sistema
 * @param argumentos uma lista de argumentos introduzida
 * pelo utilizador para a execução do comando
 */
void funcaoR(ListParque* listaParques, ListArg* argumentos){
    char* nomeparque = argumentos->primeiroParametro->proximo->palavra;
    Parque* parque = procuraParque(listaParques, nomeparque);
    NodeParque* auxNode;

    if (parque==NULL)
        printf("%s: no such parking.\n", nomeparque);
    else{
        removeParque(listaParques, nomeparque);
        ordenaLetras(listaParques);

        for(auxNode = listaParques->primeiroParque; auxNode != NULL; 
            auxNode = auxNode->proximo)
                printf("%s\n", auxNode->parque->nome);
            
    }
}


/**
 * @brief Executa os sistema de modo que o utilizador 
 * possa o utilizar.
 * 
 */
void executeSystem(){

    /*Inicializar as variáveis que correspondem a lista de parques
    no sistema e a lista de argumentos*/
    ListArg* argumentos = obtemArgumentos();
    ListParque *listaParque = criaListaParque();
    listaParque->hashTable = criaHashtable(HASHVALUE);

    /*Determinar qual é o primeiro parâmetro para determinar o
    comando a executar e executar o respetivo comando*/
    while(strcmp(argumentos->primeiroParametro->palavra, "q") != 0){
        if (strcmp(argumentos->primeiroParametro->palavra, "p") == 0)
            funcaoP(listaParque, argumentos);
        
        if(strcmp(argumentos->primeiroParametro->palavra, "e") == 0)
            funcaoE(listaParque, argumentos);
        
        if(strcmp(argumentos->primeiroParametro->palavra, "r") == 0)
            funcaoR(listaParque, argumentos);

        if(strcmp(argumentos->primeiroParametro->palavra, "s") == 0)
            funcaoS(listaParque, argumentos);

        if(strcmp(argumentos->primeiroParametro->palavra, "v") == 0)
            funcaoV(listaParque, argumentos);

        if(strcmp(argumentos->primeiroParametro->palavra, "f") == 0)
            funcaoF(listaParque, argumentos);
        freeList(argumentos);
        argumentos= obtemArgumentos();
    }

    /*libertar a memória associada a lista de parques e argumentos*/
    if(listaParque->ultimaData != NULL)
        freeDataCompleta(listaParque->ultimaData);

    freeHashTable(listaParque->hashTable);
    freeListParque(listaParque);
    freeList(argumentos);
}