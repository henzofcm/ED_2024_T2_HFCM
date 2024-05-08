#include <iostream>
#include <chrono>
#include <random>

/*  ---- IMPORTAÇÕES BASE ----  */
using std::cout;
using std::endl;
using std::swap;
using std::rand;

/*  ---- IMPORTAÇÕES DE TEMPO ----  */
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;


typedef struct Node
{
    int iValue;
    Node* ptrNext;
    Node* ptrLast;
} Node;


/*  ---- FUNÇÕES BASE DE LISTA DUPLAMENTE ENCADEADA ----  */
Node* ptrCreateList(void);
Node* ptrCreateNode(int iValue);
void vAddElemFront(Node*& ptrList, int iValue);
void vAddElemEnd(Node*& ptrList, int iValue);
void vPrintList(Node* ptrList);
void vDeleteList(Node*& ptrList);

/*  ---- FUNÇÕES DE SORT IMPLEMENTADAS ----  */
void vBubbleSort(Node*& ptrList);
void vSelectSort(Node*& ptrList);

/*  ---- FUNÇÕES EXTRAS UTILIZADAS ----  */
void vSwapElements(Node* ptrNode1, Node* ptrNode2);
Node* ptrGenerateRandomList(int iSize);
int64_t iRandomTests(int iAmount, int iSize, void (*fSort)(Node*& ptrList));


int main()
{
    // Cria uma lista e vários elementos
    Node* ptrList = ptrGenerateRandomList(10);

    cout << " ======= BUBBLE SORT ======= " << endl;
    cout << "Lista atual: ";
    vPrintList(ptrList);

    // Ordena-a com bubble sort e mostra
    vBubbleSort(ptrList);
    cout << "Lista ordenada: ";
    vPrintList(ptrList);

    // Limpa a lista da memória
    vDeleteList(ptrList);
    vPrintList(ptrList);


    // Recria outra lista
    ptrList = ptrGenerateRandomList(10);

    cout << " ======= SELECT SORT ======= " << endl;
    cout << "Nova lista: ";
    vPrintList(ptrList);

    // Ordena-a com select sort e mede o tempo gasto
    vSelectSort(ptrList);
    cout << "Lista ordenada: ";
    vPrintList(ptrList);

    // Limpa da memória
    vDeleteList(ptrList);
    vPrintList(ptrList);


    // Mede o tempo gasto em vários testes com bubble
    cout << endl << "Tempo medio gasto em 100 listas de tamanho 100 com bubble: ";

    int64_t aTotalTime = iRandomTests(100, 100, vBubbleSort);

    cout << aTotalTime / 100 << " ns" << endl;

    // Mede o tempo gasto em vários testes com select
    cout << endl << "Tempo medio gasto em 100 listas de tamanho 100 com select: ";

    aTotalTime = iRandomTests(100, 100, vSelectSort);

    cout << aTotalTime / 100 << " ns" << endl;

    // Fim
    return 0;
}


Node* ptrCreateList(void)
{
    // Retorna, apropriadamente, uma lista vazia
    return nullptr;
}

Node* ptrCreateNode(int iValue)
{
    // Aloca memória para o Node
    Node* ptrList = (Node*) malloc(sizeof(Node));

    // Atribui valores aos atributos
    ptrList->iValue = iValue;
    ptrList->ptrNext = nullptr;
    ptrList->ptrLast = nullptr;

    return ptrList;
}

void vAddElemFront(Node*& ptrList, int iValue)
{
    // Cria o novo node
    Node* ptrNewElem = ptrCreateNode(iValue);

    // Caso a lista esteja vazia, ela será o novo elemento
    if (ptrList == nullptr)
    {
        ptrList = ptrNewElem;
        return;
    }

    // E adiciona-o no inicio da lista
    ptrNewElem->ptrNext = ptrList;
    ptrList->ptrLast = ptrNewElem;
    ptrList = ptrNewElem;
}

void vAddElemEnd(Node*& ptrList, int iValue)
{
    // Cria o novo node
    Node* ptrNewElem = ptrCreateNode(iValue);

    // Caso a lista esteja vazia, ela será o novo elemento
    if (ptrList == nullptr)
    {
        ptrList = ptrNewElem;
        return;
    }

    // Percorre toda a lista com um ponteiro indicador
    Node* ptrFoo = ptrList;

    while (ptrFoo->ptrNext != nullptr)
    {
        ptrFoo = ptrFoo->ptrNext;
    }

    // E adiciona-o no fim da mesma
    ptrNewElem->ptrLast = ptrList;
    ptrList->ptrNext = ptrNewElem;
}

void vPrintList(Node* ptrList)
{
    // Foo para atravessar a lista
    Node* ptrFoo = ptrList;

    // Anda elemento a elemento, printando cada um
    // (Se estiver vazia, não printa nada)
    while(ptrFoo != nullptr)
    {
        cout << ptrFoo->iValue << " ";
        ptrFoo = ptrFoo->ptrNext;
    }

    // E termina a linha
    cout << endl;
}

void vDeleteList(Node*& ptrList)
{
    Node* ptrFoo = ptrList;

    // Itera sobre toda a lista
    while (ptrFoo != nullptr)
    {
        // Avança a lista
        ptrList = ptrList->ptrNext;

        // Libera o Foo (antecessor ao ptrList)
        free(ptrFoo);

        // Avança o Foo
        ptrFoo = ptrList;
    } 
}


void vBubbleSort(Node*& ptrList)
{
    // Caso seja vazia, não há nada a fazer
    if (ptrList == nullptr)
        return;

    // Ponteiros que percorrerão a lista
    Node* ptrSortedList = nullptr;
    Node* ptrFoo = ptrList;

    // Enquanto a lista não estiver toda organizada
    while (ptrSortedList != ptrList)
    {
        bool bSwapped  = false;

        // Percorre-a ordenando assim que possível
        while(ptrFoo->ptrNext != ptrSortedList)
        {
            // Troca-os de lugar se estiverem fora de ordem
            if (ptrFoo->iValue > ptrFoo->ptrNext->iValue)
            {
                bSwapped = true;
                vSwapElements(ptrFoo->ptrNext, ptrFoo);
            }

            ptrFoo = ptrFoo->ptrNext;
        }

        // Caso já esteja em ordem (não houve trocas), retorna
        if (bSwapped == false)
            return;

        // Por fim, ptrFoo retorna ao começo e ptrSortedList aumenta
        ptrSortedList = ptrFoo;
        ptrFoo = ptrList;
    }

}

void vSelectSort(Node*& ptrList)
{
    // Caso for vazia, não há nada a fazer
    if (ptrList == nullptr)
        return;

    // Ponteiros que percorrerão a lista
    Node* ptrUnsortedList = ptrList;
    Node* ptrFoo = ptrList;

    // Holder para o menor elemento encontrado em cada loop
    Node* ptrHolder = ptrList;

    while (ptrUnsortedList->ptrNext != nullptr)
    {
        while(ptrFoo != nullptr)
        {
            // Caso, nesta iteração, ptrFoo seja menor, salva-o
            if (ptrFoo->iValue < ptrHolder->iValue)
            {
                ptrHolder = ptrFoo;
            }

            ptrFoo = ptrFoo->ptrNext;
        }

        // Troca-os de lugar
        vSwapElements(ptrUnsortedList, ptrHolder);

        // Por fim, fazem-nos começar numa nova lista menor
        ptrUnsortedList = ptrUnsortedList->ptrNext;
        ptrFoo = ptrUnsortedList;
        ptrHolder = ptrFoo;
    }
}


void vSwapElements(Node* ptrNode1, Node* ptrNode2)
{
    // Cria uma variável temporária
    int iTempValue = ptrNode1->iValue;

    // E troca os elementos
    ptrNode1->iValue = ptrNode2->iValue;
    ptrNode2->iValue = iTempValue;
}

Node* ptrGenerateRandomList(int iSize)
{
    // Cria uma lista com elementos aleatórios de tamanho iSize
    Node* ptrNewList = ptrCreateList();

    for (int i = 0; i < iSize; i++)
    {
        vAddElemFront(ptrNewList, rand());
    }

    return ptrNewList;
}

int64_t iRandomTests(int iAmount, int iSize, void (*fSort)(Node*& ptrList))
{
    // Salvará o tempo total gasto em todas iterações
    int64_t iTotalTime = 0;
    
    for (int i = 0; i < iAmount; i++)
    {
        // Gera uma lista
        Node* ptrList = ptrGenerateRandomList(iSize);

        // Mede o tempo
        auto aTimeStart = high_resolution_clock::now();
        fSort(ptrList);
        auto aTimeEnd = high_resolution_clock::now();

        // E adiciona ao tempo total
        auto aDuration = duration_cast<nanoseconds> (aTimeEnd - aTimeStart);
        iTotalTime += aDuration.count();

        // Limpa da memória
        vDeleteList(ptrList);
    }

    return iTotalTime;
}
