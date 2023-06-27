#ifndef TP3_LOLLAPATUZA_H
#define TP3_LOLLAPATUZA_H


#include<iterator>
#include <set>
#include "tipos.h"
#include <queue>
#include "puesto.h"
#include <tuple>

class lollapatuza {
public:
    lollapatuza crearLolla(const map<IdPuesto, aed2_Puesto>& puestos, const set<Persona>& personas);

    void registrarCompra(lollapatuza lolla, IdPuesto id, Persona a, Producto item, Cantidad cantidad);

    void hackear(lollapatuza lolla, Persona a, Producto item);

    Nat gastoTotal(lollapatuza lolla, Persona a);

    Persona personaQueMasGasto(lollapatuza lolla);

    IdPuesto menorStock(lollapatuza lolla, Producto item);

    set<Persona> obtenerPersonas(lollapatuza lolla);

    map<IdPuesto, aed2_Puesto> obtenerPuesto(lollapatuza lolla);

private:
    set<Persona> _personas;
    map<IdPuesto, aed2_Puesto> _puestos;
    Persona _mayorConsumidora;
    map<Persona, map<Producto, Cantidad>> _consumosPorPersona;
    map<Persona, map<Producto, map<IdPuesto, Puesto*>>> _puestosHackeables;
    map<Producto, Nat> _precios;
    queue<tuple<Nat,iterator>>  _gastosPorPersona;
    map<Persona, Indice> _personasEnGasto;
};


#endif //TP3_LOLLAPATUZA_H
