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

Node* create_node(int digitReceived) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->digit = digitReceived;
    newNode->previousNode = NULL;
    newNode->nextNode = NULL;
    return newNode;
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

BigNumber* add_bignumbers(const BigNumber* firstBignumber, const BigNumber* secondBignumber) {
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (!result) return NULL;
    result->firstNode = NULL;
    result->lastNode = NULL;
    result->is_negative = firstBignumber->is_negative;

    Node* lastNodeFirstNumber = firstBignumber->lastNode;
    Node* lastNodeSecondNumber = secondBignumber->lastNode;
    int borrow = 0;

    while (lastNodeFirstNumber || lastNodeSecondNumber || borrow) {
        int digit1 = lastNodeFirstNumber ? lastNodeFirstNumber->digit - '0' : 0;
        int digit2 = lastNodeSecondNumber ? lastNodeSecondNumber->digit - '0' : 0;
        int sum = digit1 + digit2 + borrow;

        int digitResult = 0;
        if (sum >= 10) {
            digitResult += sum % 10;
            borrow = 1;
        }
        else {
            digitResult += sum;
            borrow = 0;
        }

        Node* newNode = create_node(digitResult + '0');
        newNode->nextNode = result->firstNode;
        newNode->previousNode = NULL;

        if (result->firstNode)
            result->firstNode->previousNode = newNode;
        else
            result->lastNode = newNode;
        result->firstNode = newNode;

        if (lastNodeFirstNumber) lastNodeFirstNumber = lastNodeFirstNumber->previousNode;
        if (lastNodeSecondNumber) lastNodeSecondNumber = lastNodeSecondNumber->previousNode;
    }

    return result;
}

BigNumber* subtract_bignumbers(const BigNumber* firstBignumber, const BigNumber* secondBignumber){
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (!result) return NULL;
    result->firstNode = NULL;
    result->lastNode = NULL;
    result->is_negative = firstBignumber->is_negative;

    Node *lastNodeFirstNumber = firstBignumber->lastNode;
    Node *lastNodeSecondNumber = secondBignumber->lastNode;
    int borrow = 0;

    while (lastNodeFirstNumber || lastNodeSecondNumber || borrow) {
        int digit1 = lastNodeFirstNumber ? lastNodeFirstNumber->digit - '0' : 0;
        int digit2 = lastNodeSecondNumber ? lastNodeSecondNumber->digit - '0' : 0;
        int subtract = digit1 - digit2 - borrow;

        if (subtract < 0) {
            subtract += 10;
            borrow = 1;
        } else
            borrow = 0;
        
        Node *newNode = create_node(subtract + '0');
        newNode->nextNode = result->firstNode;
        newNode->previousNode = NULL;
        if (result->firstNode)
            result->firstNode->previousNode = newNode;
        else
            result->lastNode = newNode;
        result->firstNode = newNode;     
       
        if (lastNodeFirstNumber) lastNodeFirstNumber = lastNodeFirstNumber->previousNode;
        if (lastNodeSecondNumber) lastNodeSecondNumber = lastNodeSecondNumber->previousNode;
    }

    return result;
}

BigNumber* multiply_bignumbers(const BigNumber* firstBignumber, const BigNumber* secondBignumber){
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (!result) return NULL;
    result->firstNode = NULL;
    result->lastNode = NULL;
    result->is_negative = firstBignumber->is_negative;

    Node *lastNodeFirstNumber = firstBignumber->lastNode;
    Node *lastNodeSecondNumber = secondBignumber->lastNode;
    int borrow = 0;

    while (lastNodeFirstNumber || lastNodeSecondNumber || borrow) {
        int digit1 = lastNodeFirstNumber ? lastNodeFirstNumber->digit - '0' : 0;
        int digit2 = lastNodeSecondNumber ? lastNodeSecondNumber->digit - '0' : 0;
        int subtract = digit1 - digit2 - borrow;

        if (subtract < 0) {
            subtract += 10;
            borrow = 1;
        } else
            borrow = 0;
        
        Node *newNode = create_node(subtract + '0');
        newNode->nextNode = result->firstNode;
        newNode->previousNode = NULL;
        if (result->firstNode)
            result->firstNode->previousNode = newNode;
        else
            result->lastNode = newNode;
        result->firstNode = newNode;     
       
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
