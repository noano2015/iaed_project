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


DataCompleta* obtemDataCompleta(char* input1, char* input2);

DataCompleta* obtemDataSimples(char* input);

void escreveData(DataCompleta* data);

int datasIguais(Data* data1, Data* data2);

void freeDataCompleta(DataCompleta* data);

void freeDataCompleta(DataCompleta* data);

int comparaDatas(DataCompleta* dataAntiga, DataCompleta* dataRecente);

int numDias(DataCompleta* data1, DataCompleta* data2);

DataCompleta* copiaData(DataCompleta* data);