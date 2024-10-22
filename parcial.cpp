#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <map>

// ----------------- Struct Libro --------------------

struct Libro_struct
{
    int id_str;
    char nombre[20];
    char autor[20];
};

struct Ejemplar_struct
{
    int id_str;
    char isbn_str[20];
    char editorial[20];
    int anio;
};
// ----------------- Clase Ejemplares ----------------

class Ejemplares
{
private:
    std::string isbn;
    int anio_impresion;
    std::string editorial;
public:
    Ejemplares();
    std::string getISBN();
    void setISBN(std::string _isbn);
    int getAnio();
    void setAnio(int anio);
    std::string getEditorial();
    void setEditorial(std::string edit);
};

Ejemplares::Ejemplares() {}

std::string Ejemplares::getISBN()
{
    return this->isbn;
}
void Ejemplares::setISBN(std::string _isbn)
{
    this->isbn = _isbn;
}

int Ejemplares::getAnio()
{
    return this->anio_impresion;
}

void Ejemplares::setAnio(int anio)
{
    this->anio_impresion = anio;
}

std::string Ejemplares::getEditorial()
{
    return this->editorial;
}
void Ejemplares::setEditorial(std::string edit)
{
    this->editorial = edit;

}
// ----------------- Clase Libro ----------------

class Libro{
private:
    int id;
    std::string nombre;
    std::string autor;
    std::vector<Ejemplares> ejemplares;
    int cant_ejem = 0;
public:
    Libro();
    int getID();
    void setID(int id_);
    int getCant();
    std::string getNombre();
    void setNombre(std::string nom);
    std::string getAutor();
    void setAutor(std::string aut);
    std::vector<Ejemplares> getEjemplares();
    void setEjemplares(Ejemplares ejemplar);
};

Libro::Libro() {}

int Libro::getCant()
{
      return this->cant_ejem;
}

int Libro::getID()
{
    return this->id;
}
void Libro::setID(int id_)
{
    this->id = id_;
}

std::string Libro::getNombre()
{
    return this->nombre;
}
void Libro::setNombre(std::string nom)
{
    this->nombre = nom;
}

std::string Libro::getAutor()
{
    return this->autor;
}
void Libro::setAutor(std::string aut)
{
    this->autor = aut;
}

std::vector<Ejemplares> Libro::getEjemplares()
{
    return this->ejemplares;
}

void Libro::setEjemplares(Ejemplares ejemplar)
{
    ejemplares.push_back(ejemplar);
    cant_ejem++;
}

// ------------------ Clase Gestor ------------------

class Gestor
{
private:
    std::vector<Libro> libros;
public:
    Gestor();
    void MayorCant();
    void CargarLibros(Libro lib);
    void EscribirArchiBin(Libro lib);
    void BuscarNombre(std::string nombre_libro);
    void DiferenteEditorial();
    void MayorEdit();
};

Gestor::Gestor() {}

void Gestor::CargarLibros(Libro lib)
{
    libros.push_back(lib);
}

void Gestor::EscribirArchiBin(Libro lib)
{
    Libro_struct libro_str;
    Ejemplar_struct ejemp;

    std::ofstream archivo1;
    
    archivo1.open("Libros.dat", std::ios::binary | std::ios::app);

    if (archivo1.fail())
    {
        std::cout << "Error" << std::endl;
        exit(1);
    }

    libro_str.id_str = lib.getID();
    strcpy(libro_str.autor, lib.getAutor().c_str());
    strcpy(libro_str.nombre, lib.getNombre().c_str());
    archivo1.write((char*)&libro_str, sizeof(libro_str));
    
    archivo1.close();

    std::ofstream archivo2;

    archivo2.open("Ejemplar.dat", std::ios::binary | std::ios::app);

    if (archivo2.fail())
    {
        std::cout << "Error ejemplar" << std::endl;
        exit(1);
    }

    auto aux_ejem = lib.getEjemplares();

for (int i = 0; i < aux_ejem.size(); i++) {
    ejemp.id_str = lib.getID();
    strcpy(ejemp.isbn_str, aux_ejem[i].getISBN().c_str());
    ejemp.anio = aux_ejem[i].getAnio();
    strcpy(ejemp.editorial, aux_ejem[i].getEditorial().c_str());
    archivo2.write((char*)&ejemp, sizeof(ejemp));
}
    archivo2.close();
}

void Gestor::BuscarNombre(std::string nombre_libro)
{   
    auto aux_ejem = libros[0].getEjemplares();

    for (int i = 0; i < libros.size(); i++)
    {
        if (libros[i].getNombre() == nombre_libro)
        {
            std::cout << "- El Libro: " << libros[i].getNombre() << " tiene: " << libros[i].getCant() << " cant. de Ejemplares " << std::endl;
            std::cout << "\nY los siguientes Datos: " << std::endl;

            std::ofstream archivoTxt;
            archivoTxt.open("Libros.txt");

            if (archivoTxt.fail()) {
                std::cout << "Error txt" << std::endl;
                exit(-1);
            }

            archivoTxt << "Libro: " << libros[i].getNombre() << std::endl;

            for (int j = 0; j < aux_ejem.size(); j++)
            {
                archivoTxt << aux_ejem[j].getISBN() << "   " << aux_ejem[j].getAnio() << "   " << aux_ejem[j].getEditorial() << std::endl; 
                std::cout << " - " << aux_ejem[j].getISBN() << " (Ejemplar " << j+1 << ")" <<std::endl;
                std::cout << " - " << aux_ejem[j].getAnio() << std::endl;
                std::cout << " - " << aux_ejem[j].getEditorial() << std::endl;
            }

            archivoTxt.close();
        }
    }
}

void Gestor::DiferenteEditorial()
{
    auto aux_ejem = libros[0].getEjemplares();
    int cant_edit = 1;

    for (int i = 0; i < aux_ejem.size(); i++)
    {
        if (aux_ejem[i].getEditorial() != aux_ejem[0].getEditorial())      
            cant_edit++;
        
        if (cant_edit > 1)
            std::cout << "\n - El libro: " << libros[i].getNombre() << " tiene " << cant_edit << " editoriales diferentes" << std::endl;
    }
}

void Gestor::MayorEdit()
{   
    std::map<std::string, int> cant_edit;

    for (int i = 0; i < libros.size(); i++) 
    {
        auto aux_ejem = libros[i].getEjemplares();

        for (int j = 0; j < aux_ejem.size(); j++)
        {
            cant_edit[aux_ejem[j].getEditorial()]++;
        }
    }
    
    auto it = std::max_element(cant_edit.begin(), cant_edit.end(), 
                                [] (const auto &a, const auto &b)
                                {return a.second < b.second;}
    );

    std::cout << "\n- La editorial: "<< it->first << " tiene " << it->second << " ejemplares"<<std::endl;
}

void Gestor::MayorCant()
{
    sort(libros.begin(), libros.end(), [] (Libro& a, Libro& b)
                                { return a.getCant() > b.getCant(); }
    );

    for (int i = 0; i < libros.size(); i++)
    {
        if (libros[i].getCant() == libros[0].getCant())
            std::cout << " - " <<libros[i].getNombre() << ": " << libros[i].getCant() << std::endl;
    }
    std::cout << "--------------------------" << std::endl;
}

int main(){

    Libro l1, l2;
    Ejemplares e1, e2, e3, e4;

    e1.setAnio(10);
    e2.setAnio(20);
    e3.setAnio(30);
    e4.setAnio(40);
    e1.setEditorial("Santillana");
    e2.setEditorial("Santillana v2");
    e3.setEditorial("ferrero");
    e4.setEditorial("ferrero");
    e1.setISBN("JKL");
    e2.setISBN("HIF");
    e3.setISBN("TYO");
    e4.setISBN("YYO");

    l1.setNombre("Caperucita");
    l2.setNombre("Caperucita v2");
    l1.setAutor("Julio Roca");
    l2.setAutor("Julio Roca Jr");
    l1.setID(1);
    l2.setID(2);
    l1.setEjemplares(e1);
    l1.setEjemplares(e2);
    l2.setEjemplares(e3);
    l2.setEjemplares(e4);

    Gestor g;

    g.CargarLibros(l1);
    g.CargarLibros(l2);

    g.EscribirArchiBin(l1);
    g.EscribirArchiBin(l2);

    std::cout << "  Los 5 libros con mas ejemplares son: " << std::endl;
    g.MayorCant();
    g.BuscarNombre("Caperucita");
    g.DiferenteEditorial();
    g.MayorEdit();

    return 0;
}