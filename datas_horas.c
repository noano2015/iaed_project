/**
 * @brief Conjunto da funções e estruturas que permitem obter,
 * controlar e comparar datas e relogios.
 * 
 * @file data_horas.c
 * @author Pedro Vicente ist1109852
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**Valor dado quando o acontecimento é verdadeiro.*/
#define TRUE 1

/**Valor dado quando o acontecimento é falso.*/
#define FALSE 0

/**Número máximo de digítos que a estrutura data tem.*/
#define MAXDATA 3

/**Número máximo de meses.*/
#define MESES 12

/**Número de caracteres numa string com dois caracteres legíveis.*/
#define PAR 3

/**Número de caracteres numa string com três caracteres legíveis.*/
#define ANO 5

/**Número máximo de caracteres que um relógio tem*/
#define RELOGIOMAX 4

typedef struct 
{
    int dia;
    int mes;
    int ano;
} Data;


typedef struct{
    int horas, minutos;
} Relogio;


typedef struct{
    Data* data;
    Relogio* relogio;
}DataCompleta;


/**
 * @brief Aloca a memória necessária para uma estrutura do tipo 
 * DataCompleta.
 * 
 * @return DataCompleta* 
 */
DataCompleta* criaDataCompleta(){
    DataCompleta* data = (DataCompleta*)malloc(sizeof(DataCompleta));
    data->data = (Data*)malloc(sizeof(Data));
    data->relogio =(Relogio*)malloc(sizeof(Relogio));

    return data;
}


/**
 * @brief Para uma dada data introduzida obtêm o seus atributos
 * a partir de um vetor de caracteres.
 * 
 * @param data a data que se pretende obter do dado input
 * @param input um vetor de caracteres com o formato da 
 * data
 */
void obtemData( Data* data, char* input){
    char dia[PAR] = {input[0], input[1], '\0'},
         mes[PAR] = {input[3], input[4], '\0'},
         ano[ANO] = {input[6], input[7], input[8], input[9], '\0'};
    

    data-> dia = atoi(dia);
    data-> mes = atoi(mes);
    data-> ano = atoi(ano);
}


/**
 * @brief Cria uma DataCompleta e obtem simplesmente o atributo
 * data da mesma colocando a NULO os restantes dados 
 * relacionados ao relógio.
 * 
 * @param input o vetor de caracteres do qual se pretende
 * obter a data
 * @return DataCompleta* 
 */
DataCompleta* obtemDataSimples(char* input){
    DataCompleta* data = criaDataCompleta();
    obtemData(data->data, input);
    data->relogio->horas = 0;
    data->relogio->minutos = 0;
    return data;
}


/**
 * @brief Faz uma cópia de todos os atributos de uma dada data
 * e devolve uma nova data com esses mesmos atributos.
 * 
 * @param data 
 * @return DataCompleta* 
 */
DataCompleta* copiaData(DataCompleta* data){

    DataCompleta* nova = criaDataCompleta();
    
    nova->data->dia = data->data->dia;
    nova->data->mes = data->data->mes;
    nova->data->ano = data->data->ano;
    nova->relogio->horas = data->relogio->horas;
    nova->relogio->minutos = data->relogio->minutos;

    return nova;
}


/**
 * @brief De um dado vetor de caracteres, obtem os atibutos
 * de um dado relógio.
 * 
 * @param relogio  o relógio que se pretende atualizar os
 * seus atributos com o conteúdo do vetor
 * @param input vetor de caracteres com o formato de 
 * relógio do qual se pretende obter tais atributos
 */
void obtemRelogio(Relogio* relogio, char* input){
    char horas[PAR], minutos[PAR];
    if (strlen(input) == RELOGIOMAX){
        horas[0]= input[0];
        horas[1]= '\0';
        minutos[0] = input[2];
        minutos[1]=input[3];
        minutos[2]='\0';
    }
    else{
        horas[0]= input[0];
        horas[1] = input[1];
        horas[2]= '\0';
        minutos[0] = input[3];
        minutos[1]=input[4];
        minutos[2]= '\0';
    }
    
    
    relogio->horas = atoi(horas);
    relogio->minutos = atoi(minutos);
}


/**
 * @brief De dois vetores de caracteres, cria uma dataCompleta
 * e atribui a essa data os atributos presentes nesses vetores
 * 
 * @param input1 vetor de caracteres com o formato de data
 * @param input2 vetor de caracteres com o formato de relógio
 * @return DataCompleta* 
 */
DataCompleta* obtemDataCompleta(char* input1, char* input2){
    DataCompleta* dataCompleta = criaDataCompleta();
    obtemData(dataCompleta->data, input1);
    obtemRelogio(dataCompleta->relogio, input2);

    return dataCompleta;
}


/**
 * @brief Escreve cada parâmetro da data sem deixar um
 * new line no final.
 * 
 * @param data 
 */
void escreveData(DataCompleta* data){

    printf("%02d-%02d-%04d %02d:%02d", data->data->dia, data->data->mes, 
    data->data->ano, data->relogio->horas, data->relogio->minutos);
}


/**
 * @brief Liberta toda a memória associada a DataCompleta introduzida.
 * 
 * @param data 
 */
void freeDataCompleta(DataCompleta* data){
    free(data->data);
    free(data->relogio);
    free(data);
}


/**
 * @brief Valida os argumentos de uma dado Relogio de uma 
 * DataCompleta. TRUE se for válido, FALSE caso contrário.
 * 
 * @param data 
 * @return int 
 */
int validaRelogio(DataCompleta* data){

    if(data->relogio->horas >= 24 || data->relogio->minutos >= 60)
        return FALSE;
    return TRUE;
}


/**
 * @brief Válida os argumentos de uma dada Data de uma 
 * DataCompleta. TRUE se for válida, FALSE caso contrário.
 * 
 * @param data 
 * @return int 
 */
int validaData(DataCompleta*data){
    int calendario[13]=
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(data-> data->dia > calendario[data->data->mes])
        return FALSE;

    if(data->data->mes > MESES)
        return FALSE;
    
    return TRUE;
}


/**
 * @brief De duas "DataCompleta" averigua se a primeira é mais
 * antiga que a segunda.
 * 
 * @param dataAntiga a DataCompleta mais antiga de entre as duas
 * @param dataRecente a DataCompleta mais recente de entre as duas
 * @return int 
 */
int comparaDatas(DataCompleta* dataAntiga, DataCompleta* dataRecente) {
    
    /*Validar se a data e o relógio da data recente são válidos*/
    if (validaRelogio(dataRecente) == FALSE || 
        validaData(dataRecente) == FALSE)
        return FALSE;  

    /*Comparar os anos*/
    if (dataAntiga->data->ano > dataRecente->data->ano)
        return FALSE;
    else if (dataAntiga->data->ano < dataRecente->data->ano)
        return TRUE;

    /*comparar os meses*/
    if (dataAntiga->data->mes > dataRecente->data->mes)
        return FALSE;
    else if (dataAntiga->data->mes < dataRecente->data->mes)
        return TRUE;

    /*comparar os dias*/
    if (dataAntiga->data->dia > dataRecente->data->dia)
        return FALSE;
    else if (dataAntiga->data->dia < dataRecente->data->dia)
        return TRUE;

    /*comparar as horas*/
    if (dataAntiga->relogio->horas > dataRecente->relogio->horas)
        return FALSE;
    else if (dataAntiga->relogio->horas < dataRecente->relogio->horas)
        return TRUE;

    /*comparar os minutos*/
    if (dataAntiga->relogio->minutos > dataRecente->relogio->minutos) 
        return FALSE;

    return TRUE;
}



/**
 * @brief Obtém a data do dia seguinte a data introduzida
 * mantendo o seu relógio.
 * 
 * @param dataAtual 
 * @return DataCompleta* 
 */
DataCompleta* proximaData(DataCompleta *dataAtual){

    int calendario[13]=
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ( ++dataAtual->data->dia > calendario[dataAtual->data->mes]){
        dataAtual->data->dia = 1;
        dataAtual->data->mes++;
    }
    if( dataAtual->data->mes > MESES){
        dataAtual->data->mes = 1;
        dataAtual->data->ano++;
    }
    
    return dataAtual;
}


/**
 * @brief Averigua se duas datas são iguais, ou seja,
 * correspondem ao mesmo dia. TRUE se corresponderem 
 * ao mesmo dia, FALSE caso contrário.
 * 
 * @param data1 
 * @param data2 
 * @return int 
 */
int datasIguais(Data* data1, Data* data2){

    if (data1->dia == data2->dia &&
        data1->mes == data2->mes &&
        data1->ano == data2->ano)
        return TRUE;
    return FALSE;
}


/**
 * @brief Calcula o número de dias entre ambas as datas.
 * 
 * @param data1 uma DataCompleta genérica
 * @param data2 uma DataCompleta mais recente que 
 * data1
 * @return int 
 */
int numDias(DataCompleta* data1, DataCompleta* data2){

    int dias = 0;
    DataCompleta* dataAux =copiaData(data1);

    while (datasIguais(dataAux->data, data2->data) == FALSE){
           
        dataAux= proximaData(dataAux);
        dias++;
    }
    freeDataCompleta(dataAux);

    return dias; 

}

