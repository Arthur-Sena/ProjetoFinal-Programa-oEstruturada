/*
[FEITO] IMPORTANTE - Declarei o input1 e input2 com tamanho maximo de caractere (10005), acho que não pode ter limite, 
    tem que usar alocação dinaminca de memória, não sei fazer isso
*/

#include "bignumber.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *firstInput = NULL, *secondInput = NULL, operador;
    size_t lenFirstInput = 0, lenSecondInput = 0;

    printf("Primeiro numero: ");
    getline(&firstInput, &lenFirstInput, stdin);
    printf("\nSegundo numero: ");
    getline(&secondInput, &lenSecondInput, stdin);
    printf("\nOperação (+, -, *, /): ");
    scanf(" %c", &operador);

    // Remover o '\n' no final das strings lidas
    firstInput[strcspn(firstInput, "\n")] = '\0';
    secondInput[strcspn(secondInput, "\n")] = '\0';

    BigNumber *num1 = create_bignumber(firstInput);
    BigNumber *num2 = create_bignumber(secondInput);
    BigNumber *result = NULL;

    if (!num1 || !num2) {
        printf("Erro ao criar BigNumber.\n");
        free_bignumber(num1);
        free_bignumber(num2);
        return 1;
    }


    //if (operador == '+')
    result = add_bignumbers(num1->lastNode, num2->lastNode);
    //else if (operador == '-')
    print_bignumber(result);

    scanf(" %c", &operador);

    //Liberando memória
    free_bignumber(num1);
    free_bignumber(num2);
    free_bignumber(result);
    return 0;
}