#include "lollapatuza.h"

Lollapatuza::Lollapatuza():  _personas(vector<Persona>()), _puestos(map<IdPuesto, Puesto> ()),_mayorConsumidora(0), _consumosPorPersona(map<Persona, map<Producto, Cantidad>> ()),
                             _puestosHackeables(map<Persona, map<Producto, map<IdPuesto, Puesto*>>>()), _precios(map<Producto, Dinero> ()), _gastosPorPersona(ColaPrior<pair<Dinero, map<Persona,Indice>::iterator>>()),
                             _personasEnGasto(map<Persona, Indice> ()) {}

Lollapatuza::Lollapatuza(const map <IdPuesto, Puesto> &puestos, const vector<Persona> &personas){ // O(A * log(A))
    _personas = personas;
    _puestos = puestos;

    //Se declara como mayor consumidora a una persona, no importa quién sea.
    _mayorConsumidora = personas[0];

    //Como los precios son los mismos no importa el puesto.
    auto it = puestos.begin();
    Puesto unPuesto = it->second;
    _precios = unPuesto.obtenerPrecios(); // O(1)
    int i = 0;
    while (i < personas.size()) { //O(A * log(A))
        //Defino a todas las personas en puestos hackeables.
        _puestosHackeables.insert(make_pair(personas[i], map<Producto, map<IdPuesto, Puesto*>>())); // es un puntero a puesto

        //Defino a todas las personas en personasEnGasto junto con el indice.
        auto itPersona = _personasEnGasto.insert(make_pair(personas[i], i)).first; //el iterador apunta a la persona y su indice en el vector personas.

        _gastosPorPersona.encolar(make_pair(0, itPersona)); // Armo el MaxHeap

        i++;
    }
}

void Lollapatuza::registrarCompra(IdPuesto id, Persona a, Producto item, Cantidad cantidad) { //O(log(A) + log(I) + log(P))
    if(_consumosPorPersona[a].count(item) == 0){ //Chequeo si ya consumio ese item. O(log(A) + log(I))
        _consumosPorPersona[a][item] = cantidad;
    }else{
        Cantidad cantAnterior = _consumosPorPersona[a][item];
        _consumosPorPersona[a][item] = cantAnterior + cantidad;
    }

    Puesto *puestoDeVenta; //creo el puntero de tipo Puesto
    puestoDeVenta = &_puestos[id]; // Me guardo la referencia al puesto. O(log(P))
    puestoDeVenta->registrarVenta(a, item, cantidad); // O(log(A) + log(I) + log(P))

    Dinero gasto = _precios[item] * cantidad; // O(log(I))

    Dinero gastoConDesc = puestoDeVenta->aplicarDescuento(gasto, puestoDeVenta->obtenerDescuento(item, cantidad)); //Aplico descuento si corresponde
    Dinero nuevoGasto = _gastosPorPersona.indexar(_personasEnGasto[a]).first + gastoConDesc; //Nuevo gasto de la persona
    auto itPersona = _gastosPorPersona.indexar(_personasEnGasto[a]).second; //Me guardo el iterador que esta en el maxHeap
    pair<Dinero, map<Persona,Indice>::iterator>aInsertar = make_pair(nuevoGasto,itPersona); //Me armo la tupla que va a reemplazar a la vieja

    _gastosPorPersona.reemplazar(_personasEnGasto[a], aInsertar); //Reemplazo la tupla vieja por la nueva con el gasto actualizado
    _gastosPorPersona.heapifyUp(_personasEnGasto[a]); //Como el gasto aumento tengo que ver si la tupla sube o si se queda ahi. O(log(A))

    auto itMayorConsumidor = (_gastosPorPersona.proximo()).second; // Actualizo mayor consumidor
    _mayorConsumidora = itMayorConsumidor->first;

    if(puestoDeVenta->obtenerDescuento(item, cantidad) == 0){ // Veo si hay que agregar el puesto a PuestosHackeables. O(log(A) + log(I) + log(P))
        if(_puestosHackeables[a].count(item) == 1){ //Si la persona ya compro ese item

            if(_puestosHackeables[a][item].count(id) == 0){ //Veo si la persona compro ese item en ese puesto
                _puestosHackeables[a][item].insert(make_pair(id, puestoDeVenta));
            }

        }else{
            map<IdPuesto, Puesto*> infoPuesto = map<IdPuesto, Puesto*>();
            infoPuesto.insert(make_pair(id, puestoDeVenta));
            _puestosHackeables[a].insert(make_pair(item, infoPuesto));
        }
    }
}

void Lollapatuza::hackear(Persona a, Producto item) { //O(log(A) + log(I)) y en caso de que el puesto deje de ser hackeable O(log(A) + log(I) + log(P))
    Nat cantidadNueva = _consumosPorPersona[a][item] -1;
    _consumosPorPersona[a][item] = cantidadNueva; //Actualizo consumos

    Dinero gastoViejo = _gastosPorPersona.indexar(_personasEnGasto[a]).first; //Me guardo el gasto viejo
    Dinero nuevoGasto = gastoViejo - _precios[item]; //Le resto al gasto viejo el precio del item hackeado
    auto itPersona = _gastosPorPersona.indexar(_personasEnGasto[a]).second; //Guardo el iterador del maxHeap
    pair<Dinero, map<Persona,Indice>::iterator>aInsertar = make_pair(nuevoGasto,itPersona); //Me armo la tupla que va a reemplazar a la vieja
    _gastosPorPersona.reemplazar(_personasEnGasto[a], aInsertar); //Reemplazo la tupla vieja por la nueva de arriba
    _gastosPorPersona.heapifyDown(_personasEnGasto[a]); //Hago heapify para abajo porque disminui el valor. O(log(A))

    _mayorConsumidora = (_gastosPorPersona.proximo().second)->first;

    auto it = _puestosHackeables[a][item].begin(); //Guardo iterador al puesto de menor id, hackeado
    Puesto* puesto = it->second;
    puesto->hackeoPuesto(a,item); //O(log(A) + log(I))

    if (puesto->obtenerCantVendidaSinDesc(a, item) == 0) { //Veo si el puesto deja de ser hackeable. O(log(P))
        _puestosHackeables[a][item].erase(it);
    }

}

Dinero Lollapatuza::gastoTotal(Persona a) const{ //O(log(A))
    Indice i = _personasEnGasto.at(a); // O(log(A))
    return _gastosPorPersona.indexar(i).first; //O(1)
}

Persona Lollapatuza::personaQueMasGasto() const{ // O(1)
    return _mayorConsumidora;
}

IdPuesto Lollapatuza::menorStock(Producto item) const{ // O(log(I) * P)
    IdPuesto res;
    auto itPuesto = _puestos.begin();
    while(itPuesto != _puestos.end()){ //Loop para guardarme el primer puesto con ese item
        if((itPuesto->second).estaEnElMenu(item)){
            res = itPuesto->first;
            break;
        }
        itPuesto++;
    }

    while(itPuesto != _puestos.end()){
        if((itPuesto->second).estaEnElMenu(item)){
            if((itPuesto->second).obtenerStock(item) < _puestos.at(res).obtenerStock(item)){ //En nuestra especificación a obtenerStock se le pasaba un puestoId, ahora le estamos pasando un Puesto
                res = itPuesto->first;
            }else {
                if (((itPuesto->second).obtenerStock(item) == _puestos.at(res).obtenerStock(item)) && (res > itPuesto->first)) {
                    res = itPuesto->first;
                }
            }
        }
        itPuesto++;
    }
    return res;
}

const vector<Persona>& Lollapatuza::obtenerPersonas() const{ //O(1)
    return _personas;
}

const  map<IdPuesto, Puesto>& Lollapatuza::obtenerPuestos() const{ // O(1)
    return _puestos;
}
