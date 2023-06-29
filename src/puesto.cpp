#include "puesto.h"

Puesto::Puesto(): _stock(map<Producto, Nat>()),
                  _vectorDescuentos(map<Producto, vector<Descuento>>()),
                  _gastoPorPersona(map<Persona, Dinero>()),
                  _precios(map<Producto, Dinero>()),
                  _ventas(map<Persona, map<Producto, pair<Cantidad, Cantidad>>>()){}

Puesto::Puesto(const Menu& precios,const Stock& stock, const Promociones& descuentos){
   _stock = stock;
    _precios = precios;

    auto itItems = descuentos.begin();
    map<Producto, vector<Descuento>> diccVectores;
    while(itItems != descuentos.end()){
        vector<Descuento> vectorDesc;
        auto itCants = (itItems->second).begin(); //Voy a recorrer el diccLog(cant,desc) inorder
        int i = 0;
        while (i < itCants->first) {
            vectorDesc.push_back(0); // No hay descuento
            i++;
        }
        while (itCants != (itItems->second).end()) {
            Descuento desc = itCants->second; // Me guardo el descuento a insertar
            itCants++; // Avanzo para poner cota con la próxima cantidad
            if (itCants != (itItems->second).end()) {
                while (i < itCants->first) {
                    vectorDesc.push_back(desc);
                    i++;
                }
            } else {
                vectorDesc.push_back(desc); // Si llegué a la última cantidad
            }
        }
        diccVectores.insert(make_pair(itItems->first, vectorDesc));
        itItems++;
    }
    _vectorDescuentos = diccVectores;
}

Cantidad Puesto::obtenerStock(Producto item) const{
    return _stock.at(item);
}

bool Puesto::estaEnElMenu(Producto item) const {
    return _stock.count(item) == 1;
}

map<Producto, Dinero> Puesto::obtenerPrecios() const{
    return _precios;
}

Descuento Puesto::obtenerDescuento(Producto item, Cantidad cantidad) const{
    Descuento res = 0;
    if(_vectorDescuentos.count(item) == 1){
        int longDesc = (_vectorDescuentos.at(item)).size();
        if(cantidad > _vectorDescuentos.at(item)[longDesc - 1]){
            res = _vectorDescuentos.at(item)[longDesc - 1];
        }else{
            res = _vectorDescuentos.at(item)[cantidad];
        }
    }
    return res;
}

Dinero Puesto::aplicarDescuento (Dinero dinero, Descuento desc) {
    return dinero * (100-desc)/100;
}

Dinero Puesto::obtenerGasto( Persona a) const{
    if(_gastoPorPersona.count(a) == 0){
        return 0;
    }else{
        return _gastoPorPersona.at(a);
    }
}

map<Producto , pair< Cantidad, Cantidad >> Puesto::obtenerVentas(Persona a) const{
    map<Producto , pair< Cantidad, Cantidad >> res;
    if(_ventas.count(a) == 1){
        res = _ventas[a];
    }
    return res;
}

Cantidad Puesto::obtenerCantVendidaSinDesc(Persona a, Producto item) const{
    return _ventas[a][item].second;
}

void Puesto::registrarVenta(Persona a, Producto item, Cantidad cant){
    Cantidad nuevoStock = _stock[item] - cant;
    _stock[item] = nuevoStock; // Actualizo stock

    Dinero gastoVentaSinDesc = _precios[item] * cant;
    Dinero gastoVentaConDesc = aplicarDescuento(gastoVentaSinDesc, obtenerDescuento(item, cant));
    Dinero nuevoGastoTotalPersona = _gastoPorPersona[a] + gastoVentaConDesc;
     _gastoPorPersona[a] = nuevoGastoTotalPersona; // Actualizo el gasto de la persona

    if(_ventas[a].count(item) == 1){ // Actualizo ventas
        pair<Cantidad, Cantidad> cantAnterior = _ventas[a][item];
        if(obtenerDescuento( item, cant) == 0){
            _ventas[a][item] = make_pair(cantAnterior.first, cantAnterior.second + cant);
        }else{
            _ventas[a][item] = make_pair(cantAnterior.first + cant, cantAnterior.second);
        }
    }else{
        if(obtenerDescuento( item, cant) == 0){
            _ventas[a].insert({item, make_pair(0, cant)});
        }else{
            _ventas[a].insert({item, make_pair(cant, 0)});
        }
    }
}

void Puesto::hackeoPuesto(Persona a, Producto item){
   Nat stockAntiguo = _stock[item];
    _stock[item] =  stockAntiguo + 1; // Actualizo stock

    Dinero gastoHackeado = _gastoPorPersona[a] - _precios[item];
    _gastoPorPersona[a] =  gastoHackeado; // Actualizo gasto

    Cantidad cantidadVendidaHackeada = _ventas[a][item].second - 1;
    Cantidad cantidadVendidaSinHackear = _ventas[a][item].first;
    _ventas[a][item] = make_pair(cantidadVendidaSinHackear, cantidadVendidaHackeada); // Actualizo ventas
}

