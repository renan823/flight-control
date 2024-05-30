#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-----------------------------------------------------------------------------------
Declaração dos tipos usados no programa.
Os nomes são sugestivos às suas aplicações aqui usadas.
O tipo 'reserva' relaciona os tipo 'voo' e 'pessoa' por meio 
do id e cpf (respectivamente).
-----------------------------------------------------------------------------------*/
struct Aviao {
    capacidade: int;
    executiva: double;
    economica: double;
}

struct Voo {
    id: char[4];
    data: char[10];
    localPartida: char[4];
    localChegada: char[4];
}

struct Pessoa {
    nome: *char;
    sobrenome: *char;
    cpf: char[14];
}

struct Reserva {
    cpfPessoa: char[14];
    idVoo: char[4];
    assento: char[4];
    classe: *char;
    valor: double;
}

/*-----------------------------------------------------------------------------------
Funções de utilidades (principalmente para alocação de memória).
Descrição específica em cada função.
-----------------------------------------------------------------------------------*/

/*
Aloca, dinamicamente, um vetor sem tipo.
O tamanho (em bytes) do vetor é definido pela multiplicação da quantidade de items e 
do tamanho (em bytes) dos items.
A quantidade de items e o tamanho são passados por parâmetro.
Se não houver memória para alocação, um erro é disparado.
Em caso de sucesso na alocação, o vetor é retornado.
*/
void *alocar (int quantidade, int tamanho) {
    void *vetor;

    if ((vetor = (void *) malloc(quantidade * tamanho)) == NULL) {
        printf("Erro: sem memória para alocar\n");
        exit(1);
    }

    return (vetor);
}

/*-----------------------------------------------------------------------------------
Funções usadas para acessar e manipular os dados. 
Descrição específica em cada função.
-----------------------------------------------------------------------------------*/

/*
Adiciona uma pessoa na lista de pessoas.
A pessoa a ser cadastrada, a lista de pessoas e o tamanho da lista
de pessoas são passados via parâmetro.
A lista é redimencionada para a inserção
Nada é retornado.
*/
void adicionarPessoa (Pessoa pessoa, Pessoa *pessoas, int tamanho) {
    
}

/*
Procura por uma pessoa na lista de pessoas baseado no cpf.
O cpf, a lista de pessoas e o tamanho da lista de pessoas são passados via parâmetro.
A pessoa encontrada é retornada.
*/
Pessoa encontrarPessoaPorCpf (char cpf[14], Pessoa *pessoas, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (pessoas[i].cpf == cpf) {
            return (pessoas[i]);
        }
    }
}

/*-----------------------------------------------------------------------------------
Funções usadas para coordenar o programa (menu, opções, saída de dados, etc).
Descrição específica em cada função.
-----------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------
Função principal.
Coordena o funcionamento das outras funções.
-----------------------------------------------------------------------------------*/

int main (void) {



    return (0);
}