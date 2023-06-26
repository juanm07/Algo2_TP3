#include "fachada_lollapatuza.h"


FachadaLollapatuza::FachadaLollapatuza(const set<Persona> &personas, const map<IdPuesto, aed2_Puesto> &infoPuestos) {
    _lollapatuza.crearLolla(infoPuestos, vector<Persona>(personas.begin(), personas.end()));
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
    // TODO: Completar
}

Nat FachadaLollapatuza::descuentoEnPuesto(IdPuesto idPuesto, const Producto &producto, Nat cantidad) const {
    // TODO: Completar
}

Nat FachadaLollapatuza::gastoEnPuesto(IdPuesto idPuesto, Persona persona) const {
    // TODO: Completar
}

set<IdPuesto> FachadaLollapatuza::idsDePuestos() const {
    // TODO: Completar
}
