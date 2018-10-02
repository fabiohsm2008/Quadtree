#include "Quadtree.h"

point::point(float a, float b){
        x = a;
        y = b;
};

point::point(){
        x = 0.0;
        y = 0.0;
};

void point::set_posicion(float a, float b){
        x = a;
        y = b;
};

bool point::operator==(point temp){
        return (x == temp.x && y == temp.y);
}

bool Quadtree::buscar(point *rpta){
    for(size_t i = 0; i < puntos.size(); i++){
        if(*puntos[i] == *rpta) return true;
    }
    return false;
}

Quadtree::Quadtree()
{
    punto_inicio = point(0.0,0.0);
    punto_fin = point(0.0,0.0);
    valido = true;
    for(size_t i = 0; i < 4; ++i){
        frames[i] = nullptr;
    }
}

Quadtree::~Quadtree()
{
    //dtor
}

Quadtree::Quadtree(point inicio, point fin, int l){
    punto_inicio = inicio;
    punto_fin = fin;
    valido = true;
    for(size_t i = 0; i < 4; ++i){
        frames[i] = nullptr;
    }
    limite = l;
}

void Quadtree::set_frame(point inicio, point fin){
    punto_inicio = inicio;
    punto_fin = fin;
}

void Quadtree::set_limite(int l){
    this->limite = l;
}

void Quadtree::comprobar(){
    if(limite - puntos.size() <= 0){
        valido = 0;
    }
}

vector<point*> mover_puntos_1(vector<point*> &a, point n){
    vector<point*> rpta;
    for(size_t i = 0; i < a.size(); ++i){
        if((*a[i]).x < n.x && (*a[i]).y > n.y){
            rpta.push_back(a[i]);
        }
    }
    return rpta;
}

vector<point*> mover_puntos_2(vector<point*> &a, point n){
    vector<point*> rpta;
    for(size_t i = 0; i < a.size(); ++i){
        if((*a[i]).x > n.x && (*a[i]).y > n.y){
            rpta.push_back(a[i]);
        }
    }
    return rpta;
}

vector<point*> mover_puntos_3(vector<point*> &a, point n){
    vector<point*> rpta;
    for(size_t i = 0; i < a.size(); ++i){
        if((*a[i]).x < n.x && (*a[i]).y < n.y){
            rpta.push_back(a[i]);
        }
    }
    return rpta;
}

vector<point*> mover_puntos_4(vector<point*> &a, point n){
    vector<point*> rpta;
    for(size_t i = 0; i < a.size(); ++i){
        if((*a[i]).x > n.x && (*a[i]).y < n.y){
            rpta.push_back(a[i]);
        }
    }
    return rpta;
}

bool Quadtree::concurrency(point *p, int rad){
    if(punto_inicio.x < (*p).x && punto_inicio.y > (*p).y && punto_fin.x > (*p).x && punto_fin.y < (*p).y){
        return true;
    }
    point up_right, down_left;
    up_right = point( punto_fin.x, punto_inicio.y );
    down_left = point( punto_inicio.x, punto_fin.y );
    if(euclidiana(punto_inicio, *p) <= rad) return true;
    if(euclidiana(up_right, *p) <= rad) return true;
    if(euclidiana(down_left, *p) <= rad) return true;
    if(euclidiana(punto_fin, *p) <= rad) return true;

    point up, right, down, left;
    up = point( (*p).x, (*p).y + rad );
    if(punto_inicio.x < up.x && punto_inicio.y > up.y && punto_fin.x > up.x && punto_fin.y < up.y){
        return true;
    }
    right = point( (*p).x + rad, (*p).y );
    if(punto_inicio.x < right.x && punto_inicio.y > right.y && punto_fin.x > right.x && punto_fin.y < right.y){
        return true;
    }
    down = point( (*p).x - rad, (*p).y );
    if(punto_inicio.x < down.x && punto_inicio.y > down.y && punto_fin.x > down.x && punto_fin.y < down.y){
        return true;
    }
    left = point( (*p).x, (*p).y - rad );
    if(punto_inicio.x < left.x && punto_inicio.y > left.y && punto_fin.x > left.x && punto_fin.y < left.y){
        return true;
    }

    return false;
}

vector<point*> Quadtree::buscar_region(point *nuevo, int rad){
    vector<point*> rpta;
    if(concurrency(nuevo, rad)){
        if(dividido){
            vector<point*> temp;
            for(int i = 0; i < 4; ++i){
                temp = (*frames[i]).buscar_region(nuevo, rad);
                rpta.insert(rpta.end(), temp.begin(), temp.end());
            }
        }
        else{
            for(int i = 0; i < puntos.size(); ++i){
                if(euclidiana(*puntos[i], *nuevo) <= rad){
                    rpta.push_back(puntos[i]);
                }
            }
        }
    }
    return rpta;
}

Quadtree* Quadtree::buscar(Quadtree *temp, point *nuevo){
    (*temp).comprobar();
    if((*temp).valido){
        return temp;
    }
    point n = point( ((*temp).punto_inicio.x + (*temp).punto_fin.x)/2, ((*temp).punto_inicio.y + (*temp).punto_fin.y)/2 );
    //cout << n.x << " " << n.y << endl;
    int i;
    if((*nuevo).x <= n.x && (*nuevo).y >= n.y){
        i = 0;
    }
    else if((*nuevo).x > n.x && (*nuevo).y > n.y){
        i = 1;
    }
    else if((*nuevo).x < n.x && (*nuevo).y < n.y){
        i = 2;
    }
    else if((*nuevo).x >= n.x && (*nuevo).y <= n.y){
        i = 3;
    }
    if ((*temp).frames[0] == nullptr){
        (*temp).dividido = true;
        (*temp).frames[0] = new Quadtree((*temp).punto_inicio, n, limite);
        (*temp).frames[1] = new Quadtree(point(n.x, (*temp).punto_inicio.y), point((*temp).punto_fin.x, n.y), limite);
        (*temp).frames[2] = new Quadtree(point((*temp).punto_inicio.x, n.y), point(n.x, (*temp).punto_fin.y), limite);
        (*temp).frames[3] = new Quadtree(n, (*temp).punto_fin, limite);

        (*(*temp).frames[0]).puntos = mover_puntos_1((*temp).puntos,n);
        (*(*temp).frames[1]).puntos = mover_puntos_2((*temp).puntos,n);
        (*(*temp).frames[2]).puntos = mover_puntos_3((*temp).puntos,n);
        (*(*temp).frames[3]).puntos = mover_puntos_4((*temp).puntos,n);

        (*temp).puntos.clear();
    }
    return buscar((*temp).frames[i], nuevo);
}

void Quadtree::insertar(point *nuevo){
    Quadtree *temp = buscar(this, nuevo);
    (*temp).puntos.push_back(nuevo);
}

void Quadtree::eliminar(point *nuevo){

}

bool Quadtree::bus_point(point *p){
    if(dividido){
        point n = point( (punto_inicio.x + punto_fin.x)/2, (punto_inicio.y + punto_fin.y)/2 );
        if((*p).x <= n.x && (*p).y >= n.y){
            return (*frames[0]).bus_point(p);
        }
        if((*p).x > n.x && (*p).y > n.y){
            return (*frames[1]).bus_point(p);
        }
        if((*p).x < n.x && (*p).y < n.y){
            return (*frames[2]).bus_point(p);
        }
        if((*p).x >= n.x && (*p).y <= n.y){
            return (*frames[3]).bus_point(p);
        }
    }
    return buscar(p);
}

double Quadtree::euclidiana(point a, point b){
    return sqrt( pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
