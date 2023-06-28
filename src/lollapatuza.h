#ifndef TP3_LOLLAPATUZA_H
#define TP3_LOLLAPATUZA_H


#include<iterator>
#include <vector>
#include "tipos.h"
#include "puesto.cpp"
#include "ColaPrior.hpp"

class Lollapatuza {
public:

    Lollapatuza();

    Lollapatuza(const map<IdPuesto, Puesto>& puestos, const vector<Persona>& personas);

    void registrarCompra(IdPuesto id, Persona a, Producto item, Cantidad cantidad);

    void hackear(Persona a, Producto item);

    Dinero gastoTotal(Persona a) const;

    Persona personaQueMasGasto() const;

    IdPuesto menorStock(Producto item)const;

    vector<Persona> obtenerPersonas()const;

    map<IdPuesto, Puesto> obtenerPuesto()const;

private:
    vector<Persona> _personas;
    map<IdPuesto, Puesto> _puestos;
    Persona _mayorConsumidora;
    map<Persona, map<Producto, Cantidad>> _consumosPorPersona;
    map<Persona, map<Producto, map<IdPuesto, Puesto*>>> _puestosHackeables;
    map<Producto, Dinero> _precios;
    ColaPrior<pair<Dinero, map<Persona,Indice>::iterator>>  _gastosPorPersona;
    map<Persona, Indice> _personasEnGasto;
};


#endif //TP3_LOLLAPATUZA_H
