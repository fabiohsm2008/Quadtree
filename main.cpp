#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include<GL/glut.h>
#include <Quadtree.h>
#define KEY_ESC 27
#define Frac_Circ 1000  // Cien fracciones de circulo
#define PI 3.1415926535897932
#define radio 100

using namespace std;

//Crear quad tree
Quadtree arbol(point(-300.0,300.0), point(300.0,-300.0), 5);
vector<point> puntos;
int x_circulo;
int y_circulo;
int ancho, alto;

void parametrizar(int &x, int &y){
    if(x >= (ancho/2)){
        x = x - (ancho/2);
    }
    else{
        x = ((ancho/2)-x)*(-1);
    }
    if(y <= (alto/2)){
        y = (alto/2) - y;
    }
    else{
        y = (y-(alto/2))*(-1);
    }
}

void parametrizar_2(double &x, double &y){
    if(x >= (ancho/2)){
        x = x - (ancho/2);
    }
    else{
        x = ((ancho/2)-x)*(-1);
    }
    if(y <= (alto/2)){
        y = (alto/2) - y;
    }
    else{
        y = (y-(alto/2))*(-1);
    }
}

void crear_lineas(Quadtree *hola){
    glBegin(GL_LINES);
        if(hola!= nullptr){
            if((*hola).d()){
                point n = point( ((*hola).punto_inicio.x + (*hola).punto_fin.x)/2, ((*hola).punto_inicio.y + (*hola).punto_fin.y)/2 );

                glVertex2d(n.x, (*hola).punto_inicio.y);
                glVertex2d(n.x, (*hola).punto_fin.y);

                glVertex2d((*hola).punto_inicio.x, n.y);
                glVertex2d((*hola).punto_fin.x, n.y);
            }

            for(size_t i = 0; i < 4; ++i){
                crear_lineas((*hola).frames[i]);
            }
        }
    glEnd();
}

void in_puntos(){
    int tam = 500;
    for(int i = 0; i < tam; ++i){
        int x = rand()%ancho;
        int y = rand()%alto;
        parametrizar(x,y);
          //cout << i+1 << ": " << x << " " << y << endl;
          if(!arbol.bus_point(new point(x,y))){
              puntos.push_back(point(x,y));
              arbol.insertar(new point(x,y));
              //cout << endl;
          }
    }
}


void llenar(){
    ifstream hola;
    hola.open("D:/Trabajos 6to semestre/EDA/Quadtree/posiciones_quadtree.txt");
    //hola.open("D:/Trabajos 6to semestre/EDA/Quadtree/posiciones_quadtree_capital.txt");
    string linea;
    getline(hola,linea);
    while(getline(hola,linea)){
        string latitud = linea.substr(0, linea.find(' '));
        string longitud = linea.substr(linea.find(' ') + 1);
        double y = stod(latitud);
        double x = stod(longitud);
        x *= ancho;
        y *= alto;
        if(!arbol.bus_point(new point(x,y))){
            puntos.push_back(point(x,y));
            arbol.insertar(new point(x,y));
        }
    }
    hola.close();
}

void graficar_circulo(int x, int y){
    glBegin(GL_POINTS);
     for (int i = 0; i < Frac_Circ + 1; i++) {  // +1 para cerrar
         glVertex2f( x + radio * cos(2.0 * PI * i / Frac_Circ),
                y + radio * sin(2.0 * PI * i / Frac_Circ) );
    }
    glEnd();
}

//dibuja un simple gizmo
void displayGizmo()
{
	glBegin(GL_LINES);
        glColor3d(255,255,255);
        crear_lineas(&arbol);
        glColor3d(255,0,0);
        ///graficar_circulo(x_circulo, y_circulo);
        /*glColor3d(255,0,0);
        glVertex2d(0, 0);
        glVertex2d(300, 0);
        glColor3d(0, 255, 0);
        glVertex2d(0, 0);
        glVertex2d(0, 300);*/
	glEnd();
}

void OnMouseClick(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
      parametrizar(x,y);
      //cout << x << " " << y << endl;
      if(!arbol.bus_point(new point(x,y))){
          puntos.push_back(point(x,y));
          arbol.insertar(new point(x,y));
          //cout << endl;
      }
    //convertir x,y
	//insertar un nuevo punto en el quadtree
  }
}

void OnMouseMotion(int x, int y)
{
    parametrizar(x,y);
    //cout << x << " " << y << endl;
    x_circulo = x;
    y_circulo = y;

}

void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	//
	glOrtho(-ancho/2, ancho/2, -alto/2, alto/2, -1.0f, 1.0f);

	//dibujar quadTree (qt->draw())
	//dibuja el gizmo
	displayGizmo();

	glBegin(GL_POINTS);
    for(int i = 0; i < puntos.size(); i++){
        glColor3d(255,255,255);
        glVertex2f(puntos[i].x, puntos[i].y);
    }
    vector<point*> zona = arbol.buscar_region(new point(x_circulo, y_circulo), radio);
    for(int i = 0; i < zona.size(); ++i){
        glColor3d(255,0,0);
        glVertex2f((*zona[i]).x, (*zona[i]).y);
    }
    glEnd();
    glPointSize(2.0f);

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//
//el programa principal
//

int main(int argc, char** argv) {
    srand(time(NULL));
    int a;
    cout << "Ingrese ancho de la ventana: ";
    cin >> ancho;
    cout << "Ingrese alto de la ventana: ";
    cin >> alto;
    cout << "Ingrese un maximo de puntos por cuadrante: ";
    cin >> a;
    arbol.set_frame(point(-ancho/2, alto/2), point(ancho/2, -alto/2));
    arbol.set_limite(a);
    //in_puntos();
    llenar();
	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(ancho, alto); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("TP2 bis OpenGL : Chicago"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL
	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutPassiveMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);

	//qt = new quadTree();
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
