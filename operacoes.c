#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "operacoes.h" 
#include "estruturas.h"
#include "auxiliares.h"

// Adiciona um novo paciente na lista
void novoPaciente(Lista *registros) {
    // Para a data de entrada
    time_t t = time(NULL);
    struct tm tm;
    Registro reg; // Dados do paciente
    while (1) {
        pegaInputUsuario("Digite o RG do paciente com 9 digitos (sem pontuacao): ", reg.rg, sizeof(reg.rg));
        if (checaRGValido(reg.rg)) { // Se for valido continua
            break;
        }
        printf("RG invalido!\n");
    }
    ELista *atual = registros->inicio;
    while (atual != NULL) {
        if (strcmp(reg.rg, atual->dados.rg) == 0) { // Se ja existe o RG na lista, sai da funcao
            printf("Paciente ja consta no sistema.\n");
            return;
        }
        atual = atual->proximo;
    }
    while (1) {
        pegaInputUsuario("Digite o nome do paciente: ", reg.nome, sizeof(reg.nome));
        if (checaNome(reg.nome)) { // Se for valido continua
            break;
        }
        printf("Nome invalido!\n");
    }
    while (1) {
        printf("Digite a idade do paciente: ");
        if (scanf("%d", &(reg.idade))) { // Continua se for uma idade que faca sentido
            if (reg.idade >= 0 || reg.idade <= 120) {
                break;
            }
            printf("Valor invalido.\n");
        }
        else {
            printf("Valor invalido!\n");
            limpaBuffer(); // Caso aperte algum caractere que nao seja numero, vai ficar com erro no buffer, precisa entao liberar ele
        }
    }
    // Data atual para a entrada
    tm = *localtime(&t);
    reg.entrada.ano = tm.tm_year + 1900;
    reg.entrada.mes = tm.tm_mon + 1;
    reg.entrada.dia = tm.tm_mday;
    insereLista(registros, reg);
    printf("Paciente cadastrado com sucesso!\n");
}

// Busca por um RG para mostrar os dados do paciente respectivo
void consultaCadastrado(Lista *registros) {
    ELista *registro = procuraRG(registros);
    if (registro != NULL) {
        printf("Nome: %s | Idade: %d | RG: %s | Entrada: %02d/%02d/%d\n",  registro->dados.nome, registro->dados.idade, registro->dados.rg, registro->dados.entrada.dia, registro->dados.entrada.mes, registro->dados.entrada.ano);
    }
}

void attDados(Lista *registros) {
    ELista *registro = procuraRG(registros);
    Registro reg;
    if (registro != NULL) {
        printf("Digite -1 em qualquer um dos prompts para manter os dados atuais do paciente naquela categoria.\n"); // Caso nao queira mudar tudo e nao lembre dos dados antigos
        while (1) {
            pegaInputUsuario("Digite o novo RG com 9 digitos: ", reg.rg, sizeof(reg.rg));
            if (strcmp(reg.rg, "-1") == 0 || checaRGValido(reg.rg)) { // Mesmo check mas aceita -1 tambem para manter o RG
                break;
            }
            printf("RG invalido!\n");
        }
        if (strcmp(reg.rg, "-1") == 0) {
            strcpy(reg.rg, registro->dados.rg); // Caso seja -1, vai "atualizar" para o mesmo valor
        }
        while (1) {
            pegaInputUsuario("Digite o novo nome: ", reg.nome, sizeof(reg.nome));
            if (strcmp(reg.nome, "-1") == 0 || checaNome(reg.nome)) { // Mesma logica acima
                break;
            }
            printf("Nome invalido!\n");
        }
        if (strcmp(reg.nome, "-1") == 0) {
            strcpy(reg.nome, registro->dados.nome); // Mesma logica acima
        }
        while (1) {
            printf("Digite a nova idade: ");
            if (scanf("%d", &(reg.idade))) {
                if (reg.idade >= -1 || reg.idade <= 120) { // Mesma logica acima
                    break;
                }
                printf("Valor invalido.\n");
            }
            else {
                printf("Valor invalido!\n");
                limpaBuffer();
            }
        }
        if (reg.idade == -1) {
            reg.idade = registro->dados.idade; // Mesma logica acima
        }
        // Atualizando os dados com os novos/"novos" com valores antigos se escolheram -1
        strcpy(registro->dados.rg, reg.rg);
        strcpy(registro->dados.nome, reg.nome);
        registro->dados.idade = reg.idade;
        printf("Dados atualizados com sucesso!\n");
        return;
    }
}

// Remove um paciente da lista baseado no seu RG
void removerPaciente(Lista *registros) {
    ELista *registro = procuraRG(registros);
    if (registro != NULL) {
        removeLista(registros, registro->dados.rg);
        printf("Paciente removido com sucesso!\n");   
    }
}

// -----------
// |  Menus  |
// -----------

void cadastrar(Lista *registros) {
    char menu[] = "--------------\n|  Cadastrar  |\n--------------\n\n\t1. Cadastrar paciente\n\t2. Consultar paciente\n\t3. Lista completa\n\t4. Atualizar dados de paciente\n\t5. Remover paciente";
    int escolha;
    void (*operacao[]) (Lista *registros) = {novoPaciente, consultaCadastrado, mostraLista, attDados, removerPaciente};
    
    while(1) {
        escolha = checaEscolha(menu, 5);
        if (escolha == -1) {
            return;
        }
        operacao[escolha](registros);
        printf("\n");
    }
    return;
}

void placeholderAtendimento(Lista *registros) { // Horrivel, mas precisei fazer isso para
// manter a estrutura de array com ponteiro das funcoes para evitar ficar colocando
// Switch Case
    return;
}

void enfileiraPaciente(Lista *registros, Fila *fila) {
    ELista *paciente = procuraRG(registros);
    if (paciente == NULL) {
        return;
    }
    EFila *ef = criaEFila(paciente->dados);
    enfileira(fila, ef);
    return;
}

void atendimento(Lista *registros, Fila *fila) {
    char menu[] = "---------------\n| Atendimento |\n---------------\n\n\t1. Enfileirar paciente\n\t2. Desenfileirar paciente\n\t3. Mostrar fila";
    int escolha;

    while(1) {
        escolha = checaEscolha(menu, 3);
        if (escolha == -1) {
            return;
        }
        switch (escolha) { // Tava tentando evitar, mas nesse caso fica melhor
            case 0:
                enfileiraPaciente(registros, fila);
                break;
            case 1:
                desenfileira(fila);
                break;
            case 2:
                mostraFila(fila);
                break;
        }
        printf("\n");
    }
    return;
}

void pesquisa(Lista *registros) {
    char menu[] = "------------\n| Pesquisa |\n------------\n\n\t1. Ordernar por ano\n\t2. Ordenar por mes\n\t3. Ordenar por dia\n\t4. Ordenar por idade";
    int escolha;
    while(1) {
        escolha = checaEscolha(menu, 4);
        if (escolha == -1) {
            return;
        }
        if (registros->qtde == 0) {
            printf("Nao ha pacientes cadastrados.\n");
            return;
        }
        ABB *arvere = criaABB();
        ELista *atual = registros->inicio;
        while (atual != NULL) {
            inserePaciente(arvere, atual->dados, escolha);
            atual = atual->proximo;
        }
        ordenaEABB(arvere->raiz);
        free(arvere);
        free(atual);
        printf("\n");
    }
    return;
}

// Salva os dados no arquivo binario "db_pacientes"
void salva(Lista *registros) {
    if (registros->qtde == 0) {
        printf("Sem dados para salvar!\n"); // Sair cedo caso nao haja pacientes cadastrados
        return;
    }
    FILE *db_pacientes = fopen("db_pacientes", "wb"); // Colocando os dados em binario
    ELista *atual = registros->inicio;
    while (atual != NULL) {
        fwrite(&(atual->dados), sizeof(Registro), 1, db_pacientes); // Escrevendo um por um
        atual = atual->proximo;
    }
    fclose(db_pacientes);
    printf("Arquivo salvo!\n");
}

// Carrega os dados encontrados no arquivo binario "db_pacientes"
void carrega(Lista *registros) {
    resetaLista(registros); // Resetando a lista para nao ficar dando append
    // Podia deixar sem, mas nao quero que junte files diferentes
    FILE *db_pacientes = fopen("db_pacientes", "rb");
    Registro reg;
    while (fread(&reg, sizeof(Registro), 1, db_pacientes)) { // Atravessa o db_pacientes enquanto adiciona os dados na lista
        insereLista(registros, reg);
    }
    fclose(db_pacientes);
    printf("Arquivo carregado!\n");
    printf("Qtde carregada: %d\n", registros->qtde);
    return;
}

void carrega_salva(Lista *registros) {
    char menu[] = "---------------------\n| Carregar / Salvar |\n---------------------\n\n\t1. Carregar arquivo\n\t2. Salvar arquivo";
    int escolha;
    void (*operacao[]) (Lista *registros) = {carrega, salva};

    while(1) {
        escolha = checaEscolha(menu, 2);
        if (escolha == -1) {
            return;
        }
        operacao[escolha](registros);
        printf("\n");
    }
    return;
}

void sobre(Lista *registros) { // Nao usa os registros, mas mantenho pra usar o array de ponteiro de funcoes 
    printf("---------\n| Sobre |\n---------\n");
    printf("Igor Vicente Cutalo R.A. 22.123.062-6\n");
    printf("Ciencia da computacao - 4 ciclo\n");
    printf("CC4652 - Estrutura de Dados\n");
    printf("13/05/2024?\n");
}