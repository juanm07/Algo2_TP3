#include "lollapatuza.h"

 lollapatuza lollapatuza::crearLolla(const map <IdPuesto, Puesto> &puestos, const vector<Persona> &personas) {
    _personas = personas;
    _puestos = puestos;

    //Se declara como mayor consumidora a una persona, no importa quién sea.
    _mayorConsumidora = personas[0];

    //Como los precios son los mismos no importa el puesto.
    auto it = puestos.begin();
    Puesto unPuesto = it->second;
    _precios = unPuesto.precios;

    int i = 0;
    while (i < personas.size()) {
        //Defino a todas las personas en puestos hackeables.
        _puestosHackeables.insert({personas[i], info});

        //Defino a todas las personas en personasPorGasto junto con el indice.
        _personasPorGasto.insert({personas[i], i});


        // FALTA ARMAR EL MAXHEAP


        i++;
    }

void Lollapatuza::registrarCompra(Lollapatuza lolla, IdPuesto id, Persona a, Producto item, Cantidad cantidad) {
    if(_consumosPorPersona.count(a) == 0){
        _consumosPorPersona.insert({a, _consumosPorPersona[a].insert({item,cantidad})});
    }else{
        if(_consumosPorPersona[a].count(item) == 0){
            _consumosPorPersona[a].insert({item,cantidad});
        }else{
            cantAnterior = _consumosPorPersona[a][item];
            _consumosPorPersona[a][item].insert(cantAnterior + cantidad);
        }
    }

    Puesto* puestoDeVenta;
    puestoDeVenta = &_puestos[id];
    registrarVenta(*puestoDeVenta, a, item, cantidad);

    gasto = _precios[item] * cantidad;

    _gastosPorPersona[_personasEnGasto[a]].first += aplicarDescuento(gasto, obtenerDescuento(*puestoDeVenta, item, cantidad));

    heapify(_gastosPorPersona, _personasEnGasto[a]);

    _mayorConsumidora = *(proximo(_gastosPorPersona).second);

    if(obtenerDescuento(*puestoDeVenta, item, cantidad)){
        if(_puestosHackeables[a].count(item) == 1){
            if(_puestosHackeables[a][item].count(id) == 0){
                _puestosHackeables[a][item].insert({id, puestoDeVenta});
            }
        }else{
            _puestosHackeables[a].insert({id, map<id, puestoDeVenta>});

        }

    }
}
