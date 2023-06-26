#ifndef TP3_PUESTO_H
#define TP3_PUESTO_H

#include<iterator>
#include <vector>
#include "tipos.h"
#include "puesto.cpp"

class Puesto {
public:
    Puesto CrearPuesto(const Menu& precios,const Stock& stock, const Promociones& descuentos);

    static Cantidad obtenerStock(Puesto puesto,Producto item);

    static bool estaEnElMenu(Puesto puesto, Producto item);

    static Descuento obtenerDescuento(Puesto puesto, Producto item, Cantidad cantidad);

    static Dinero aplicarDescuento (Dinero dinero, Descuento desc);

    Dinero obtenerGasto(Puesto puesto, Persona a);

    map<Producto , pair< Cantidad, Cantidad >> obtenerVentas(Puesto puesto, Persona a);

    Cantidad obtenerCantVendidaSinDesc(Puesto puesto, Persona a, Producto item);

    static void registrarVenta(Puesto puesto, Persona a, Producto item, Cantidad cantidad);

    static void hackeoPuesto(Puesto puesto, Persona a, Producto item);

private:

    map<Producto, Nat> _stock;
    map<Producto, vector<Descuento>> _vectorDescuentos;
    map<Persona, Dinero> _gastoPorPersona;
    map<Producto, Dinero> _precios;
    map<Persona, map<Producto, pair<Cantidad, Cantidad>>> _ventas;

};


#endif //TP3_PUESTO_DE_COMIDA_H
