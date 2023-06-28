#include "lollapatuza.h"
#include "puesto.h"
#include "puesto.cpp"

lollapatuza lollapatuza::crearLolla(const map <IdPuesto, Puesto> &puestos, const vector<Persona> &personas) {
    _personas = personas; //porque no toma los iguales
    _puestos = puestos;

    //Se declara como mayor consumidora a una persona, no importa quién sea.
    _mayorConsumidora = personas[0];

    //Como los precios son los mismos no importa el puesto.
    auto it = puestos.begin();
    Puesto unPuesto = it->second;
    _precios = unPuesto.obtenerPrecios(); //uso la funcion de puesto.cpp

    int i = 0;
    while (i < personas.size()) {
        //Defino a todas las personas en puestos hackeables.
        _puestosHackeables.insert({personas[i], info}); //que es info

        //Defino a todas las personas en personasPorGasto junto con el indice.
        _personasEnGasto.insert({personas[i], i}); //es personas en gasto???


        // FALTA ARMAR EL MAXHEAP


        i++;
    }
}

void lollapatuza::registrarCompra(IdPuesto id, Persona a, Producto item, Cantidad cantidad) {

    if(_consumosPorPersona.count(a) == 0){
        _consumosPorPersona.insert({a, _consumosPorPersona[a].insert({item,cantidad})}); //como insertamos en un map
    }else{void lollapatuza::hackear(lollapatuza lolla, Persona a, Producto item) {
        Nat cantidadNueva = lolla._consumosPorPersona[a][item] -1;
        lolla._consumosPorPersona[a][item] = cantidadNueva; //Actualizo consumos
        int gastoViejo = _gastosPorPersona.indexar(lolla._personasEnGasto[a]).first;
        (lolla._gastosPorPersona.indexar(lolla._personasEnGasto[a])).first = gastoViejo - lolla._precios[item];

    }
    if(_consumosPorPersona[a].count(item) == 0){
        _consumosPorPersona[a].insert({item,cantidad});
    }else{
        Cantidad cantAnterior = _consumosPorPersona[a][item];
        _consumosPorPersona[a][item].insert(cantAnterior + cantidad);
    }
    }

    Puesto* puestoDeVenta;
    puestoDeVenta = &_puestos[id];
    //registrarVenta(*puestoDeVenta, a, item, cantidad);
    //registrar compra o registar venta???
    *puestoDeVenta.registrarVenta(a,item,cantidad);

    Dinero gasto = _precios[item] * cantidad;
     _gastosPorPersona[_personasEnGasto[a]].first += aplicarDescuento(gasto,*puestoDeVenta->obtenerDescuento(item, cantidad));

     heapify(_gastosPorPersona, _personasEnGasto[a]);

     _mayorConsumidora = *(proximo(_gastosPorPersona).second);

     if(*puestoDeVenta->obtenerDescuento(item, cantidad)){ // obtener descuento no toma un puesto de venta)
         if(_puestosHackeables[a].count(item) == 1){
             if(_puestosHackeables[a][item].count(id) == 0) {
                 _puestosHackeables[a][item].insert({id, puestoDeVenta});
             }
         }else{
             _puestosHackeables[a].insert({id, map<id, puestoDeVenta>});

         }

     }
}

void lollapatuza::hackear(Persona a, Producto item) {
    Nat cantidadNueva = _consumosPorPersona[a][item] -1;
    _consumosPorPersona[a][item] = cantidadNueva; //Actualizo consumos
    Dinero gastoViejo = _gastosPorPersona.indexar(_personasEnGasto[a]).first;
    (_gastosPorPersona.indexar(_personasEnGasto[a])).first = gastoViejo - _precios[item];

}

Nat lollapatuza::gastoTotal(Persona a){
    int indicePersona = _personasEnGasto[a];
    return (lolla._gastosPorPersona[indicePersona]).first;
}

Persona lollapatuza::personaQueMasGasto(lollapatuza lolla) {
    return lolla._mayorConsumidora;
}

IdPuesto lollapatuza::menorStock(Producto item){ //dudoso
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



}

set<Persona> lollapatuza::obtenerPersonas(lollapatuza lolla) {
    return _personas;
}

map<IdPuesto ,aed2_Puesto> lollapatuza::obtenerPuesto(lollapatuza lolla) {
    return _puestos;
}
