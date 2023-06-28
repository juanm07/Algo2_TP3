#ifndef TP3_PUESTO_H
#define TP3_PUESTO_H

#include<iterator>
#include <vector>
#include "tipos.h"


class Puesto {
public:
    Puesto();

    Puesto(const Menu& precios,const Stock& stock, const Promociones& descuentos);

    Cantidad obtenerStock(Producto item) const;

    bool estaEnElMenu(Producto item) const;

    map<Producto, Dinero> obtenerPrecios() const;

    Descuento obtenerDescuento(Producto item, Cantidad cant) const;

    Dinero aplicarDescuento (Dinero dinero, Descuento desc);

    Dinero obtenerGasto(Persona a) const;

    map<Producto , pair< Cantidad, Cantidad >> obtenerVentas(Persona a) const;

    Cantidad obtenerCantVendidaSinDesc(Persona a, Producto item) const;

    void registrarVenta(Persona a, Producto item, Cantidad cantidad);

    void hackeoPuesto(Persona a, Producto item);

private:

    map<Producto, Nat> _stock;
    map<Producto, vector<Descuento>> _vectorDescuentos;
    map<Persona, Dinero> _gastoPorPersona;
    map<Producto, Dinero> _precios;
    map<Persona, map<Producto, pair<Cantidad, Cantidad>>> _ventas;

};


#endif //TP3_PUESTO_H
