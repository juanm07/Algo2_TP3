#ifndef _COLAPRIOR_H_
#define _COLAPRIOR_H_

#include <vector>

using namespace std;


template<class T>
class ColaPrior {
public:
    ColaPrior();

    // Cantidad de elementos en la cola.
    int tam() const;

    // Encola un elemento.
    //
    // Nota: si se implementa usando un vector, es O(n) en peor caso.
    //
    void encolar(const pair<int,map<int,int>::iterator>& elem);

    // Devuelve el elemento de mayor prioridad.
    // Pre: tam() > 0
    const pair<int,map<int,int>::iterator>& proximo() const;

    // Saca el elemento de mayor prioridad.
    //
    // Nota: si se implementa usando un vector, es O(n) en peor caso.
    //
    // Pre: tam() > 0
    void desencolar();

    pair<int,map<int,int>::iterator> indexar(int i) const;
    // Constructor que hace heapify.
    ColaPrior(const vector<pair<int,map<int,int>::iterator>>& elems);

    int padre(int n);

    void heapifyUp(int indice);
    void heapifyDown(int indice);



private:
    vector<pair<int,map<int,int>::iterator>> heap;


};



#endif /*_COLAPRIOR_H_*/
