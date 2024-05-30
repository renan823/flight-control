#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

*/
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

int main (void) {

    

    return (0);
}