#ifndef AUXILIARES
#define AUXILIARES
/*
enum tipo_conta {COMUM, PLUS};

typedef struct Cliente {
    char nome[100];
    char cpf[12];
    enum tipo_conta tipo;
    float valor;
    float valor_poupanca;
    char senha[50];
} Cliente;

extern float taxa[];

extern int limite_negativo[];

void pegaNome(Cliente *cliente);

void pegaCPF(Cliente *cliente);

void pegaTipo(Cliente *cliente);

float pegaValor();

void pegaSenha(Cliente *cliente);

int checaNome(char *nome);

int checaCPFValido(char *cpf);

int checaCPFDB(char *cpf);

int checaSenha(char *senha);

void adicionaExtrato(char *cpf, float valor_bruto, float valor_com_taxa, float saldo_final, char *operacao);

*/
int pegaInput(char *var, int tamanho);

int checaEscolha(char *texto, int n_escolhas);

void limpaBuffer();

#endif