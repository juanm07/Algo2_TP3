#ifndef TP3_PUESTO_H
#define TP3_PUESTO_H

#include<iterator>
#include <vector>
#include "tipos.h"


class Puesto {
public:
    Puesto(const Menu& precios,const Stock& stock, const Promociones& descuentos);

    Cantidad obtenerStock(Producto item);

    bool estaEnElMenu(Producto item);

     map<Producto, Dinero> obtenerPrecios();

    Descuento obtenerDescuento(Producto item, Cantidad cant);

    Dinero aplicarDescuento (Dinero dinero, Descuento desc);

    Dinero obtenerGasto(Persona a);

    map<Producto , pair< Cantidad, Cantidad >> obtenerVentas(Persona a);

    Cantidad obtenerCantVendidaSinDesc(Persona a, Producto item);

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
