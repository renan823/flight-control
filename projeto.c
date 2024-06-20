#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX_STRING 50

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
    char origem[4];
    char destino[4];
} Voo;

typedef struct {
    char *nome;
    char *sobrenome;
    char cpf[15];
} Pessoa;

/*
Une pessoa e voo.
É a entidade principal do programa.
*/
typedef struct {
    Pessoa *pessoa;
    Voo *voo;
    char assento[5];
    char classe[10];
    float valor;
} Reserva;

/*
No arquivo, armazena informações do avião e do voo.
*/
typedef struct {
    int vendas, fechado;
    Aviao aviao;
} Cabecalho;

/*
Usado na identificação dos campos string de Pessoa.
Colocado enre reservas no arquivo.
*/
typedef struct {
    int tam_nome, tam_sobr;
} Separador;

/*-----------------------------------------------------------------------------------
Funções de utilidades (principalmente para alocação de memória 
e manipulação de arquivos).
Descrição específica em cada função.
-----------------------------------------------------------------------------------*/

/*
Aloca, dinamicamente, um vetor sem tipo.
Parametros: quantidade de items e tamanho (bytes) do item.
Retorno: o vetor alocado (sucesso) ou erro (falha).
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
Redimensiona um vetor/ponteiro.
Permite adicionar ou remover espaço.
Parametros: o vetor/ponteiro, quantidade de items e tamanho (bytes) do item.
Retorno: o vetor realocado (sucesso) ou erro (falha).
*/
void *realocar (void *ponteiro, int quantidade, int tamanho) {
    if ((ponteiro = (void *) realloc(ponteiro, quantidade * tamanho)) == NULL) {
        printf("Erro: sem memória para alocar\n");
        exit(1);
    }

    return (ponteiro);
}

/*
Libera a memória alocada para as reservas.
Parametros: ponteiro para as reservas e tamanho do vetor.
Nada é retornado.
 */
void liberarMemoria(Reserva *ponteiro, int tamanho){
    for(int i = 0; i < tamanho; i++){
        free(ponteiro[i].pessoa->nome);
        free(ponteiro[i].pessoa->sobrenome);
        free(ponteiro[i].pessoa);
        free(ponteiro[i].voo);
    }
    free(ponteiro);
}

/*
Abre um arquivo.
Parametros: nome do arquivo e modo de abertura.
Retorno: o arquivo (sucesso) ou NULL (falha);
*/
FILE *abrirArquivo (char *nome, char *modo) {
	FILE *arquivo;

	if ((arquivo = fopen(nome, modo)) == NULL) {
		//printf("Erro: nenhum arquivo encontrado\n");
		//exit(1);
		return (NULL);
	}

	return (arquivo);
}

/* 
Grava todos os dados do programa para um arquivo binario "dados.dat".
A ordem de gravação é:
    - Cabeçalho: contém dados do aviao e total de vendas
    - Reservas
Para cada reserva, coloca-se no arquivo:
    - Reserva: dados da reserva
    - Voo: dados do voo
    - Pessoa: dados da pessoa
    - Separador: indica o tamanho da proximas strings
        a serem salvas (nome e sobrenome)
    -Nome e Sobrenome: salvos separadamente da pessoa

...A ordem se repete até o fim do vetor de reservas
Os ponteiros anteriormente alocados são liberados.
O vetor de reservas é liberado.

Parametros: vetor de reservas, aviao, total de reservas e voo fechado (1 ou 0);
Nada é retornado.
*/
void gravarDados (Reserva *reservas, Aviao av, int tamanho, int fechado) {
    FILE *arquivo = abrirArquivo("dados.dat", "wb");

    Cabecalho c = { tamanho, fechado, av };
    fwrite(&c, sizeof(Cabecalho), 1, arquivo);

    Separador s;

    for (int i = 0; i < tamanho; i++) {
        fwrite(&reservas[i], sizeof(Reserva), 1, arquivo);
        
        fwrite(&(*reservas[i].voo), sizeof(Voo), 1, arquivo);
        fwrite(&(*reservas[i].pessoa), sizeof(Pessoa), 1, arquivo);

        s.tam_nome = strlen(reservas[i].pessoa->nome);
        s.tam_sobr = strlen(reservas[i].pessoa->sobrenome);

        fwrite(&s, sizeof(Separador), 1, arquivo);

        fwrite(&(*reservas[i].pessoa->nome), sizeof(char), s.tam_nome, arquivo);
        fwrite(&(*reservas[i].pessoa->sobrenome), sizeof(char), s.tam_sobr, arquivo);
        
        free(reservas[i].pessoa->nome);
        free(reservas[i].pessoa->sobrenome);
        free(reservas[i].pessoa);
        free(reservas[i].voo);
    }

    free(reservas);
}

/*
Lê os dados anteriormente salvos no arquivo "dados.dat" (binário).
Um vetor de reservas é alocado baseado no número de reservas a serem lidas,
valor disponivel no cabeçalho.
A ordem de leitura é:
    - Cabeçalho: contém dados do aviao e total de vendas
    - Reservas
Para cada reserva, coloca-se no vetor:
    - Reserva: dados da reserva
    - Voo: dados do voo
    - Pessoa: dados da pessoa (nome e sobrenome lidos separadamente)

...A ordem se repete até o fim do arquivo.

Parametros: vetor de reservas, endereço do aviao, total de reservas e voo fechado (1 ou 0);
Todos os parametros são passados como endereço (por referencia)
Nada é retornado.
*/
void lerDados (Reserva **reservas, Aviao *av, int *tamanho, int *fechado) {
    FILE *arquivo = abrirArquivo("dados.dat", "rb");

    if (arquivo != NULL) {
        Cabecalho c;
        fread(&c, sizeof(Cabecalho), 1, arquivo);

        *tamanho = c.vendas;
        *fechado = c.fechado;
        *av = c.aviao;
        
        Separador s;

        *reservas = (Reserva *) realocar(*reservas, (*tamanho), sizeof(Reserva));

        for (int i = 0; i < *tamanho; i++) {
            Pessoa *p = alocar(1, sizeof(Pessoa));
            Voo *v = (Voo *) alocar(1, sizeof(Voo));
            Reserva *r = &(*reservas)[i];

            fread(r, sizeof(Reserva), 1, arquivo);
            fread(v, sizeof(Voo), 1, arquivo);
            fread(p, sizeof(Pessoa), 1, arquivo);

            
            fread(&s, sizeof(Separador), 1, arquivo);

            p->nome = (char *) alocar(s.tam_nome, sizeof(char));
            fread(p->nome, sizeof(char), s.tam_nome, arquivo);

            p->sobrenome = (char *) alocar(s.tam_sobr, sizeof(char));
            fread(p->sobrenome, sizeof(char), s.tam_sobr, arquivo);
            
            r->pessoa = p;
            r->voo = v;
        }

        fclose(arquivo); 
    } 
}

/*-----------------------------------------------------------------------------------
Funções usadas de entrada /saída de dados.
Descrição específica em cada função.
-----------------------------------------------------------------------------------*/

/*
Exibe uma linha com 50 caracteres.
Não há parametros.
Nada é retornado.
*/
void linha () {
    printf("--------------------------------------------------\n");
}

/*
Permite a leitura de uma string.
Um ponteiro char é alocado.
Caso a string não ocupe o tamanho especificado,
o ponteiro será redimensionado apenas para ocupar seu tamanho (strlen).
Parametros: tamanho max da string.
Retorno: a string lida.
*/
char *lerString (int tamanho) {
    char *string = (char *) alocar(tamanho, sizeof(char));

    scanf("%s", string);

    if (string[strlen(string) - 1] == '\n') {
        string[strlen(string) - 1] = '\0';
    }

    string = (char *) realocar(string, strlen(string), sizeof(char));

    return (string);
}

/*
Permite a leitura de datas.
O tamanho 11 acomoda a data e '\0' no final.
O programa recebe como entrada do usuário dia, mês e ano.
A entrada é processada de modo a formatar a data como
dd/mm/AAAA.
Não há parametros.
Retorno: a data formatada.
*/
char *lerData () {
    char *data = (char *) alocar(11, sizeof(char));
    char temp;

    getc(stdin);
    for (int i = 0; i < 10; i++) {
        scanf("%c", &temp);

        if (temp == ' ') {
            temp = '/';
        }

        data[i] = temp;
    }
    data[10] = '\0';
    return (data);
}

/*
Recebe os dados do cadastro de pessoa.
Um ponteiro pessoa é alocado.
Os dados são lidos usando a leitura de strings.
Não há parametros.
Retorno: a pessoa cadastrada.
*/
Pessoa *lerPessoa () {
    Pessoa *p = (Pessoa *) alocar(1, sizeof(Pessoa));

    char *nome = lerString(TAM_MAX_STRING);
    char *sobrenome = lerString(TAM_MAX_STRING);

    p->nome = (char *) alocar(strlen(nome), sizeof(char));
    p->sobrenome = (char *) alocar(strlen(sobrenome), sizeof(char));

    strcpy(p->nome, nome);
    strcpy(p->sobrenome, sobrenome);
    strcpy(p->cpf, lerString(15));

    free(nome);
    free(sobrenome);

    return (p);
}

/*
Exibe os dados de uma reserva de modo formatado.
*/
void exibirReserva (Reserva r) {
    printf("%s\n", r.pessoa->cpf);
    printf("%s %s\n", r.pessoa->nome, r.pessoa->sobrenome);
    printf("%s\n", r.voo->data);
    printf("Voo: %s\n", r.voo->id);
    printf("Assento: %s\n", r.assento);
    printf("Classe: %s\n", r.classe);
    printf("Trecho: %s %s\n", r.voo->origem, r.voo->destino);
    printf("Valor: %.2f\n", r.valor);
    linha();
}

/*
Exibe os dados do voo ao ser fechado.
Inclui a soma de todas as passagens vendidas.
*/
void exibirVoo (Reserva *reservas, int tamanho) {
    float total = 0;

    for (int i = 0; i < tamanho; i++) {
        total += reservas[i].valor;

        printf("%s\n", reservas[i].pessoa->cpf);
        printf("%s %s\n", reservas[i].pessoa->nome, reservas[i].pessoa->sobrenome);
        printf("%s\n\n", reservas[i].assento);
    }

    printf("Valor Total: %.2f\n", total);
    linha();
}

/*-----------------------------------------------------------------------------------
Casos de uso.
Descrição específica em cada função.
-----------------------------------------------------------------------------------*/

/*
Recebe a primeira entrada do programa, os dados do voo.
Os dados serão sempre os mesmos para todas as consultas futuras.
O voo com os dados recebidos é retornado.
*/
Aviao aberturaVoo () {
    Aviao aviao;

    scanf("%d %f %f", &aviao.capacidade, &aviao.economica, &aviao.executiva);

    return (aviao);
}

/*
Realiza uma reserva.
Recebe os dados da pessoa, do voo e da reserva.
O vetor reservas é redimensionado para acomodar a nova reserva.
Parametros: aviao, endereço de reservas e endereço de tamanho.
Nada é retornado.
*/
void realizarReserva (Aviao a, Reserva **reservas, int *tamanho) {
    if (*tamanho == 0) {
        *reservas = (Reserva *) alocar(1, sizeof(Reserva));
    } else {
        *reservas = (Reserva *) realocar(*reservas, (*tamanho)+1, sizeof(Reserva));
    }

    Pessoa *p = lerPessoa();
    Voo *v = (Voo *) alocar(1, sizeof(Voo));

    Reserva *r = &(*reservas)[*tamanho];

    strcpy(v->data, lerData());
    strcpy(v->id, lerString(5));
    strcpy(r->assento, lerString(5));
    strcpy(r->classe, lerString(10));
    scanf("%f", &r->valor);
    strcpy(v->origem, lerString(4));
    strcpy(v->destino, lerString(4));

    r->pessoa = p;
    r->voo = v;

    (*tamanho)++;
}

/*
Permite consultar uma reserva.
Para realizar a consulta, o cpf deve ser fornecido pelo usuário.
Caso a reserva exista, seus dados são exibidos.
Senão, uma mensagem de erro é exibida.
Não há parametros.
Nada é retornado.
*/
void consultarReserva (Reserva *reservas, int tamanho) {
    char *cpf = alocar(15, sizeof(char));
    strcpy(cpf, lerString(15));

    int achou = 0;

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(cpf, reservas[i].pessoa->cpf) == 0) {
            exibirReserva(reservas[i]);
            achou = 1;
            break;
        }
    }

    if (!achou) {
        printf("Nenhuma reserva encontrada com %s!\n", cpf);
    }

    free(cpf);
}

/*
Pemite alterar alguns dados de uma reserva existente.
Os dados que podem ser alterados são: nome, sobrenome, cpf
e assento.
Para fazer a alteração, é necessário fornecer o cpf da reserva feita.
Os dados são inseridos e a reserva, se existir uma, será alterado.
A alteração é exibida na tela.
Parametros: reservas e tamanho (quantidade de reservas)
Nada é retornado.
*/
void modificarReserva (Reserva *reservas,  int tamanho) {
    char *cpf = alocar(15, sizeof(char));
    strcpy(cpf, lerString(15));

    Pessoa *p = lerPessoa();
    int achou = 0;

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(cpf, reservas[i].pessoa->cpf) == 0) {
            free(reservas[i].pessoa->nome);
            free(reservas[i].pessoa->sobrenome);

            free(reservas[i].pessoa);
            
            (&(reservas[i]))->pessoa = p;
            strcpy((&(reservas[i]))->assento, lerString(5));

            printf("Reserva Modificada:\n");
            exibirReserva(reservas[i]);
            achou = 1;
            break;
        }
    }

    if (!achou) {
        printf("Nenhuma reserva encontrada com %s!\n", cpf);
    }

    free(cpf);
}

/*
Permite remover uma reserva.
Para a remoção, é necessário informar o cpf presente na reserva.
Se a reserva existir, ela será removida.
O vetor reservas será redimensionado, removendo 1 espaço.
Caso a reserva com o cpf informado não exista, um erro será exibido.
Parametros: reservas e tamanho (quantidade de reservas). Ambos são endereços.
Nada é retornado.
*/
void cancelarReserva(Reserva **reservas, int *tamanho) {
    char *cpf = alocar(15, sizeof(char));
    strcpy(cpf, lerString(15));

    int pos = -1;

   
    for (int i = 0; i < *tamanho; i++) {
        if (strcmp(cpf, (*reservas)[i].pessoa->cpf) == 0) {
            pos = i;
            break;
        }
    }

    if (pos != -1) {
        
        free((*reservas)[pos].pessoa->nome);
        free((*reservas)[pos].pessoa->sobrenome);
        free((*reservas)[pos].pessoa);
        free((*reservas)[pos].voo);

        
        for (int i = pos; i < (*tamanho) - 1; i++) {
            (*reservas)[i] = (*reservas)[i + 1];
        }

        (*tamanho)--;

        if (*tamanho > 0) {
            *reservas = (Reserva *)realocar(*reservas, *tamanho, sizeof(Reserva));
        } else {
            
            free(*reservas);
            *reservas = NULL;
        }

        printf("Reserva excluida!\n");
    } else {
        printf("Nenhuma reserva encontrada com %s!\n", cpf);
    }

    free(cpf);
}


/*
Exibe os dados (valores e n° de reservas) do voo até aquele dia.
Parametros: vetor de reservas e tamanho (quantidade de reservas).
Nada é retornado.
*/
void fecharDia (Reserva *reservas, int tamanho) {
    float total = 0;

    printf("Fechamento do dia: \n");
    printf("Quantidade de reservas: %d\n", tamanho);

    for (int i = 0; i < tamanho; i++) {
        total += reservas[i].valor;
    }

    printf("Posição: %.2f\n", total);
    linha();
}

/*
Realiza o fechamento do voo.
Após o fechamento, apenas consultas são permitidas.
Os dados do voo são exibidos.
Parametros: vetor de reservas, aviao e tamanho (quantidade de reservas).
Nada é retornado.
*/
void fecharVoo (Reserva *reservas, Aviao av, int tamanho) {
    printf("Voo fechado!\n\n");

    exibirVoo(reservas, tamanho);
}

/*-----------------------------------------------------------------------------------
Função principal.
Coordena o funcionamento das outras funções.
-----------------------------------------------------------------------------------*/

/*
É responsável por juntar as funcionalidades anteriormente descritas.
Recebe os códigos para ação, armazena o vetor reservas e os dados do voo.
Executa a leitura do arquivo toda abertura de programa.
Grava no arquivo quando necessário.
Controla o acesso às funcionalidades após o fechamento do voo.
Não há parametros.
Retorno: valor inteiro para o S.O.
*/
int main (void) {

    char codigo[3];
    int vendas = 0, fechado = 0;

    Reserva *reservas = NULL;
    Aviao aviao;

    lerDados(&reservas, &aviao, &vendas, &fechado);
    
    do {
        scanf("%s", codigo);

        if (strcmp("AV", codigo) == 0) {
            aviao = aberturaVoo();
        } else if (strcmp("RR", codigo) == 0) {
            if (!fechado && vendas < aviao.capacidade) {
                realizarReserva(aviao, &reservas, &vendas);
            } else if(fechado){
                printf("\nVoo fechado! Apenas consultas são permitidas!\n");
                fecharVoo(reservas, aviao, vendas);
            }else{
                printf("\nVoo lotado!\n");
            }
        } else if (strcmp("CR", codigo) == 0) {
            consultarReserva(reservas, vendas);
        } else if (strcmp("MR", codigo) == 0) {
            if (!fechado) {
                modificarReserva(reservas, vendas);
            } else {
                printf("Voo fechado! Apenas consultas são permitidas!\n");
            }
        } else if (strcmp("CA", codigo) == 0) {
            if (!fechado) {
                cancelarReserva(&reservas, &vendas);
            } else {
                printf("Voo fechado! Apenas consultas são permitidas!\n");
            }
        } else if (strcmp("FD", codigo) == 0) {
            fecharDia(reservas, vendas);
            gravarDados(reservas, aviao, vendas, 0);
            return 0;
        } else if (strcmp("FV", codigo) == 0) {
            fecharVoo(reservas, aviao, vendas);
            gravarDados(reservas, aviao, vendas, 1);
            return 0;
        }
    } while (1);
    liberarMemoria(reservas, vendas);

    return (0);
}