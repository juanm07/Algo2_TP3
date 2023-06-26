#include "lollapatuza.h"

 Lollapatuza Lollapatuza::crearLolla(const map <IdPuesto, Puesto> &puestos, const vector<Persona> &personas) {
     _personas = personas;
     _puestos = puestos;

     //Se declara como mayor consumidora a una persona, no importa quién sea.
     _mayorConsumidora = personas[0];

     //Como los precios son los mismos no importa el puesto.
     auto it = puestos.begin();
     Puesto unPuesto = it->second;
     _precios = unPuesto.obtenerPrecios(); // Antes estabamos accediendo a la variable interna 'precios' del módulo Puesto, lo que rompia el encapsulamiento

     int i = 0;
     while (i < personas.size()) {
         //Defino a todas las personas en puestos hackeables.
         _puestosHackeables.insert(make_pair(personas[i], map<Producto, map<IdPuesto, Puesto*>>()));

         //Defino a todas las personas en personasEnGasto junto con el indice.
         auto itPersona = _personasEnGasto.insert(make_pair(personas[i], i));

         _gastosPorPersona.encolar(make_pair(0, itPersona)); // Armo el MaxHeap

         i++;
     }
 }

void Lollapatuza::registrarCompra(IdPuesto id, Persona a, Producto item, Cantidad cantidad) {
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
    *puestoDeVenta.registrarVenta(a, item, cantidad);

    Dinero gasto = _precios[item] * cantidad;

    _gastosPorPersona[_personasEnGasto[a]].first += aplicarDescuento(gasto, *puestoDeVenta.obtenerDescuento(item, cantidad));

    _gastosPorPersona.heapifyDown(_personasEnGasto[a]);

    _mayorConsumidora = *((_gastosPorPersona.proximo()).second); // Actualizo mayor consumidor

    if(*puestoDeVenta.obtenerDescuento(item, cantidad) == 0){ // Veo si hay que agregar el puesto a PuestosHackeables
        if(_puestosHackeables[a].count(item) == 1){
            if(_puestosHackeables[a][item].count(id) == 0){
                _puestosHackeables[a][item].insert({id, puestoDeVenta});
            }
        }else{
            _puestosHackeables[a].insert({id, map<id, puestoDeVenta>});

        }
    }
}
  
void Lollapatuza::hackear(Persona a, Producto item) {
    Nat cantidadNueva = _consumosPorPersona[a][item] -1;
    _consumosPorPersona[a][item] = cantidadNueva; //Actualizo consumos
    int gastoViejo = _gastosPorPersona.indexar(_personasEnGasto[a]).first;
    (_gastosPorPersona.indexar(_personasEnGasto[a])).first = gastoViejo - _precios[item];

}

Dinero Lollapatuza::gastoTotal(Persona a){
    Indice i = _personasEnGasto[a];
    return _gastosPorPersona[i].first;
}

Persona Lollapatuza::personaQueMasGasto(){
    return _mayorConsumidora;
}

IdPuesto Lollapatuza::menorStock(Producto item){ //dudoso
    IdPuesto res = nullptr;
    auto itPuesto = _puestos.begin();
    while(itPuesto != _puestos.end()){
        if((itPuesto->second).estaEnElMenu(item)){
            res = itPuesto->first;
            break;
        }
        itPuesto++;
    }

    while(itPuesto != _puestos.end()){
        if((itPuesto->second).estaEnElMenu(item)){
            if((itPuesto->second).obtenerStock(item) < res.obtenerStock(item)){
                res = itPuesto->first;
            }else {
                if (((itPuesto->second).obtenerStock(item) == res.obtenerStock(item)) && (res > itPuesto->first)) { //copie el algoritmo pero no creo que este bien. A obtenerStock(res, item) le estamos pasando un puestoId en vez de un puesto
                    res = itPuesto->first;
                }
            }
        }
        itPuesto++;
    }
    return res;
}

vector<Persona> Lollapatuza::obtenerPersonas(){
    return _personas;
}

map<IdPuesto, Puesto> Lollapatuza::obtenerPuesto(){
    return _puestos;
}
