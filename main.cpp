#include <iostream>
#include "Fibonacci_Heap.h"
#include "Pruebas.h"

int main() {
    FHeap<int> heap;
    Prueba_1(heap);
    Prueba_2(heap);
    Prueba_3(heap);
    Prueba_4(heap);
    Prueba_5(heap);
    return 0;
}