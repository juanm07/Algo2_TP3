#ifndef TP3_LOLLAPATUZA_H
#define TP3_LOLLAPATUZA_H


#include<iterator>
#include <vector>
#include "tipos.h"
#include "puesto.h"
#include "ColaPrior.hpp"

class Lollapatuza {
public:
    Lollapatuza crearLolla(const map<IdPuesto, Puesto>& puestos, const vector<Persona>& personas);

    void registrarCompra(Lollapatuza lolla, IdPuesto id, Persona a, Producto item, Cantidad cantidad);

    void hackear(Lollapatuza lolla, Persona a, Producto item);

    Dinero gastoTotal(Lollapatuza lolla, Persona a);

    Persona personaQueMasGasto(Lollapatuza lolla);

    IdPuesto menorStock(Lollapatuza lolla, Producto item);

    set<Persona> obtenerPersonas(Lollapatuza lolla);

    map<IdPuesto, Puesto> obtenerPuesto(Lollapatuza lolla);

private:
    vector<Persona> _personas;
    map<IdPuesto, Puesto> _puestos;
    Persona _mayorConsumidora;
    map<Persona, map<Producto, Cantidad>> _consumosPorPersona;
    map<Persona, map<Producto, map<IdPuesto, *Puesto>>> _puestosHackeables;
    map<Producto, Nat> _precios;
    ColaPrior<<Nat, iterator>>  _gastosPorPersona;
    map<Persona, Indice> _personasEnGasto;
};


#endif //TP3_LOLLAPATUZA_H
