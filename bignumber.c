/* A fazer e melhorias
1 - [FEITO +/-, Dando erro com num mt grande e num negativo] Soma 
2 - Subtração
3 - Multiplicação
4 - Divisao
5 - Melhorias
6 - Precisa rodar naquele bglh la que o professor usa na aula pra ver se tem 
    alguma coisa vazando na memória (não sei fazer isso no windows :´(  
    valgrind --leak-check=yes

7 - Testar tudo com aquelas entradas do .In q o prof liberou
8 - Fazer extras 
*/
#include "bignumber.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

BigNumber* create_bignumber(const char* numberInString) {
    if (!numberInString) return NULL;

    BigNumber* ptrBignumber = (BigNumber*)malloc(sizeof(BigNumber));
    if (!ptrBignumber) return NULL;
    ptrBignumber->firstNode = NULL;
    ptrBignumber->lastNode = NULL;
    ptrBignumber->is_negative = (numberInString[0] == '-');

    int startLoopingIn = (ptrBignumber->is_negative ? 1 : 0); //Se for negativo elimina primeira casa (sinal negativo)
    for (int i = startLoopingIn; numberInString[i] != '\0'; i++) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (!newNode) {
            free_bignumber(ptrBignumber);
            return NULL;
        }
        newNode->digit = numberInString[i];
        newNode->nextNode = NULL;
        newNode->previousNode = ptrBignumber->lastNode;

        if (ptrBignumber->lastNode)
            ptrBignumber->lastNode->nextNode = newNode;
        else 
            ptrBignumber->firstNode = newNode;
        ptrBignumber->lastNode = newNode;
    }

    return ptrBignumber;
}

void free_bignumber(BigNumber* ptrBignumber) {
    if (!ptrBignumber) return;

    Node* current = ptrBignumber->firstNode;
    while (current) {
        Node* nextNode = current->nextNode;
        free(current);
        current = nextNode;
    }
    free(ptrBignumber);
}

BigNumber* add_bignumbers(const Node* lastNodeFirstNumber, const Node* lastNodeSecondNumber) {
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (!result) return NULL;
    result->firstNode = NULL;
    result->lastNode = NULL;
    result->is_negative = 0;

    int carry = 0;

    while (lastNodeFirstNumber || lastNodeSecondNumber || carry) {
        int digit_a = lastNodeFirstNumber ? lastNodeFirstNumber->digit - '0' : 0;
        int digit_b = lastNodeSecondNumber ? lastNodeSecondNumber->digit - '0' : 0;
        int sum = digit_a + digit_b + carry;

        int digit_result = 0;
        if (sum >= 10) {
            digit_result += sum % 10;
            carry = 1;
        }
        else {
            digit_result += sum;
            carry = 0;
        }

        Node* new_node = (Node*)malloc(sizeof(Node));
        if (!new_node) {
            free_bignumber(result);
            return NULL;
        }

        new_node->digit = digit_result + '0';
        new_node->nextNode = result->firstNode;
        new_node->previousNode = NULL;

        if (result->firstNode)
            result->firstNode->previousNode = new_node;
        else
            result->lastNode = new_node;
        result->firstNode = new_node;

        if (lastNodeFirstNumber) lastNodeFirstNumber = lastNodeFirstNumber->previousNode;
        if (lastNodeSecondNumber) lastNodeSecondNumber = lastNodeSecondNumber->previousNode;
    }

    return result;
}

void print_bignumber(const BigNumber* ptrBignumber) {
    if (!ptrBignumber) return;

    if (ptrBignumber->is_negative)
        printf("-");

    Node* current = ptrBignumber->firstNode;
    while (current) {
        printf("%c", current->digit);
        current = current->nextNode;
    }
    printf("\n");
}
