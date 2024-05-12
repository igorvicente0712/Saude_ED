#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "auxiliares.h"

// Pega um RG do usuario e checa se o RG existe na lista
ELista *procuraRG(Lista *registros) {
    if (registros->inicio == NULL) { // Sair cedo caso nao haja pacientes cadastrados
        printf("Nao ha pacientes cadastrados.\n");
        return NULL;
    }
    char rg[10];
    while (1) {
        pegaInputUsuario("Digite o RG do paciente com 9 digitos (sem pontuacao): ", rg, sizeof(rg));
        if (checaRGValido(rg)) {
            break;
        }
        else {
            printf("RG invalido!\n");
        }
    }
    ELista *atual = registros->inicio;
    while (atual != NULL) {
        if (strcmp(atual->dados.rg, rg) == 0) { // Caso achar o RG, mostra os dados
            return atual;
        }
        atual = atual->proximo;
    }
    printf("RG nao registrado\n");
    return NULL;
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

// Codigo adaptado do Stackoverflow
void pegaInputUsuario(char mensagem[], char buffer[], int buffer_size) {
    while (1)
    {
        char *p;
        fputs(mensagem, stdout); // Mostra a mensagem
        if (fgets(buffer, buffer_size, stdin) == NULL) // Tenta ler uma linha de input
        {
            printf("Erro de leitura.\n");
            exit(EXIT_FAILURE); // Retornar o erro
        }
        p = strchr( buffer, '\n' ); // Procura \n
        
        // Testando se o input nao era maior que o buffer)
        if (p == NULL)
        {
            int c;
            // Faltar um caractere \n nao importa se o proximo
            // eh um \n ou se chegou no EOF (end of file)
            if (!feof(stdin) && (c=getchar()) != '\n')
            {
                printf("Input longo demais!\n");
                // Limpa o buffer jogando fora o que tiver alem do limite
                do
                {
                    if ( c == EOF )
                    {
                        printf("Erro de leitura.\n");
                        exit( EXIT_FAILURE );
                    }

                    c = getchar();

                } while ( c != '\n' );
                // Pede de novo o prompt do usuario
                continue;
            }
        }
        else
        {
            *p = '\0'; // Colocando fim de string no lugar da newline
        }
        break;
    }
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

int checaRGValido(char *rg) {
    if (strlen(rg) < 9) {
        return 0;
    }
    if (strlen(rg) > 9 || rg[9] != '\0') { // Checa se o RG tem 9 caracteres, se não tiver retorna falso
        limpaBuffer();
        return 0;
    }
    else {
        for (int i = 0; i < 9; i++) { // Atravessa o input
            if (rg[i] < '0' || rg[i] > '9') { // Se tiver algum caractere que não seja númerico, retorna falso
                return 0;
            }
        }
    }
    return 1;
}

void checaBuffer(char *c) {
    if (strchr(c, '\n') == NULL) {
        limpaBuffer();
        c[sizeof(c)] = '\0';
        return;
    }
}

void limpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Enquanto tiver algo no buffer diferente de newline ou end of line, continua pegando e jogando fora
    return;
}
