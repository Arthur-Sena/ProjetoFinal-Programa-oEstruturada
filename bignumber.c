#include "bignumber.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma region "Manipuladores de bignumber"

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

void append_node(BigNumber* number, Node* newNode) {
    if (!number || !newNode) return;

    if (!number->firstNode) {
        number->firstNode = newNode;
        number->lastNode = newNode;
    } else {
        number->lastNode->nextNode = newNode;
        newNode->previousNode = number->lastNode;
        number->lastNode = newNode;
    }
}

BigNumber* copy_bignumber(const BigNumber* original) {
    if (!original) return NULL;

    BigNumber* copy = (BigNumber*)malloc(sizeof(BigNumber));
    if (!copy) return NULL;

    copy->firstNode = NULL;
    copy->lastNode = NULL;
    copy->is_negative = original->is_negative;

    Node* currentNode = original->firstNode;
    while (currentNode) {
        Node* newNode = create_node(currentNode->digit);
        if (!newNode) {
            free_bignumber(copy);
            return NULL;
        }

        append_node(copy, newNode);
        currentNode = currentNode->nextNode;
    }
    return copy;
}

void remove_leading_zeros(BigNumber* number) {
    while (number->firstNode && number->firstNode->digit == '0') {
        Node* temp = number->firstNode;
        number->firstNode = number->firstNode->nextNode;
        free(temp);
    }
}

#pragma endregion

#pragma region "Funções auxiliares"
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

// Função para comparar dois números grandes (retorna 0 se iguais, 1 se maior, -1 se menor)
int compare_bignumbers(const BigNumber* first, const BigNumber* second, const bool ignoreSign) {
    if (!first || !second) return 0;
    
    if (!ignoreSign){
        if (!first->is_negative && second->is_negative) return 1;
        else if (first->is_negative && !second->is_negative) return -1;
    }

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

BigNumber* lenghtDigits_bignumber(const BigNumber* bignumber) {
    int count = 0;
    Node* current = bignumber->firstNode;
    while (current != NULL) {
        count++;
        current = current->nextNode;
    }

    int digits_needed = snprintf(NULL, 0, "%d", count);
    char* countAsString = (char*)malloc(digits_needed + 1);

    if (!countAsString) {
        printf("Erro ao alocar memória para a string de contagem.\n");
        return NULL;
    }
    snprintf(countAsString, digits_needed + 1, "%d", count);
    BigNumber* result = create_bignumber(countAsString);
    free(countAsString);

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
#pragma endregion 

BigNumber* add_bignumbers(const BigNumber* firstBignumber, const BigNumber* secondBignumber) {
    if (!firstBignumber || !secondBignumber) return NULL;
    
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
        if (!newNode) {
            free_bignumber(result);
            return NULL;
        }
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

    free(lastNodeFirstNumber);
    free(lastNodeSecondNumber);

    return result;
}

BigNumber* subtract_bignumbers(const BigNumber* firstBignumber, const BigNumber* secondBignumber) {
    if (!firstBignumber || !secondBignumber) return NULL;

    if (firstBignumber->is_negative != secondBignumber->is_negative)
        return add_bignumbers(firstBignumber, secondBignumber);

    int comparison = compare_bignumbers(firstBignumber, secondBignumber, false);
    if (comparison == 0)
        return create_bignumber("0");

    const BigNumber* larger = (comparison > 0) ? firstBignumber : secondBignumber;
    const BigNumber* smaller = (comparison > 0) ? secondBignumber : firstBignumber;

    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (!result) return NULL;
    result->firstNode = NULL;
    result->lastNode = NULL;
    if (comparison > 0 ) 
        result->is_negative = firstBignumber->is_negative;
    else if (secondBignumber->is_negative)
        result->is_negative = false;
    else 
        result->is_negative = true;

    Node* lastNodeLarger = larger->lastNode;
    Node* lastNodeSmaller = smaller->lastNode;
    int borrow = 0;

    while (lastNodeLarger || lastNodeSmaller || borrow) {
        int digit1 = lastNodeLarger ? lastNodeLarger->digit - '0' : 0;
        int digit2 = lastNodeSmaller ? lastNodeSmaller->digit - '0' : 0;
        int subtract = digit1 - digit2 - borrow;

        if (subtract < 0) {
            subtract += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        Node* newNode = create_node(subtract + '0');
        if (!newNode) {
            free_bignumber(result);
            return NULL;
        }
        newNode->nextNode = result->firstNode;
        if (result->firstNode) {
            result->firstNode->previousNode = newNode;
        } else {
            result->lastNode = newNode;
        }
        result->firstNode = newNode;

        if (lastNodeLarger) lastNodeLarger = lastNodeLarger->previousNode;
        if (lastNodeSmaller) lastNodeSmaller = lastNodeSmaller->previousNode;
    }

    remove_leading_zeros(result);
    return result;
}

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

BigNumber* divide_bignumbers(BigNumber* firstBignumber, BigNumber* secondBignumber, const bool returnRest) {
    if (!firstBignumber || !secondBignumber) return NULL;

    if (secondBignumber->firstNode == NULL || (secondBignumber->firstNode->digit == '0' && secondBignumber->firstNode->nextNode == NULL)) {
        printf("Erro: Divisao por zero.\n");
        return NULL;
    }

    if (secondBignumber->firstNode->digit == '1' && secondBignumber->firstNode->nextNode == NULL) {
        return firstBignumber;
    }

    BigNumber* tamanhoFirstBigNumber = lenghtDigits_bignumber(firstBignumber);
    BigNumber* tamanhoSecondBigNumber = lenghtDigits_bignumber(secondBignumber);

    BigNumber* increment = create_bignumber("1");
    BigNumber* counter = create_bignumber("0");
    if (!counter) return NULL;
    
    BigNumber* result = (BigNumber*)malloc(sizeof(BigNumber));
    if (!result) return NULL;
    result = copy_bignumber(firstBignumber);

    BigNumber* difTamanhoBigNumbers = subtract_bignumbers(tamanhoFirstBigNumber, tamanhoSecondBigNumber);
    BigNumber* two = create_bignumber("2");
    BigNumber* ten = create_bignumber("10");
    if (compare_bignumbers(difTamanhoBigNumbers, two, true) > 0)
    {
        free_bignumber(counter);
        free_bignumber(result);
        
        BigNumber* subtract_increment = subtract_bignumbers(difTamanhoBigNumbers, increment);
        counter = power_bignumbers(ten, subtract_increment);
        free_bignumber(subtract_increment);

        BigNumber* multiplo = (BigNumber*)malloc(sizeof(BigNumber));
        multiplo = multiply_bignumbers(secondBignumber, counter);
        result = subtract_bignumbers(firstBignumber, multiplo); 

        int isEqual = compare_bignumbers(result, firstBignumber, true);
        if (isEqual == 0){
            free_bignumber(difTamanhoBigNumbers);
            free_bignumber(tamanhoFirstBigNumber);
            free_bignumber(tamanhoSecondBigNumber);
            free_bignumber(two);
            free_bignumber(ten);
            free_bignumber(result);
            free_bignumber(increment);
            if (returnRest) {
                if(counter != NULL)
                    free_bignumber(counter);
                counter = NULL;
                return create_bignumber("0");
            } else {
                return counter;
            }
        }
    } 
    free_bignumber(difTamanhoBigNumbers);
    free_bignumber(tamanhoFirstBigNumber);
    free_bignumber(tamanhoSecondBigNumber);
    free_bignumber(two);
    free_bignumber(ten);

    int comparison = compare_bignumbers(firstBignumber, secondBignumber, true);
    if (comparison < 0) {
        free_bignumber(result);
        return create_bignumber("0");
    } else if (comparison == 0) {
        free_bignumber(result);
        return create_bignumber("1");
    }
    
    bool resultIsNegative = (firstBignumber->is_negative != secondBignumber->is_negative);
    firstBignumber->is_negative = false;
    secondBignumber->is_negative = false;

    while(true){
        BigNumber* newDividend = subtract_bignumbers(result, secondBignumber);
        if (!newDividend) {
            free_bignumber(result);
            return NULL;
        }

        BigNumber* newCounter = add_bignumbers(counter, increment);

        if (!newCounter) {
            if(newDividend != NULL){
                free_bignumber(newDividend);
                newDividend = NULL;
            }
            if(returnRest && counter != NULL)
                free_bignumber(counter);
            free_bignumber(result);
            return NULL;
        } else {
            if (counter != NULL)
                free_bignumber(counter);
            counter = NULL;
            counter = copy_bignumber(newCounter);
        }
                
        free_bignumber(newCounter);
        if (compare_bignumbers(newDividend, secondBignumber, true) < 0){
            free_bignumber(result);
            result = returnRest ? newDividend : counter;
            if(newDividend != NULL){
                free_bignumber(newDividend);
                newDividend = NULL;
            }
            free_bignumber(newCounter);
            break;
        }
        if (result != NULL)
            free_bignumber(result);
        result = NULL;
        result = copy_bignumber(newDividend);
        if(newDividend != NULL){
                free_bignumber(newDividend);
                newDividend = NULL;
            }
        free_bignumber(newCounter);
    }

    free_bignumber(increment);
    if(returnRest && counter != NULL)
        free_bignumber(counter);
    result->is_negative = resultIsNegative;
    return result;
}

BigNumber* power_bignumbers(const BigNumber* base, const BigNumber* exponent) {    
    BigNumber* result = create_bignumber("1");
    BigNumber* numberTwo = create_bignumber("2");

    BigNumber* baseCopy = (BigNumber*)malloc(sizeof(BigNumber));
    baseCopy = copy_bignumber(base);
    
    BigNumber* exponentCopy = (BigNumber*)malloc(sizeof(BigNumber));
    exponentCopy = copy_bignumber(exponent);
    
    while (!(exponentCopy->firstNode->digit == '0' && exponentCopy->firstNode->nextNode == NULL)) {
        if ((exponentCopy->lastNode->digit - '0') % 2 != 0) {
            BigNumber* temp = multiply_bignumbers(result, baseCopy);
            free_bignumber(result);
            result = temp;
        }

        BigNumber* temp = multiply_bignumbers(baseCopy, baseCopy);
        free_bignumber(baseCopy);
        baseCopy = temp;        

        //BigNumber* half_exp = create_bignumber(long_to_string( bignumber_to_long(exponent) / 2));
        BigNumber* half_exp = divide_bignumbers(exponentCopy, numberTwo, false);
        free_bignumber(exponentCopy);
        exponentCopy = half_exp;
    }

    free_bignumber(baseCopy);
    free_bignumber(exponentCopy);
    free_bignumber(numberTwo);
    return result;
}

//Karatsuba artigo
//https://www.ime.usp.br/~pf/analise_de_algoritmos/aulas/karatsuba.html

//"Sejam u e v dois números com no máximo n dígitos cada"
//"Seja p o número formado pelos n/2 primeiros dígitos (dígitos mais significativos) de u"
//"Seja q o número formado pelos n/2 últimos dígitos (dígitos menos significativos) de u"

// n = o número de dígitos -> maior número de dígitos entre os dois números.
//p -> n/2 primeiros números do firstBignumber
//q -> n/2 últimos números do firstBignumber
//r -> n/2 primeiros números do secondBignumber
//s -> n/2 últimos números do secondBignumber

//INICIALMENTE:
//u = p × 10^n/2 + q
//v = r × 10^n/2 + s
//ENTÃO:
//u = p × 10^4 + q
//v = r × 10^4 + s
//y = (p + q) × (r + s)
//Equação karatsuba: u × v = p × r × 10^n + ( y − p × r − q × s) × 10^n/2 + q × s 

BigNumber* karatsuba_multiply_bignumbers(const BigNumber* firstBignumber, const BigNumber* secondBignumber, const BigNumber* nNumber) {
   /* Seguir esse raciocínio [ ETAPAS] */
   /* 
   Karatsuba (u, v, n)
    ☑ 1  se n ≤ 3
    ☑ 2  devolva u × v e pare
    ☑ 3  m := ⌈n/2⌉
    ☑ 4  p := ⌊u/10m⌋
    ☑ 5  q := u mod 10m
    ☑ 6  r := ⌊v/10m⌋
    ☑ 7  s := v mod 10m
    ☑ 8  pr := Karatsuba (p, r, m)
    ☑ 9  qs := Karatsuba (q, s, m)
    ☑ 10  y := Karatsuba (p + q, r + s, m+1)
    ☑ 11  uv := pr × 102m + (y − pr − qs) × 10m + qs
    ☑ 12  devolva uv
    */

    BigNumber* bgnTen = create_bignumber("10");
    BigNumber* bgnThree = create_bignumber("3");
    BigNumber* bgnZero = create_bignumber("0");
    BigNumber* bgnTwo = create_bignumber("2");
    BigNumber* bgnOne = create_bignumber("1");
    BigNumber* n = (BigNumber*)malloc(sizeof(BigNumber));

    if(compare_bignumbers(nNumber, bgnZero, false) == 1){
        n = nNumber;
    } else if(compare_bignumbers(nNumber, bgnZero, false) == 0) {
        int moreBignumber = compare_bignumbers(firstBignumber, secondBignumber, false);
        n = lenghtDigits_bignumber(secondBignumber);
        if(moreBignumber == 1) n = lenghtDigits_bignumber(firstBignumber);
    } else {
        free_bignumber(bgnTen);
        free_bignumber(bgnThree);
        free_bignumber(bgnOne);
        free_bignumber(bgnTwo);
        free_bignumber(n);
        return bgnZero;
    }

   if(compare_bignumbers(n, bgnThree, false) != 1) {
        free_bignumber(n);
        free_bignumber(bgnTen);
        free_bignumber(bgnThree);
        free_bignumber(bgnZero);
        free_bignumber(bgnOne);
        free_bignumber(bgnTwo);
        return multiply_bignumbers(firstBignumber, secondBignumber);
   }

   BigNumber* m = divide_bignumbers(n, bgnTwo, false);

    BigNumber* expoenteBaseTen = power_bignumbers(bgnTen, m);

    BigNumber* p = divide_bignumbers(firstBignumber, expoenteBaseTen, false);
    BigNumber* q = divide_bignumbers(firstBignumber, expoenteBaseTen, true);

    BigNumber* r = divide_bignumbers(secondBignumber, expoenteBaseTen, true);
    BigNumber* s = divide_bignumbers(secondBignumber, expoenteBaseTen, false);

    BigNumber* prKaratsuba = karatsuba_multiply_bignumbers(p, r, m);
    BigNumber* qsKaratsuba = karatsuba_multiply_bignumbers(q, s, m);
    
    BigNumber* pSumQ = (BigNumber*)malloc(sizeof(BigNumber));
    pSumQ = add_bignumbers(p, q);
    
    BigNumber* rSumS = (BigNumber*)malloc(sizeof(BigNumber));
    rSumS = add_bignumbers(r, s);
    
    BigNumber* mSumBgnOne = (BigNumber*)malloc(sizeof(BigNumber));
    mSumBgnOne = add_bignumbers(m, bgnOne);
    BigNumber* yKaratsuba = karatsuba_multiply_bignumbers(pSumQ, rSumS, mSumBgnOne);
    free_bignumber(pSumQ);
    free_bignumber(rSumS);
    free_bignumber(mSumBgnOne);

    BigNumber* mMultiplyBgnTwo = (BigNumber*)malloc(sizeof(BigNumber));
    mMultiplyBgnTwo = multiply_bignumbers(m, bgnTwo);
    BigNumber* expoenteBaseTenDoisM = power_bignumbers(bgnTen, mMultiplyBgnTwo);
    free_bignumber(mMultiplyBgnTwo);

    BigNumber* yKaratsuba_Subtract_prKaratsuba = (BigNumber*)malloc(sizeof(BigNumber));
    yKaratsuba_Subtract_prKaratsuba = subtract_bignumbers(yKaratsuba, prKaratsuba);

    BigNumber* secondValueUV = (BigNumber*)malloc(sizeof(BigNumber));
    secondValueUV = subtract_bignumbers(yKaratsuba_Subtract_prKaratsuba, qsKaratsuba);

    BigNumber* firstResultKaratsuba = (BigNumber*)malloc(sizeof(BigNumber));
    firstResultKaratsuba = karatsuba_multiply_bignumbers(prKaratsuba, expoenteBaseTenDoisM, bgnZero);
    
    BigNumber* secondResultKaratsuba = (BigNumber*)malloc(sizeof(BigNumber));
    secondResultKaratsuba = karatsuba_multiply_bignumbers(secondValueUV, expoenteBaseTen, bgnZero);

    BigNumber* sumResults = (BigNumber*)malloc(sizeof(BigNumber));
    sumResults = add_bignumbers(firstResultKaratsuba, secondResultKaratsuba);
    
    BigNumber* uv = add_bignumbers(sumResults, qsKaratsuba);

    free_bignumber(firstResultKaratsuba);
    free_bignumber(secondResultKaratsuba);
    free_bignumber(sumResults);
    free_bignumber(yKaratsuba_Subtract_prKaratsuba);
    free_bignumber(secondValueUV);
    free_bignumber(prKaratsuba);
    free_bignumber(qsKaratsuba);
    free_bignumber(yKaratsuba);
    free_bignumber(expoenteBaseTen);
    free_bignumber(expoenteBaseTenDoisM);
    free_bignumber(p);
    free_bignumber(q);
    free_bignumber(r);
    free_bignumber(s);
    free_bignumber(n);
    free_bignumber(m);
    free_bignumber(bgnTen);
    free_bignumber(bgnThree);
    free_bignumber(bgnZero);
    free_bignumber(bgnOne);
    free_bignumber(bgnTwo);
    return uv;
}
