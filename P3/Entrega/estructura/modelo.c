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
#include "estructura.h"

/**	void initModel()

Inicializa el modelo y de las variables globales


**/
using namespace std;
void
initModel ()
{
  
}

//Estructura de datos que apoya el movimiento de la pinza
typedef struct mov_pinza{
  bool estado=false, abrir=false, parar=false;
  const float min=0, max=52;
  float actual=0;
}mov_pinza;
//Estructura de datos que apoya el movimiento de las piernas
typedef struct mov_piernas{
  bool abrir=false;
  const float min=0, max=30;
  float actual=0.0;
}mov_piernas;
//Estructura de datos que apoya el movimiento de la cola
typedef struct mov_cola{
  bool estado=false, abrir=false, parar=false;
  const float min=0, max=32;
  float actual=0.0;
}mov_cola;

//Declaramos las estructuras
mov_pinza m1;
vector<mov_piernas> m2(4);
mov_cola m3;

//Inicializamos el vector de las piernas a valores sinusoidales
int parte=0;
void inicializa(){
  if(parte==0){
    m2[0].actual=0;
    m2[1].actual=15;
    m2[2].actual=30;
    m2[3].actual=15;  
    parte=1;
  }
}

//Activamos el movimiento de las pinzas
void activar_pinza(){
  m1.estado=true; 
}

//Activamos el movimiento de la cola
void activar_cola(){
  m3.estado=true;
}

//Funcion que realiza el calculo del valor de la posicion de la pinza en su movimiento
void mover_pinza(){
  if(m1.estado==true){
    if(m1.abrir==true){
      m1.actual +=2;
    }else{
      m1.actual -=2;
    }
    if(m1.min>m1.actual){
      m1.abrir=true;
      if(m1.parar==true && m1.abrir==true){
        m1.actual=0;
        m1.estado=false;
        m1.parar=false;
      }
    }else if(m1.actual>m1.max){
      m1.abrir=false;
      m1.parar=true;
    }
  }
}

//Funcion que realiza el calculo del valor de la posicion de la cola en su movimiento
void mover_cola(){
  if(m3.estado==true){
    if(m3.abrir==true){
      m3.actual+=2;
    }else{
      m3.actual-=2;  
    }
    
    if(m3.min>m3.actual){
      m3.abrir=true;
      if(m3.parar==true && m3.abrir==true){
        m3.actual=0;
        m3.estado=false;
        m3.parar=false;
      }
    }else if(m3.actual>m3.max){
      m3.abrir=false;
      m3.parar=true;
    }
  }
}

//Funcion que realiza el calculo del valor de la posicion de las piernas en su movimiento
void mover_piernas(){
  for(int i=0; i<m2.size(); i++){
    if(m2[i].abrir==true){
      m2[i].actual+=2;
    }else{
      m2[i].actual-=2;
    }
    if(m2[i].min>m2[i].actual){
      m2[i].abrir=true;
      m2[i].actual=0;
    }else if(m2[i].actual>m2[i].max){
      m2[i].abrir=false;
      m2[i].actual=30;
    }
  }
}

//Clase piramide para los dientes de la pinza
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
Piramide p1(4, 7);

/**	void Dibuja( void )

Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.

**/

//Funcion que crea la mandibula de la pinza
void mandibula (){
  glScaled(0.2, 0.2, 0.2);
  //Dibujamos el diente numero uno
  glPushMatrix();
    glRotatef(90,1,0,0);
    glTranslatef(10.0, 0.0, -2.0);
    glEnable(GL_NORMALIZE);
    p1.draw();
    glDisable(GL_NORMALIZE);
  glPopMatrix();

  //Dibujamos el diente numero dos
  glPushMatrix();
    glRotatef(90,1,0,0);
    glTranslatef(-10.0, 0.0, -2.0);
    glEnable(GL_NORMALIZE);
    p1.draw();
    glDisable(GL_NORMALIZE);
  glPopMatrix();

  //Dibujamos el diente numero tres
  glPushMatrix();
    glRotatef(90,1,0,0);
    glTranslatef(-20.0, -1.0, -2.0);
    glEnable(GL_NORMALIZE);
    p1.draw();
    glDisable(GL_NORMALIZE);
  glPopMatrix();

  //Dibujamos el diente numero cuatro
  glPushMatrix();
    glRotatef(90,1,0,0);
    glTranslatef(0.0, 0.0, -2.0);
    glEnable(GL_NORMALIZE);
    p1.draw();
    glDisable(GL_NORMALIZE);
  glPopMatrix();
  
  //Generamos el cuerpo de la mandibula
  glPushMatrix();
    glScaled(5.0, 1.0, 0.5);
    glEnable(GL_NORMALIZE);
    glutSolidSphere(5, 50, 50);
    glDisable(GL_NORMALIZE);
  glPopMatrix();
}

//Funcion que genera la pinza
void pinza(){
  glPushMatrix();
    glTranslated(17, 0, 0);
    glRotated(25, 0, 1, 0);
    //Generamos una mandibula
    glPushMatrix();
      glRotatef(110, 0, 1, 0);
      glTranslated(-5, 0, 0);   
      mandibula();
    glPopMatrix();
    
    //Generamos otra mandibula
    glPushMatrix();
      glRotatef(180, 0, 1, 0);
      glRotatef(180, 1, 0, 0);
      //-------------------
      glRotated(m1.actual,0, 1, 0);
      //-------------------
      glTranslated(-5, 0, 0);
      glEnable(GL_NORMALIZE);
      mandibula();
      glDisable(GL_NORMALIZE);
    glPopMatrix();
    
    //Generamos la muñeca de la pinza
    glPushMatrix();
      glEnable(GL_NORMALIZE);
      glutSolidSphere(1.6, 50, 50);
      glDisable(GL_NORMALIZE);
    glPopMatrix();
  glPopMatrix();

  //Generamos el brazo de la pinza
  glPushMatrix();
    glTranslated(9, 0, 0);
    glScaled(3, 0.5, 0.5);
    glEnable(GL_NORMALIZE);
    glutSolidSphere(3, 50, 50);
    glDisable(GL_NORMALIZE);
  glPopMatrix();
}

//Dibujamos una pata de la langosta
void pata(int par){
  int num=0;
  if(par==1){
    num=m2[0].actual;
  }else if(par==2){
    num=m2[1].actual;
  }else if(par==3){
    num=m2[2].actual;
  }else if(par==4){
    num=m2[3].actual;
  }else{
    cout<<"Error imposible";
  }
  //Dibujamos la parte inferior de la langosta
  glRotated(num, 0, 0, 1); //Movimiento parte superior
  glPushMatrix();
    glTranslated(3, -3, 0);
    glPushMatrix();
      glRotated(num,0,0,1); //Movimiento parte de abajo
      glRotated(280, 0, 0, 1);
      glTranslated(2.2, 0, 0);
      glScaled(0.8, 0.15, 0.15);
      glEnable(GL_NORMALIZE);
      glutSolidSphere(3, 50, 50);
      glDisable(GL_NORMALIZE);
    glPopMatrix();
  glPopMatrix();
  //Dibujamos la parte superior de la pata
  glPushMatrix();
    glRotated(-45, 0, 0, 1);
    glTranslated(2.2, 0, 0);
    glScaled(0.8, 0.15, 0.15);
    glEnable(GL_NORMALIZE);
    glutSolidSphere(3, 50, 50);
    glDisable(GL_NORMALIZE);
  glPopMatrix(); 
}

//Dibujamos un par de piernas de la langosta
void piernas(float x, int par){
  glPushMatrix();
    glTranslated(0,0,x);
      //Dibujamos una pata
      glPushMatrix();
        glTranslated(5,0,0);
        pata(par);
      glPopMatrix();
      //Dibujamos la pata del otro lado del cuerpo
      glPushMatrix();
        glRotated(180, 0, 1, 0);
        pata(par);
      glPopMatrix();
  glPopMatrix();  
}

//Funcion para la creacion de la cola
void cola(){
  glPushMatrix();
    glRotated(90, 0, 1, 0);
    glTranslated(-8, 0, 3);
    glScaled(1.8, 0.35, 1);
    glEnable(GL_NORMALIZE);
    glutSolidSphere(3, 50, 50);
    glDisable(GL_NORMALIZE);
  glPopMatrix(); 
  glPushMatrix(); 
    glScaled(1.2, 0.1, 0.8);
    glTranslated(2.5, 0, 4);
    glEnable(GL_NORMALIZE);
    glutSolidSphere(3, 50, 50);
    glDisable(GL_NORMALIZE);
  glPopMatrix();
}

void Dibuja (void){
  static GLfloat  pos[4] = { 5.0, 5.0, 10.0, 0.0 };	// Posicion de la fuente de luz

  float  color[4] = { 1, 0.0, 0.0, 1 };
  //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
  glPushMatrix ();		// Apila la transformacion geometrica actual

  glClearColor (0.0, 0.0, 0.0, 1.0);	// Fija el color de fondo a negro

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer

  transformacionVisualizacion ();	// Carga transformacion de visualizacion

  glLightfv (GL_LIGHT0, GL_POSITION, pos);	// Declaracion de luz. Colocada aqui esta fija en la escena

  ejesCoordenadas.draw();			// Dibuja los ejes
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  //-----------------------------------------------------
  //CUERPO
  glPushMatrix();
  glRotated(90, 0, 1, 0);
  glScaled(4,0.8,1);
  glTranslated(-3,0,2.6);
  glEnable(GL_NORMALIZE);
  glutSolidSphere(3, 50, 50);
  glDisable(GL_NORMALIZE);
  glPopMatrix();
  //-----------------------------------------------------

  //-------------------------------------------------
  //COLA
  glPushMatrix();
    glRotated(m3.actual-16,1,0,0);
    glScaled(0.8, 0.8, 0.8);
    glTranslated(0.2, 0, -10);
    cola();
  glPopMatrix();
  //----------------------------------------------------

  //----------------------------------------------------
  //PINZA IZQUIERDA
  glPushMatrix();
  glTranslated(4, 0, 20);
  glScaled(0.4, 0.4, 0.4);
  glRotated(-35,0,1,0);
  pinza();
  glPopMatrix();
  //----------------------------------------------------

  //----------------------------------------------------
  //PINZA DERECHA
  glPushMatrix();
  glRotated(180,0,0,1);
  glTranslated(-1, 0, 20);
  glScaled(0.4, 0.4, 0.4);
  glRotated(-35,0,1,0);
  pinza();
  glPopMatrix();
  //----------------------------------------------------

  //-----------------------------------------------------
  //Piernas
  piernas(6, 1);
  piernas(9.5, 2);
  piernas(13, 3);
  piernas(16.5, 4);
  //-----------------------------------------------------

  glPopMatrix ();		// Desapila la transformacion geometrica

  glutSwapBuffers ();		// Intercambia el buffer de dibujo y visualizacion
}


/**	void idle()

Procedimiento de fondo. Es llamado por glut cuando no hay eventos pendientes.

**/
void idle (int v)
{
  //Funciones de movimiento
  mover_piernas();
  mover_pinza();
  mover_cola();
  inicializa();
  glutPostRedisplay ();		// Redibuja
  glutTimerFunc (30, idle, 0);	// Vuelve a activarse dentro de 30 ms
}
