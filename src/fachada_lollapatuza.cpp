#include "fachada_lollapatuza.h"
#include "puesto.h"


FachadaLollapatuza::FachadaLollapatuza(const set<Persona> &personas, const map<IdPuesto, aed2_Puesto> &infoPuestos) {
    //info puestos tiene stock promociones menu
    map<IdPuesto,Puesto> puestos = map<IdPuesto ,Puesto>();
    auto it = infoPuestos.begin();
    while(it!= infoPuestos.end()){
        IdPuesto puestoId = it -> first;
        Stock stock = it->second.stock;
        Menu precios = it->second.menu;
        Promociones descuento = it->second.promociones;
        puestos.insert(make_pair(puestoId,Puesto(precios,stock,descuento)));
        it++;

    }

    _lollapatuza = Lollapatuza(puestos, vector<Persona>(personas.begin(), personas.end()));
}

void FachadaLollapatuza::registrarCompra(Persona persona, Producto producto, Nat cant, IdPuesto idPuesto) {
    _lollapatuza.registrarCompra(idPuesto, persona, producto, cant);
}

void FachadaLollapatuza::hackear(Persona persona, Producto producto) {
    _lollapatuza.hackear(persona, producto);
}

Nat FachadaLollapatuza::gastoTotal(Persona persona) const {
    return _lollapatuza.gastoTotal(persona);
}

Persona FachadaLollapatuza::mayorGastador() const {
    return _lollapatuza.personaQueMasGasto();
}

IdPuesto FachadaLollapatuza::menorStock(Producto producto) const {
    return _lollapatuza.menorStock(producto);
}

const set<Persona> &FachadaLollapatuza::personas() const {
    vector<Persona> vectorPersonas = _lollapatuza.obtenerPersonas();
    set<Persona> res(vectorPersonas.begin(), vectorPersonas.end());
    return res;
}

Nat FachadaLollapatuza::stockEnPuesto(IdPuesto idPuesto, const Producto &producto) const {
    return _lollapatuza.obtenerPuesto().at(idPuesto).obtenerStock(producto);
}

Nat FachadaLollapatuza::descuentoEnPuesto(IdPuesto idPuesto, const Producto &producto, Nat cantidad) const {
    return _lollapatuza.obtenerPuesto().at(idPuesto).obtenerDescuento(producto,cantidad);
}

Nat FachadaLollapatuza::gastoEnPuesto(IdPuesto idPuesto, Persona persona) const {
    return _lollapatuza.obtenerPuesto().at(idPuesto).obtenerGasto(persona);
}

set<IdPuesto> FachadaLollapatuza::idsDePuestos() const {
    map<IdPuesto,Puesto> puestos = _lollapatuza.obtenerPuesto();
    auto it = puestos.begin();
    set<IdPuesto> res;
    while(it != puestos.end()){
        res.insert(it->first);
        it ++;
    }
    return res;
}
