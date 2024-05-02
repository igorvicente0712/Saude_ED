#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h"

// Registro

/*

// ----------------
// |    Lista     |
// ----------------
LDE *cria_LDE() {
    LDE *lde = malloc(sizeof(LDE));
    lde->primeiro = NULL;
    lde->qtde = 0;
    return lde;
}

Celula *cria_Celula(int valor) {
    Celula *celula = malloc(sizeof(Celula));
    celula->proximo = NULL;
    celula->valor = valor;
    return celula;
}

void inserir(LDE *lde, int valor) {
    Celula *novo = cria_Celula(valor);
    if (lde->primeiro == NULL) {
        lde->primeiro = novo;
        lde->qtde++;
    }
    else {
        Celula *atual = lde->primeiro;
        Celula *anterior = NULL;
        while (atual != NULL && atual->valor <= valor) {
            anterior = atual;
            atual = atual->proximo;
        }
        if (anterior == NULL && atual != NULL) {
            novo->proximo = atual;
            lde->primeiro = novo;
            lde->qtde++;
        }
        if (anterior != NULL && atual == NULL) {
            anterior->proximo = novo;
            lde->qtde++;
        }
        if (anterior != NULL && atual != NULL) {
            anterior->proximo = novo;
            novo->proximo = atual;
            lde->qtde++;
        }
    }
}

void remove_valor(LDE *lde, int valor) {
    if (lde->qtde == 0) {
        return;
    }
    Celula *atual = lde->primeiro;
    Celula *anterior = NULL;
    while (atual != NULL) {
        if (atual->valor == valor) {
            if (anterior == NULL) {
                lde->primeiro = atual->proximo;
            }
            else {
                anterior->proximo = atual->proximo;
            }
            lde->qtde--;
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return;
}

void mostrar(LDE *lde) {
    Celula *atual = lde->primeiro;
    while (atual != NULL) {
        printf("%d",  atual->valor);
        atual = atual->proximo;
    }
    printf("\n");
}

// ----------------
// |     Fila     |
// ----------------
Fila *cria_queue() {
  Fila *f = malloc(sizeof(Fila));
  f->head = 0;
  f->tail = 0;
  f->qtd = 0;
  return f;
}

int is_full(Fila *f) {
  return f->qtd == CAP;
}

int enqueue(Fila *f, int valor) {
  if (is_full(f)) {
    return 0;
  }
  f->valores[f->tail % CAP] = valor;
  f->tail++;
  f->qtd++;
  return 1;
}

void show(Fila *f) {
  for (int i = f->head; i < f->tail; i++) {
    printf("%d ", f->valores[i % CAP]);
  }
  printf("\n");
}

int is_empty(Fila *f) {
  return f->head == f->tail;
}

int dequeue(Fila *f) {
  if (is_empty(f)) {
    return -1;
  }
  int valor = f->valores[f->head % CAP];
  f->head++;
  f->qtd--;
  return valor;
}


// ----------------
// |     ABB      |
// ----------------
void in_ordem(Vertice * raiz) {
  if (raiz != NULL) {
    in_ordem(raiz->esq);
    printf("%d ", raiz->valor);
    in_ordem(raiz->dir);
  }
}

  void pre_ordem(Vertice * raiz) {
    if (raiz != NULL) {
      printf("%d ", raiz->valor);
      pre_ordem(raiz->esq);
      pre_ordem(raiz->dir);
    }
  }

  void pos_ordem(Vertice * raiz) {
    if (raiz != NULL) {
      pos_ordem(raiz->esq);
      pos_ordem(raiz->dir);
      printf("%d ", raiz->valor);
    }
  }

Vertice *cria_vertice(int valor){
  Vertice* novo = malloc(sizeof(Vertice));
  novo->dir = NULL;
  novo->esq = NULL;
  novo->pai = NULL;
  novo->valor = valor;

  return novo;
}

Arvore *cria_arvore(){
  Arvore* arvore = malloc(sizeof(Arvore));
  arvore->raiz = NULL;
  arvore->qtde = 0;

  return arvore;
}

void inserir(Arvore* arvore, int valor){ 
  Vertice *novo = cria_vertice(valor);
  if (arvore->qtde == 0) {
    arvore->raiz = novo;
    arvore->qtde++;
    return;
  }
  Vertice *atual = arvore->raiz;
  Vertice *anterior = NULL; 
  while (atual != NULL) {
    anterior = atual;
    if (atual->valor > novo->valor) {
      atual = atual->esq;
    }
    else {
      atual = atual->dir;
    }
  }
  novo->pai = anterior;
  if (anterior->valor > novo->valor) {
    anterior->esq = novo;
  }
  else {
    anterior->dir = novo;
  }
  arvore->qtde++;
}

int remover_vertice(Arvore *arvore, Vertice *vertice) {
  if (vertice == NULL) {
      return 0; 
  }
  if (vertice->esq == NULL && vertice->dir == NULL) { // n tem filho
    if (vertice->pai != NULL) {
      if (vertice == vertice->pai->esq) {
        vertice->pai->esq = NULL;
      } else {
        vertice->pai->dir = NULL;
      }
    } else {
      arvore->raiz = NULL;
    }
    free(vertice);
  } else if (vertice->esq == NULL || vertice->dir == NULL) {
    Vertice *filho = (vertice->esq != NULL) ? vertice->esq : vertice->dir;
    if (vertice->pai != NULL) {
      if (vertice == vertice->pai->esq) {
        vertice->pai->esq = filho;
      } else {
        vertice->pai->dir = filho;
      }
    } else {
      arvore->raiz = filho;
    }

    filho->pai = vertice->pai;
    free(vertice);
  } else {  
    Vertice* maior_na_esq = vertice->esq;
    while (maior_na_esq->dir != NULL) {
        maior_na_esq = maior_na_esq->dir;
    }

    int temp = vertice->valor;
    vertice->valor = maior_na_esq->valor;
    maior_na_esq->valor = temp;

    remover_vertice(arvore, maior_na_esq);
  }
  arvore->qtde--;
  return 1;
}

Vertice *buscar_valor(Arvore* arvore, int valor){
  Vertice* atual = arvore->raiz;
  while(atual != NULL){
    if(valor < atual->valor){
      atual = atual->esq;
    }else{
      if(valor > atual->valor){
        atual = atual->dir;
      }else{
        return atual;
      }
    }
  }
  return NULL;
}

*/