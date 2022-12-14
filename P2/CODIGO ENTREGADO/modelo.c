/*	Prácticas de Informática Gráfica

	Grupo C					Curso 2022-23
 	
	Codigo base para la realización de las practicas de IG
	
	Estudiante: 

=======================================================
	G. Arroyo, J.C. Torres 
	Dpto. Lenguajes y Sistemas Informticos
	(Univ. de Granada)

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details 
 http://www.gnu.org/copyleft/gpl.html

=======================================================/
modulo modelo.c
    Representación del modelo
    Funciones de dibujo
    Función Idle

*/
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"
#include <vector>
#include "file_ply_stl.h"

/**	void initModel()

Inicializa el modelo y de las variables globales


**/
using namespace std;
void
initModel ()
{

}

typedef struct Vertice{
  float x, y, z;
  float n1, n2, n3;
}Vertice;

typedef struct Cara{
  int p1, p2, p3;     
  float n1, n2, n3;
}Cara;

class Malla:public Objeto3D{
  public:
    vector<Vertice> vertices_ply;
    vector<Cara> caras_ply;
  
  Malla(){}
  
  Malla(char nombre_archivo[30]){
    vector<float> vertices;
    vector<int> caras;
    ply::read(nombre_archivo, vertices, caras);
    //Vamos a traducir a nuestra estructura
    vertices_ply.resize (vertices.size()/3);
    caras_ply.resize (caras.size()/3);
    traduce_vertices(vertices);
    traduce_caras(caras);
    calcula_normal_cara();
    calcular_normal_vertice();
  }

  void draw(){ //Este draw es el original de la P1
    glBegin (GL_TRIANGLES);//gl_triangle
    {				    
      for(int i=0; i<caras_ply.size(); i++){
          glNormal3f (caras_ply[i].n1, caras_ply[i].n2, caras_ply[i].n3);	
          glVertex3f (vertices_ply[caras_ply[i].p1].x, vertices_ply[caras_ply[i].p1].y, vertices_ply[caras_ply[i].p1].z);
          glVertex3f (vertices_ply[caras_ply[i].p2].x, vertices_ply[caras_ply[i].p2].y, vertices_ply[caras_ply[i].p2].z);
          glVertex3f (vertices_ply[caras_ply[i].p3].x, vertices_ply[caras_ply[i].p3].y, vertices_ply[caras_ply[i].p3].z);  
      }
    }
    glEnd ();
  }

  void draw2(){ //Este draw es para el GL_FLAT
    glBegin (GL_TRIANGLES);//gl_triangle
    {				    
      for(int i=0; i<caras_ply.size(); i++){
          glVertex3f (vertices_ply[caras_ply[i].p1].x, vertices_ply[caras_ply[i].p1].y, vertices_ply[caras_ply[i].p1].z);
          glVertex3f (vertices_ply[caras_ply[i].p2].x, vertices_ply[caras_ply[i].p2].y, vertices_ply[caras_ply[i].p2].z);
          glNormal3f (caras_ply[i].n1, caras_ply[i].n2, caras_ply[i].n3);	
          glVertex3f (vertices_ply[caras_ply[i].p3].x, vertices_ply[caras_ply[i].p3].y, vertices_ply[caras_ply[i].p3].z);  
      }
    }
    glEnd ();
  }

  void draw3(){ //Este draw es para el GL_SMOOTH
    glBegin (GL_TRIANGLES);//gl_triangle
    {				    
      for(int i=0; i<caras_ply.size(); i++){	
          glNormal3f (vertices_ply[caras_ply[i].p1].n1, vertices_ply[caras_ply[i].p1].n2, vertices_ply[caras_ply[i].p1].n3);
          glVertex3f (vertices_ply[caras_ply[i].p1].x, vertices_ply[caras_ply[i].p1].y, vertices_ply[caras_ply[i].p1].z);
          glNormal3f (vertices_ply[caras_ply[i].p2].n1, vertices_ply[caras_ply[i].p2].n2, vertices_ply[caras_ply[i].p2].n3);
          glVertex3f (vertices_ply[caras_ply[i].p2].x, vertices_ply[caras_ply[i].p2].y, vertices_ply[caras_ply[i].p2].z);
          glNormal3f (vertices_ply[caras_ply[i].p3].n1, vertices_ply[caras_ply[i].p3].n2, vertices_ply[caras_ply[i].p3].n3);
          glVertex3f (vertices_ply[caras_ply[i].p3].x, vertices_ply[caras_ply[i].p3].y, vertices_ply[caras_ply[i].p3].z);  
      }
    }
    glEnd ();
  }

  void traduce_vertices(vector<float> vertices){
    int cont=0;
    //Traducimos los vertices
    for(int i=0; i<vertices.size(); i+=3){
      vertices_ply[cont].x=vertices[i];
      vertices_ply[cont].y=vertices[i+1];
      vertices_ply[cont].z=vertices[i+2];
      cont++;
    }
  }
  void traduce_caras(vector<int> caras){
    int cont=0;
    //Traducimos las caras
    for(int i=0; i< caras.size(); i+=3){
      caras_ply[cont].p1=caras[i];
      caras_ply[cont].p2=caras[i+1];
      caras_ply[cont].p3=caras[i+2];
      cont++;
    }
  }

  void calcula_normal_cara(){
      for(int i=0; i<caras_ply.size(); i++){
        Vertice v1, v2, normal_c;
        //P1P2
        v1.x=vertices_ply[caras_ply[i].p2].x-vertices_ply[caras_ply[i].p1].x;
        v1.y=vertices_ply[caras_ply[i].p2].y-vertices_ply[caras_ply[i].p1].y;
        v1.z=vertices_ply[caras_ply[i].p2].z-vertices_ply[caras_ply[i].p1].z;
        //P1P3
        v2.x=vertices_ply[caras_ply[i].p3].x-vertices_ply[caras_ply[i].p1].x;
        v2.y=vertices_ply[caras_ply[i].p3].y-vertices_ply[caras_ply[i].p1].y;
        v2.z=vertices_ply[caras_ply[i].p3].z-vertices_ply[caras_ply[i].p1].z;
        //Calculamos el producto vectorial
        normal_c.x=(v1.y*v2.z)-(v1.z*v2.y);
        normal_c.y=-((v1.x*v2.z)-(v1.z*v2.x));
        normal_c.z=(v1.x*v2.y)-(v1.y*v2.x);
        //Normalizamos el vector
        caras_ply[i].n1=normal_c.x/(sqrt(pow(normal_c.x,2)+pow(normal_c.y,2)+pow(normal_c.z,2)));
        caras_ply[i].n2=normal_c.y/(sqrt(pow(normal_c.x,2)+pow(normal_c.y,2)+pow(normal_c.z,2)));
        caras_ply[i].n3=normal_c.z/(sqrt(pow(normal_c.x,2)+pow(normal_c.y,2)+pow(normal_c.z,2)));
      }
  }

  void calcular_normal_vertice(){
    //Inicializamos la normal de los vertices a 0
    for(int i=0; i<vertices_ply.size(); i++){
      vertices_ply[i].n1=0;
      vertices_ply[i].n2=0;
      vertices_ply[i].n3=0;
    }
    //Sumamos la normal de las caras a sus respectivos vertices
    for(int i=0; i<caras_ply.size(); i++){
      //Vertice 1
      vertices_ply[caras_ply[i].p1].n1+=caras_ply[i].n1;
      vertices_ply[caras_ply[i].p1].n2+=caras_ply[i].n2;
      vertices_ply[caras_ply[i].p1].n3+=caras_ply[i].n3;
      //Vertice 2
      vertices_ply[caras_ply[i].p2].n1+=caras_ply[i].n1;
      vertices_ply[caras_ply[i].p2].n2+=caras_ply[i].n2;
      vertices_ply[caras_ply[i].p2].n3+=caras_ply[i].n3;
      //Vertice 3
      vertices_ply[caras_ply[i].p3].n1+=caras_ply[i].n1;
      vertices_ply[caras_ply[i].p3].n2+=caras_ply[i].n2;
      vertices_ply[caras_ply[i].p3].n3+=caras_ply[i].n3;
    }
    //Calculo la normal del vertice
    for(int i=0; i<vertices_ply.size(); i++){
      vertices_ply[i].n1=vertices_ply[i].n1/sqrt(pow(vertices_ply[i].n1,2)+pow(vertices_ply[i].n2,2)+pow(vertices_ply[i].n3,2));
      vertices_ply[i].n2=vertices_ply[i].n2/sqrt(pow(vertices_ply[i].n1,2)+pow(vertices_ply[i].n2,2)+pow(vertices_ply[i].n3,2));
      vertices_ply[i].n3=vertices_ply[i].n3/sqrt(pow(vertices_ply[i].n1,2)+pow(vertices_ply[i].n2,2)+pow(vertices_ply[i].n3,2));
    }
  }
};

class Revolucion:public Malla{
  public:
    int instancias;
    int num_vertices;

  Revolucion(char nombre_archivo[30], int num_instancias){
    vector<float> vertices;
    ply::read_vertices(nombre_archivo, vertices);
    instancias=num_instancias;
    num_vertices=vertices.size()/3;
    generar_vertices(vertices);
    generar_caras();
    calcula_normal_cara();
    calcular_normal_vertice();
  }

  //Generamos los vertices de la figura a partir de los iniciales
  void generar_vertices(vector<float> vertices){
    //Añadimos los vertices que obtenemos del archivo al vector de vertices
    for(int i=0; i<vertices.size(); i+=3){
      Vertice v;
      v.x=vertices[i];
      v.y=vertices[i+1];
      v.z=vertices[i+2];
      vertices_ply.push_back(v);
    }
    //Generamos los nuevos vertices de la figura en funcion de los primeros
    int tam=vertices_ply.size();
    for(int i=0; i<instancias; i++){
      float radianes=((2*i*M_PI)/(instancias-1));
      for(int j=0; j<tam; j++){
        Vertice v;
        v.x=vertices_ply[j].x*cos(radianes);
        v.y=vertices_ply[j].y;
        v.z=vertices_ply[j].x*sin(radianes);
        vertices_ply.push_back(v);
      }  
    }
  }
  
  void generar_caras(){
    //Calculamos las caras con las instancias
    for(int i=0; i<instancias-1; i++){
      for(int j=0; j<num_vertices-1; j++){
        int k=num_vertices*i+j;
        Cara c;
        c.p1=k;
        c.p2=k+num_vertices+1;
        c.p3=k+num_vertices;
        caras_ply.push_back(c);
        Cara c2;
        c2.p1=k;
        c2.p2=k+1;
        c2.p3=k+num_vertices+1;
        caras_ply.push_back(c2);
      }
    }
    //Calculamos las caras que unen la ultima instancia con la primera
    for(int i=0; i<num_vertices-1; i++){
      int k=num_vertices*(instancias-1)+i;
      Cara c;
      c.p1=k;
      c.p2=i+1;
      c.p3=i;
      caras_ply.push_back(c);
      Cara c2;
      c2.p1=k;
      c2.p2=k+1;
      c2.p3=i+1;
      caras_ply.push_back(c2);
    }
  }
};

class Ejes:Objeto3D 
{ 
public: 
    float longitud = 30;
// Dibuja el objeto
void draw( )
{
  glDisable (GL_LIGHTING);
  glBegin (GL_LINES);
  {
    glColor3f (0, 1, 0);
    glVertex3f (0, 0, 0);
    glVertex3f (0, longitud, 0);

    glColor3f (1, 0, 0);
    glVertex3f (0, 0, 0);
    glVertex3f (longitud, 0, 0);

    glColor3f (0, 0, 1);
    glVertex3f (0, 0, 0);
    glVertex3f (0, 0, longitud);
  }
  glEnd ();
  glEnable (GL_LIGHTING);

}
} ; 

Ejes ejesCoordenadas;

Malla m1("beethoven");  
Malla m2("big_dodge");
Revolucion r1("perfil", 20);
//Malla m1("buddha50K");
//Malla m1("bunny");
//Malla m1("bunny2");
//Malla m1("castle");
//Malla m1("cow");
//Malla m1("cubo");
//Malla m1("dentadura");
//Malla m1("fichero1");
//Malla m1("footbones");
//Malla m1("perfil");
//Malla m1("shark");
//Malla m1("test");
//Malla m1("topmod");
//Malla m1("topmod-basique");

/**	void Dibuja( void )

Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.

**/

void Dibuja (void)
{
  static GLfloat  pos[4] = { 5.0, 5.0, 10.0, 0.0 };	// Posicion de la fuente de luz

  float  color[4] = { 0.8, 0.0, 1, 1 };
  float  color2[4] = { 0, 1, 0, 1 };
  float  color3[4] = { 1, 0, 0, 1 };
  //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
  glPushMatrix ();		// Apila la transformacion geometrica actual

  glClearColor (0.0, 0.0, 0.0, 1.0);	// Fija el color de fondo a negro

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer

  transformacionVisualizacion ();	// Carga transformacion de visualizacion

  glLightfv (GL_LIGHT0, GL_POSITION, pos);	// Declaracion de luz. Colocada aqui esta fija en la escena

  ejesCoordenadas.draw();			// Dibuja los ejes
  //Pintamos el peon
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  glScaled(1.5,1.5,1.5);
  glShadeModel(GL_FLAT);
  r1.draw2();
  //Pintamos Beethoven
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
  glScaled(0.5,0.5,0.5);
  glTranslatef(-7.5,0.0,0.0);
  glShadeModel(GL_SMOOTH);
  m1.draw3();
  //Pintamos el coche
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color3);
  glTranslatef(17.5,0.0,0.0);
  glShadeModel(GL_FLAT);
  m2.draw2();
  

  glPopMatrix ();		// Desapila la transformacion geometrica

  glutSwapBuffers ();		// Intercambia el buffer de dibujo y visualizacion
}


/**	void idle()

Procedimiento de fondo. Es llamado por glut cuando no hay eventos pendientes.

**/
void idle (int v)
{

  glutPostRedisplay ();		// Redibuja
  glutTimerFunc (30, idle, 0);	// Vuelve a activarse dentro de 30 ms
}
