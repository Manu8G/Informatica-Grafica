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
modulo entradaTeclado.c
	Gestion de eventos de teclado
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"
#include "visual.h"



/** 

Imprime en la consola las instrucciones del programa

**/
void printHelp ()
{

  printf ("\n\n     Prácticas de Informática Gráfica	Curso 2022-2023");
  printf ("\n\n Dpto. Lenguajes y Sistemas Informaticos");
  printf ("\n E.T.S.I. Informatica		Univ. de Granada ");
  printf ("\n");
  printf ("\n Opciones: \n\n");
  printf ("h, H: Imprime informacion de ayuda \n");
  printf ("PgUp, PgDn: avanza y retrocede la cámara \n\n");
  printf ("+,-: avanza y retrocede la cámara \n\n");
  printf ("Teclas de movimiento de cursor: giran la camara\n");
  // Anyade la informacion de las opciones que introduzcas aqui !!       

  printf ("\n Escape: Salir");
  printf ("\n\n\n");
}




/* @teclado ---------------------------------------------------------------- */

/** 		void letra (unsigned char k, int x, int y)

Este procedimiento es llamado por el sistema cuando se pulsa una tecla normal
El codigo k es el ascii de la letra

Para anyadir nuevas ordenes de teclado coloca el correspondiente case.

Parametros de entrada:

k: codigo del caracter pulsado

x:

y:

**/

float rotxCamara = 0, rotyCamara = 0;
float CX = 0;
float CY = 0;
float CZ = 25;
bool orto=false;
bool cambio1=false;

void setPosCamara(float rx, float ry){
  rotxCamara = rx;
  rotyCamara = ry;
}



void letra (unsigned char k, int x, int y)
{

  switch (k)
    {
    case 'h':
    case 'H':
      printHelp ();		// H y h imprimen ayuda
      break;
    case '+':			// acerca la cámara
      CZ -= 5.0;
      break;
    case '-':			// aleja la cámara
      CZ += 5.0;
      break;
    case '1':	          //Activamos la luz 2 y desactviamos la 1
      cambiarLuz(1);
      break;
    case '2':           //Activamos la luz 1 y desactivamos la 2 
      cambiarLuz(2);
      break;
    case '3':
      cambiarLuz(3);    //Desactivamos la luz 1 y la 2
      break;
    case 'a':	          //Desplazamos la camara a la izquierda
    case 'A':
      CX += 5.0;
      break;
    case 'd':	          //Desplazamos la camara a la derecha
    case 'D':
      CX -= 5.0;
      break;
    case 'w':	          //Desplazamos la camara a la hacia delante
    case 'W':
      CZ -= 5.0;
      break;
    case 's':	          //Desplazamos la camara a la atras
    case 'S':
      CZ += 5.0;
      break;
    case 27:			// Escape  Terminar
      exit (0);
    default:
      return;
    }
  setCamara (rotxCamara, rotyCamara, CX, CZ, orto);
  glutPostRedisplay ();		// Algunas de las opciones cambian paramentros
}				// de la camara. Es necesario actualziar la imagen

/**		void especial(int k, int x, int y)
Este procedimiento es llamado por el sistema cuando se pulsa una tecla
especial. El codigo k esta definido en glut mediante constantes

Parametros de entrada:

k: codigo del caracter pulsado (definido en glut mediante constantes).

x:

y:

**/
void especial (int k, int x, int y)
{

  switch (k)
    {
    case GLUT_KEY_UP:
      rotxCamara += 5.0;	// Cursor arriba + rotacion x
      if (rotxCamara > 360)
	      rotxCamara -= 360;
      break;
    case GLUT_KEY_DOWN:
      rotxCamara -= 5.0;
      if (rotxCamara < 0)
	rotxCamara += 360;
      break;
    case GLUT_KEY_LEFT:
      rotyCamara += 5.0;
      if (rotyCamara > 360)
	rotyCamara -= 360;
      break;
    case GLUT_KEY_RIGHT:
      rotyCamara -= 5.0;
      if (rotyCamara < 0)
	rotyCamara += 360;
      break;
    case GLUT_KEY_PAGE_DOWN:	// acerca la cámara
      CZ -= 5.0;
      break;
    case GLUT_KEY_PAGE_UP:	// aleja la cámara
      CZ += 5.0;
      break;
    case GLUT_KEY_F1:	//Vista planta
      rotxCamara = 90, rotyCamara = 0; //CX=0; CZ=25;
      if(orto==true){
        cambio1=true;
      }
      orto=false;
      break;
    case GLUT_KEY_F2:	//Vista alzado
      rotxCamara = 0, rotyCamara = -90; //CX=0; CZ=25;
      if(orto==true){
        cambio1=true;
      }
      orto=false;
      break;
    case GLUT_KEY_F3:	//Vista perfil
      rotxCamara = 0, rotyCamara = 0; //CX=0; CZ=25;
      if(orto==false){
        cambio1=true;
      }
      orto=true;
      break;
    default:
      return;
    }
  setCamara (rotxCamara, rotyCamara, CX, CZ, orto);
  if(cambio1){
    cambio1=false;
    setCambio();
  }
  glutPostRedisplay ();		// Actualiza la imagen (ver proc. letra)
}
