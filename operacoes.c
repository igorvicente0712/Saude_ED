#include <stdio.h>
#include <string.h>
#include "operacoes.h" 
#include "auxiliares.h"
/*
// Cria novo cliente utilizando das funções auxiliares
void novoCliente() {
  Cliente cliente;
  printf("\n-----Nova conta-----\n");
  pegaNome(&cliente);
  pegaCPF(&cliente);
  pegaTipo(&cliente);
  cliente.valor = pegaValor();
  cliente.valor_poupanca = 0.0; // Inicia a poupança valor
  pegaSenha(&cliente);
  FILE *db_clientes = fopen("db_clientes", "ab"); // Coloca a conta nova no final da DB
  fwrite(&cliente, sizeof(Cliente), 1, db_clientes);
  fclose(db_clientes);
  printf("Conta de CPF %s criada com sucesso\n", cliente.cpf);
  return;
}

// Função para apagar cliente baseado no CPF
void apagaCliente() {
  FILE *db_clientes;
  FILE *db_clientes_tmp;
  Cliente cliente;
  char cpf[12];
  printf("\n-----Apagando conta-----\n");
  printf("Digite o CPF a ser apagado com 11 digitos (apenas digitos): ");
  if (pegaInput(cpf, sizeof(cpf)) && checaCPFValido(cpf)) { // Pega o input do usuário relativo ao CPF e checa se ele é válido
    db_clientes = fopen("db_clientes", "rb");
    db_clientes_tmp = fopen("tmp", "wb"); // Usado para atualizar a db_clientes
    while (fread(&cliente, sizeof(Cliente), 1, db_clientes)) { // Atravessa o db_clientes enquanto aloca os dados na variavel cliente
      if (strcmp(cliente.cpf, cpf) != 0) { // Caso não seja o mesmo CPF digitado, coloca no temporario sem alterações
        fwrite(&cliente, sizeof(Cliente), 1, db_clientes_tmp);
      }
      else {
        printf("Conta de CPF %s apagada\n", cliente.cpf); // Caso for o mesmo, não coloca no temporario e aponta que foi "apagado"
      }
    }
    // Aqui troca o temporario pelo original, só que sem o CPF digitado, independente de ter encontrado ou não
    fclose(db_clientes_tmp);
    fclose(db_clientes);
    remove("db_clientes");
    rename("tmp", "db_clientes");
    return;
  }
  else {
    printf("CPF invalido\n");
  }
  return;
}

// Função para debitar de uma conta baseado em CPF e senha, considerando o limite e a taxa da conta
void debito() {
  FILE *db_clientes;
  FILE *db_clientes_tmp;
  Cliente cliente;
  char cpf[12];
  char senha[50];
  float valor_a_debitar, valor_com_taxa, valor_final;
  int sucesso = 0; // Apontar o CPF e senha estão corretos e se conseguiu realizar o debito
  printf("\n-----Debitar da conta-----\n");
  printf("Digite o CPF com 11 digitos (apenas digitos): ");
  if (pegaInput(cpf, sizeof(cpf)) && checaCPFValido(cpf)) {
    limpaBuffer();
    printf("Digite a senha: ");
    if (pegaInput(senha, sizeof(senha)) && checaSenha(senha)) {
      db_clientes = fopen("db_clientes", "rb");
      db_clientes_tmp = fopen("tmp", "wb"); // Usado para atualizar a db_clientes
      while (fread(&cliente, sizeof(Cliente), 1, db_clientes)) {
        if (strcmp(cliente.cpf, cpf) != 0 || strcmp(cliente.senha, senha) != 0) { // Se o CPF ou a senha estiverem diferentes, reescreve sem mudancas
          fwrite(&cliente, sizeof(Cliente), 1, db_clientes_tmp); // Dados sem mudanca
        }
        else { // Caso ambos CPF e senha estiverem iguais com o atual do loop, pega o valor a debitar
          valor_a_debitar = pegaValor();
          sucesso = 1; // Encontrou o CPF e a senha corretos
          if (cliente.valor - ((int)((valor_a_debitar * taxa[cliente.tipo]) * 100))/100.0 >= limite_negativo[cliente.tipo]) { // Só permite o debito caso nao for ultrapassar o limite da conta
            sucesso++; // Apontar que é possivel realizar o débito
            cliente.valor -= ((int)((valor_a_debitar * taxa[cliente.tipo]) * 100))/100.0; // Arredondar para duas casas decimais
            valor_com_taxa = ((int)((valor_a_debitar * taxa[cliente.tipo]) * 100))/100.0;
            valor_final = cliente.valor;
            printf("Valor debitado com sucesso\n");
          }
          fwrite(&cliente, sizeof(Cliente), 1, db_clientes_tmp);
        }
      }
      fclose(db_clientes_tmp);
      fclose(db_clientes);
      remove("db_clientes");
      rename("tmp", "db_clientes");
      if (sucesso > 1) { // Caso tenha conseguido realizar o débito, estará 2
        adicionaExtrato(cpf, valor_a_debitar, valor_com_taxa, valor_final, "Debito");
      }
      else if (sucesso == 1) { // Caso tenha encontrado a conta, porém não tinha saldo dentro do limite do tipo de conta
        printf("Valor excede o limite da conta\n");
      }
      else { // Não vou apontar qual entre os dois está errado, não faria sentido
        printf("CPF ou senha invalidos\n");
      }
      return;
    }
  }
  else {
    printf("CPF invalido\n");
  }
  return;
}

// Função para depositar numa conta baseado no CPF da mesma
void deposito() {
  FILE *db_clientes;
  FILE *db_clientes_tmp;
  Cliente cliente;
  char cpf[12];
  float valor_a_depositar, valor_final;
  int sucesso = 0; // Apontar que foi possível realizar o depósito
  printf("\n-----Depositar na conta-----\n");
  printf("Digite o CPF com 11 digitos (apenas digitos): ");
  if (pegaInput(cpf, sizeof(cpf)) && checaCPFValido(cpf)) { // Pegando e checando o CPF
    limpaBuffer();
    db_clientes = fopen("db_clientes", "rb");
    db_clientes_tmp = fopen("tmp", "wb"); // Para atualizar a db
    while (fread(&cliente, sizeof(Cliente), 1, db_clientes)) { // Atravessando a db, alocando na structure cliente
      if (strcmp(cliente.cpf, cpf) != 0) { // Se não for o cliente, reescreve sem alterações
        fwrite(&cliente, sizeof(Cliente), 1, db_clientes_tmp);
      }
      else { // Caso for o cliente, pega o valor e adiciona na conta
        valor_a_depositar = pegaValor();
        cliente.valor += valor_a_depositar;
        sucesso = 1;
        printf("Valor depositado com sucesso\n");
        fwrite(&cliente, sizeof(Cliente), 1, db_clientes_tmp);
        valor_final = cliente.valor;
      }
    }
    fclose(db_clientes_tmp);
    fclose(db_clientes);
    remove("db_clientes");
    rename("tmp", "db_clientes");
    if (sucesso) {
      adicionaExtrato(cpf, valor_a_depositar, valor_a_depositar, valor_final, "Credito");
    }
    else {
      printf("CPF nao encontrado\n");
    }
    return;	
  }
  else {
    printf("CPF invalido\n");
  }
  return;
}

// Função que mostra o extrato da conta baseado no CPF e senha
void extrato() {
  FILE *db_extratos;
  FILE *db_clientes;
  Cliente cliente;
  char cpf[12], cpf_extrato[12];
  char senha[50];
  char linha[255];
  int sucesso = 0; // Mostrar que foi possível encontrar a conta e se há extratos da mesma
  printf("\n-----Extrato-----\n");
  printf("Digite o CPF da conta a mostrar os extratos com 11 digitos (apenas digitos): ");
  if (pegaInput(cpf, sizeof(cpf)) && checaCPFValido(cpf)) {
    limpaBuffer();
    printf("Digite a senha: ");
    if (pegaInput(senha, sizeof(senha)) && checaSenha(senha)) {
      db_clientes = fopen("db_clientes", "rb");
      db_extratos = fopen("db_extratos", "rb");
      while (fread(&cliente, sizeof(Cliente), 1, db_clientes)) { // Procura se o cliente e a senha estão corretos
        if (strcmp(cliente.cpf, cpf) == 0 || strcmp(cliente.senha, senha) == 0) { // Se o CPF e a senha estiverem iguais, avisa que pode pegar o extrato
          sucesso = 1;
          break; // Não precisa continuar o loop
        }
      }
      if (sucesso) { // Se achou o CPF e senha corretos, continua
        while (fread(&linha, sizeof(linha), 1, db_extratos)) {
          for (int i = 0; i < 11; i++) { // Pegando a parte do CPF dentro do arquivo de extratos, visto que sempre terão 11 caracteres
            cpf_extrato[i] = linha[i];
          }
          cpf_extrato[11] = '\0';
          if (strcmp(cpf, cpf_extrato) == 0) { // Se encontrou o CPF, mostra para o usuario
            sucesso++; // Caso tenha encontrado algum extrato, sinaliza
            for (int i = 12; i < strlen(linha); i++) { // Mostrando o extrato depois do CPF, que representam os primeiros 11 caracteres
              printf("%c", linha[i]);
            }
            printf("\n");
          }
        }
        if (sucesso == 1) { // Caso não tenha encontrado nenhum extrato da conta, avisa
          printf("Sem registros de extrato\n");
        }
      }
      else {
        printf("CPF ou senha invalidos\n");
      }
      fclose(db_extratos);
      fclose(db_clientes);
      return;
    }
    else {
      printf("Senha invalida\n");
    }
  }
  else {
    printf("CPF invalido\n");
  }
  return;
}

// Função para realizar transferencia entre contas baseado no CPF e senha da primeira e no CPF da segunda
// Seria possível realizar o mesmo utilizando as funções de débito e depósito, porém implicaria em mais pesquisas na DB
void transferenciaEntreContas() {
  FILE *db_clientes;
  FILE *db_clientes_tmp;
  Cliente cliente_transferindo;
  Cliente cliente_recebendo;
  Cliente cliente; // variavel genérica para atualizar a db, não é realmente necessária, só serve para não confundir
  char cpf_transferindo[12];
  char cpf_recebendo[12];
  char senha[50];
  float valor_a_transferir;
  int conta_transferindo_encontrada = 0, conta_recebendo_encontrada = 0;
  printf("\n-----Transferencia-----\n");
  printf("Digite o CPF da conta transferindo com 11 digitos (apenas digitos): ");
  if (pegaInput(cpf_transferindo, sizeof(cpf_transferindo)) && checaCPFValido(cpf_transferindo)) { // Pegando CPF da conta transferindo
    limpaBuffer(); // Sem isso, solta um backspace no proximo input, nesse caso a senha
    printf("Digite a senha: ");
    if (pegaInput(senha, sizeof(senha)) && checaSenha(senha)) { // Pega senha da conta transferindo
      db_clientes = fopen("db_clientes", "rb");
      while (fread(&cliente_transferindo, sizeof(Cliente), 1, db_clientes)) { // Procurar a conta transferindo na db
        if (strcmp(cliente_transferindo.cpf, cpf_transferindo) == 0 && strcmp(cliente_transferindo.senha, senha) == 0) { // Se achar o CPF com a senha certa, sai do loop
          conta_transferindo_encontrada = 1; // Apontar que encontrou
          break;
        }
      }
      if (conta_transferindo_encontrada) { // Só pede a outra conta caso tenha achado CPF + senha da conta que irá transferir
        printf("Digite o CPF da conta a receber com 11 digitos (apenas digitos): ");
        if (pegaInput(cpf_recebendo, sizeof(cpf_recebendo)) && checaCPFValido(cpf_recebendo)) { // Pegando CPF da conta recebendo
          limpaBuffer();
          if (strcmp(cpf_recebendo, cpf_transferindo) == 0) { // Impedir transferências para a mesma conta
            printf("Nao eh possivel realizar transferencia para a mesma conta\n");
            fclose(db_clientes);
            return;
          }
          rewind(db_clientes); // Volta para o inicio da DB para procurar a conta recebendo
          while (fread(&cliente_recebendo, sizeof(Cliente), 1, db_clientes)) { // Procurar a conta recebendo na db
            if (strcmp(cliente_recebendo.cpf, cpf_recebendo) == 0) { // Se achar o CPF sai do loop
              conta_recebendo_encontrada = 1; // Apontar que encontrou
              break;
            }
          }
          if (conta_recebendo_encontrada) { // Se encontrou, tenta realizar a transferencia
            valor_a_transferir = pegaValor();
            if (cliente_transferindo.valor - ((int)((valor_a_transferir * taxa[cliente_transferindo.tipo]) * 100))/100.0 >= limite_negativo[cliente_transferindo.tipo]) { 
              // Só realiza se a conta transferindo tiver saldo/credito o suficiente
              db_clientes_tmp = fopen("tmp", "wb"); // Para atualizar a db
              rewind(db_clientes); // Volta para o inicio da DB para começar a atualização
              while (fread(&cliente, sizeof(Cliente), 1, db_clientes)) {
                if (strcmp(cliente.cpf, cpf_transferindo) == 0) { // Se achar o CPF da conta transferindo, coloca o valor atualizado
                  cliente.valor -= ((int)((valor_a_transferir * taxa[cliente_transferindo.tipo]) * 100))/100.0;
                }
                else if (strcmp(cliente.cpf, cpf_recebendo) == 0) { // Se achar o CPF da conta recebendo, coloca o valor atualizado
                  cliente.valor += valor_a_transferir;
                  printf("Transferencia realizada\n");
                }
                fwrite(&cliente, sizeof(Cliente), 1, db_clientes_tmp);
              }
              // Atualizando a db
              fclose(db_clientes_tmp);
              fclose(db_clientes);
              remove("db_clientes");
              rename("tmp", "db_clientes");
              // Extratos de ambos
              adicionaExtrato(cpf_transferindo, valor_a_transferir, ((int)((valor_a_transferir * taxa[cliente_transferindo.tipo]) * 100))/100.0, 
                cliente_transferindo.valor - ((int)((valor_a_transferir * taxa[cliente_transferindo.tipo]) * 100))/100.0, "Debito");
              adicionaExtrato(cpf_recebendo, valor_a_transferir, valor_a_transferir, 
                cliente_recebendo.valor + valor_a_transferir, "Credito");
              return;
            }
            else {
              printf("Conta sem saldo para transferencia\n");
              fclose(db_clientes);
            }
          }
          else {
            printf("CPF destinatario nao encontrado\n");
            fclose(db_clientes);
          }
        }
        else {
          printf("CPF a receber invalido\n");
          fclose(db_clientes);
        }
      }
      else {
        printf("CPF ou senha invalidos\n");
        fclose(db_clientes);
      }
    }
    else {
      printf("Senha invalida\n");
    }
  }
  else {
    printf("CPF invalido\n");
  }
  return;
}

// A função livre, funciona depositando dinheiro da conta nessa "segunda conta" que irá render mensalmente com um valor aleatório entre 0 e 1% 
// Pode retirar o valor da conta contanto que não a torne negativa, porém a conta corrente ainda pode estar negativa para depositar contanto que dentro do limite
// Não há taxas de transferencia entre um e outro
// (deletar o arquivo de data para testar, será mostrado no extrato)
void poupanca() {
  FILE *db_clientes;
  FILE *db_clientes_tmp;
  Cliente cliente;
  int escolha;
  char cpf[12];
  char senha[50];
  float valor, valor_final_poupanca, valor_final_corrente;
  int sucesso = 0; // Apontar o CPF e senha estão corretos
  printf("\n-----Poupanca-----\n");
  printf("Digite o CPF com 11 digitos (apenas digitos): ");
  if (pegaInput(cpf, sizeof(cpf)) && checaCPFValido(cpf)) { // Pegando o CPF
    limpaBuffer();
    printf("Digite a senha: ");
    if (pegaInput(senha, sizeof(senha)) && checaSenha(senha)) { // Pegando a senha
      db_clientes = fopen("db_clientes", "rb");
      db_clientes_tmp = fopen("tmp", "wb"); // Para atualização da DB
      while (fread(&cliente, sizeof(Cliente), 1, db_clientes)) {
        if (strcmp(cliente.cpf, cpf) != 0 || strcmp(cliente.senha, senha) != 0) { // Se o CPF ou a senha estiverem diferentes, reescreve sem mudancas
          fwrite(&cliente, sizeof(Cliente), 1, db_clientes_tmp); // Dados sem mudanca
        }
        else { // Caso ambos CPF e senha estiverem iguais com o atual do loop, pega o valor a debitar
          sucesso = 1; // Apontar que encontrou CPF + senha corretos
          while (1) { // Continuar enquanto não digitar uma opção válida
            printf("Saldo na poupanca: %.2f\n", cliente.valor_poupanca); // Mostrar o saldo, já que já foi checado o CPF e senha
            printf("Escolha a operacao atraves de seu digito\n\t1. Depositar\n\t2. Retirar\nOperacao: ");
            if (scanf("%d", &escolha)) { // Pega o input e testa para ver se é um int
              if (escolha == 1) { // Depositar na poupança
                limpaBuffer();
                valor = pegaValor();
                if (cliente.valor - valor >= limite_negativo[cliente.tipo]) { // Se o depósito está dentro dos limites do tipo de conta, realiza a transferencia para a poupança
                  sucesso = 2; // Para saber se foi realizada e qual foi a operação
                  cliente.valor -= valor; // Tirando da corrente
                  valor_final_corrente = cliente.valor; // Para o extrato
                  cliente.valor_poupanca += valor; // Adicionando na poupança
                  valor_final_poupanca = cliente.valor_poupanca; // Para o extrato
                  printf("Deposito na poupanca realizado com sucesso\n");
                }
                else { // Caso contrario não faz nada
                  printf("Sem saldo na conta corrente\n");
                }
                break;
              }
              else if (escolha == 2) { // Retirar da poupança
                limpaBuffer();
                valor = pegaValor();
                if (cliente.valor_poupanca - valor >= 0) { // Se a retirada não deixa a poupança negativa, realiza a transferencia para a corrente
                  sucesso = 3; // Para saber se foi realizada e qual foi a operação
                  cliente.valor += valor; // Adicionando na corrente
                  valor_final_corrente = cliente.valor; // Para o extrato
                  cliente.valor_poupanca -= valor; // Retirando da poupança
                  valor_final_poupanca = cliente.valor_poupanca; // Para o extrato
                  printf("Retirada da poupanca realizada com sucesso\n");
                }
                else {
                  printf("Sem saldo na conta poupanca\n");
                }
                break;
              }
            }
            else { // Caso não for um int mostra que não é valido e esvazia o buffer do input
              printf("Digito invalido\n");
              limpaBuffer();
            }
          }
          fwrite(&cliente, sizeof(Cliente), 1, db_clientes_tmp);
        }
      }
      // Atualizando a DB
      fclose(db_clientes_tmp);
      fclose(db_clientes);
      remove("db_clientes");
      rename("tmp", "db_clientes");

      if (sucesso == 2) { // Caso tenha encontrado o CPF e senha e tenha realizado o depósito, estará valor 2 e coloca no extrato
        adicionaExtrato(cpf, valor, valor, valor_final_poupanca, "Deposito poupanca");
        adicionaExtrato(cpf, valor, valor, valor_final_corrente, "Debito");
      }
      else if (sucesso == 3) { // Caso tenha encontrado o CPF e senha e tenha realizado a retirada, estará valor 3 e coloca no extrato
        adicionaExtrato(cpf, valor, valor, valor_final_poupanca, "Retirada poupanca");
        adicionaExtrato(cpf, valor, valor, valor_final_corrente, "Credito");
      }
      else if (sucesso == 0) { // Caso não tenha encontrado CPF + senha
        printf("CPF ou senha invalidos\n");
      }
      return;
    }
  }
  else {
    printf("CPF invalido\n");
  }
  return;
}

void listaClientes() { // Funcao utilizada para teste, mostra todos os clientes registrados
    Cliente cliente;
    FILE *db_clientes = fopen("db_clientes", "rb");
  char *tipos[] = {"Comum", "Plus"};
  printf("Lista clientes aqui\n");
    if (db_clientes) {
        while (fread(&cliente, sizeof(cliente), 1, db_clientes)) {
      printf("Nome: %s\nCPF: %s\nTipo: %s\nValor na conta: %.2f\nValor na poupanca: %.2f\nSenha: %s\n", 
        cliente.nome, cliente.cpf, tipos[cliente.tipo], cliente.valor, cliente.valor_poupanca, cliente.senha);
        }
        fclose(db_clientes);
        return;
    }
    else {
        return;
    }
}
*/

// Possivel jeito mais inteligente de fazer o seguinte:
// Matrix com os menus
// Array com os numeros de itens de cada operacao
// char *arr[] = {"Menu1", "Menu2", "Menu3"};
// for (int i = 0; i < 3; i++) 
// {
//   printf("%s\n", arr[i]);
// }
// Ai nao sei como fazer pra puxar as funcoes de cada haha

void cadastrar() {
    char menu[] = "--------------\n| Cadastrar  |\n--------------\n\n\t1. Cadastrar paciente\n\t2. Consultar paciente\n\t3. Lista completa\n\t4. Atualizar dados de paciente\n\t5. Remover paciente";
    int escolha;
    //void (*operacao[]) () = {novoPaciente, consultaCadastrado, mostraLista, attDados, removerPaciente};
    
    while(1) {
        escolha = checaEscolha(menu, 5);
        if (escolha == -1) {
          return;
        }
        //operacao[escolha]();
    }
    return;
}

void atendimento() {
    char menu[] = "---------------\n| Atendimento |\n---------------\n\n\t1. Enfileirar paciente\n\t2. Desenfileirar paciente\n\t3. Mostrar fila";
    int escolha;
    //void (*operacao[]) () = {enfileira, desenfileira, mostraFila};

    while(1) {
        escolha = checaEscolha(menu, 3);
        if (escolha == -1) {
          return;
        }
        //operacao[escolha]();
    }
    return;
}

void pesquisa() {
    char menu[] = "------------\n| Pesquisa |\n------------\n\n\t1. Ordernar por ano\n\t2. Ordenar por mes\n\t3. Ordenar por dia\n\t4. Ordenar por idade";
    int escolha;
    //void (*operacao[]) () = {ordenaAno, ordenaMes, ordenaDia, ordenaIdade};

    while(1) {
        escolha = checaEscolha(menu, 4);
        if (escolha == -1) {
          return;
        }
        //operacao[escolha]();
    }
    return;
}

void carrega_salva() {
    char menu[] = "---------------------\n| Carregar / Salvar |\n---------------------\n\n\t1. Carregar arquivo\n\t2. Salvar arquivo";
    int escolha;
    //void (*operacao[]) () = {carregar, salvar};

    while(1) {
        escolha = checaEscolha(menu, 2);
        if (escolha == -1) {
          return;
        }
        //operacao[escolha]();
    }
    return;
}

void sobre() {
    printf("---------\n| Sobre |\n---------\n");
    printf("Igor Vicente Cutalo R.A. 22.123.062-6\n");
    printf("Ciencia da computacao - 4 ciclo\n");
    printf("CC4652 - Estrutura de Dados\n");
    printf("13/05/2024?\n");
}