#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <windows.h>

#define MAX_CARTAS 10
#define MAX_PALOS 4
#define MAX_JUGADORES 4
#define MAX_BAR 40

#define BASTOS 0
#define COPAS 1
#define ESPADAS 2
#define OROS 3

void set_color_num(int c) {
    if (c == 1) printf("\033[31m");      
    else if (c == 2) printf("\033[32m"); 
    else if (c == 3) printf("\033[33m"); 
    else if (c == 6) printf("\033[36m"); 
}
void set_color_negrita() { printf("\033[1m"); }
void reset_color() { printf("\033[0m"); }

typedef struct {
    unsigned int num;
    unsigned int palo;
} tcarta;

typedef struct {
    unsigned int numcartas;
    tcarta mazo[MAX_CARTAS];
} tjugador;

void generar_baraja(tcarta baraja[MAX_BAR]);
void mezclar_baraja(tcarta baraja[MAX_CARTAS]);
void repartir_cartas(tcarta baraja[MAX_BAR], tjugador jugadores[MAX_JUGADORES]);
void mostrar_cartas(tcarta baraja[MAX_BAR], tjugador jugadores[MAX_JUGADORES], char opc1, char opc, int *turno);
void inicializar_tablero(tcarta tablero[MAX_CARTAS][MAX_JUGADORES]);
void mostrar_tablero();
void cinco_oros(int *turno, char *pal, tcarta tablero[MAX_CARTAS][MAX_PALOS], tjugador jugadores[]);
void mostrar_tablero_evo(tcarta tablero[MAX_CARTAS][MAX_PALOS]);
int realizar_tirada(int *turno, tjugador jugadores[], tcarta tablero[MAX_CARTAS][MAX_PALOS], tcarta op_pos[MAX_CARTAS], int *nop, char opc, int *ganador, int *fin);

int main() {
    SetConsoleOutputCP(CP_UTF8); 

    char pal, opc='r', opc1='r';
    int k, auxp, auxn, palo, num_azar, reparto, turno, colocar, encontrado, nop=0, ganador, fin = 0;
    tcarta baraja[MAX_BAR], tablero[MAX_CARTAS][MAX_PALOS], op_pos[MAX_CARTAS];
    
    srand(time(NULL)); 
    
    tjugador jugadores[MAX_JUGADORES]; 
    printf("\n");
    do{	
	    set_color_negrita();
	    printf("¿Hay algún jugador humano? [s\\n] ");
	    scanf(" %c%*c", &opc);
    }while(opc!='s' && opc!='n' );
    
    printf("\n");
    
    do{
	    printf("¿Quieres ver todas las cartas? [s\\n] ");
	    scanf(" %c%*c", &opc1);
    }while(opc1!='s' && opc1!='n' );
    
    reset_color();
	
    for (int p = 0; p < MAX_JUGADORES; p++) {
        jugadores[p].numcartas = 0;
    }
	
    generar_baraja(baraja);
    mezclar_baraja(baraja);
    repartir_cartas(baraja, jugadores);
    mostrar_cartas(baraja, jugadores, opc1, opc, &turno);
    inicializar_tablero(tablero);
    mostrar_tablero();
    
    sleep(2); 
    
    encontrado = 0;

    do{
        cinco_oros(&turno, &pal, tablero, jugadores);
        printf("\n");
        printf("\n");
        reset_color();
        mostrar_cartas(baraja, jugadores, opc1, opc, &turno);
        printf("\n");
        mostrar_tablero_evo(tablero);
	    printf("\n");

        if(fin == 1){
            reset_color();
            printf("El ganador es el jugador #%d\n", ganador);
            break;
        }

        if(turno == 0){
	        reset_color();
	        set_color_negrita();
	        printf("Turno de JUG#1");
        }else{
	        reset_color();
	        set_color_negrita();
	        printf("Turno de JUG#%d", turno+1);
        }
	    printf("\n");
	
	    colocar = realizar_tirada(&turno, jugadores, tablero, op_pos, &nop, opc, &ganador, &fin);
	    sleep(2); 
	
    } while(encontrado==0);

}

void generar_baraja(tcarta baraja[MAX_BAR])
{
    int i,j, palo;
    i = 0;
    while (i < MAX_BAR) {
        palo = i / 10; 

        for (j = 1; j <= 7; j++) { 
            baraja[i].num = j;
            baraja[i].palo = palo;
            i++;
        }

        for (j = 10; j <= 12; j++) { 
            baraja[i].num = j;
            baraja[i].palo = palo;
            i++;
        }
    }
}

void mezclar_baraja(tcarta baraja[MAX_CARTAS]){
    int i, num_azar, auxp, auxn;
    for (i = 0; i < MAX_BAR; i++) {
        num_azar = rand() % MAX_BAR; 
        
        auxp = baraja[i].palo;
        auxn = baraja[i].num;
        baraja[i].palo = baraja[num_azar].palo;
        baraja[i].num = baraja[num_azar].num;
        baraja[num_azar].palo = auxp;
        baraja[num_azar].num = auxn;
    }
}

void repartir_cartas(tcarta baraja[MAX_BAR], tjugador jugadores[MAX_JUGADORES]){
	int i, reparto;
	
    for (i = 0; i < MAX_BAR; i++) {
        reparto = (i / 5) % MAX_JUGADORES;
        jugadores[reparto].mazo[jugadores[reparto].numcartas] = baraja[i]; 
        jugadores[reparto].numcartas++;
    }
}

void mostrar_cartas(tcarta baraja[MAX_BAR], tjugador jugadores[MAX_JUGADORES], char opc1,char opc, int *turno) {
    int i, j;
    char pal;

    printf("\n");

    for (j = 0; j < 1; j++) {
        printf("        ");
        for (i = 0; i < MAX_CARTAS; i++) {
			if(i==9){
				printf("[%d ]", i + 1);
			}else{
                printf("[ %d ]", i + 1);
			}
        }
    }
    printf("\n");

    for (i = 0; i < MAX_JUGADORES; i++) {
        printf("JUG #%d: ", i + 1);

        for (j = 0; j < jugadores[i].numcartas; j++) {
            switch (jugadores[i].mazo[j].palo) {
                case BASTOS:
                    set_color_num(2);
                    pal = 'B';
                    break;
                case COPAS:
                    set_color_num(1);
                    pal = 'C';
                    break;
                case ESPADAS:
                    set_color_num(6);
                    pal = 'E';
                    break;
                case OROS:
                    set_color_num(3);
                    pal = 'O';
                    break;
            }
            if (i == 0 && opc == 's') {
                printf("[%c%2d]", pal, jugadores[i].mazo[j].num);
            } else {
                if (opc1 == 'n') {
					reset_color();
					set_color_negrita();
                    printf("[? ?]");
                } else {
                    printf("[%c%2d]", pal, jugadores[i].mazo[j].num);
                }
	        }
            reset_color();
        }
        printf("\n");
    }
}

void mostrar_tablero(){
    int i, j;
  
    printf("\nTablero inicial:\n");
    set_color_num(2);
	printf(" [BAS]");
	set_color_num(1);
	printf(" [COP]");
	set_color_num(6);
	printf(" [ESP]"); 
	set_color_num(3);
	printf(" [ORO]\n");
	reset_color();
	
    for (i = 0; i < MAX_CARTAS; i++) {
        for (j = 0; j < MAX_PALOS; j++) {
            set_color_negrita();
            printf(" [- -]");
        }
        printf("\n");
    }	
    reset_color();
}

void cinco_oros(int *turno, char *pal, tcarta tablero[MAX_CARTAS][MAX_PALOS], tjugador jugadores[]){
	for (int i = 0; i < MAX_JUGADORES; i++) {
	   for (int j = 0; j < jugadores[i].numcartas; j++) {
        	if(jugadores[i].mazo[j].num == 5 && jugadores[i].mazo[j].palo == 3){
        	    tablero[5][3]=jugadores[i].mazo[j];
        	    *pal = 'O';
        	    *turno = (i+1)%4;
        		for(int k = j; k < jugadores[i].numcartas -1; k++) {
        		    jugadores[i].mazo[k] = jugadores[i].mazo[k+1];
        		}
        	    jugadores[i].numcartas = jugadores[i].numcartas - 1;
        	}   
        }
    }
}
  
void mostrar_tablero_evo(tcarta tablero[MAX_CARTAS][MAX_PALOS]){
    int i, j;
    char pal;

    set_color_num(2);
	printf(" [BAS]");
	set_color_num(1);
	printf(" [COP]");
	set_color_num(6);
	printf(" [ESP]"); 
	set_color_num(3);
	printf(" [ORO]\n");
	reset_color();
	
    for (i = 0; i < MAX_CARTAS; i++) {
        for (j = 0; j < MAX_PALOS; j++) {
        	if (tablero[i][j].num != 0) {
                switch(tablero[i][j].palo){
	                case OROS: pal='O'; set_color_num(3); break;
                    case COPAS: pal='C'; set_color_num(1); break;
	                case ESPADAS: pal='E'; set_color_num(6); break;
	                case BASTOS: pal='B'; set_color_num(2); break;
	            }
	            printf(" [%c%2u]", pal, tablero[i][j].num);
	  
	        }else{
	            reset_color();
                set_color_negrita();
	            printf(" [- -]");
	        }
	    } 
	    printf(" \n");
    }
}
   
void inicializar_tablero(tcarta tablero[MAX_CARTAS][MAX_JUGADORES]){
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 4; j++) {     
            tablero[i][j].num = 0;   
        }
    }
}

int realizar_tirada(int *turno, tjugador jugadores[], tcarta tablero[MAX_CARTAS][MAX_PALOS], tcarta op_pos[MAX_CARTAS], int *nop, char opc, int *ganador, int *fin) {
    unsigned int colocada = 0, opcion = 0;
    int j = 0, k, cartas_colocadas = 0;
  
    *nop = 0;  

    while (j < jugadores[*turno].numcartas) {
        int fila = jugadores[*turno].mazo[j].num;
        if (fila > 9) {
            fila = 12 - fila; 
        } else {
            fila = 10 - fila;
        }
        int palo = jugadores[*turno].mazo[j].palo;

        if ((fila - 1 >= 0 && tablero[fila - 1][palo].num == jugadores[*turno].mazo[j].num + 1) || 
            (fila + 1 < MAX_CARTAS && tablero[fila + 1][palo].num == jugadores[*turno].mazo[j].num - 1) || 
            jugadores[*turno].mazo[j].num == 5) {
            op_pos[*nop] = jugadores[*turno].mazo[j];  
            (*nop)++;
        } else if (
				(tablero[fila + 1][palo].num == 7 && jugadores[*turno].mazo[j].num == 10) || 
				(tablero[fila + 1][palo].num == 10 && jugadores[*turno].mazo[j].num == 11) ||
				(jugadores[*turno].mazo[j].num == 12 && tablero[fila +1][palo].num == 11))
		{
				op_pos[*nop] = jugadores[*turno].mazo[j];
				(*nop)++;
		}
        j++;
    }
    set_color_negrita();	

    if (*nop == 0) {
        printf("Tiradas posibles: \n");
        printf("Ninguna! :-/ Paso.\n");
        *turno = (*turno + 1) % MAX_JUGADORES; 
        return 0;
    }

    printf("Tiradas posibles: ");
    for (int i = 0; i < *nop; i++) { 
        set_color_negrita();
		if(op_pos[i].palo == 0){
			printf("%d: ", i + 1);
			set_color_num(2);
			printf("[B %2d]  ",op_pos[i].num);
			reset_color();
		}else if(op_pos[i].palo == 1){
			printf("%d: ", i + 1);
			set_color_num(1);
			printf("[C %2d]  ",op_pos[i].num);
			reset_color();
		}else if(op_pos[i].palo == 2){
			printf("%d: ", i + 1);
			set_color_num(6);
			printf("[E %2d]  ",op_pos[i].num);
			reset_color();
		}else if(op_pos[i].palo == 3){
			printf("%d: ", i + 1);
			set_color_num(3);
			printf("[O %2d]  ",op_pos[i].num);
			reset_color();
		}
    }
    set_color_negrita();	

    char op_jug_real;
   
    do{
        printf("\n¿Qué tirada realizas? ");
	    if (opc == 's' && *turno == 0) { 
   		    scanf(" %c%*c", &op_jug_real);
   		    opcion = op_jug_real - '0';
            
            // Added this error message so you know when you messed up!
            if (opcion < 1 || opcion > (unsigned int)*nop) {
                set_color_num(1); // Make it red
                printf("¡Tirada inválida! Por favor, elige un número del 1 al %d.\n", *nop);
                reset_color();
            }
	    } else {  
        	opcion = (rand() % *nop) + 1;
        	printf("%d\n", opcion);
	    }	 
    // FIX: Removed the "1 +" that was causing the bug
    }while (opcion < 1 || opcion > (unsigned int)*nop);
    
    reset_color();

    int fila = op_pos[opcion - 1].num;
    int palo = op_pos[opcion - 1].palo;

    if (fila > 9) {
        fila = 12 - fila; 
    } else {
        fila = 10 - fila;
    }

    tablero[fila][palo] = op_pos[opcion - 1]; 
    colocada = 1;
    cartas_colocadas++;

    for (k = 0; k < jugadores[*turno].numcartas; k++) {
		if(colocada){
            if (jugadores[*turno].mazo[k].num == op_pos[opcion - 1].num &&
                jugadores[*turno].mazo[k].palo == op_pos[opcion - 1].palo) {
                for (int l = k; l < jugadores[*turno].numcartas - 1; l++) {
                    jugadores[*turno].mazo[l] = jugadores[*turno].mazo[l + 1];
                }
                jugadores[*turno].numcartas--;
                break;
            }
        }
    }
	if(jugadores[*turno].numcartas == 0 ){    
	    *ganador = *turno +1;
	    *fin = 1;
	} else {
   	    *turno = (*turno + 1) % MAX_JUGADORES;
   	    return cartas_colocadas;
    }
    
    return 0;
}