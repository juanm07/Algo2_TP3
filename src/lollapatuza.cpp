#include "lollapatuza.h"
Lollapatuza::Lollapatuza(): _personas(vector<Persona>()), _puestos(map<IdPuesto, Puesto> ()),_mayorConsumidora(0), _consumosPorPersona(map<Persona, map<Producto, Cantidad>> ()),
 _puestosHackeables(map<Persona, map<Producto, map<IdPuesto, Puesto*>>>()), _precios(map<Producto, Dinero> ()), _gastosPorPersona(ColaPrior<pair<Dinero, map<Persona,Indice>::iterator>>()),
_personasEnGasto(map<Persona, Indice> ()) {}

 Lollapatuza::Lollapatuza(const map <IdPuesto, Puesto> &puestos, const vector<Persona> &personas){
     _personas = personas;
     _puestos = puestos;

     //Se declara como mayor consumidora a una persona, no importa quién sea.
     _mayorConsumidora = personas[0];

     //Como los precios son los mismos no importa el puesto.
     auto it = puestos.begin();
     Puesto unPuesto = it->second;
     _precios = unPuesto.obtenerPrecios(); // Antes estabamos accediendo a la variable interna 'precios' del módulo Puesto, lo que rompia el encapsulamiento

     int i = 0;
     while (i < personas.size()) {
         //Defino a todas las personas en puestos hackeables.
         _puestosHackeables.insert(make_pair(personas[i], map<Producto, map<IdPuesto, Puesto*>>()));

         //Defino a todas las personas en personasEnGasto junto con el indice.
         auto itPersona = _personasEnGasto.insert(make_pair(personas[i], i)).first;

         _gastosPorPersona.encolar(make_pair(0, itPersona)); // Armo el MaxHeap

         i++;
     }
 }

void Lollapatuza::registrarCompra(IdPuesto id, Persona a, Producto item, Cantidad cantidad) {
     if(_consumosPorPersona.count(a) == 0){ //actualizo consumos
        _consumosPorPersona[a][item] = cantidad;
    }else{
        if(_consumosPorPersona[a].count(item) == 0){
            _consumosPorPersona[a][item] = cantidad; //creo que con esta linea ya estaria. Revisar cuando funcione todo
        }else{
            Cantidad cantAnterior = _consumosPorPersona[a][item];
            _consumosPorPersona[a][item] = cantAnterior + cantidad;
        }
    }

    Puesto *puestoDeVenta;
    puestoDeVenta = &_puestos.at(id); // creamos un puntero al puesto
    puestoDeVenta->registrarVenta(a, item, cantidad);

    Dinero gasto = _precios[item] * cantidad;

    Dinero gastoConDesc = puestoDeVenta->aplicarDescuento(gasto, puestoDeVenta->obtenerDescuento(item, cantidad)); //Aplico descuento si corresponde
    Dinero nuevoGasto = _gastosPorPersona.indexar(_personasEnGasto[a]).first + gastoConDesc; //Nuevo gasto de la persona
    auto itPersona = _gastosPorPersona.indexar(_personasEnGasto[a]).second; //Me guardo el iterador que esta en el maxHeap
    pair<Dinero, map<Persona,Indice>::iterator>aInsertar = make_pair(nuevoGasto,itPersona); //Me armo la tupla que va a reemplazar a la vieja

    _gastosPorPersona.reemplazar(_personasEnGasto[a], aInsertar); //Reemplazo la tupla vieja por la nueva con el gasto actualizado
    _gastosPorPersona.heapifyUp(_personasEnGasto[a]); //Como el gasto aumento tengo que ver si la tupla sube o si se queda ahi

    auto itMayorConsumidor = (_gastosPorPersona.proximo()).second; // Actualizo mayor consumidor
    _mayorConsumidora = itMayorConsumidor->first;

    if(puestoDeVenta->obtenerDescuento(item, cantidad) == 0){ // Veo si hay que agregar el puesto a PuestosHackeables
        if(_puestosHackeables[a].count(item) == 1){
            if(_puestosHackeables[a][item].count(id) == 0){
                _puestosHackeables[a][item].insert(make_pair(id, puestoDeVenta));
            }
        }else{
            map<IdPuesto, Puesto*> infoPuesto = map<IdPuesto, Puesto*>();
            infoPuesto.insert(make_pair(id, puestoDeVenta));
            _puestosHackeables[a].insert(make_pair(item, infoPuesto));
        }
    }
}

void Lollapatuza::hackear(Persona a, Producto item) {
     Nat cantidadNueva = _consumosPorPersona[a][item] -1;
    _consumosPorPersona[a][item] = cantidadNueva; //Actualizo consumos

    Dinero gastoViejo = _gastosPorPersona.indexar(_personasEnGasto[a]).first;
    Dinero nuevoGasto = gastoViejo - _precios[item];
    auto itPersona = _gastosPorPersona.indexar(_personasEnGasto[a]).second;
    pair<Dinero, map<Persona,Indice>::iterator>aInsertar = make_pair(nuevoGasto,itPersona); //Me armo la tupla que va a reemplazar a la vieja
    _gastosPorPersona.reemplazar(_personasEnGasto[a], aInsertar); //Reemplazo la tupla vieja por la nueva de arriba
    _gastosPorPersona.heapifyDown(_personasEnGasto[a]); //Hago heapify para abajo porque disminui el valor
    _mayorConsumidora = (_gastosPorPersona.proximo().second)->first;
    auto it = _puestosHackeables[a][item].begin();
    Puesto* puesto = it->second;
    puesto->hackeoPuesto(a,item);

    if (puesto->obtenerCantVendidaSinDesc(a, item) == 0) {
        _puestosHackeables[a][item].erase(it);
    }

}

Dinero Lollapatuza::gastoTotal(Persona a) const{
    Indice i = _personasEnGasto.at(a);
    return _gastosPorPersona.indexar(i).first;
}

Persona Lollapatuza::personaQueMasGasto() const{
    return _mayorConsumidora;
}

IdPuesto Lollapatuza::menorStock(Producto item) const{ //dudoso
    IdPuesto res;
    auto itPuesto = _puestos.begin();
    while(itPuesto != _puestos.end()){
        if((itPuesto->second).estaEnElMenu(item)){
            res = itPuesto->first;
            break;
        }
        itPuesto++;
    }

    while(itPuesto != _puestos.end()){
        if((itPuesto->second).estaEnElMenu(item)){
            if((itPuesto->second).obtenerStock(item) < _puestos.at(res).obtenerStock(item)){
                res = itPuesto->first;
            }else {
                if (((itPuesto->second).obtenerStock(item) == _puestos.at(res).obtenerStock(item)) && (res > itPuesto->first)) { //copie el algoritmo pero no creo que este bien. A obtenerStock(res, item) le estamos pasando un puestoId en vez de un puesto
                    res = itPuesto->first;
                }
            }
        }
        itPuesto++;
    }
    return res;
}

vector<Persona> Lollapatuza::obtenerPersonas() const{
    return _personas;
}

map<IdPuesto, Puesto> Lollapatuza::obtenerPuestos() const{
    return _puestos;
}

