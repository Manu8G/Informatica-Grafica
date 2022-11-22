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
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"


/**	void initModel()

Inicializa el modelo y de las variables globales


**/
void
initModel ()
{
  

}

int modo=GL_FILL;

void setModo(int m){
  //printf("Me ha llegado esto %d", m);
  modo=m;
}

bool luz=true;
bool cambio=false;
void changeLight(){
  if(modo==GL_FILL){
    if(luz==true){
      luz=false;
    }else{
      luz=true;
    }
  }
}

class Cubo:Objeto3D{
  public:
    float lado;

  Cubo(float l){
    lado=l;
  } 

  void draw( )
  {
  glBegin (GL_QUAD_STRIP);
  {				/* Caras transversales */
    glNormal3f (0.0, 0.0, 1.0);	/*Vertical delantera */
    glVertex3f (lado, lado, lado);
    glVertex3f (0, lado, lado);
    glVertex3f (lado, 0, lado);
    glVertex3f (0, 0, lado);
    glNormal3f (0.0, -1.0, 0.0);	/*Inferior */
    glVertex3f (lado, 0, 0);
    glVertex3f (0, 0, 0);
    glNormal3f (0.0, 0.0, -1.0);	/* Vertical hacia atras */
    glVertex3f (lado, lado, 0);
    glVertex3f (0, lado, 0);
    glNormal3f (0.0, 1.0, 0.0);	/* Superior, horizontal */
    glVertex3f (lado, lado, lado);
    glVertex3f (0, lado, lado);
  }
  glEnd ();
  glBegin (GL_QUADS);
  {				/* Costados */
    glNormal3f (1.0, 0.0, 0.0);
    glVertex3f (lado, 0, 0);
    glVertex3f (lado, lado, 0);
    glVertex3f (lado, lado, lado);
    glVertex3f (lado, 0, lado);
    glNormal3f (-1.0, 0.0, 0.0);
    glVertex3f (0, 0, 0);
    glVertex3f (0, 0, lado);
    glVertex3f (0, lado, lado);
    glVertex3f (0, lado, 0);
  }
  glEnd ();
  }
};

Cubo c1(10);

class Piramide:Objeto3D{
  public:
    float lado;
    float alto;
    float n_lado;
    float n_alto;

  Piramide(float l, float h){
    lado=l;
    alto=h;
    n_lado=((l)/(sqrt(pow(l,2)+pow(h,2))));
    n_alto=((h)/(sqrt(pow(l,2)+pow(h,2))));
  } 

  void draw( )
  {
  glBegin (GL_TRIANGLES);//gl_triangle
  {				/* Caras transversales */
    /*Vertical delantera */
    glNormal3f (0.0, n_alto, n_lado);	
    glVertex3f (lado/2, alto, lado/2);
    glVertex3f (0, 0, lado);
    glVertex3f (lado, 0, lado);
    /*Vertical derecha */
    glNormal3f (n_lado, n_alto, 0.0);	
    glVertex3f (lado/2, alto, lado/2);
    glVertex3f (lado, 0, lado);
    glVertex3f (lado, 0, 0);
    /*Vertical trasera */
    glNormal3f (0.0, n_alto, -n_lado);	
    glVertex3f (lado/2, alto, lado/2);
    glVertex3f (lado, 0, 0);
    glVertex3f (0, 0, 0);
    /*Vertical izquierda */
    glNormal3f (-n_lado, n_alto, 0.0);	
    glVertex3f (lado/2, alto, lado/2);
    glVertex3f (0, 0, 0);
    glVertex3f (0, 0, lado);
    /*Vertical abajo mitad A */
    glNormal3f (0.0, -1.0, 0.0);	
    glVertex3f (0, 0, 0);
    glVertex3f (lado, 0, lado);
    glVertex3f (0, 0, lado);
    /*Vertical abajo mitad B */
    glNormal3f (0.0, -1.0, 0.0);	
    glVertex3f (0, 0, 0);
    glVertex3f (lado, 0, 0);
    glVertex3f (lado, 0, lado);
  }
  glEnd ();
  }
};

Piramide p1(10,10);

class Ejes:Objeto3D 
{ 
public: 
    float longitud = 30;
// Dibuja el objeto
void draw( )
{
  //se desactiva para que no se le apliquen las sombras, mas abajo se vuelve a activar
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


/**	void Dibuja( void )

Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.

**/

void Dibuja (void)
{
  static GLfloat  pos[4] = { 5.0, 5.0, 10.0, 0.0 };	// Posicion de la fuente de luz

  float  color[4] = { 0.8, 0.0, 1, 1 };
  float  color2[4] = { 1, 0.2, 0.2, 1 };
  float  color3[4] = {0.0, 0.0, 1, 1};

  glPointSize(8);
  glPushMatrix ();		// Apila la transformacion geometrica actual
  if(luz==true){
    glPolygonMode(GL_FRONT_AND_BACK, modo); //Ponemos el modo de visión
  }
  glClearColor (0.0, 0.0, 0.0, 1.0);	// Fija el color de fondo a negro
  //poner todo a 1 para que se ponga blanco
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer

  transformacionVisualizacion ();	// Carga transformacion de visualizacion

  glLightfv (GL_LIGHT0, GL_POSITION, pos);	// Declaracion de luz. Colocada aqui esta fija en la escena

  ejesCoordenadas.draw();			// Dibuja los ejes
    if(luz==true){
      glEnable(GL_LIGHTING);
    }else{
      glDisable(GL_LIGHTING);
    }
  glTranslatef(12.0,0.0,0.0);
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  glColor3f (color[0], color[1], color[2]);
  c1.draw();
  
  glTranslatef(-12.0,0.0,12.0);
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
  glColor3f (color2[0], color2[1], color2[2]);
  p1.draw();

  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color3);
  glColor3f (color3[0], color3[1], color3[2]);
  //Piramide superior
  glTranslatef(10.0,20.0,10.0);
  p1.draw();
  //Piramide inferior
  glTranslatef(0.0,-10.0, 10.0);
  glRotatef(-180,1,0,0);
  p1.draw();
  //Piramide trasera
  glTranslatef(0.0,0.0, 10.0);
  glRotatef(90,1,0,0);
  p1.draw();
  //Piramide derecha
  glTranslatef(10.0, 0.0, 0.0);
  glRotatef(-90,0,0,1);
  p1.draw();
  //Piramide delatera
  glTranslatef(10.0, 0.0, 0.0);
  glRotatef(270,0,0,1);
  p1.draw();
  //Piramide izquierda
  glTranslatef(10.0, 0.0, 0.0);
  glRotatef(-90,0,0,1);
  p1.draw();


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
