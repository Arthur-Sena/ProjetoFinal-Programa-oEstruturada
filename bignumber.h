#ifndef BIGNUMBER_H
#define BIGNUMBER_H
#include <stdbool.h>

#pragma region "Lista duplamente encadeada"
/*
    Region contem a definicao dos "tipos" Node (número quebrado/ algarismo por algarismo) e 
    BigNumber (recebe o ponteiro do primeiro algarismo do número )
    
    Node.previousNode = Ponteiro do nó anterior
    Node.nextNode     = Ponteiro para o proximo nó 

    BigNumber.is_negative = Bool (0 ou 1) verigicando se o nó atual contem número positivo ou negativo
*/
typedef struct Node {
    char digit;
    struct Node *previousNode, *nextNode; 
} Node;

typedef struct {
    Node* firstNode; 
    Node* lastNode; 
    bool is_negative;
} BigNumber;
#pragma endregion

#pragma region "Funções públlicas"

BigNumber* create_bignumber(const char* number);
void free_bignumber(BigNumber* ptrBignumber);
BigNumber* add_bignumbers(const BigNumber* firstNumber, const BigNumber* secondNumber);
BigNumber* subtract_bignumbers(const BigNumber* firstNumber, const BigNumber* secondNumber);
BigNumber* multiply_bignumbers(const BigNumber* firstNumber, const BigNumber* secondNumber);
BigNumber* divide_bignumbers(BigNumber* firstNumber, BigNumber* secondNumber, const bool returnRest);
BigNumber* power_bignumbers(const BigNumber* firstNumber, const BigNumber* secondNumber);
BigNumber* karatsuba_multiply_bignumbers(const BigNumber* firstNumber, const BigNumber* secondNumber, const BigNumber* nNumber);
void print_bignumber(const BigNumber* ptrBignumber);

#pragma endregion

#endif // BIGNUMBER_H

/*
Comentários
ifndef: Impede que um arquivo de cabeçalho seja duplicado
pragma: Funciona como o #region do c#, só pra estruturar melhor o código ;)
*/