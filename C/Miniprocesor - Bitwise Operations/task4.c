#include <stdio.h>
#include <stdlib.h>
#define BYTE_DIM 8
// functie de adaugare a bit-ului in secventa
void add_bit(int* x, int dim_seq, unsigned int bit) {
    if (bit == 1) {
        *x = *x | (1 << dim_seq);
    }
}
// functie de extragere a unei secvente de biti de dimensiunea dim din nr
int extract_sequence(unsigned int nr, int dim) {
    int temp = 0;
    unsigned int bit = 0;
    int nrBitesInt = sizeof(unsigned int) * BYTE_DIM;
    for (int i = nrBitesInt - 1; i >= nrBitesInt - dim; i--) {
        bit = (nr >> i) & 1;
        add_bit(&temp, dim - (nrBitesInt - i), bit);
    }
    return temp;
}
int main() {
    unsigned int seq = 0;
    scanf("%u", &seq);
    int N = extract_sequence(seq, 3) + 1;
    seq = seq << 3;
    char* operators = malloc(N * sizeof(char));
    if (!operators) {
        printf("Alocarea a esuat");
        return 0;
    }
    for (int i = 0; i < N; i++) {
        int op = extract_sequence(seq, 2);
        switch (op) {
            case 0:
            operators[i] = '+';
            break;
            case 1:
            operators[i] = '-';
            break;
            case 2:
            operators[i] = '*';
            break;
            case 3:
            operators[i] = '/';
            break;
        }
        seq = seq << 2;
    }
    int dim_operators = extract_sequence(seq, 4) + 1;;
    int nrBitesShort = BYTE_DIM * sizeof(unsigned short);
    // nrOperands - totalul numerelor de tip short citite
    int nrOperands = ((N + 1) * dim_operators) / nrBitesShort;
    if ((N + 1) * dim_operators > (nrBitesShort * nrOperands)) {
        nrOperands = nrOperands + 1;
    }
    // stocam numerele citite de la tastatura in vectorul operands
    unsigned short* operands = (unsigned short*) malloc(nrOperands * sizeof(unsigned short));
    if (!operands) {
        printf("Alocarea a esuat");
        free(operators);
        return 0;
    }
    for (int i = 0; i < nrOperands; i++) {
        scanf("%hu", operands + i);
    }
    int* numbers = (int*) calloc((N + 1), sizeof(int));
    if (!numbers) {
        printf("Alocarea a esuat");
        free(operators);
        free(operands);
        return 0;
    }
    // extragem din vectorul operands numerele necesare, bit cu bit in vectorul numbers
    // pozitia bit-ului este retinuta de indexBit
    // pentru fiecare element din operands sunt nrOperandsPerShort numere
    // index este index-ul numarului din vectorul operands din care trebuie sa luam bit-ul
    for (int i = 0; i < N + 1; i++) {
        int n = sizeof(unsigned short) * BYTE_DIM;
        for (int indexBit = 0; indexBit < dim_operators; indexBit++) {
            int index = (i * dim_operators + indexBit) / nrBitesShort;
            int pos = n - 1 - (i * dim_operators + indexBit) % nrBitesShort;
            int bit = (operands[index] >> pos) & 1;
            add_bit(numbers + i, dim_operators - 1 - indexBit, bit);
        }
    }
    int *result_temp = (int*) calloc((N + 1), sizeof(int));
    if (!result_temp) {
        printf("Alocarea a esuat");
        free(operators);
        free(operands);
        free(numbers);
        return 0;
    }
    int tempIndex = 0;
    int numbersIndex = 0;
    // calculam rezultatul operatiilor tinand cont de precedenta operatiilor
    // parcurgem de doua ori vectorul de operators
    // la prima parcurgere efectuam toate inmultirile si impartirile, iar rezultatele le stocam in result_temp
    for (int operatorsIndex = 0; operatorsIndex < N; operatorsIndex++) {
        int temp = numbers[numbersIndex];
        numbersIndex++;
        while (operators[operatorsIndex] == '*' || operators[operatorsIndex] == '/') {
            if (operators[operatorsIndex] == '*') {
                temp = temp * numbers[numbersIndex];
                numbersIndex++;
            } else {
                temp = temp / numbers[numbersIndex];
                numbersIndex++;
            }
            operatorsIndex++;
            if (operatorsIndex > N) {
                break;
            }
        }
        result_temp[tempIndex] = temp;
        tempIndex++;
    }
    // atunci cand ultima operatie este adunare sau scadere si mai este un singur numar ramas,
    // atunci el nu va fi pus in result_temp, deci trebuie adaugat manual
    if (numbersIndex == N) {
        result_temp[tempIndex] = numbers[numbersIndex];
        tempIndex++;
    }
    // la a doua parcurgere efecutam adunarile si scaderile, si stocam rezultatul in variabila result
    tempIndex = 0;
    int result = result_temp[0];
    for (int operatorsIndex = 0; operatorsIndex < N; operatorsIndex++) {
        while (operators[operatorsIndex] == '+' || operators[operatorsIndex] == '-') {
            if (operators[operatorsIndex] == '+') {
                tempIndex++;
                result = result + result_temp[tempIndex];
            } else {
                tempIndex++;
                result = result - result_temp[tempIndex];
            }
            operatorsIndex++;
            if (operatorsIndex > N) {
                break;
            }
        }
    }
    printf("%d", result);
    printf("\n");
    free(operands);
    free(operators);
    free(numbers);
    free(result_temp);
    return 0;
}
