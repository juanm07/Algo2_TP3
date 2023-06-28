#include "puesto.h"

Puesto::Puesto(): _stock(map<Producto, Nat>()),
 _vectorDescuentos(map<Producto, vector<Descuento>>()),
 _gastoPorPersona(map<Persona, Dinero>()),
 _precios(map<Producto, Dinero>()),
 _ventas(map<Persona, map<Producto, pair<Cantidad, Cantidad>>>()){}

Puesto::Puesto(const Menu& precios,const Stock& stock, const Promociones& descuentos){
    _stock = stock;
    _precios = precios;
    map<Persona, Dinero> _gastoPorPersona;
    map<Persona, map<Producto, pair<Cantidad, Cantidad>>> _ventas ;

    auto itItems = descuentos.begin();
    map<Producto, vector<Descuento>> diccVectores;
    while(itItems != descuentos.end()){
        vector<Descuento> vectorDesc;
        auto itCants = (itItems->second).begin(); //Voy a recorrer el diccLog(cant,desc) inorder
        int i = 0;
        while(i < itCants->first){
            vectorDesc.push_back(0); // No hay descuento
            i++;
        }
        while(itCants != (itItems->second).end()){
            Descuento desc = itCants->second; // Me guardo el descuento a insertar
            itCants++; // Avanzo para poner cota con la próxima cantidad
            while(i < itCants->first){
                vectorDesc.push_back(desc);
                i++;
            }
            if(itCants == (itItems->second).end()){
                vectorDesc.push_back(itCants->second); //Si llegue a la última cantidad
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

bool Puesto::estaEnElMenu(Producto item) const{
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

Dinero Puesto::aplicarDescuento (Dinero dinero, Descuento desc){
    return dinero * (100-desc)/100;
}

Dinero Puesto::obtenerGasto( Persona a) const{
    return _gastoPorPersona.at(a);
}

map<Producto , pair< Cantidad, Cantidad >> Puesto::obtenerVentas(Persona a){
    map<Producto , pair< Cantidad, Cantidad >> res;
    if(_ventas.count(a) == 1){
        res = _ventas[a];
    }
    return res;
}

Cantidad Puesto::obtenerCantVendidaSinDesc(Persona a, Producto item){
    return _ventas[a][item].second;
}

void Puesto::registrarVenta(Persona a, Producto item, Cantidad cant){
    Cantidad nuevoStock = _stock[item] - cant;
    _stock.insert(make_pair(item, nuevoStock)); // Actualizo stock

    Dinero gastoVentaSinDesc = _precios[item] * cant;
    Dinero gastoVentaConDesc = aplicarDescuento(gastoVentaSinDesc, obtenerDescuento(item, cant));
    Dinero nuevoGastoTotalPersona = _gastoPorPersona[a] + gastoVentaConDesc;
    _gastoPorPersona.insert({a, nuevoGastoTotalPersona}); // Actualizo el gasto de la persona

    if(_ventas[a].count(item) == 1){ // Actualizo ventas
        pair<Cantidad, Cantidad> cantAnterior = _ventas[a][item];
        if(obtenerDescuento( item, cant) == 0){
            _ventas[a].insert({item, make_pair(cantAnterior.second + cant, cantAnterior.first)});
        }else{
            _ventas[a].insert({item, make_pair(cantAnterior.second, cantAnterior.first + cant)});
        }
    }else{
        if(obtenerDescuento( item, cant) == 0){
            _ventas[a].insert({item, make_pair(cant, 0)});
        }else{
            _ventas[a].insert({item, make_pair(0, cant)});
        }
    }
}

void Puesto::hackeoPuesto(Persona a, Producto item){
    _stock.insert({item, _stock[item] + 1}); // Actualizo stock

    Dinero gastoHackeado = _gastoPorPersona[a] - _precios[item];
    _gastoPorPersona.insert({a, gastoHackeado}); // Actualizo gasto

    Cantidad cantidadVendidaHackeada = _ventas[a][item].second - 1;
    Cantidad cantidadVendidaSinHackear = _ventas[a][item].first;
    _ventas[a].insert({item, make_pair(cantidadVendidaSinHackear, cantidadVendidaHackeada)}); // Actualizo ventas
}
