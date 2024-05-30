#include <stdio.h>
#include <stdlib.h>

struct Airplane {
    capacity: int;
    executive: double;
    economic: double;
}

struct Flight {
    id: char[4];
    date: char[10];
    from: char[4];
    to: char[4];
}

struct Person {
    name: *char;
    lastname: *char;
    cpf: char[14];
}

struct Reservation {
    personCpf: char[14];
    flightId: char[4];
    seat: char[4];
    class: *char;
    value: double;
}

int main (void) {
    return (0);
}