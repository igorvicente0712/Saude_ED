#ifndef ESTRUTURAS
#define ESTRUTURAS

// Contexto
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    char nome[100];
    int idade;
    char rg[10];
    Data entrada;
} Registro;

// Lista para cadastro
typedef struct ELista {
    Registro dados;
    struct ELista *proximo;
} ELista;

typedef struct {
    ELista *inicio;
    int qtde;
} Lista;

Lista *criaLista();

ELista *criaELista(Registro reg);

void insereLista(Lista *lde, Registro reg);

int removeLista(Lista *lde, char *rg);

void resetaLista(Lista *lde);

void mostraLista(Lista *lde);

// Fila para atendimento
typedef struct EFila {
    Registro dados;
    struct EFila *proximo;
} EFila;

typedef struct {
    EFila *head;
    EFila *tail;
    int qtde;
} Fila;

Fila *criaFila();

EFila *criaEFila(Registro reg);

void enfileira(Fila *f, EFila *ef);

void desenfileira(Fila *f);

void mostraFila(Fila *f);

// Arvore binaria de busca para pesquisa
typedef struct EABB {
    Registro dados;
    struct EABB *fEsq;
    struct EABB *fDir;
} EABB;

typedef struct ABB {
    EABB *raiz;
    int qtde;
} ABB;

void ordenaEABB(EABB *raiz);

EABB *criaEABB(Registro reg);

ABB *criaABB();

void inserePaciente(ABB *arvore, Registro reg, int tipoOrdenacao);

#endif