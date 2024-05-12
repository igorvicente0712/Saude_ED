#ifndef OPERACOES
#define OPERACOES
#include "estruturas.h"

// Cadastrar
void novoPaciente(Lista *registros);

void consultaCadastrado(Lista *registros);

void attDados(Lista *registros);

void removerPaciente(Lista *registros);

void cadastrar(Lista *registros);

void placeholderAtendimento(Lista *registros); // Horrivel mas precisei fazer isso para
// manter a estrutura de array com ponteiro das funcoes para evitar ficar colocando
// Switch Case

// Atendimento
void enfileiraPaciente(Lista *registros, Fila *fila);

void atendimento(Lista *registros, Fila *fila);

// Pesquisa
void pesquisa(Lista *registros);

// Carregar / Salvar
void salva(Lista *registros);

void carrega(Lista *registros);

void carrega_salva(Lista *registros);

void sobre(Lista *registros);

#endif