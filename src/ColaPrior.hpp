#include "ColaPrior.h"

template<class T>
ColaPrior<T>::ColaPrior(): heap() {}

template<class T>
int ColaPrior<T>::tam() const {
	return heap.size();
}

template<class T>
void ColaPrior<T>::encolar(const pair<int,map<int,int>::iterator>& elem) {
	heap.push_back(elem);
    int n = heap.size()-1;
    while (n > 0 && heap[padre(n)].first < heap[n].first){
        swap(heap[padre(n)],heap[n]);
        n = padre(n);
    }
}

template<class T>
const pair<int,map<int,int>::iterator>& ColaPrior<T>::proximo() const {
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
ColaPrior<T>::ColaPrior(const vector<pair<int,map<int,int>::iterator>>& elems) {
	heap = elems;
    int _size = heap.size();
    for(int i = (_size/2)-1;i>=0;i--){
        heapifyDown(i);
    }
}

template<class T>
void ColaPrior<T>::heapifyDown(int indice) {
    int size = heap.size();
    int hijoIzq = 2 * indice + 1;
    int hijoDer = 2 * indice + 2;
    int masGrande = indice;

    if (hijoIzq < size && heap[hijoIzq].first > heap[masGrande].first)
        masGrande = hijoIzq;
    else{
        if (hijoIzq < size && heap[hijoIzq].first == heap[masGrande].first
        && heap[hijoIzq].second->first > heap[masGrande].second->first){
            masGrande = hijoIzq;
        }
    }
    if (hijoDer < size && heap[hijoDer].first > heap[masGrande].first)
        masGrande = hijoDer;
    else{
        if (hijoDer < size && heap[hijoDer].first == heap[masGrande].first
            && heap[hijoDer].second->first > heap[masGrande].second->first){
            masGrande = hijoDer;
        }
    }

    if (masGrande != indice) {
	heap[indice].second->second = masGrande; //Actualizo los indices en el diccionario
        heap[masGrande].second->second = indice; //Actualizo los indices en el diccionario
        std::swap(heap[indice], heap[masGrande]);
        heapifyDown(masGrande);
    }
}

template<class T>
void ColaPrior<T>::heapifyUp(int indice) {
	while(indice > 0){
	int padre = (indice - 1)/2;
	if(heap[indice].first > heap [padre].first){
	    heap[indice].second->second = padre; //Actualizo los indices en el diccionario
	    heap[padre].second->second = indice; //Actualizo los indices en el diccionario
	    swap(heap[indice],heap[padre]);
	}else{
	    if(heap[indice].first == heap[padre].first &&
	       heap[indice].second->first > heap[padre].second->first){
		heap[indice].second->second = padre; //Actualizo los indices en el diccionario
		heap[padre].second->second = indice; //Actualizo los indices en el diccionario
		swap(heap[indice],heap[padre]);
	    }
	    else{
		break;
	    }
	}
	indice = padre;
	}
}

template<class T>
int ColaPrior<T>::padre(int n) {
    int res;
    if (n % 2 == 0){
        res = (n/2)-1;
    }
    else{
        res = (n/2);
    }
    return res;
}

template<class T>
pair<int, map<int, int>::iterator> ColaPrior<T>::indexar(int i) const {
    return heap.at(i);
}

template<class T>
void ColaPrior<T>::reemplazar(int indice, const T& nuevoValor) {
    heap[indice] = nuevoValor;
}
