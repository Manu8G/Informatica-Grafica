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
	modulo mouse.c
	Gestion de eventos de raton

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"
/**	 void clickRaton( int boton, int estado, int x, int y )

Procedimiento para gestionar los eventos de pulsacion de los botones del raton.

Argumentos:

boton: Codigo glut del boton que ha manipulado el usuario.

estado: Estado en el que esta el boton que se ha pulsado.

x,y: Posicion, en coordenadas de pantalla, en que se encuantra el cursor.

**/
using namespace std;

bool MOVIENDO_CAMARA=false;
float xant=0, yant=0;
float iniX=0, iniY=0;
float priX=0, priY=0;
float postX=0, postY=0;
bool reset=true;
 

void clickRaton (int boton, int estado, int x, int y){
	if(boton == GLUT_MIDDLE_BUTTON){
		if(estado == GLUT_DOWN){
			MOVIENDO_CAMARA=true;
		}else{
			MOVIENDO_CAMARA=false;
			reset=true;
		}
	}
}


/**	void RatonMovido( int x, int y )

Procedimiento para gestionar los eventos de movimiento del raton.

Argumentos:

x,y: Posicion, en coordenadas de pantalla, en que se encuantra el cursor.

**/

void RatonMovido (int x, int y){
	if(MOVIENDO_CAMARA){
		//Girar la camara usando segun el vector(x−xant, y−yant);
		if(reset==true){
			priX=x;
			priY=y;
			reset=false;
		}
		xant=iniX+(postX-priX);
		yant=iniY+(postY-priY);
		ajustarV(xant, yant);
		setPosCamara(xant, yant);
	}
	
	glutPostRedisplay ( ) ;
}

void ajustarM(float x, float y){
	cout<<"Ajustar mouse antes x:"<<xant<<", y: "<<yant<<", ahora x:"<<x<<", y: "<<y<<endl;
	xant=x;
	yant=y;
}
