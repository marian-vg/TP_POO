#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>

class Curso
{
private:
    int Nro_examenes = 2;
    int Nro_recuperatorios = 1;

public:
    Curso();
    int getNro_examenes();
    void setNro_examenes(int Nro_examenes);
    int getNro_recuperatorios();
    void setNro_recuperatorios(int Nro_recuperatorios);
};

Curso::Curso() {}

int Curso::getNro_examenes()
{
    return this->Nro_examenes;
}

void Curso::setNro_examenes(int Nro_examenes)
{
    this->Nro_examenes = Nro_examenes;
}

int Curso::getNro_recuperatorios()
{
    return this->Nro_recuperatorios;
}

void Curso::setNro_recuperatorios(int Nro_recuperatorios)
{
    this->Nro_recuperatorios = Nro_recuperatorios;
}

class Alumno : public Curso
{
private:
    char dni[8];
    char *nya;
    std::map<int, std::vector<int>> notasPR;
    int promedio;
    int cant_examenes;

public:
    Alumno();
    char *getDni();
    void setDni(const char *dni);
    char *getNya();
    void setNya(const char *nya);
    int getNotaP(int key);
    void setNotaP(int notaP, int key);
    int getNotaR(int key);
    void setNotaR(int notaR, int key);
    int getCant_examenes();
    void setCant_examenes(int cant_examenes);
    int getPromedio() const;
    void setPromedio();
    std::string estado();
    void CargarMapa(int key, int nota_p, int nota_r);
};

Alumno::Alumno() {}

char *Alumno::getDni()
{
    return this->dni;
}

void Alumno::setDni(const char *dni)
{
    strncpy(this->dni, dni, 8);
}

char *Alumno::getNya()
{
    return nya;
}

void Alumno::setNya(const char *nya)
{
    this->nya = new char[strlen(nya) + 1];
    strcpy(this->nya, nya);
}

int Alumno::getNotaP(int key)
{
    int notaP = notasPR[key][0];
    return notaP;
}

int Alumno::getNotaR(int key)
{
    int notaR = notasPR[key][1];
    return notaR;
}

void Alumno::setNotaP(int notaP, int key)
{
    this->notasPR[key].emplace_back(notaP);
}

void Alumno::setNotaR(int notaR, int key)
{
    this->notasPR[key].emplace_back(notaR);
}

int Alumno::getCant_examenes()
{
    return this->cant_examenes;
}

void Alumno::setCant_examenes(int cant_e)
{
    this->cant_examenes = cant_e;
}

int Alumno::getPromedio() const
{
    return this->promedio;
}

void Alumno::setPromedio()
{
    this->promedio = 0;
    for (auto it = this->notasPR.begin(); it != this->notasPR.end(); ++it)
    {
        this->promedio = (it->second[0] + it->second[1]) + this->promedio;
    }

    this->promedio = this->promedio / this->cant_examenes;
}

std::string Alumno::estado()
{

    int min_nota = std::min_element(this->notasPR.begin(), this->notasPR.end(),
                                    [](const auto &a, const auto &b)
                                    {
                                        return a.second[0] < b.second[0];
                                    })
                       ->second[0];
    std::cout << "minima nota: " << min_nota << std::endl;

    if (this->cant_examenes < this->getNro_examenes())
    {
        return "Libre Ausente";
    }
    if (this->promedio < 55 || min_nota < 50)
    {
        return "Libre";
    }
    if (this->promedio > 55 && this->promedio < 75)
    {
        return "Regular";
    }
    if (this->promedio > 75 && min_nota >= 70)
    {
        return "Promocionado";
    }

    return "No tiene estado ";
}

void Alumno::CargarMapa(int key, int nota_p, int nota_r = -1)
{
    std::vector<int> notas(2);

    notas[0] = nota_p;

    if (nota_r != -1)
        notas[1] = nota_r;

    std::cout << "notas " << notas[0] << " ," << notas[1] << std::endl;

    this->notasPR[key] = notas;
}

// ---------------------------------------------------

class GestorAlumno
{
private:
    std::vector<Alumno> alumnos;
    std::multimap<int, Alumno> mayor_prom;

public:
    GestorAlumno();
    void MayorP();
    void CargarAlumno(Alumno a);
    void escribir();
};

GestorAlumno::GestorAlumno() {}

void GestorAlumno::CargarAlumno(Alumno a)
{
    this->alumnos.push_back(a);
}

void GestorAlumno::MayorP()
{

    sort(alumnos.begin(), alumnos.end(),
         [](const auto &a, const auto &b)
         {
             return a.getPromedio() > b.getPromedio();
         });

    for (int i = 0; i < alumnos.size(); i++)
    {
        this->mayor_prom.insert(std::make_pair(alumnos[i].getPromedio(), alumnos[i]));
        std::cout << "[" << i << "] par cargado" << std::endl;
    }

    std::cout << "\nSize Alumnos: " << alumnos.size() << std::endl;
    std::cout << "Size Promedio: " << mayor_prom.size() << std::endl;

    for (auto it = this->mayor_prom.find(alumnos[0].getPromedio()); it != this->mayor_prom.upper_bound(alumnos[0].getPromedio()); ++it)
    {
        std::cout << it->second.getNya() << std::endl;
    }
}

int main()
{
    GestorAlumno g;

    Alumno a, otro_a, ausente_a;

    a.setNya("Jorge");
    otro_a.setNya("Carlos");
    ausente_a.setNya("Maria");

    a.setCant_examenes(2);
    otro_a.setCant_examenes(2);
    ausente_a.setCant_examenes(1);

    a.CargarMapa(1, 80);
    a.CargarMapa(2, 80);
    a.setPromedio();
    otro_a.CargarMapa(1, 80);
    otro_a.CargarMapa(2, 80);
    otro_a.setPromedio();
    ausente_a.CargarMapa(2, 30);
    ausente_a.setPromedio();

    g.CargarAlumno(a);
    g.CargarAlumno(otro_a);
    g.CargarAlumno(ausente_a);

    std::cout << "El estado del alumno: " << a.getNya() << " ," << a.estado() << " ," << a.getPromedio() << std::endl;
    std::cout << "El estado del alumno: " << otro_a.getNya() << " ," << otro_a.estado() << " ," << otro_a.getPromedio() << std::endl;
    std::cout << "El estado del alumno: " << ausente_a.getNya() << " ," << ausente_a.estado() << " ," << ausente_a.getPromedio() << std::endl;

    std::cout << "\nEl/los mayores promedios son: \n";

    g.MayorP();

    return 0;
}