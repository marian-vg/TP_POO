#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <algorithm>

// TODO: -------------- Estructura para guardar en el archiv binario --------------

struct Producto_viajes{
    int id;
    char nombre[20];
    double monto;
    char tipo;
};

// TODO: ---------- Clase Producto ----------

class Producto 
{
protected: 
    double monto;
    char* nombre;
    char tipo[1];
    int cant_exc = 0;
public:
    Producto();
    char* getNombre();
    void setNombre(const char* nombre);
    int getCant();
    virtual char getTipo() = 0;
    virtual double getMonto() = 0; 
};

Producto::Producto() {}

char* Producto::getNombre()
{
    return this->nombre;   
}

void Producto::setNombre(const char* nom)
{
    this->nombre = new char[strlen(nom) + 1];
    strcpy(this->nombre, nom);
}

int Producto::getCant()
{
    return this->cant_exc;
}

// TODO: ---------- Clase Excursiones ---------------

class Excursiones : public Producto
{
public:
    Excursiones();
    double getMonto();
    char getTipo();
    void setMonto(double monto);
};

Excursiones::Excursiones() {}

double Excursiones::getMonto()
{
    return this->monto;
}

void Excursiones::setMonto(double monto)
{
    this->monto = monto;
}

char Excursiones::getTipo()
{
    return 'E';
}
// TODO: ----------- Clase Viaje -------------------

class Viaje : public Producto
{
private:
    int km_recorridos;
public:
    Viaje();
    int getKilometros();
    void setKilometros(int kms);
    double getMonto();
    void setMonto(double monto);
    char getTipo();
};

Viaje::Viaje() {}

int Viaje::getKilometros()
{
    return this->km_recorridos;
}

void Viaje::setKilometros(int kms)
{
    this->km_recorridos = kms;
}

double Viaje::getMonto()
{
    return this->monto;
}

void Viaje::setMonto(double monto)
{
    this->monto = monto * km_recorridos;
}

char Viaje::getTipo()
{
    return 'V';
}
// TODO: ------------- Clase Tour --------------------

class Tour : public Producto
{
private:
    std::vector<Excursiones> excursiones;
    Viaje* viaje;
public:
    Tour();
    void cargarExcursion(Excursiones& exc);
    void setViaje(Viaje* Viaje);
    double getMonto();
    char getTipo();
};

Tour::Tour() {}

void Tour::cargarExcursion(Excursiones& exc)
{
    excursiones.push_back(exc);
    cant_exc++;
}

void Tour::setViaje(Viaje* viaje)
{
    this->viaje = viaje;
}

double Tour::getMonto()
{
    int monto_excursion = 0;
    for (int i = 0; i < excursiones.size(); i++)
    {
        monto_excursion += excursiones[i].getMonto();
    }

    return this->monto = monto_excursion + viaje->getMonto();
}

char Tour::getTipo()
{
    return 'T';
}
// TODO: ---------------- Clase Gestor --------------------

class Gestor
{
private:
    std::vector<Producto *> productos;
    int contador;
public:
    Gestor() {}
    void ProdBarato();
    void MayoresTour();
    void CargarProductos(Producto* p);
    void Mostrar();
    void EscribirArchivo(Producto* p);
};

void Gestor::CargarProductos(Producto* p)
{
    productos.push_back(p);
}

void Gestor::Mostrar() {
    for (int i = 0; i < productos.size(); i++)
    {
        std::cout << " - "<< productos[i]->getNombre() << ": " << productos[i]->getMonto() << std::endl;
    }
    std::cout << "-------------------------------------" << std::endl;
}

void Gestor::EscribirArchivo(Producto* p)
{
    Producto_viajes prod;

    std::ofstream archivo;

    archivo.open("productos.dat", std::ios::binary | std::ios::app);

    if (archivo.fail())
    {
        std::cout << "No se pudo abrir el archivo" << std::endl;
        exit;
    }
    
    prod.id = ++contador;
    strcpy(prod.nombre, p->getNombre());
    prod.monto = p->getMonto();
    prod.tipo = p->getTipo();
    archivo.write((char*)&prod, sizeof(prod));

    archivo.close();
    
}

void Gestor::ProdBarato()
{   
    sort(productos.begin(), productos.end(), [] (const auto& a, const auto& b)
                                            {
                                                return a->getMonto() < b->getMonto();
                                            });

    for (int i = 0; i < productos.size(); i++)
    {
        if (productos[i]->getMonto() == productos[0]->getMonto())
        std::cout << productos[i]->getNombre() << ": " << productos[i]->getMonto() << std::endl;
    }
}

void Gestor::MayoresTour()
{
    sort(productos.begin(), productos.end(), [] (const auto& a, const auto& b)
                                        {
                                            return a->getCant() > b->getCant();
                                        });
    
    std::ofstream archivo_tour;
    archivo_tour.open("tours.txt");

    if (archivo_tour.fail())
    {
        std::cout << "Error archivo tour" << std::endl;
        exit;
    }

    for (int i = 0; i < productos.size(); i++)
    {
        if (productos[i]->getTipo() == 'T')
        archivo_tour << productos[i]->getNombre() << ": " << productos[i]->getCant() << std::endl;
    }

    archivo_tour.close();

    //muestro por pantalla
    for (int i = 0; i < 5; i++)
    {
        if (productos[i]->getTipo() == 'T')
        std::cout << " - "<< productos[i]->getNombre() << ": " << productos[i]->getCant() << " cantidad de excursiones"<<std::endl;
    }
}

// TODO: --------------------- Main --------------------

int main() {

    Excursiones p1;
    Excursiones p2;
    Excursiones p3;
    Excursiones p4;
    Excursiones p5;
    Excursiones p6;

    p1.setNombre("Playa");
    p1.setMonto(1000);
    p2.setNombre("Montaña");
    p2.setMonto(1000);
    p3.setNombre("Sendero");
    p3.setMonto(1000);
    p4.setNombre("Playa_v2");
    p4.setMonto(2000);
    p5.setNombre("Sendero_v2");
    p5.setMonto(2000);
    p6.setNombre("Montaña_v2");
    p6.setMonto(2000);

    Viaje v1, v2;

    v1.setNombre("Buenos Aires");
    v1.setKilometros(35);
    v1.setMonto(5000);

    v2.setNombre("Bahia Blanca");
    v2.setKilometros(31);
    v2.setMonto(4860);

    Tour t1, t2;

    t1.setNombre("Cancun");
    t1.cargarExcursion(p1);
    t1.cargarExcursion(p2);
    t1.cargarExcursion(p3);
    t1.setViaje(&v1);

    t2.setNombre("Santa Barbara");
    t2.cargarExcursion(p4);
    t2.cargarExcursion(p5);
    t2.cargarExcursion(p6);
    t2.setViaje(&v2);

    Gestor gestorProd;

    gestorProd.CargarProductos(&p1);
    gestorProd.CargarProductos(&p2);
    gestorProd.CargarProductos(&p3);
    gestorProd.CargarProductos(&v1);
    gestorProd.CargarProductos(&t1);

    gestorProd.Mostrar();

    gestorProd.CargarProductos(&p4);
    gestorProd.CargarProductos(&p5);
    gestorProd.CargarProductos(&p6);
    gestorProd.CargarProductos(&v2);
    gestorProd.CargarProductos(&t2);

    gestorProd.Mostrar();

    gestorProd.EscribirArchivo(&p1);
    gestorProd.EscribirArchivo(&p2);
    gestorProd.EscribirArchivo(&p3);
    gestorProd.EscribirArchivo(&v1);
    gestorProd.EscribirArchivo(&t1);

    std::cout << "\nEl/Los productos mas baratos son: " << std::endl;
    gestorProd.ProdBarato();

    std::cout << "\nLos 5 tours con mas excursiones son: " << std::endl;
    gestorProd.MayoresTour();

    return 0;
}
