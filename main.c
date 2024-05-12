#include <stdio.h>
#include <string.h>
#include "operacoes.h"
#include "auxiliares.h"
#include "estruturas.h"

int main(int argc, char const *argv[])
{
    int escolha; // Escolha do usuário 
    char menu[] = "---------------------\n| Sistema de Saude  |\n---------------------\n\n\t1. Cadastrar\n\t2. Atendimento\n\t3. Pesquisa\n\t4. Carregar / Salvar\n\t5. Sobre";
    void (*operacao[]) (Lista *registros) = {cadastrar, placeholderAtendimento, pesquisa, carrega_salva, sobre}; // Array com as funcoes por motivos de organizacao
    FILE *db_pacientes = fopen("db_pacientes", "ab"); // Criar a db caso, por algum motivo, não exista
    fclose(db_pacientes);
    Lista *registros = criaLista();
    Fila *fila = criaFila();
    
    while (1) { // Continuar perguntando ate sair
        escolha = checaEscolha(menu, 5);
        if (escolha == -1) { // Escolheu sair
            return 0;
        }
        if (escolha == 1) { // Sem isso nao salvaria a fila, teria que ficar criando
            // toda vez que entrasse nessa opcao
            printf("\n");
            atendimento(registros, fila);
            printf("\n");
            continue;
        }
        printf("\n");
        operacao[escolha](registros); // Chama a respectiva funcao baseado na escolha
        printf("\n");
    }
    return 0;
}