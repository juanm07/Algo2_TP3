#include "puesto.h"

Puesto Puesto::CrearPuesto(const Menu& precios,const Stock& stock, const Promociones& descuentos){
    _stock = stock;
    _precios = precios;
    _gastoPorPersona = map<Persona, Dinero>;
    _ventas = map<Persona, map<Producto, pair<Cantidad, Cantidad>>>;

    auto itItems = descuentos.begin();
    map<Producto, vector<Descuento>> diccVectores;
    while(itItems->first != nullptr){
        vector<Descuento> vectorDesc;
        auto itCants = (itItems->second).begin(); //Voy a recorrer el diccLog(cant,desc) inorder
        int i = 0;
        while(i < itCants->first){
            vectorDesc[i] = 0; // No hay descuento
            i++;
        }
        while(itCants->first != nullptr){
            Descuento desc = itCants->second; // Me guardo el descuento a insertar
            itCants++; // Avanzo para poner cota con la próxima cantidad
            while(i < itCants->first){
                vectorDesc[i] = desc;
                i++;
            }
            if(itCants->first == nullptr){
                vectorDesc[i] = itCants->second; //Si llegue a la última cantidad
            }
        }
        diccVectores.insert({itItems->first, vectorDesc});
        itItems++;
    }
    _vectorDescuentos = diccVectores;
}

Cantidad Puesto::obtenerStock(Puesto puesto,Producto item){
    return puesto._stock[item];
}

bool Puesto::estaEnElMenu(Puesto puesto, Producto item){
    return puesto._stock[item].count == 1;
}

Descuento Puesto::obtenerDescuento(Puesto puesto, Producto item, Cantidad cantidad){
    Descuento res = 0;
    if(puesto._vectorDescuentos.count(item) == 1){
        int longDesc = (puesto._vectorDescuentos[item]).size();
        if(cantidad > puesto._vectorDescuentos[item][longDesc - 1]){
            res = puesto._vectorDescuentos[item][longDesc - 1];
        }else{
            res = puesto._vectorDescuentos[item][cantidad];
        }
    }
    return res;
}

Dinero Puesto::aplicarDescuento (Dinero dinero, Descuento desc){
    return dinero * (100-desc)/100;
}

Dinero Puesto::obtenerGasto(Puesto puesto, Persona a){
    return puesto.gastoPorPersona[a];
}

map<Producto , pair< Cantidad, Cantidad >> Puesto::obtenerVentas(Puesto puesto, Persona a){
    if(puesto._ventas.count(a) == 1){
        return puesto._ventas[a];
    }else{
        return map<Persona, map<Producto, pair<Cantidad, Cantidad>>>;
    }
}

Cantidad Puesto::obtenerCantVendidaSinDesc(Puesto puesto, Persona a, Producto item){
    return puesto._ventas[a][item].second;
}

void Puesto::registrarVenta(Puesto puesto, Persona a, Producto item, Cantidad cantidad){
    Cantidad nuevoStock = puesto._stock[item] - cantidad;
    puesto._stock.insert({item, nuevoStock}); // Actualizo stock

    Dinero gastoVentaSinDesc = puesto._precios[item] * cantidad;
    Dinero gastoVentaConDesc = aplicarDescuento(gastoVentaSinDesc, obtenerDescuento(puesto, item, cantidad));
    Dinero nuevoGastoTotalPersona = puesto._gastoPorPersona[a] + gastoVentaConDesc;
    puesto._gastoPorPersona.insert({a, nuevoGastoTotalPersona}); // Actualizo el gasto de la persona

    if(puesto._ventas[a].count(item) == 1){ // Actualizo ventas
        pair<Cantidad, Cantidad> cantAnterior = puesto._ventas[a][item];
        if(obtenerDescuento(puesto, item, cantidad) == 0){
            puesto._ventas[a].insert({item, pair<cantAnterior.second + cantidad, cantAnterior.first>});
        }else{
            puesto._ventas[a].insert({item, pair<cantAnterior.second, cantAnterior.first + cantidad>});
        }
    }else{
        if(obtenerDescuento(puesto, item, cantidad) == 0){
            puesto._ventas[a].insert({item, pair<cantidad, 0>});
        }else{
            puesto._ventas[a].insert({item, pair<0, cantidad>});
        }
    }
}

void Puesto::hackeoPuesto(Puesto puesto, Persona a, Producto item){
    puesto._stock.insert({item, puesto._stock[item] + 1}); // Actualizo stock

    Dinero gastoHackeado = puesto._gastoPorPersona[a] - puesto._precios[item];
    puesto._gastoPorPersona.insert({a, gastoHackeado}); // Actualizo gasto

    Cantidad cantidadVendidaHackeada = puesto._ventas[a][item].second - 1;
    Cantidad cantidadVendidaSinHackear = puesto._ventas[a][item].first;
    puesto._ventas[a].insert({item, pair<cantidadVendidaSinHackear, cantidadVendidaHackeada>}); // Actualizo ventas
}