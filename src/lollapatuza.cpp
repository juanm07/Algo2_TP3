#include "lollapatuza.h"

 Lollapatuza Lollapatuza::crearLolla(const map <IdPuesto, Puesto> &puestos, const vector<Persona> &personas) {
     _personas = personas;
     _puestos = puestos;

     //Se declara como mayor consumidora a una persona, no importa quién sea.
     _mayorConsumidora = personas[0];

     //Como los precios son los mismos no importa el puesto.
     auto it = puestos.begin();
     Puesto unPuesto = it->second;
     _precios = unPuesto._precios;

     int i = 0;
     while (i < personas.size()) {
         //Defino a todas las personas en puestos hackeables.
         _puestosHackeables.insert({personas[i], map<Producto, map<IdPuesto, *Puesto>>});

         //Defino a todas las personas en personasEnGasto junto con el indice.
         _personasEnGasto.insert({personas[i], i});

         auto itPersona = [personas[i]].begin();
         _gastosPorPersona.encolar(pair<0, itPersona>); // Armo el MaxHeap

         i++;
     }
 }

void Lollapatuza::registrarCompra(Lollapatuza lolla, IdPuesto id, Persona a, Producto item, Cantidad cantidad) {
    if(_consumosPorPersona.count(a) == 0){ //actualizo consumos
        _consumosPorPersona.insert({a, _consumosPorPersona[a].insert({item,cantidad})});
    }else{
        if(_consumosPorPersona[a].count(item) == 0){
            _consumosPorPersona[a].insert({item,cantidad});
        }else{
            Cantidad cantAnterior = _consumosPorPersona[a][item];
            _consumosPorPersona[a][item].insert(cantAnterior + cantidad);
        }
    }

    Puesto* puestoDeVenta;
    puestoDeVenta = &_puestos[id]; // creamos un puntero al puesto
    registrarVenta(*puestoDeVenta, a, item, cantidad);

    Dinero gasto = _precios[item] * cantidad;

    _gastosPorPersona[_personasEnGasto[a]].first += aplicarDescuento(gasto, obtenerDescuento(*puestoDeVenta, item, cantidad));

    _gastosPorPersona.heapifyDown(_personasEnGasto[a]);

    _mayorConsumidora = *((_gastosPorPersona.proximo()).second); // Actualizo mayor consumidor

    if(obtenerDescuento(*puestoDeVenta, item, cantidad) == 0){ // Veo si hay que agregar el puesto a PuestosHackeables
        if(_puestosHackeables[a].count(item) == 1){
            if(_puestosHackeables[a][item].count(id) == 0){
                _puestosHackeables[a][item].insert({id, puestoDeVenta});
            }
        }else{
            _puestosHackeables[a].insert({id, map<id, puestoDeVenta>});

        }
    }
}
  
void Lollapatuza::hackear(Lollapatuza lolla, Persona a, Producto item) {
    Nat cantidadNueva = lolla._consumosPorPersona[a][item] -1;
    lolla._consumosPorPersona[a][item] = cantidadNueva; //Actualizo consumos
    int gastoViejo = lolla._gastosPorPersona.indexar(lolla._personasEnGasto[a]).first;
    (lolla._gastosPorPersona.indexar(lolla._personasEnGasto[a])).first = gastoViejo - lolla._precios[item];

}

Dinero Lollapatuza::gastoTotal(Lollapatuza lolla, Persona a){
    Indice i = lolla._personasEnGasto[a];
    return lolla._gastosPorPersona[i].first;
}

Persona Lollapatuza::personaQueMasGasto(Lollapatuza lolla){
    return lolla._mayorConsumidora;
}

IdPuesto Lollapatuza::menorStock(Lollapatuza lolla, Producto item){ //dudoso
    IdPuesto res = nullptr;
    auto itPuesto = lolla._puestos.begin();
    while(itPuesto->first != nullptr){
        if(estaEnElMenu(itPuesto->second, item)){
            res = itPuesto->first;
            break;
        }
        itPuesto++;
    }

    while(itPuesto->first != nullptr){
        if(estaEnElMenu(itPuesto->second, item)){
            if(obtenerStock(itPuesto->second, item) < obtenerStock(res, item)){
                res = itPuesto->first;
            }else {
                if ((obtenerStock(itPuesto->second, item) == obtenerStock(res, item)) && (res > itPuesto->first)) { //copie el algoritmo pero no creo que este bien. A obtenerStock(res, item) le estamos pasando un puestoId en vez de un puesto
                    res = itPuesto->first;
                }
            }
        }
        itPuesto++;
    }
    return res;
}

set<Persona> Lollapatuza::obtenerPersonas(Lollapatuza lolla){
    return lolla._personas;
}

map<IdPuesto, Puesto> Lollapatuza::obtenerPuesto(Lollapatuza lolla){
    return lolla._puestos;
}
