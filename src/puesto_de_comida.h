#ifndef TP3_PUESTO_DE_COMIDA_H
#define TP3_PUESTO_DE_COMIDA_H

#include<iterator>
#include <set>
#include "tipos.h"

class puesto_de_comida {
public:
    puesto_de_comida CrearPuesto(map<Producto ,int >& precios,map<Producto ,int >& stock, map<Producto , map <int , int>>& descuentos);

    int obtenerStock(puesto_de_comida puesto,Producto item);

    bool estaEnElMenu(puesto_de_comida puesto, Producto item);

    int obtenerDescuento(puesto_de_comida puesto, Producto item, int cantidad);

    int aplicarDescuento (int dinero, int cantidad);

    int obtenerGasto(puesto_de_comida puesto, Persona a);

    map<Producto , pair< int, int >> obtenerVentas(puesto_de_comida puesto, Persona a);

    int obtenerCantVendidaSinDesc(puesto_de_comida puesto, Persona a, Producto item);

    void registrarVenta(puesto_de_comida puesto, Persona a, Producto item, int cantidad);

    void hackeoPuesto(puesto_de_comida puesto, Persona a, Producto item);

private:
    map<Producto ,int > _precios;
    map<Producto ,int > _stock;
    map<Producto , vector<int>> _vectorDescuentos;
    map<Persona ,int> _gastoPorPersona;
    map<Persona,map<Producto,pair<int,int>>> _ventas;

    
};


#endif //TP3_PUESTO_DE_COMIDA_H
