#include <iostream>
#include <cmath>
#include <limits>

using namespace std;

template <typename T>
struct Nodo {
    T data;
    Nodo<T>* derecha;
    Nodo<T>* izquierda;
    Nodo<T>* padre;
    Nodo<T>* hijo;
    bool mark; // indica si perdió o no un hijo
    int rank;  // cantidad de hijos
    Nodo() : data(0), derecha(nullptr), izquierda(nullptr), padre(nullptr), hijo(nullptr), mark(false), rank(0) {}
    Nodo(T valor) : data(valor), derecha(nullptr), izquierda(nullptr), padre(nullptr), hijo(nullptr), mark(false), rank(0) {}
};



template <typename T>
class FHeap {
private:
    Nodo<T>* rootlist; // lista raíz del heap
    Nodo<T>* ptr_min;  // puntero al nodo min de rootlist
    int cont;          // contador de la cantidad de nodos en el heap

    void visualizar_heap(Nodo<T>* nodo, int nivel);
public:
    FHeap() : rootlist(nullptr), ptr_min(nullptr), cont(0) {}

    void insert(T valor);
    T valor_min();
    Nodo<T>* extractMin();
    void decreaseNode(Nodo<T>* nodo, T valor);
    void deleteNode(Nodo<T>* nodo);
    FHeap Union(FHeap<T>);

    bool empty();
    int size();
    void consolidate();
    void print() { print_nodo(rootlist); }
    void visualizar();   
    void cortar(Nodo<T>* hijo, Nodo<T>* padre);
    void cascada(Nodo<T>* nodo);
    Nodo<T>* findNode(T valor); // Método para encontrar un nodo por su valor
    void deleteByKey(T valor);  // Método para eliminar un nodo por su clave
    void decreaseByKey(T valor, T nuevo_valor);  // Método para disminuir un valor por su clave
    Nodo<T>* findNode1(Nodo<T>* nodo, T valor);
    
};

template<typename T>
void FHeap<T>::decreaseNode(Nodo<T>* nodo, T nuevo_valor) {
    if (nodo == nullptr || nuevo_valor > nodo->data) {
        std::cerr << "Error: El nuevo valor debe ser menor que el valor actual del nodo." << std::endl;
        return;
    }
    
    nodo->data = nuevo_valor; // Disminuir el valor del nodo
    Nodo<T>* padre = nodo->padre;
    
    // Si la nueva clave es menor que el padre, cortar el nodo
    if (padre != nullptr && nodo->data < padre->data) {
        cortar(nodo, padre);
        cascada(padre);
    }
    
    // Si el nuevo valor es menor que el mínimo actual, actualizar el puntero mínimo
    if (nodo->data < ptr_min->data) {
        ptr_min = nodo;
    }
}


template<typename T>
void FHeap<T>::deleteNode(Nodo<T>* nodo) {
    decreaseNode(nodo, std::numeric_limits<T>::min()); // Reducir el valor a -∞ (mínimo posible)
    extractMin(); // Extraer el nodo
}



//MÉTODOS DE APOYO
template<typename T>
Nodo<T>* FHeap<T>::findNode1(Nodo<T>* nodo, T valor) { 
    if (nodo == nullptr) return nullptr;

    Nodo<T>* temp = nodo;
    do {
        if (temp->data == valor) return temp;
        if (temp->hijo != nullptr) {
            Nodo<T>* res = findNode1(temp->hijo, valor);  // Recursivamente buscar en los hijos
            if (res != nullptr) return res;
        }
        temp = temp->derecha;
    } while (temp != nodo);
    return nullptr;
}


template<typename T>
Nodo<T>* FHeap<T>::findNode(T valor) { 
    return findNode1(rootlist, valor); // Llamar a la función sobrecargada
}



template<typename T>
Nodo<T> *linking(Nodo<T> *N1, Nodo<T> *N2) {
    Nodo<T> *NDown = N1->data >= N2->data ? N1 : N2;
    Nodo<T> *NUp = NDown == N1 ? N2 : N1;

    if (NUp->hijo == nullptr) { //Si el menor no tiene hijos, el mayor se vuelve su hijo único
        NUp->hijo = NDown;
        NDown->izquierda = NDown;
        NDown->derecha = NDown;
    } else {
        Nodo<T> *primerHijo = NUp->hijo;
        Nodo<T> *ultimoHijo = primerHijo->izquierda;

        NDown->izquierda = ultimoHijo;
        NDown->derecha = primerHijo;
        ultimoHijo->derecha = NDown;
        primerHijo->izquierda = NDown;
    }
    NDown->padre = NUp;
    NUp->rank++; //Se aumenta la cantidad de hijos del nodo menor
    return NUp;
}


//MÉTODOS QUE PERMITEN MANTENER LA INTEGRIDAD DE NUESTRO HEAP
template<typename T>
void FHeap<T>::cortar(Nodo<T>* hijo, Nodo<T>* padre) {
    // Remover el nodo hijo de la lista de hijos del padre
    if (padre->hijo == hijo) {
        if (hijo->derecha == hijo) {
            padre->hijo = nullptr;
        } else {
            padre->hijo = hijo->derecha;
        }
    }
    hijo->izquierda->derecha = hijo->derecha;
    hijo->derecha->izquierda = hijo->izquierda;

    // Agregar el nodo hijo a la lista raíz
    hijo->izquierda = rootlist->izquierda;
    hijo->derecha = rootlist;
    rootlist->izquierda->derecha = hijo;
    rootlist->izquierda = hijo;
    hijo->padre = nullptr;
    hijo->mark = false;

    padre->rank--; // Reducir el número de hijos del padre
}

template<typename T>
void FHeap<T>::cascada(Nodo<T>* nodo) {
    Nodo<T>* padre = nodo->padre;
    if (padre != nullptr) {
        if (!nodo->mark) {
            nodo->mark = true;
        } else {
            cortar(nodo, padre);
            cascada(padre);
        }
    }
}

template<typename T>
void FHeap<T>::consolidate() {
    // Máxima cantidad de hijos de un árbol
    int D = static_cast<int>(std::log(this->cont) / std::log(2)) + 1;
    auto **lista = new Nodo<T> *[D]; // Puntero de punteros a Nodo de largo log_2(n_nodos en FHeap) + 1
    for (int i = 0; i < D; i++) {
        *(lista + i) = nullptr;
    }

    // Recorremos toda la rootList
    auto temp = rootlist;
    Nodo<T> *temp2 = temp;
    do {
        auto temp3 = temp2->derecha; // Guarda lo que estaba originalmente a la derecha de temp2

        // Mientras el rango de temp2 ya esté ocupado en la lista
        while (*(lista + temp2->rank) != nullptr) {
            int d = temp2->rank;   // Guardas el rank actual
            temp2 = linking(temp2, *(lista + d));  // Unes los árboles con el mismo rango
            *(lista + d) = nullptr;  // Limpias la entrada
        }

        *(lista + temp2->rank) = temp2;  // Asigna temp2 a su rango en lista
        temp2 = temp3;  // Mueve temp2 a la derecha

    } while (temp != temp2);  // Continúa hasta que completes el ciclo

    // Creamos un nuevo heap con cada valor de la lista.
    this->rootlist = nullptr;
    for (int i = 0; i < D; i++) {
        if (*(lista + i) != nullptr) {
            if (rootlist == nullptr) {
                rootlist = *(lista + i);
                rootlist->izquierda = rootlist->derecha = rootlist;
            } else {
                (*(lista + i))->izquierda = rootlist->izquierda;
                (*(lista + i))->derecha = rootlist;
                rootlist->izquierda->derecha = *(lista + i);
                rootlist->izquierda = *(lista + i);
            }
            if ((*(lista + i))->data < ptr_min->data) ptr_min = *(lista + i);
        }
    }
    delete[] lista;
}


template <typename T>
T FHeap<T>::valor_min(){
    return ptr_min->data;
}

template <typename T>
bool FHeap<T>::empty(){
    return rootlist == nullptr;
}

template <typename T>
int FHeap<T>::size(){
    return cont;
}


//FUNCIONES PRINCIPALES
template <typename T>
void FHeap<T>::insert(T valor){
    // creamos una pequeña lista circular con el nodo a insertar
    Nodo<T>* nnodo = new Nodo<T>(valor);
    nnodo->derecha = nnodo->izquierda = nnodo;

    // preguntamos si el rootlist esta vacío o no
    if(rootlist == nullptr){
        // en caso lo sea, el nuevo nodo ocupada ese lugar
        rootlist = nnodo;
        // al igual que el puntero mínimo
        ptr_min = nnodo;
    } else {
        // en caso rootlist no sea vacío
        nnodo->derecha = rootlist;
        nnodo->izquierda = rootlist->izquierda;
        rootlist->izquierda->derecha = nnodo;
        rootlist->izquierda = nnodo;
        // actualizamos el puntero mínimo si es el caso
        if (nnodo->data < ptr_min->data){
            ptr_min = nnodo;
        }
    }
    // incrementamos en 1 la cantidad de nodos en el Heap
    cont++;
}

template<typename T>
Nodo<T>* FHeap<T>::extractMin() {
    auto z = this->ptr_min;
    if (this->rootlist != nullptr) {
        if (cont == 1) { //Si solo hay un nodo, lo eliminamos
            ptr_min = nullptr;
            rootlist = nullptr;
            cont--;
            return z;
        }

        if (z->hijo != nullptr) {
            // Añadir los hijos del nodo mínimo a la lista raíz
            z->hijo->derecha->izquierda = rootlist;
            z->hijo->derecha = rootlist->derecha;
            rootlist->derecha->izquierda = z->hijo;
            rootlist->derecha = z->hijo->derecha;

            auto y = z->hijo;
            do{
                y->padre = nullptr;
                y = y->derecha;
            } while(y != z->hijo);
            z->hijo = nullptr;
        }

        // Eliminar el nodo mínimo
        ptr_min->izquierda->derecha = ptr_min->derecha;
        ptr_min->derecha->izquierda = ptr_min->izquierda;
        ptr_min = ptr_min->derecha; // Esto de forma momentánea.
        this->cont--;

        // Llamamos a la función consolidate
        consolidate();
    }
    return z;
}


// Método para eliminar un nodo ingresando su clave (key)
template<typename T>
void FHeap<T>::deleteByKey(T valor) {
    Nodo<T>* nodo = findNode(valor);
    if (nodo != nullptr) {
        deleteNode(nodo);
    } else {
        std::cout << "Nodo con valor " << valor << " no encontrado." << std::endl;
    }
}

// Método para disminuir el valor de un nodo ingresando su clave (key)
template<typename T>
void FHeap<T>::decreaseByKey(T valor, T nuevo_valor) {
    Nodo<T>* nodo = findNode(valor);
    if (nodo != nullptr) {
        decreaseNode(nodo, nuevo_valor);
    } else {
        std::cerr << "Nodo con valor " << valor << " no encontrado." << std::endl;
    }
}

//FUNCIONES DE VISUALIZACIÓN
template <typename T>
void FHeap<T>::visualizar_heap(Nodo<T>* nodo, int nivel) {
    if (nodo == nullptr) return;

    Nodo<T>* temp = nodo;
    do {
        // Imprimir el valor del nodo con una indentación basada en el nivel
        for (int i = 0; i < nivel; ++i) {
            std::cout << "  "; // 2 espacios por cada nivel de profundidad
        }
        std::cout << "└─ Nodo: " << temp->data << " (rank: " << temp->rank << ")" << std::endl;

        // Si el nodo tiene hijos, llamamos recursivamente para imprimirlos
        if (temp->hijo != nullptr) {
            visualizar_heap(temp->hijo, nivel + 1);
        }

        temp = temp->derecha;
    } while (temp != nodo);
}

template <typename T>
void FHeap<T>::visualizar() {
    std::cout << "Visualización del Fibonacci Heap:" << std::endl;
    if (rootlist == nullptr) {
        std::cout << "Heap vacío." << std::endl;
        return;
    }
    visualizar_heap(rootlist, 0);
}


//OTRAS FUNCIONES

template<typename T>
FHeap<T> FHeap<T>::Union(FHeap<T> FH2) {
    if (FH2.rootlist != nullptr) {
        if (this->rootlist == nullptr) {
            this->rootlist = FH2.rootlist;
            this->ptr_min = FH2.ptr_min;
            this->cont = FH2.cont;
        } else {
            this->ptr_min = this->valor_min() <= FH2.valor_min() ? this->ptr_min : FH2.ptr_min; //Obtenemos el nodo con menor valor

            //Empezamos con la unión
            auto temp = rootlist->izquierda;
            this->rootlist->izquierda->derecha = FH2.rootlist;
            this->rootlist->izquierda = FH2.rootlist->izquierda;
            FH2.rootlist->izquierda->derecha = this->rootlist;
            FH2.rootlist->izquierda = temp;

            this->cont += FH2.cont;
        }

        //Eliminamos FH2
        FH2.rootlist = nullptr;
        FH2.ptr_min = nullptr;
        FH2.cont = 0;
    }
    return *this;
}


int main() {
    FHeap<int> heap;

    // PRUEBA 1: Insertar elementos en el heap
    cout<<"\nPRUEBA 1: Insertar elementos en el heap"<<endl;
    heap.insert(10);
    heap.insert(20);
    heap.insert(5);
    heap.insert(15);
    heap.insert(30);
    heap.visualizar();
    cout<<"- Valor mínimo actual: "<<heap.valor_min()<<endl;
    cout<<"- Tamaño actual del Fib Heap: "<<heap.size()<<endl;
    
    // PRUEBA 2: Eliminar un nodo ingresando la clave (15)
    cout<<"\n PRUEBA 2: Eliminar un nodo ingresando la clave (15)"<<endl;
    heap.deleteByKey(15);
    heap.visualizar();
    cout<<"- Valor mínimo actual: "<<heap.valor_min()<<endl;
    cout<<"- Tamaño actual del Fib Heap: "<<heap.size()<<endl;

    // PRUEBA 3: Disminuir el valor de un nodo ingresando la clave (30 -> 2)
    cout<<"\n PRUEBA 3: Disminuir el valor de un nodo ingresando la clave (30 -> 2)"<<endl;
    heap.decreaseByKey(30, 2);
    heap.visualizar();
    cout<<"- Valor mínimo actual: "<<heap.valor_min()<<endl;
    cout<<"- Tamaño actual del Fib Heap: "<<heap.size()<<endl;

    // PRUEBA 4: Extraer el valor mínimo
    cout<<"\n PRUEBA4: Extraer el valor mínimo"<<endl;
    heap.extractMin();
    heap.visualizar();
    cout<<"- Valor mínimo actual: "<<heap.valor_min()<<endl;
    cout<<"- Tamaño actual del Fib Heap: "<<heap.size()<<endl;

    //PRUEBA 5: Unión de Fib Heaps
    cout<<"\n PRUEBA 5: Unión de Fib Heaps"<<endl;
    FHeap<int> heap2;
    heap2.insert(25);
    heap2.insert(7);

    cout << "\nPrimer heap:" << endl;
    heap.visualizar();

    cout << "\nSegundo heap:" << endl;
    heap2.visualizar();

    cout << "\nUniendo el segundo heap al primero" << endl;
    heap = heap.Union(heap2);
    cout << "Visualizar heaps unidos (se actualizó el primer heap):" << endl;
    heap.visualizar();
    return 0;
}
