#ifndef QUADTREE_H
#define QUADTREE_H
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

class point{
public:
    float x, y;
    point(float a, float b);
    point();
    void set_posicion(float a, float b);
    bool operator==(point temp);
};

class Quadtree
{
    public:
        point punto_inicio, punto_fin;
        Quadtree* frames[4];
        Quadtree();
        Quadtree(point, point, int);
        void set_frame(point, point);
        void set_limite(int);
        void comprobar();
        bool d(){return dividido;};
        bool concurrency(point *, int);
        Quadtree* buscar(Quadtree*, point*);
        vector<point*> buscar_region(point*, int);
        void insertar(point*);
        void eliminar(point*);
        bool bus_point(point*);
        virtual ~Quadtree();

    protected:

    private:
        int limite = 5;
        bool valido = true;
        bool dividido = false;
        double euclidiana(point, point);
        bool buscar(point *rpta);
        vector<point*> puntos;
};

#endif // QUADTREE_H
