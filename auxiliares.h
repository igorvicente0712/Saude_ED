#ifndef AUXILIARES
#define AUXILIARES
#include "estruturas.h"

ELista *procuraRG(Lista *registros);

int checaNome(char *nome);

void pegaInputUsuario(char mensagem[], char buffer[], int buffer_size);

int checaEscolha(char *texto, int n_escolhas);

int checaRGValido(char *rg);

void checaBuffer(char *c);

void limpaBuffer();

#endif