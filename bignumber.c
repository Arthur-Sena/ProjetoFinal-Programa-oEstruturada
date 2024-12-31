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

Node* get_bignumber_node_by_index(int index, BigNumber* bignumber) {
    Node* lastNode = bignumber->lastNode;
    for (int i = 0; i < index; i++)
        lastNode = lastNode->previousNode;
    return lastNode;
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
    result->is_negative = (firstBignumber->is_negative != secondBignumber->is_negative);

    Node *lastNodeFirstNumber = firstBignumber->lastNode;
    Node *lastNodeSecondNumber = secondBignumber->lastNode;
    int borrow = 0;
    //Esse número indica a posição do número do multiplicando que o multiplicador indicado está multiplicando - mandante - contador para trás
    int starterFirstNum = 0;
    //Esse número indica a posição do número multiplicador que está multiplicando
    int starterSecondNum = 0;

    /*  8 -> multiplicando
        x
        2 -> multiplicador
    --------
        16 -> produto
    */

   /* Lógica MULTIPLICAÇÃO:
    O loop irá percorrer o second (multiplicador), que controlará o inicio do somatório.
    O somatório é a soma do numero do multiplicador indicado vezes o numero do multiplicando indicado mais o próximo número
    do multiplicador indicado vezes o número antecessor ao multiplicando indicado.
    Se o número antecessor ao multiplicando indicado não existir, no caso de ser o 0 e o antecessor o -1, então essa
    multiplicação e suas seguintes não entram no somatório.
    Ex: 1012 * 141 = 142692

    Passo 1: 
      2 * 1 = 2
      Resultado: 2
   
   Passo 2:
      1 * 1 = 1
      4 * 2 = 8
      Soma: 1 + 8 = 9
      Resultado: 9

   Passo 3:
      1 * 0 = 0
      4 * 1 = 4
      1 * 2 = 2
      Soma: 0 + 4 + 2 = 6
      Resultado: 6

    OBS: observe que os valores de cada passo batem com os valores de trás para frente do resultado da multiplicação.
     */

    /* Toda somatório haverá a incrementação do starterFirstNum, a nao ser que o lastNodeSecondNumber
     tenha finalizado a linha, ai não incrementa nessa etapa */


    while (lastNodeSecondNumber) {

        while (lastNodeFirstNumber) {
            printf("S:\n");
            printf(" %c | %c \n", lastNodeFirstNumber->digit, lastNodeSecondNumber->digit);
                
            Node *copylastNodeSecondNumber = lastNodeSecondNumber;
            Node *copylastNodeFirstNumber = lastNodeFirstNumber;
            while (lastNodeSecondNumber) {
                printf("%c x %c \n", lastNodeFirstNumber->nextNode->digit, lastNodeSecondNumber->previousNode->digit);
                if(lastNodeSecondNumber) lastNodeSecondNumber = lastNodeSecondNumber->previousNode;
                if(lastNodeFirstNumber) lastNodeFirstNumber = lastNodeFirstNumber->nextNode;
            }
            lastNodeSecondNumber = copylastNodeSecondNumber;
            lastNodeFirstNumber = copylastNodeFirstNumber;

            if(lastNodeFirstNumber->previousNode) starterFirstNum++;
            if(lastNodeFirstNumber) lastNodeFirstNumber = lastNodeFirstNumber->previousNode;
        }

        if(lastNodeSecondNumber->previousNode) starterSecondNum++;
        if(lastNodeSecondNumber) lastNodeSecondNumber = lastNodeSecondNumber->previousNode;
    }

    return result;
}

BigNumber* divide_bignumbers(const BigNumber* firstBignumber, const BigNumber* secondBignumber){

/*   8 -> dividendo
        /
        2 -> divisor
    --------
        4 -> quociente (e pode ter resto)
    */

    /* Lógica DIVISÃO:
        EM Aálise
     */
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
