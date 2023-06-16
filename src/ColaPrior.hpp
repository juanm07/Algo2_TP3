#include "ColaPrior.h"
template<class T>
ColaPrior<T>::ColaPrior(): heap() {}

template<class T>
int ColaPrior<T>::tam() const {
	return heap.size();
}

template<class T>
void ColaPrior<T>::encolar(const T& elem) {
	heap.push_back(elem);
    heapifyUp(heap.size()-1);
}

template<class T>
const T& ColaPrior<T>::proximo() const {
	return heap[0];
}

template<class T>
void ColaPrior<T>::desencolar() {
	//Bajo la raiz
    heap[0] = heap.back();
    heap.pop_back();
    heapifyDown(0);
}

template<class T>
ColaPrior<T>::ColaPrior(const vector<T>& elems) {
	heap = elems;
    int _size = heap.size();
    for(int i = (_size/2)-1;i>=0;i--){
        heapifyDown(i);
    }
}

template<class T>
void ColaPrior<T>::heapifyDown(int indice) {
    int size = heap.size();
    while (true) {
        int hijoIzq = 2 * indice + 1;
        int hijoDer = 2 * indice + 2;
        int masGrande = indice;

        if (hijoIzq < size && heap[hijoIzq] > heap[masGrande])
            masGrande = hijoIzq;
        if (hijoDer < size && heap[hijoDer] > heap[masGrande])
            masGrande = hijoDer;

        if (masGrande != indice) {
            std::swap(heap[indice], heap[masGrande]);
            indice = masGrande;
        } else {
            break;
        }
    }
}

template<class T>
void ColaPrior<T>::heapifyUp(int indice) {
    while(indice > 0){
        T padre = (indice - 1)/2;
        if(heap[indice] > heap [padre]){
            swap(heap[indice],heap[padre]);
        }
        else{
            break;
        }
        indice = padre;
    }
}

template<class T>
T ColaPrior<T>::indexar(int i) {
    return heap[i];
}
