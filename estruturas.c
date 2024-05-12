#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h"
#include "auxiliares.h"

// Registro

// ----------------
// |    Lista     |
// ----------------

// Cria uma lista onde serao colocados os dados dos pacientes
Lista *criaLista() {
    Lista *lde = malloc(sizeof(Lista));
    lde->inicio = NULL;
    lde->qtde = 0;
    return lde;
}

// Cria uma ELista onde estarao os dados de registro dos pacientes
ELista *criaELista(Registro reg) {
    ELista *celula = malloc(sizeof(ELista));
    celula->proximo = NULL;
    celula->dados = reg;
    return celula;
}

// Adiciona os dados do paciente numa lista
void insereLista(Lista *lde, Registro reg) {
    // So vai colocando um na frente do outro, entao nao coloca de forma ordenada
    ELista *novo = criaELista(reg);
    if (lde->inicio == NULL) {
        lde->inicio = novo;
    }
    else {
        ELista *atual = lde->inicio;
        ELista *anterior = NULL;
        while (atual != NULL) {
            anterior = atual;
            atual = atual->proximo;
        }
        if (anterior != NULL && atual == NULL) {
            anterior->proximo = novo;
        }
    }
    lde->qtde++;
}

// Remove os dados de um paciente de uma lista baseado no seu RG
int removeLista(Lista *lde, char *rg) {
    if (lde->qtde == 0) { // Se estiver sem paciente, nao faz nada
        return 0;
    }
    ELista *atual = lde->inicio;
    ELista *anterior = NULL;
    while (atual != NULL) {
        if (strcmp(atual->dados.rg, rg) == 0) { // Se encontrar o RG, remove
            if (anterior == NULL) { // Caso seja o primeiro item
                lde->inicio = atual->proximo;
            }
            else {
                anterior->proximo = atual->proximo;
            }
            lde->qtde--;
            free(atual);
            return 1;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return 0;
}

// Reseta a lista para manter o ponteiro original
void resetaLista(Lista *lde) {
    if (lde->qtde == 0) {
        return;
    }
    ELista *atual = lde->inicio;
    ELista *proximo = NULL;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    lde->inicio = NULL;
    lde->qtde = 0;
}

// Mostra os dados de todos os pacientes dentro de uma lista
void mostraLista(Lista *lde) {
    if (lde->qtde == 0) {
        printf("Nao ha pacientes cadastrados.\n");
        return;
    }
    ELista *atual = lde->inicio;
    while (atual != NULL) {
        printf("Nome: %s | Idade: %d | RG: %s | Entrada: %02d/%02d/%d\n",  atual->dados.nome, atual->dados.idade, atual->dados.rg, atual->dados.entrada.dia, atual->dados.entrada.mes, atual->dados.entrada.ano);
        atual = atual->proximo;
    }
    //printf("\n");
}


// ----------------
// |     Fila     |
// ----------------

Fila *criaFila() {
    Fila *f = malloc(sizeof(Fila));
    f->head = NULL;
    f->tail = NULL;
    f->qtde = 0;
    return f;
}

EFila *criaEFila(Registro reg) {
    EFila *ef = malloc(sizeof(EFila));
    ef->dados = reg;
    ef->proximo = NULL;
    return ef;
}

void enfileira(Fila *f, EFila *ef) {
    if (f->head == NULL) {
        f->head = ef;
    }
    else {
        f->tail->proximo = ef;
    }
    f->tail = ef;
    f->qtde++;
    printf("Paciente enfileirado com sucesso!\n");
    return;
}

void desenfileira(Fila *f) {
    if (f->head == NULL) {
        printf("Fila vazia!\n");
        return;
    }
    EFila *atual = f->head;
    f->head = f->head->proximo;
    if (f->head == NULL) { // Se o proximo da head for null, so tem ele
        f->tail = NULL;
    }
    free(atual);
    f->qtde--;
    printf("Paciente desenfileirado com sucesso!\n");
    return;
}

void mostraFila(Fila *f) {
    int i = 1;
    EFila *atual = f->head;
    if (f->head == NULL) {
        printf("Fila vazia!\n");
        return;
    }
    while (atual != NULL) {
        printf("%d: Paciente %s\n", i, atual->dados.nome);
        i++;
        atual = atual->proximo;
    }
    printf("\n");
    return;
}


// ----------------
// |     ABB      |
// ----------------

void ordenaEABB(EABB *raiz) {
    if (raiz != NULL) {
        ordenaEABB(raiz->fEsq);
        printf("Nome: %s | Idade: %d | RG: %s | Entrada: %02d/%02d/%d\n",  raiz->dados.nome, raiz->dados.idade, raiz->dados.rg, raiz->dados.entrada.dia, raiz->dados.entrada.mes, raiz->dados.entrada.ano);
        ordenaEABB(raiz->fDir);
    }
}

EABB *criaEABB(Registro reg) {
    EABB* novo = malloc(sizeof(EABB));
    novo->fDir = NULL;
    novo->fEsq = NULL;
    novo->dados = reg;
    return novo;
}

ABB *criaABB() {
    ABB* arvore = malloc(sizeof(ABB));
    arvore->raiz = NULL;
    arvore->qtde = 0;
    return arvore;
}

void inserePaciente(ABB *arvore, Registro reg, int tipoOrdenacao) {
    // Tipo ordenacao:
    // 0 -> ano
    // 1 -> mes
    // 2 -> dia
    // 3 -> idade
    EABB *novo = criaEABB(reg);
    if (arvore->qtde == 0) {
        arvore->raiz = novo;
        arvore->qtde++;
        return;
    }
    EABB *atual = arvore->raiz;
    EABB *anterior = NULL;
    switch (tipoOrdenacao) {
        case 0:
            while (atual != NULL) {
                anterior = atual;
                if (atual->dados.entrada.ano > novo->dados.entrada.ano) {
                    atual = atual->fEsq;
                }
                else {
                    atual = atual->fDir;
                }
            }
            if (anterior->dados.entrada.ano > novo->dados.entrada.ano) {
                anterior->fEsq = novo;
            }
            else {
                anterior->fDir = novo;
            }
            break;
        case 1:
            while (atual != NULL) {
                anterior = atual;
                if (atual->dados.entrada.mes > novo->dados.entrada.mes) {
                    atual = atual->fEsq;
                }
                else {
                    atual = atual->fDir;
                }
            }
            if (anterior->dados.entrada.mes > novo->dados.entrada.mes) {
                anterior->fEsq = novo;
            }
            else {
                anterior->fDir = novo;
            }
            break;
        case 2:
            while (atual != NULL) {
                anterior = atual;
                if (atual->dados.entrada.dia > novo->dados.entrada.dia) {
                    atual = atual->fEsq;
                }
                else {
                    atual = atual->fDir;
                }
            }
            if (anterior->dados.entrada.dia > novo->dados.entrada.dia) {
                anterior->fEsq = novo;
            }
            else {
                anterior->fDir = novo;
            }
            break;
        case 3:
            while (atual != NULL) {
                anterior = atual;
                if (atual->dados.idade > novo->dados.idade) {
                    atual = atual->fEsq;
                }
                else {
                    atual = atual->fDir;
                }
            }
            if (anterior->dados.idade > novo->dados.idade) {
                anterior->fEsq = novo;
            }
            else {
                anterior->fDir = novo;
            }
            break;
    }
    arvore->qtde++;
}