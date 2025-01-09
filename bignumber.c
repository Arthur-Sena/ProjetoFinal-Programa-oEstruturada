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

//Karatsuba artigo
//https://www.ime.usp.br/~pf/analise_de_algoritmos/aulas/karatsuba.html

BigNumber* multiply_bignumbers(const BigNumber* firstBignumber, const BigNumber* secondBignumber) {
    if (!firstBignumber || !secondBignumber) return NULL;

    BigNumber* result = create_bignumber("0");
    if (!result) return NULL;

    Node* secondNode = secondBignumber->lastNode;
    int positionOffset = 0;

    while (secondNode) {
        int secondDigit = secondNode->digit - '0';
        int carry = 0;

        BigNumber* partialResult = (BigNumber*)malloc(sizeof(BigNumber));
        if (!partialResult) {
            free_bignumber(result);
            return NULL;
        }
        partialResult->firstNode = NULL;
        partialResult->lastNode = NULL;
        partialResult->is_negative = false;

        for (int i = 0; i < positionOffset; i++) {
            Node* zeroNode = create_node('0');
            zeroNode->nextNode = partialResult->firstNode;
            if (partialResult->firstNode)
                partialResult->firstNode->previousNode = zeroNode;
            else
                partialResult->lastNode = zeroNode;
            partialResult->firstNode = zeroNode;
        }

        Node* firstNode = firstBignumber->lastNode;
        while (firstNode) {
            int firstDigit = firstNode->digit - '0';
            int product = firstDigit * secondDigit + carry;

            carry = product / 10;
            int digitResult = product % 10;

            Node* newNode = create_node(digitResult + '0');
            newNode->nextNode = partialResult->firstNode;
            if (partialResult->firstNode)
                partialResult->firstNode->previousNode = newNode;
            else
                partialResult->lastNode = newNode;
            partialResult->firstNode = newNode;

            firstNode = firstNode->previousNode;
        }

        if (carry > 0) {
            Node* carryNode = create_node(carry + '0');
            carryNode->nextNode = partialResult->firstNode;
            if (partialResult->firstNode)
                partialResult->firstNode->previousNode = carryNode;
            else
                partialResult->lastNode = carryNode;
            partialResult->firstNode = carryNode;
        }

        BigNumber* newResult = add_bignumbers(result, partialResult);
        free_bignumber(result);
        free_bignumber(partialResult);
        result = newResult;

        secondNode = secondNode->previousNode;
        positionOffset++;
    }

    result->is_negative = (firstBignumber->is_negative != secondBignumber->is_negative);
    return result;
}

// Função para adicionar um nó ao final da lista ligada
void append_node(BigNumber* number, Node* newNode) {
    if (!number || !newNode) return;

    if (!number->firstNode) {
        number->firstNode = newNode;
    } else {
        Node* current = number->firstNode;
        while (current->nextNode) {
            current = current->nextNode;
        }
        current->nextNode = newNode;
    }
}

// Função para remover zeros à esquerda de um número grande
void remove_leading_zeros(BigNumber* number) {
    while (number->firstNode && number->firstNode->digit == '0') {
        Node* temp = number->firstNode;
        number->firstNode = number->firstNode->nextNode;
        free(temp);
    }
}
// Função para comparar dois números grandes (retorna 0 se iguais, 1 se maior, -1 se menor)
int compare_bignumbers(const BigNumber* first, const BigNumber* second) {
    if (!first || !second) return 0;

    Node* firstNode = first->firstNode;
    Node* secondNode = second->firstNode;

    // Verificar se os números têm o mesmo número de dígitos
    int lenFirst = 0, lenSecond = 0;
    while (firstNode) { lenFirst++; firstNode = firstNode->nextNode; }
    while (secondNode) { lenSecond++; secondNode = secondNode->nextNode; }

    if (lenFirst > lenSecond) return 1;
    if (lenFirst < lenSecond) return -1;

    // Se tiverem o mesmo número de dígitos, comparar digit por digit
    firstNode = first->firstNode;
    secondNode = second->firstNode;
    while (firstNode && secondNode) {
        if (firstNode->digit > secondNode->digit) return 1;
        if (firstNode->digit < secondNode->digit) return -1;
        firstNode = firstNode->nextNode;
        secondNode = secondNode->nextNode;
    }

    return 0;
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
    if (!firstBignumber || !secondBignumber) return NULL;
    if (secondBignumber->firstNode == NULL) return NULL; // Divisor não pode ser zero

    BigNumber* result = create_bignumber("0");
    if (!result) return NULL;

    BigNumber* currentDividend = create_bignumber("0");
    if (!currentDividend) {
        free_bignumber(result);
        return NULL;
    }

    Node* dividendNode = firstBignumber->firstNode;

    while (dividendNode) {
        // Adicionar o próximo dígito ao currentDividend
        Node* newNode = create_node(dividendNode->digit);
        append_node(currentDividend, newNode);

        // Remover zeros à esquerda do currentDividend
        remove_leading_zeros(currentDividend);

        // Calcular quantas vezes o divisor cabe no currentDividend
        int count = 0;
        while (compare_bignumbers(currentDividend, divisor) >= 0) {
            BigNumber* newDividend = subtract_bignumbers(currentDividend, secondBignumber);
            free_bignumber(currentDividend);
            currentDividend = newDividend;
            count++;
        }

        // Adicionar o count ao resultado
        Node* resultNode = create_node(count + '0');
        append_node(result, resultNode);

        dividendNode = dividendNode->nextNode;
    }

    // Remover zeros à esquerda do resultado
    remove_leading_zeros(result);

    // Ajustar sinal do resultado
    result->is_negative = (firstBignumber->is_negative != secondBignumber->is_negative);

    free_bignumber(currentDividend);
    return result;
}

long bignumber_to_long(const BigNumber* bignumber) {
    long result = 0;
    Node* current = bignumber->firstNode;
    while (current) {
        result = result * 10 + (current->digit - '0');
        current = current->nextNode;
    }
    return bignumber->is_negative ? -result : result;
}

char* long_to_string(long number) {
    int length = snprintf(NULL, 0, "%ld", number);
    char* str = (char*)malloc(length + 1);
    snprintf(str, length + 1, "%ld", number);
    return str;
}

BigNumber* power_bignumbers(const BigNumber* base, const BigNumber* exponent) {    
    BigNumber* result = create_bignumber("1");
    BigNumber* baseCopy = base;
    BigNumber* numberTwo = create_bignumber("2");

    while (!(exponent->firstNode->digit == '0' && exponent->firstNode->nextNode == NULL)) {
        if ((exponent->lastNode->digit - '0') % 2 != 0) {
            BigNumber* temp = multiply_bignumbers(result, baseCopy);
            free_bignumber(result);
            result = temp;
        }

        BigNumber* temp = multiply_bignumbers(baseCopy, baseCopy);
        free_bignumber(baseCopy);
        baseCopy = temp;        
        /*
            QUANDO A FUNÇÂO DE DIVISÃO ESTIVER FEITA, É BOM DESCOMENTAR ESSE CÓDIGO E APAGAR
            AS FUNÇÕES bignumber_to_long E long_to_string
            
            SUBSTITUIR O CÓDIGO "BigNumber* half_exp = create_bignumber(long_to_string((exponentLong / 2)))" 
            POR ESSE:           BigNumber* half_exp = divide_bignumbers(exponent, numberTwo)
        */
        BigNumber* half_exp = create_bignumber(long_to_string( bignumber_to_long(exponent) / 2));
        free_bignumber(exponent);
        exponent = half_exp;
    }

    free_bignumber(baseCopy);
    free_bignumber(exponent);
    free_bignumber(numberTwo);
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
