#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <fstream>

#define up (GetKeyState(VK_UP) & 0x8000)
#define down (GetKeyState(VK_DOWN) & 0x8000)
#define left (GetKeyState(VK_LEFT) & 0x8000)
#define right (GetKeyState(VK_RIGHT) & 0x8000)
#define esc (GetKeyState(27) & 0x8000)
#define space (GetKeyState(32) & 0x8000)

/*
	for (i = 0; i < 2; i++){
		for (j = 0; j < 2; j++){
			
		}
	}
	hacerlo de 80x50 queda mejor
*/

using namespace std;

class Enemigo{
	private: // 100 = frutas, 200 = enemigos, 300 = bloques, 400 = otros objetos
		int tipo;
		int diren;
	public:
		int xe, ye;
		Enemigo(int);
		~Enemigo();
		void mover(int);
		void posicionar();
		bool chocar(int,int);
		bool hitboxvacioenemy(int,int);
};

class Fruta{
	private:
		int tipo;
		int dirfr;
	public:
		bool active;
		int xf, yf;
		Fruta(int);
		~Fruta();
		void interactuar();
};

Enemigo::Enemigo(int _tipo){
	tipo = _tipo;
	diren = 0;
}

Enemigo::~Enemigo(){}

Fruta::Fruta(int _tipo){
	tipo = _tipo;
	dirfr = 0;
	active = false;
}

Fruta::~Fruta(){}

void gotoxy(int,int);

void vaciarnivel();
void cargarnivel();
void llenarnivel();
void mostrarnivel();
void vaciarvariables();
void mostrarfrutas();

void moverprota();
void posicionprota();
void interprota();
void aproxmov();
void compmovimientoX();
void compmovimientoY();
void comphielo();
void interhielo();
void crearhielo(int);
void romperhielo(int);
void adminhielo();

bool hitboxbloques();
bool hitboxvacio(int);
bool hitboxhielo(int);
bool hitboxenemigos();
bool hitboxfruta();

void adminobjetos();
void llenarfrutas();
void adminfrutas();
void llenarbarrafrutas();

void tiempo();
void ganar();
void perder();

Enemigo toroverde[10] = Enemigo(200);
Fruta banana[30] = Fruta(100);
Fruta uva[30] = Fruta(101);
Fruta sandia[30] = Fruta(102);

char levelobj[25][80];
int i, j;
int levelenemy[160][50];
int levelfruit[160][50];
int xp, yp;
float yp2, xp2;
int dirper = 0, diriceP1[10];
int timper = 5;
int xcP1[10], ycP1[10];
int actice[10];
int cantice = -1;
bool actspace = false;
float waiticeP1 = 3;
bool confice[10];
int cantenemy1 = 0, cantenemy2 = 0;
int cantfrutas = 0;
int maxfrutas = 0;
int milseg = 0;

int main(){
	system("mode con: cols=160 lines=50");
	vaciarvariables();
	vaciarnivel();
	cargarnivel();
	llenarnivel();
	llenarbarrafrutas();
	while(!esc){
		tiempo();
		if (cantfrutas <= 1 && xp%2==0 && yp%2==0){
			llenarfrutas();
		}
		if (milseg%200==0){
			moverprota();
		}
		
		mostrarnivel();
		mostrarfrutas();
		adminhielo();
		adminobjetos();
	}
	
	getch();
	return 0;
}

void gotoxy(int x1, int y1){
	HANDLE hCon;
	COORD dwPos;
	
	dwPos.Y = y1;
	dwPos.X = x1;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hCon,dwPos);
}

void aproxmov(){
	xp = (int) xp2;
	yp = (int) yp2;
}

void vaciarvariables(){
	for (i = 0; i < 10; i++){
		diriceP1[i] = 0;
		actice[i] = 0;
		confice[i] = false;
	}
}

void vaciarnivel(){
	for (i = 0; i < 160; i++){
		for (j = 0; j < 50; j++){
			levelenemy[i][j] = 0;
			levelfruit[i][j] = 0;
		}
	}
}

void moverprota(){
	
	if (!left && !right && !down && !up){
		aproxmov();
		compmovimientoX();
		compmovimientoY();
		
		timper = 5;
		xp2 = xp; yp2 = yp;
	}
	
	if ((up || down)){
		compmovimientoX();
		if (up){
			dirper = 2;
			timper -= 2;
			if (timper <= 0){
				yp2 -= 0.75;
			}
			if (hitboxenemigos()){
				perder();
			}
			if (hitboxbloques()){
				yp2 += 0.75;
			}
			if (hitboxfruta()){
				adminfrutas();
			}
		} else{
			dirper = 0;
			timper -= 1;
			if (timper <= 2){
				yp2 += 0.75;
			}
			if (hitboxenemigos()){
				perder();
			}
			if (hitboxbloques()){
				yp2 -= 0.75;
			}
			if (hitboxfruta()){
				adminfrutas();
			}
		}
		posicionprota();
	} else if (left || right){
		compmovimientoY();
		if (left){
			dirper = 1;
			timper -= 2;
			if (timper <= 0){
				xp2 -= 0.75;
			}
			if (hitboxenemigos()){
				perder();
			}
			if (hitboxbloques()){
				xp2 += 0.75;
			}
			if (hitboxfruta()){
				adminfrutas();
			}
		} else {
			dirper = 3;
			timper -= 1;
			if (timper <= 2){
				xp2 += 0.75;
			}
			if (hitboxenemigos()){
				perder();
			}
			if (hitboxbloques()){
				xp2 -= 0.75;
			}
			if (hitboxfruta()){
				adminfrutas();
			}
		}
		posicionprota();
	}
	
	if (space && waiticeP1 == 3 && cantice < 9 && (xp%2==0) && (yp%2==0) && !actspace){
		actspace = true;
		comphielo();
	}
	if (!space) actspace = false;
}

void posicionprota(){
	aproxmov();
	for (i = 0; i < 2; i++){
		for (j = 0; j < 2; j++){
			levelenemy[xp+i][yp+j] = 1;
			levelfruit[xp+i][yp+j] = 1;
		}
	}
	for (i = -1; i < 3; i++){
		if (levelenemy[xp-1][yp+i] == -2){
			levelenemy[xp-1][yp+i] = 0; //borrar la posicion anterior del protagonista
		}
		if (levelenemy[xp+2][yp+i] == -2){
			levelenemy[xp+2][yp+i] = 0;
		}
		if (levelenemy[xp+3][yp+i] == -2){
			levelenemy[xp+3][yp+i] = 0;
		}
		if (levelenemy[xp+i][yp-1] == -2){
			levelenemy[xp+i][yp-1] = 0;
		}
		if (levelenemy[xp+i][yp+2] == -2){
			levelenemy[xp+i][yp+2] = 0;
		}
		if (levelenemy[xp+i][yp+3] == -2){
			levelenemy[xp+i][yp+3] = 0;
		}
	}
	for (i = -1; i < 3; i++){
		if (levelfruit[xp-1][yp+i] == -2){
			levelfruit[xp-1][yp+i] = 0;
		}
		if (levelfruit[xp+2][yp+i] == -2){
			levelfruit[xp+2][yp+i] = 0;
		}
		if (levelfruit[xp+3][yp+i] == -2){
			levelfruit[xp+3][yp+i] = 0;
		}
		if (levelfruit[xp+i][yp-1] == -2){
			levelfruit[xp+i][yp-1] = 0;
		}
		if (levelfruit[xp+i][yp+2] == -2){
			levelfruit[xp+i][yp+2] = 0;
		}
		if (levelfruit[xp+i][yp+3] == -2){
			levelfruit[xp+i][yp+3] = 0;
		}
	}
}

void mostrarnivel(){
	for (i = 0; i < 160; i++){
		for (j = 0; j < 50; j++){
			if (i == 159 && j == 49){
			} else {
				switch(levelenemy[i][j]){
					case 0:
						if (levelfruit[i][j] > -100 || levelfruit[i][j] <= -200){
							gotoxy(i,j); printf(" "); //vaciar espacio
							levelenemy[i][j] = -1;
						}
						break;
					case 1:
						gotoxy(i,j); printf("#"); //mostrar espacio del jugador
						levelenemy[i][j] = -2;
						break;
					case 300:
						gotoxy(i,j); printf("B"); //mostrar un bloque
						levelenemy[i][j] = -300;
						break;
					case 301:
						if (j%2==1){
							gotoxy(i,j); printf("H"); //mostrar la mitad inferior de un bloque de hielo
							levelenemy[i][j] = -301;
						} else {
							levelenemy[i][j] = -311;
						}
						break;
					case -311:
						gotoxy(i,j); printf("H"); //mostrar la parte superio de un bloque de hielo
						levelenemy[i][j] = -301;
						break;
					case 311:
						if (milseg%175==85){
						gotoxy(i,j); printf("A"); // mostrar sprite 1 de romper el hielo }
						levelenemy[i][j] = -321; //											 }
						}
						break; // 														 }
					case -321: //														 } animacion para romper
						if (milseg%175==0){
						gotoxy(i,j); printf("S"); // mostrar sprite 2 de romper el hielo } el hielo
						levelenemy[i][j] = -331; //											 }
						}
						break; //														 }
					case -331: //														 }
						if (milseg%175==85){
						gotoxy(i,j); printf("D"); // mostrar sprite 3 y eliminar el bloque}
						levelenemy[i][j] = 0;
						}
						break;
					case 200:
						gotoxy(i,j); printf("E");
						levelenemy[i][j] = -200;
						break;
					default: break;
				}
			}
		}
	}
}

void mostrarfrutas(){
	for (i = 0; i < 160; i++){
		for (j = 0; j < 50; j++){
			if (i == 159 && j == 49){
			} else {
				switch(levelfruit[i][j]){
					case 0:
						gotoxy(i,j); printf(" ");
						levelfruit[i][j] = -1;
						break;
					case 1:
						gotoxy(i,j); printf("#");
						levelfruit[i][j] = -2;
						break;
					case 100:
						gotoxy(i,j); printf("b");
						levelfruit[i][j] = -100;
						if (levelenemy[i][j] <= -300 && levelenemy[i][j] > -400){
							if (i%2==1 || j%2==1){
								gotoxy(i,j); printf("H");
							}
							levelfruit[i][j] = -110;
						}
						break;
					case 101:
						gotoxy(i,j); printf("u");
						levelfruit[i][j] = -101;
						if (levelenemy[i][j] <= -300 && levelenemy[i][j] > -400){
							if (i%2==1 || j%2==1){
								gotoxy(i,j); printf("H");
							}
							levelfruit[i][j] = -111;
						}
						break;         
					case 102:
						gotoxy(i,j); printf("s");
						levelfruit[i][j] = -102;
						if (levelenemy[i][j] <= -300 && levelenemy[i][j] > -400){
							if (i%2==1 || j%2==1){
								gotoxy(i,j); printf("H");
							}
							levelfruit[i][j] = -112;
						}
						break;
					case -100:
						if (levelenemy[i][j] != -1 && (milseg%500== 0)){
							levelfruit[i][j] = 100;
						} break;
					case -101:
						if (levelenemy[i][j] != -1 && (milseg%500== 0)){
							levelfruit[i][j] = 101;
						} break;
					case -102:
						if (levelenemy[i][j] != -1 && (milseg%500== 0)){
							levelfruit[i][j] = 102;
						} break;
					case -110:
						if (levelenemy[i][j] == 0){
							levelfruit[i][j] = 100;
						} break;
					case -111:
						if (levelenemy[i][j] == 0){
							levelfruit[i][j] = 101;
						} break;
					case -112:
						if (levelenemy[i][j] == 0){
							levelfruit[i][j] = 102;
						} break;
				}
			}
		}
	}
}

void llenarnivel(){
	for (i = 0; i < 80; i++){
		for (j = 0; j < 25; j++){
			switch(levelobj[j][i]){
				case 'n':
					levelenemy[i*2][j*2] = -1;
					levelenemy[(i*2)+1][j*2] = -1;
					levelenemy[i*2][(j*2)+1] = -1;
					levelenemy[(i*2)+1][(j*2)+1] = -1; //colocar espacios vacios
					levelfruit[i*2][j*2] = -1;
					levelfruit[(i*2)+1][j*2] = -1;
					levelfruit[i*2][(j*2)+1] = -1;
					levelfruit[(i*2)+1][(j*2)+1] = -1;
					break;
				case 'p':
					levelenemy[i*2][j*2] = 1;
					levelenemy[(i*2)+1][j*2] = 1;
					levelenemy[i*2][(j*2)+1] = 1;
					levelenemy[(i*2)+1][(j*2)+1] = 1; //colocar al personaje
					levelfruit[i*2][j*2] = 1;
					levelfruit[(i*2)+1][j*2] = 1;
					levelfruit[i*2][(j*2)+1] = 1;
					levelfruit[(i*2)+1][(j*2)+1] = 1;
					xp = i*2; yp = j*2;
					xp2 = xp; yp2 = yp;
					break;
				case 'b':
					levelenemy[i*2][j*2] = 300;
					levelenemy[(i*2)+1][j*2] = 300;
					levelenemy[i*2][(j*2)+1] = 300;
					levelenemy[(i*2)+1][(j*2)+1] = 300; //colocar bloques solidos
					levelfruit[i*2][j*2] = 300;
					levelfruit[(i*2)+1][j*2] = 300;
					levelfruit[i*2][(j*2)+1] = 300;
					levelfruit[(i*2)+1][(j*2)+1] = 300;
					break;
				case 'h':                    
					levelenemy[i*2][j*2] = -311;
					levelenemy[(i*2)+1][j*2] = -311;
					levelenemy[i*2][(j*2)+1] = 301;
					levelenemy[(i*2)+1][(j*2)+1] = 301; //colocar bloques de hielo
					levelfruit[i*2][j*2] = -311;
					levelfruit[(i*2)+1][j*2] = -311;
					levelfruit[i*2][(j*2)+1] = 301;
					levelfruit[(i*2)+1][(j*2)+1] = 301;
					break;
				case 'e':
					levelenemy[i*2][j*2] = 200;
					levelenemy[(i*2)+1][j*2] = 200;
					levelenemy[i*2][(j*2)+1] = 200;
					levelenemy[(i*2)+1][(j*2)+1] = 200; //colocar enemigos
					toroverde[cantenemy1].xe = i*2;
					toroverde[cantenemy1].ye = j*2; //establecer posicion del enemigo
					cantenemy1++; //sumar la cantidad de enemigos
					break;
				default: break;
			}
		}
	}
}

void llenarfrutas(){
	cantfrutas = 0;
	for (j = 0; j < 25; j++){
		switch(levelobj[j][0]){
			case 'B': 
				for (int k = 0; k < 80; k++){
					for (int l = 0; l < 25; l++){
						if (levelobj[l][k] == 'B'){
							levelfruit[k*2][l*2] = 100;
							levelfruit[(k*2)+1][l*2] = 100;
							levelfruit[k*2][(l*2)+1] = 100;
							levelfruit[(k*2)+1][(l*2)+1] = 100; //colocar frutas (bananas)
							banana[cantfrutas].xf = k*2;
							banana[cantfrutas].yf = l*2;
							banana[cantfrutas].active = true;
							cantfrutas++;
							levelobj[l][k] = 'N';
							maxfrutas = cantfrutas;
						}
					}
				} return;
			case 'U':
				for (int k = 0; k < 80; k++){
					for (int l = 0; l < 25; l++){
						if (levelobj[l][k] == 'U'){
							levelfruit[k*2][l*2] = 101;
							levelfruit[(k*2)+1][l*2] = 101;
							levelfruit[k*2][(l*2)+1] = 101;
							levelfruit[(k*2)+1][(l*2)+1] = 101; //colocar frutas (uvas)
							uva[cantfrutas].xf = k*2;
							uva[cantfrutas].yf = l*2;
							uva[cantfrutas].active = true;
							cantfrutas++;
							levelobj[l][k] = 'N';
							maxfrutas = cantfrutas;
						}
					}
				} return;
			case 'S':
				for (int k = 0; k < 80; k++){
					for (int l = 0; l < 25; l++){
						if (levelobj[l][k] == 'S'){
							levelfruit[k*2][l*2] = 102;
							levelfruit[(k*2)+1][l*2] = 102;
							levelfruit[k*2][(l*2)+1] = 102;
							levelfruit[(k*2)+1][(l*2)+1] = 102; //colocar frutas (sandías)
							sandia[cantfrutas].xf = k*2;
							sandia[cantfrutas].yf = l*2;
							sandia[cantfrutas].active = true;
							cantfrutas++;
							levelobj[l][k] = 'N';
							maxfrutas = cantfrutas;
						}
					}
				} return;
			default: break;
		}
	}
	ganar();
}

void llenarbarrafrutas(){
	for (j = 0; j < 25; j++){
		switch(levelobj[j][0]){
			case 'B':
				levelfruit[0][j*2] = 100;
				levelfruit[1][j*2] = 100;
				levelfruit[0][(j*2)+1] = 100;
				levelfruit[1][(j*2)+1] = 100; break;
			case 'U':
				levelfruit[0][j*2] = 101;
				levelfruit[1][j*2] = 101;
				levelfruit[0][(j*2)+1] = 101;
				levelfruit[1][(j*2)+1] = 101; break;
			case 'S':
				levelfruit[0][j*2] = 102;
				levelfruit[1][j*2] = 102;
				levelfruit[0][(j*2)+1] = 102;
				levelfruit[1][(j*2)+1] = 102; break;
			default: break;
		}
	}
}

void cargarnivel(){
	ifstream archivo;
	
	archivo.open("Levels//Level1.lvl", ios::in);
	
	if (archivo.fail()){
		return;
	}
	
	for (i = 0; i < 2001; i++){
		archivo.getline(levelobj[false],2001); //cargar el contenido del archivo al nivel
	}
	
	archivo.close();
}

bool hitboxbloques(){
	aproxmov();
	if ((levelenemy[xp][yp] <= -300 && levelenemy[xp][yp] > -400) ||
	(levelenemy[xp+1][yp] <= -300 && levelenemy[xp+1][yp] > -400) ||
	(levelenemy[xp][yp+1] <= -300 && levelenemy[xp][yp+1] > -400) ||
	(levelenemy[xp+1][yp+1] <= -300 && levelenemy[xp+1][yp+1] > -400)
	){
		return true;
	} else {
		return false;
	}
}

bool hitboxenemigos(){
	aproxmov();
	if ((levelenemy[xp][yp] <= -200 && levelenemy[xp][yp] > -300) ||
	(levelenemy[xp+1][yp] <= -200 && levelenemy[xp+1][yp] > -300) ||
	(levelenemy[xp][yp+1] <= -200 && levelenemy[xp][yp+1] > -300) ||
	(levelenemy[xp+1][yp+1] <= -200 && levelenemy[xp+1][yp+1] > -300) ||
	(levelenemy[xp][yp] >= 200 && levelenemy[xp][yp] < 300) ||
	(levelenemy[xp+1][yp] >= 200 && levelenemy[xp+1][yp] < 300) ||
	(levelenemy[xp][yp+1] >= 200 && levelenemy[xp][yp+1] < 300) ||
	(levelenemy[xp+1][yp+1] >= 200 && levelenemy[xp+1][yp+1] < 300)
	){
		return true;
	} else {
		return false;
	}
}

bool hitboxfruta(){
	aproxmov();
	for (i = 0; i < maxfrutas; i++){
		for (j = -1; j < 2; j++){
			if ((xp == banana[i].xf && yp == banana[i].yf && banana[i].active) ||
			(xp == banana[i].xf+j && yp == banana[i].yf-1 && banana[i].active) ||
			(xp == banana[i].xf+j && yp == banana[i].yf+1 && banana[i].active) ||
			(xp == banana[i].xf-1 && yp == banana[i].yf+j && banana[i].active) ||
			(xp == banana[i].xf+1 && yp == banana[i].yf+j && banana[i].active) ||
			(xp == uva[i].xf && yp == uva[i].yf && uva[i].active) ||
			(xp == uva[i].xf+j && yp == uva[i].yf-1 && uva[i].active) ||
			(xp == uva[i].xf+j && yp == uva[i].yf+1 && uva[i].active) ||
			(xp == uva[i].xf-1 && yp == uva[i].yf+j && uva[i].active) ||
			(xp == uva[i].xf+1 && yp == uva[i].yf+j && uva[i].active) ||
			(xp == sandia[i].xf && yp == sandia[i].yf && sandia[i].active) ||
			(xp == sandia[i].xf+j && yp == sandia[i].yf-1 && sandia[i].active) ||
			(xp == sandia[i].xf+j && yp == sandia[i].yf+1 && sandia[i].active) ||
			(xp == sandia[i].xf-1 && yp == sandia[i].yf+j && sandia[i].active) ||
			(xp == sandia[i].xf+1 && yp == sandia[i].yf+j && sandia[i].active)
			){
				return true;
			}
		}
	}
	return false;
}

bool hitboxvacio(int _i){
	if (levelenemy[xcP1[_i]][ycP1[_i]-1] == -1 &&
	levelenemy[xcP1[_i]+1][ycP1[_i]-1] == -1 &&
	levelenemy[xcP1[_i]][ycP1[_i]] == -1 &&
	levelenemy[xcP1[_i]+1][ycP1[_i]] == -1){
		return true;
	} else {
		return false;
	}

	if (levelfruit[xcP1[_i]][ycP1[_i]-1] == -1 &&
	levelfruit[xcP1[_i]+1][ycP1[_i]-1] == -1 &&
	levelfruit[xcP1[_i]][ycP1[_i]] == -1 &&
	levelfruit[xcP1[_i]+1][ycP1[_i]] == -1){
		return true;
	} else {
		return false;
	}
}

bool hitboxhielo(int _i){
	if (levelenemy[xcP1[_i]][ycP1[_i]-1] == -301 &&
	levelenemy[xcP1[_i]+1][ycP1[_i]-1] == -301 &&
	levelenemy[xcP1[_i]][ycP1[_i]] == -301 &&
	levelenemy[xcP1[_i]+1][ycP1[_i]] == -301){
		return true;
	} else {
		return false;
	}
}

void compmovimientoX(){
	if (xp%2!=0){
		switch(dirper){
			case 1:
				xp--; break;
			case 3:
				xp++; break;
		}
		xp2 = xp; yp2 = yp;
		if (hitboxfruta()){
			adminfrutas();
		}
		posicionprota();
	}
}

void compmovimientoY(){
	if (yp%2!=0){
		switch(dirper){
			case 0:
				yp++; break;
			case 2:
				yp--; break;
		}
		xp2 = xp; yp2 = yp;
		if (hitboxfruta()){
			adminfrutas();
		}
		posicionprota();
	}
}

void comphielo(){
	waiticeP1 = 2;
	do {
		cantice++;
	} while (confice[cantice] && cantice < 10);
	if (cantice >= 10){
		cantice = -1;
		return;
	}
	confice[cantice] = true;
	diriceP1[cantice] = dirper;
	switch (diriceP1[cantice]){
		case 0:
			if (levelenemy[xp][yp+2] == -1){
				actice[cantice] = 1;
				xcP1[cantice] = xp;
				ycP1[cantice] = yp+3;
			} else if (levelenemy[xp][yp+2] == -301){
				xcP1[cantice] = xp;
				ycP1[cantice] = yp+3;
				actice[cantice] = 2;
			} else {
				cantice--;
				confice[cantice] = false;
			} break;
		case 1:
			if (levelenemy[xp-2][yp] == -1){
				xcP1[cantice] = xp-2;
				ycP1[cantice] = yp+1;
				actice[cantice] = 1;
			} else if (levelenemy[xp-2][yp] == -301){
				actice[cantice] = 2;
				xcP1[cantice] = xp-2;
				ycP1[cantice] = yp+1;
			} else {
				cantice--;
				confice[cantice] = false;
			} break;
		case 2:
			if (levelenemy[xp][yp-2] == -1){
				actice[cantice] = 1;
				xcP1[cantice] = xp;
				ycP1[cantice] = yp-1;
			} else if (levelenemy[xp][yp-2] == -301){
				actice[cantice] = 2;
				xcP1[cantice] = xp;
				ycP1[cantice] = yp-1;
			} else {
				cantice--;
				confice[cantice] = false;
			} break;
		case 3:
			if (levelenemy[xp+2][yp] == -1){
				xcP1[cantice] = xp+2;
				ycP1[cantice] = yp+1;
				actice[cantice] = 1;
			} else if (levelenemy[xp+2][yp] == -301){
				xcP1[cantice] = xp+2;
				ycP1[cantice] = yp+1;
				actice[cantice] = 2;
			} else {
				cantice--;
				confice[cantice] = false;
			} break;
	}
}

void crearhielo(int _i){
	switch (diriceP1[_i]){
		case 0:
			if (hitboxvacio(_i)){
				levelenemy[xcP1[_i]][ycP1[_i]] = 301;
				levelenemy[xcP1[_i]+1][ycP1[_i]] = 301;
				levelenemy[xcP1[_i]][ycP1[_i]-1] = 301;
				levelenemy[xcP1[_i]+1][ycP1[_i]-1] = 301;
			} else {
				actice[_i] = 0;
				confice[_i] = false;
				cantice = -1;
			}
			ycP1[_i] += 2;
			break;
		case 1:
			if (hitboxvacio(_i)){
				levelenemy[xcP1[_i]][ycP1[_i]] = 301;
				levelenemy[xcP1[_i]+1][ycP1[_i]] = 301;
				levelenemy[xcP1[_i]][ycP1[_i]-1] = 301;
				levelenemy[xcP1[_i]+1][ycP1[_i]-1] = 301;
			} else {
				actice[_i] = 0;
				confice[_i] = false;
				cantice = -1;
			}
			xcP1[_i] -= 2;
			break;
		case 2:
			if (hitboxvacio(_i)){
				levelenemy[xcP1[_i]][ycP1[_i]] = 301;
				levelenemy[xcP1[_i]+1][ycP1[_i]] = 301;
				levelenemy[xcP1[_i]][ycP1[_i]-1] = 301;
				levelenemy[xcP1[_i]+1][ycP1[_i]-1] = 301;
			} else {
				actice[_i] = 0;
				confice[_i] = false;
				cantice = -1;
			}
			ycP1[_i] -= 2;
			break;
		case 3:
			if (hitboxvacio(_i)){
				levelenemy[xcP1[_i]][ycP1[_i]] = 301;
				levelenemy[xcP1[_i]+1][ycP1[_i]] = 301;
				levelenemy[xcP1[_i]][ycP1[_i]-1] = 301;
				levelenemy[xcP1[_i]+1][ycP1[_i]-1] = 301;
			} else {
				actice[_i] = 0;
				confice[_i] = false;
				cantice = -1;
			}
			xcP1[_i] += 2;
			break;
	}
}

void romperhielo(int _i){
	switch(diriceP1[_i]){
		case 0:
			if (hitboxhielo(_i)){
				levelenemy[xcP1[_i]][ycP1[_i]-1] = 311;
				levelenemy[xcP1[_i]+1][ycP1[_i]-1] = 311;
				levelenemy[xcP1[_i]+1][ycP1[_i]] = 311;
				levelenemy[xcP1[_i]][ycP1[_i]] = 311;
			} else {
				actice[_i] = 0;
				confice[_i] = false;
				cantice = -1;
			}
			ycP1[_i] += 2; break;
		case 1:
			if (hitboxhielo(_i)){
				levelenemy[xcP1[_i]][ycP1[_i]-1] = 311;
				levelenemy[xcP1[_i]+1][ycP1[_i]-1] = 311;
				levelenemy[xcP1[_i]+1][ycP1[_i]] = 311;
				levelenemy[xcP1[_i]][ycP1[_i]] = 311;
			} else {
				actice[_i] = 0;
				confice[_i] = false;
				cantice = -1;
			}
			xcP1[_i] -= 2; break;
		case 2:
			if (hitboxhielo(_i)){
				levelenemy[xcP1[_i]][ycP1[_i]-1] = 311;
				levelenemy[xcP1[_i]+1][ycP1[_i]-1] = 311;
				levelenemy[xcP1[_i]+1][ycP1[_i]] = 311;
				levelenemy[xcP1[_i]][ycP1[_i]] = 311;
			} else {
				actice[_i] = 0;
				confice[_i] = false;
				cantice = -1;
			}
			ycP1[_i] -= 2; break;
		case 3:
			if (hitboxhielo(_i)){
				levelenemy[xcP1[_i]][ycP1[_i]-1] = 311;
				levelenemy[xcP1[_i]+1][ycP1[_i]-1] = 311;
				levelenemy[xcP1[_i]+1][ycP1[_i]] = 311;
				levelenemy[xcP1[_i]][ycP1[_i]] = 311;
			} else {
				actice[_i] = 0;
				confice[_i] = false;
				cantice = -1;
			}
			xcP1[_i] +=2; break;
	}
}

void adminhielo(){
	if (milseg%150==0){
		for (i = 0; i < 10; i++){
			if (actice[i] == 1){
				crearhielo(i);
			} else if (actice[i] == 2){
				romperhielo(i);
			}
		}
	}
	if (waiticeP1 <= 2){waiticeP1 -= 0.1;}
	if  (waiticeP1 <= -4){waiticeP1 = 3;}
}

void adminobjetos(){
	if(milseg%1000==0){
		for (int o = 0; o < cantenemy1; o++){
			toroverde[o].mover(0);
		}
	}
	if (hitboxenemigos()){
		perder();
	}
	if (hitboxfruta()){
		adminfrutas();
	}
}

void adminfrutas(){
	if (banana[i].active){
		banana[i].interactuar();
	}
	if (uva[i].active){
		uva[i].interactuar();
	}
	if (sandia[i].active){
		sandia[i].interactuar();
	}
}

void Enemigo::mover(int nullenemy){
	if (nullenemy < 4){
		switch(tipo){
			case 200:
				switch(diren){
					case 0:
						if (!chocar(0,2)){
							ye++;
						} else {
							diren = 1;
							mover(nullenemy+1);
						} break;
					case 1:
						if (!chocar(-1,0)){
							xe--;
						} else {
							diren = 2;
							mover(nullenemy+1);
						}break;
					case 2:
						if (!chocar(0,-1)){
							ye--;
						} else{
							diren = 3;
							mover(nullenemy+1);
						} break;
					case 3:
						if (!chocar(2,0)){
							xe++;
						} else{
							diren = 0;
							mover(nullenemy+1);
						} break;
				}
				posicionar();
				break;
		}
	}
}

void Enemigo::posicionar(){
	for (i = 0; i < 2; i++){
		for (j = 0; j < 2; j++){
			levelenemy[xe+i][ye+j] = tipo; //establecer posicion del enemigo
		}
	}
	for (i = -1; i < 3; i++){
		if (levelenemy[xe-1][ye+i] == (tipo-(tipo*2))){ //borrar la posicion anterior del enemigo
			levelenemy[xe-1][ye+i] = 0;
		}
		if (levelenemy[xe+2][ye+i] == (tipo-(tipo*2))){
			levelenemy[xe+2][ye+i] = 0;
		}
		if (levelenemy[xe+i][ye-1] == (tipo-(tipo*2))){
			levelenemy[xe+i][ye-1] = 0;
		}
		if (levelenemy[xe+i][ye+2] == (tipo-(tipo*2))){
			levelenemy[xe+i][ye+2] = 0;
		}
	}
}

bool Enemigo::chocar(int xe2, int ye2){
	if (diren%2==0){
		if ((levelenemy[xe][ye+ye2] <= -200 && levelenemy[xe+xe2][ye+ye2] > -400) ||
		(levelenemy[xe+1][ye+ye2] <= -200 && levelenemy[xe+1+xe2][ye+ye2] > -400) ||
		(levelenemy[xe][ye+ye2] >= 200 && levelenemy[xe+xe2][ye+ye2] < 400) ||
		(levelenemy[xe+1][ye+ye2] >= 200 && levelenemy[xe+1+xe2][ye+ye2] < 400)
		){
			return true;
		} else{
			return false;
		}
	} else {
		if ((levelenemy[xe+xe2][ye] <= -200 && levelenemy[xe+xe2][ye+1+ye2] > -400) ||
		(levelenemy[xe+xe2][ye+1] <= -200 && levelenemy[xe+1+xe2][ye+1+ye2] > -400) ||
		(levelenemy[xe+xe2][ye] >= 200 && levelenemy[xe+xe2][ye+1+ye2] < 400) ||
		(levelenemy[xe+xe2][ye+1] >= 200 && levelenemy[xe+1+xe2][ye+1+ye2] < 400)
		){
			return true;
		} else {
			return false;
		}
	}
	
}

bool Enemigo::hitboxvacioenemy(int _i, int enemydir){
	if (((levelenemy[xe+_i][ye+1] == -1 && levelenemy[xe+_i][ye] == -1) ||
	(levelenemy[xe+_i][ye+1] == -2 && levelenemy[xe+_i][ye] == -2))
	&& enemydir == 0){
		return true;
	}
	
	if (((levelenemy[xe+1][ye+_i] == -1 && levelenemy[xe][ye+_i] == -1) ||
	(levelenemy[xe+1][ye+_i] == -2 && levelenemy[xe][ye+_i] == -2))
	&& enemydir == 1){
		return true;
	}
	
	return false;
}

void Fruta::interactuar(){
	cantfrutas--;
	active = false;
}

void tiempo(){
	milseg++;
	if (milseg >= 10000){
		milseg = 0;
	}
}

void perder(){
	gotoxy(0,0); cout<<"perdiste";
}

void ganar(){
	gotoxy(0,1); cout<<"Ganaste";
}
