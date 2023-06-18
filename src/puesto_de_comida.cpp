#include "puesto_de_comida.h"

puesto_de_comida puesto_de_comida::CrearPuesto(map<Producto, int> &precios, map<Producto, int> &stock,
                                               map<Producto, map<int, int>> &descuentos) {
    _stock = stock;
    _precios = precios;
    _gastoPorPersona = std::map<Producto, int>();
    _ventas = std::map<Persona, std::map<Producto, std::pair<int, int>>>();



};

int puesto_de_comida::obtenerStock(puesto_de_comida puesto, Producto item) {
    return _stock.at(item);
};

bool puesto_de_comida ::estaEnElMenu(puesto_de_comida puesto, Producto item) {};

int puesto_de_comida ::obtenerDescuento(puesto_de_comida puesto, Producto item, int cantidad) {};

int puesto_de_comida ::aplicarDescuento(int dinero, int cantidad) {};

int puesto_de_comida ::obtenerGasto(puesto_de_comida puesto, Persona a) {
    return _gastoPorPersona.at(a);
};

map<Producto , pair<int,int>> puesto_de_comida ::obtenerVentas(puesto_de_comida puesto, Persona a) {};

int puesto_de_comida ::obtenerCantVendidaSinDesc(puesto_de_comida puesto, Persona a, Producto item) {
    
};

void puesto_de_comida ::registrarVenta(puesto_de_comida puesto, Persona a, Producto item, int cantidad) {};

void puesto_de_comida ::hackeoPuesto(puesto_de_comida puesto, Persona a, Producto item) {};
