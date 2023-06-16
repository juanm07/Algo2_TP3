#include "lollapatuza.h"

lollapatuza lollapatuza::crearLolla(const map <IdPuesto, aed2_Puesto> &puestos, const set <Persona> &personas) {
    lollapatuza lola;
    lola._personas =
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
