#include <stdio.h>
#include "operacoes.h"
#include "auxiliares.h"

int main(int argc, char const *argv[])
{
    int escolha; // Escolha do usuário 
    char menu[] = "---------------------\n| Sistema de Saude  |\n---------------------\n\n\t1. Cadastrar\n\t2. Atendimento\n\t3. Pesquisa\n\t4. Carregar / Salvar\n\t5. Sobre";
    void (*operacao[]) () = {cadastrar, atendimento, pesquisa, carrega_salva, sobre}; // Array com as funcoes, mais por organizacao
    // Criar a db caso, por algum motivo, não exista
    FILE *db_clientes = fopen("db_clientes", "ab");
    fclose(db_clientes);
    while (1) {
        escolha = checaEscolha(menu, 5);
        if (escolha == -1) {
            return 1;
        }
        operacao[escolha]();
        printf("\n");
    }
    return 0;
}