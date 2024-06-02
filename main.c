#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX_NOME 100

/*-----------------------------------------------------------------------------------
Declaração dos tipos usados no programa.
Os nomes são sugestivos às suas aplicações aqui usadas.
O tipo 'reserva' relaciona os tipos 'voo' e 'pessoa'.
-----------------------------------------------------------------------------------*/
typedef struct {
    int capacidade;
    double executiva;
    double economica;
} Aviao;

typedef struct Voo {
    char id[5];
    char data[11];
    char localPartida[5];
    char localChegada[5];
} Voo;

typedef struct {
    char *nome;
    char *sobrenome;
    char cpf[15];
} Pessoa;

typedef struct {
    Pessoa pessoa;
    Voo voo;
    char assento[5];
    char classe[10];
    double valor;
} Reserva;

/*-----------------------------------------------------------------------------------
Funções de utilidades (principalmente para alocação de memória 
e manipulação de arquivos).
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

/*
Responsável por abrir um arquivo com determinado
modo de operação (leitura, escritas, etc).
O nome do arquivo e a operação são passados por parâmetro.
Se algo der errado, um erro é disparado.
Senão, um ponteiro para o arquivo é retornado.
*/
FILE *abrirArquivo (char *nome, char *operacao) {
    FILE *arquivo;

    if ((arquivo = fopen(nome, operacao)) == NULL) {
        printf("Erro: não foi possível abrir o arquivo\n");
        exit(1);
    }

    return (arquivo);
}

/*-----------------------------------------------------------------------------------
Funções usadas para coordenar o programa (menu, opções, saída de dados, etc).
Descrição específica em cada função.
-----------------------------------------------------------------------------------*/

/*
Decodifica os caracteres para um número simbólico.
O número determina o tipo de ação.
O código (char) é recebido por parâmetro.
O número correspondente é retornado.

1 - Abertura do voo (AV)
2 - Realizar reserva (RR)
3 - Consultar reserva (CR)
4 - Modificar reserva (MR)
5 - Cancelar reserva (CA)
6 - Fechamento do dia (FD)
7 - Fechamento do voo (FV)
*/
int decodificar (char codigo[2]) {
    if (codigo[1] == 'R') {
        if (codigo[0] == 'R') {
            return (2);
        } else {
            if (codigo[0] == 'C') {
                return (3);
            } else {
                return (4);
            }
        }
    } 

    if (codigo[1] == 'V') {
        if (codigo[0] == 'A') {
            return (1);
        } else {
            return (7);
        }
    }

    if (codigo[0] == 'F') {
        return (6);
    }
    
    return (5);
}

/*
Exibe uma linha com 50 caracteres
*/
void linha () {
    printf("--------------------------------------------------\n");
}

/*-----------------------------------------------------------------------------------
Funções usadas para acessar e manipular os dados. 
Descrição específica em cada função.
-----------------------------------------------------------------------------------*/

/*

*/
void aberturaVoo (Aviao *aviao) {
    scanf("%d %lf %lf", &aviao->capacidade, &aviao->economica, &aviao->executiva);
}

/*
Salva uma nova reserva no arquivo.
Os dados da reserva são passados por parâmetro.
O arquivo é aberto e os dados são gravados (separadamente, apenas pra facilitar
a visualização no código).
Nada é retornado.
*/
void adicionarReserva (Reserva reserva) {
    FILE *reservas = abrirArquivo("voos.txt", "w");

    fprintf(reservas, "%s %s %s ", reserva.pessoa.nome, reserva.pessoa.sobrenome, reserva.pessoa.cpf);
    fprintf(reservas, "%s %s ", reserva.voo.data, reserva.voo.id);
    fprintf(reservas, "%s %s %lf ", reserva.assento, reserva.classe, reserva.valor);
    fprintf(reservas, "%s %s\n", reserva.voo.localPartida, reserva.voo.localChegada);

    fclose(reservas);
}

void modificarReserva (char *nome, char *sobrenome, char cpf[14], char assento[3]) {
    FILE *reservas = abrirArquivo("voos.txt", "w");

    fclose(reservas);
}

void consultarReserva (char cpf[15]) {
    FILE *reservas = abrirArquivo("voos.txt", "w");



    fclose(reservas);

    
}

/*-----------------------------------------------------------------------------------
Função principal.
Coordena o funcionamento das outras funções.
-----------------------------------------------------------------------------------*/

int main (void) {

    char codigo[2];
    int num;
    Aviao aviao;

    do {
        scanf("%s", &codigo);
        
        num = decodificar(codigo);

        switch (num) {
            case 1:
                aberturaVoo(&aviao);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5: 
                break;
            case 6: 
                break;
            default:
                printf("fechar voo\n");
        }
    } while (num != 7);

    return (0);
}