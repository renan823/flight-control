#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX_STRING 100

/*-----------------------------------------------------------------------------------
Declaração dos tipos usados no programa.
Os nomes são sugestivos às suas aplicações aqui usadas.
O tipo 'reserva' relaciona os tipos 'voo' e 'pessoa'.
-----------------------------------------------------------------------------------*/
typedef struct {
    int capacidade;
    float executiva;
    float economica;
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
    float valor;
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

/*
Permite a leitura de uma string.
O tamanho máximo da string é passado por parâmetro.
Inicializado com tamanho máximo, o ponteiro para char recebe a
entrada do usuário.
Em seguida, é removido o caracter '\n' (se existir).
Então, o ponteiro passa por uma realocação de memória,
ocupando apenas o espaço necessário para acomodar a string.
Se a realocação não for bem sucedida, um erro é mostrado.
Senão, a string é retornada.
*/
char *lerString (int tamanho) {
    char *string = (char *) alocar(tamanho, sizeof(char));

    scanf("%s", string);

    //trocar \n por \0
    if (string[strlen(string) - 1] == '\n') {
        string[strlen(string) - 1] = '\0';
    }

    //redimensionar
    if ((string = (char *) realloc(string, strlen(string) * sizeof(char))) == NULL) {
        printf("Erro: sem memória para alocar\n");
    }

    return (string);
}

/*
Permite a leitura de datas.
O tamanho 11 acomoda a data e '\0' no final.
O programa recebe como entrada do usuário dia, mês e ano.
A entrada é processada de modo a formatar a data como
dd/mm/YYYY.
A data formatada é retornada.
*/
char *lerData () {
    char *data = (char *) alocar(11, sizeof(char));

    fgets(data, 11, stdin); //Não tá funcionando!!!!

    for (int i = 0; i < strlen(data); i++) {
        if (data[i] == ' ') {
            data[i] = '/';
        }
    }

    return (data);
}

/*-----------------------------------------------------------------------------------
Funções usadas para acessar e manipular os dados. 
Descrição específica em cada função.
-----------------------------------------------------------------------------------*/

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
    fprintf(reservas, "%s %s %f ", reserva.assento, reserva.classe, reserva.valor);
    fprintf(reservas, "%s %s\n", reserva.voo.localPartida, reserva.voo.localChegada);

    fclose(reservas);
}

void editarReserva (char *nome, char *sobrenome, char cpf[14], char assento[3]) {
    FILE *reservas = abrirArquivo("voos.txt", "w");

    fclose(reservas);
}

void consultarReserva (char cpf[15]) {
    FILE *reservas = abrirArquivo("voos.txt", "w");



    fclose(reservas);

    
}

/*-----------------------------------------------------------------------------------
Funções para entrada de dados (em cada caso descrito).
Descrição específica em cada função.
-----------------------------------------------------------------------------------*/

/*

*/
void aberturaVoo (Aviao *aviao) {
    scanf("%d %f %f", &aviao->capacidade, &aviao->economica, &aviao->executiva);
}

/*

*/
void realizarReserva () {
    Pessoa p = { lerString(TAM_MAX_STRING), lerString(TAM_MAX_STRING), lerString(15) };
    char *data = lerData(); 
    printf("%s\n", data);

}

/*

*/
void modificarReserva () {

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
                realizarReserva();
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