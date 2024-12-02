
#ifndef PROYECTO_AED_PRUEBAS_H
#define PROYECTO_AED_PRUEBAS_H

#include "Fibonacci_Heap.cpp"

// PRUEBA 1: Insertar elementos en el heap
void Prueba_1(FHeap<int>& heap){
    cout<<"\nPRUEBA 1: Insertar elementos en el heap"<<endl;
    heap.insert(10);
    heap.insert(20);
    heap.insert(5);
    heap.insert(15);
    heap.insert(30);
    heap.visualizar();
    cout<<"- Valor minimo actual: "<<heap.valor_min()<<endl;
    cout<<"- Tamaño actual del Fib Heap: "<<heap.size()<<endl;
}

// PRUEBA 2: Eliminar un nodo ingresando la clave (15)
void Prueba_2(FHeap<int>& heap){
    cout<<"\n PRUEBA 2: Eliminar un nodo ingresando la clave (15)"<<endl;
    heap.deleteByKey(15);
    heap.visualizar();
    cout<<"- Valor minimo actual: "<<heap.valor_min()<<endl;
    cout<<"- Tamaño actual del Fib Heap: "<<heap.size()<<endl;
}

// PRUEBA 3: Disminuir el valor de un nodo ingresando la clave (30 -> 2)
void Prueba_3(FHeap<int>& heap){
    cout<<"\n PRUEBA 3: Disminuir el valor de un nodo ingresando la clave (30 -> 2)"<<endl;
    heap.decreaseByKey(30, 2);
    heap.visualizar();
    cout<<"- Valor minimo actual: "<<heap.valor_min()<<endl;
    cout<<"- Tamaño actual del Fib Heap: "<<heap.size()<<endl;

}

// PRUEBA 4: Extraer el valor mínimo
void Prueba_4(FHeap<int>& heap){
    cout<<"\n PRUEBA4: Extraer el valor minimo"<<endl;
    heap.extractMin();
    heap.visualizar();
    cout<<"- Valor minimo actual: "<<heap.valor_min()<<endl;
    cout<<"- Tamaño actual del Fib Heap: "<<heap.size()<<endl;
}

//PRUEBA 5: Unión de Fib Heaps
void Prueba_5(FHeap<int>& heap){
    cout<<"\n PRUEBA 5: Union de Fib Heaps"<<endl;
    FHeap<int> heap2;
    heap2.insert(25);
    heap2.insert(7);

    cout << "\nPrimer heap:" << endl;
    heap.visualizar();

    cout << "\nSegundo heap:" << endl;
    heap2.visualizar();

    cout << "\nUniendo el segundo heap al primero" << endl;
    heap = heap.Union(heap2);
    cout << "Visualizar heaps unidos (se actualizo el primer heap):" << endl;
    heap.visualizar();
}


#endif //PROYECTO_AED_PRUEBAS_H
