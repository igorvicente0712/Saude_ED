#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "auxiliares.h"
/*
float taxa[] = {1.05, 1.03}; // Taxa dependendo do tipo de conta
int limite_negativo[] = {-1000, -5000}; // Limite dependendo do tipo de conta

void pegaNome(Cliente *cliente) {
    while (1) {
    printf("Digite o nome: "); 
    pegaInput(cliente->nome, sizeof(cliente->nome)); // Pegando input
    if (checaNome(cliente->nome)) { // Checa se o nome é valido (tem pelo menos 3 caracteres, não tem números ou sinais)
      break;
    }
    else {
      printf("Nome invalido\n");
    }
  }
    return;
}

// Pega CPF, checa e joga diretamente na structure
void pegaCPF(Cliente *cliente) {
    while (1) { // Para continuar pedindo por um CPF válido
        printf("Digite o CPF com 11 digitos (apenas digitos): ");
        pegaInput(cliente->cpf, sizeof(cliente->cpf)); // Pega CPF e joga na structure
        if (!checaCPFValido(cliente->cpf)) { // Checa se não é valido (tem 11 digitos e nada mais)
            printf("CPF invalido\n");
      }
        else if (checaCPFDB(cliente->cpf)) { // Checa se o CPF já está na DB
            printf("CPF já registrado\n");
            limpaBuffer();
        }
        else { // Caso não seja inválido e não esteja na DB, sai da função
            limpaBuffer(); 
            return;
        }
    }
}

// Função para pegar o tipo de conta
void pegaTipo(Cliente *cliente) {
    int tipo;
    while (1) { // Continuar pedindo até ser um tipo válido
    printf("Digite o tipo:\n\t1. Comum\n\t2. Plus\nTipo: ");
        if (scanf("%d",&tipo) && (tipo == 1 || tipo == 2)) { // Se for um tipo valido, transforma em 0 para comum e 1 para plus
            cliente->tipo = tipo - 1;
            limpaBuffer();
            break;
        }
        else {
            limpaBuffer();
            printf("Tipo invalido\n");
        }
  }
    return;
}

// Para pegar valores com duas casas decimais
float pegaValor() {
    char tmp[255];
    int n_pontuacao = 0; // Para saber se foi colocado mais de um . ou ,
    int i;
    while (1) {
        int n_pontuacao = 0;
    printf("Digite o valor: ");
        if (pegaInput(tmp, sizeof(tmp))) {
            for (i = 0; i < strlen(tmp); i++) {
                if ((tmp[i] < '0' || tmp[i] > '9') && tmp[i] != ',' && tmp[i] != '.') { // Procurando valores invalidos
                    break; // Saindo do loop para apontar que i não chegou até o final
                }
                else if (tmp[i] == ',' || tmp[i] == '.') { // Substituindo virgula por ponto e contando pontuações
                    tmp[i] = '.';
                    n_pontuacao++;
                }
            }
            if (i == strlen(tmp) && n_pontuacao <= 1 && i <= 38) { // Se chegou no final com todos valores validos,
            // e encontrou apenas uma/nenhuma pontuacao e o valor não tem chance de passar do limite de floats 
                return ((int)((atof(tmp)) * 100))/100.0; // Arredondando para pegar apenas 2 casas decimais
            }
            else {
                printf("Valor invalido\n", i, strlen(tmp), n_pontuacao);
            }
        }
        else {
            printf("Valor invalido\n");
        }
  }
}

// Função para pegar senha e checar se é válida
void pegaSenha(Cliente *cliente) {
    while (1) {
    printf("Digite a senha com pelo menos 5 caracteres: "); 
    if((pegaInput(cliente->senha, sizeof(cliente->senha))) && (checaSenha(cliente->senha))) { // Pega a senha e vê se é válida (pelo menos 5 caracteres)
            break;
        }
    else {
      printf("Senha invalida\n");
    }
  }
    return;
}

// Função para checar se o nome é válido
int checaNome(char *nome) {
    int len = strlen(nome);
    int letras = 0;
    if (len < 3) { // Se for um nome sem pelo menos 3 caracteres, retorna falso
        return 0;
    }
    for (int i = 0; i < len; i++) {
        if (!((nome[i] >= 'a' && nome[i] <= 'z') || (nome[i] >= 'A' && nome[i] <= 'Z') || (nome[i] == ' '))) { // Caso tiver algum caractere não alfabético ou espaço, retorna falso
            printf("i: %d \n", i);
            return 0;
        }
        else {
            letras++;
        }
    }
    if (letras < 3) { // Impedir de fazer um nome com varios espacos
        return 0;
    }
    return 1;
}

// Checa se o CPF tem 11 caracteres
int checaCPFValido(char *cpf) {
    if (strlen(cpf) != 11 || cpf[11] != '\0') { // Checa se o CPF tem 11 caracteres, se não tiver retorna falso
        //limpaBuffer();
        return 0;
    }
    else {
        for (int i = 0; i < 11; i++) { // Atravessa o input
            if (cpf[i] < '0' || cpf[i] > '9') { // Se tiver algum caractere que não seja númerico, retorna falso
                return 0;
            }
        }
    }
    return 1;
}

// Checa se existe o CPF na DB
int checaCPFDB(char *cpf) {
    Cliente cliente;
    FILE *db_clientes = fopen("db_clientes", "rb");
    while (fread(&cliente, sizeof(Cliente), 1, db_clientes)) {
        if (strcmp(cliente.cpf, cpf) == 0) { // Se achou o CPF na DB, retorna verdadeiro
            fclose(db_clientes);
            return 1;
        }
    }
    fclose(db_clientes);
    return 0; // Caso não tenha achado, retorna falso
}

// Checar se a senha é válida
int checaSenha(char *senha) {
    int len = strlen(senha);
    if (len < 5) { // Se for menor que 5 caracteres, retorna falso
        return 0;
    }
    for (int i = 0; i < len; i++) { // Atravessando o input
        if (senha[i] == ',' || senha[i] == ' ') { // Se tiver virgula ou espaço, retorna falso
            return 0;
        }
    }
    return 1;
}

// Função para adicionar extrato de transação
void adicionaExtrato(char *cpf, float valor_bruto, float valor_com_taxa, float saldo_final, char *operacao) {
    FILE *db_extratos;
    time_t seg_desde_1970 = time(NULL); // Para pegar o tempo atual
    char linha[255];
    struct tm data_local = *localtime(&seg_desde_1970); // Aqui é o tempo atual real
    db_extratos = fopen("db_extratos", "ab"); // Colocar no final do arquivo
    sprintf(linha ,"%s Data: %d-%02d-%02d %02d:%02d %s: %.2f Tarifa: %.2f Saldo final: %.2f", cpf,
        data_local.tm_year + 1900, data_local.tm_mon + 1, data_local.tm_mday, data_local.tm_hour, data_local.tm_min,
        operacao, valor_bruto, valor_com_taxa - valor_bruto, saldo_final); // Extrato já estruturado com o CPF no inicio para busca posterior
    fwrite(linha, sizeof(linha), 1, db_extratos);
    fclose(db_extratos);
    return;
}

*/

// Auxiliar com a tomada de input
int pegaInput(char *var, int tamanho) {
    if (!fgets(var, tamanho, stdin)) { // Se deu erro no input, retorna falso
        return 0;
    }
    var[strcspn(var, "\n")] = 0; // Tirando a newline do input, strcspn retorna o indice que encontrar a primeira newline, transformando em 0 para sinalizar final da string
    //printf("Input: %s\n",var); // Ver o que está sendo atribuido realmente, apenas para """debug"""
    return 1;
}

int checaEscolha(char *texto, int n_escolhas) {
  int escolha;
  while (1) {
    printf("%s\n\t%d. Sair\n\n", texto, n_escolhas + 1);
    printf("Operacao: ");
    if (scanf("%d", &escolha)) { // Pega o input e testa para ver se eh um int
        if (escolha == n_escolhas + 1) { // Sair se escolha for a ultima
            return -1;
        }
        else if (escolha >= 1 && escolha <= n_escolhas) { // Se o valor estiver no range certo, retorna o indice correto para executar a funcao
          limpaBuffer(); // Le e descarta os caracteres no buffer do input ate chegar numa linha nova, impede de pegar novas linhas automaticamente em cada loop pelo scanf
          return escolha - 1;
        }
        else {
            printf("\nDigito invalido\n");
            limpaBuffer();
        }
    }
    else {
        printf("\nDigito invalido\n");
        limpaBuffer();
    }
  }
}

void limpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Enquanto tiver algo no buffer diferente de newline ou end of line, continua pegando e jogando fora
    return;
}
