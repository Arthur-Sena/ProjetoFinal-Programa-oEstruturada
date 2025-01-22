#include "bignumber.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#pragma region "Função auxiliar"
bool first_bignum_is_bigger(const BigNumber* firstBignum, const BigNumber* secondBignum) {
    int lengthFirstBignum = 0, lengthSecondBignum = 0;
    Node* currentNodeFirstBignum = firstBignum->firstNode;
    Node* currentNodeSecondBignum = secondBignum->firstNode;

    while (currentNodeFirstBignum) {
        lengthFirstBignum++;
        currentNodeFirstBignum = currentNodeFirstBignum->nextNode;
    }
    while (currentNodeSecondBignum) {
        lengthSecondBignum++;
        currentNodeSecondBignum = currentNodeSecondBignum->nextNode;
    }

    bool firstBignumIsBigger = false;
    if (lengthFirstBignum == lengthSecondBignum){
        firstBignumIsBigger = (firstBignum->firstNode->digit - '0') > (secondBignum->firstNode->digit - '0');
    }
    else 
        firstBignumIsBigger = (lengthFirstBignum > lengthSecondBignum);
    return firstBignumIsBigger; 
}

BigNumber* verify_sum_bignumbers(const BigNumber* firstBignumber, const BigNumber* secondBignumber) {
    if (firstBignumber->is_negative == secondBignumber->is_negative) 
        return add_bignumbers(firstBignumber, secondBignumber);
    else {
        bool firstBignumIsBigger = first_bignum_is_bigger(firstBignumber, secondBignumber);
        BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
        if (firstBignumIsBigger)
            result = subtract_bignumbers(firstBignumber, secondBignumber);        
        else
            result = subtract_bignumbers(secondBignumber, firstBignumber);
        return result;
    }
}

#pragma endregion

int main() {
    char *firstInput = NULL, *secondInput = NULL, operador;
    size_t lenFirstInput = 0, lenSecondInput = 0;

    printf("Primeiro numero: ");
    getline(&firstInput, &lenFirstInput, stdin);
    printf("\nSegundo numero: ");
    getline(&secondInput, &lenSecondInput, stdin);
    printf("\nOperação (+, -, *, /): ");
    scanf(" %c", &operador);

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

    if (operador == '+')
        result = verify_sum_bignumbers(num1, num2);
    else if (operador == '-')
        result = subtract_bignumbers(num1, num2);
    else if (operador == '*')
        result = karatsuba_multiply_bignumbers(num1, num2);
    else if (operador == '^')
        result = power_bignumbers(num1, num2);
    else if (operador == '/')
        result = divide_bignumbers(num1, num2);

    print_bignumber(result);

    free_bignumber(num1);
    free_bignumber(num2);
    free_bignumber(result);
    
    return 0;
}