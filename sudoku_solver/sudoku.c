
/*
Programma per risolvere un sudoku.
Leonardo Lavagna
Inserendo la matrice che rappresenta un sudoku,
questo programma lo risolve.
L’idea è di fare backtraking con il seguente algoritmo:
1. trovare una riga o una colonna con una cella vuota
2. se non c’è nessuna riga o colonna con una cella vuota
ritorna vero (1)
3. per i=1,...,9
	a. se non c’è violazione delle regole per un
	numero in quella cella assegnare il numero 
	alla cella
	b. se la ricorsione è un successo ritorna vero
	c. altrimenti rimuovere il numero e provarne 
	un altro
4. se tutti i numeri sono stati provati e nessuno andava bene
ritorna falso.
*/


#include <stdio.h>
#include <stdlib.h>


// numero di righe, colonne, celle in un quadrato 3x3
#define DIM 9

int tentativo=0;

// legge la matrice del sudoku riga per riga
void leggiSudoku(int s[][DIM])
/*
Inserire i numeri uno attaccato all’altro inserendo 0 al posto
di una cella vuota oppure "x".
Gli input sono presi riga per riga e ogni riga termina con
il comando <<a capo>> (invio).
*/
{
int i,j;
int cella;
	for (i=0; i<DIM; i++)
	{
		for (j=0; j<DIM; j++)
		{
			cella=getchar();
			if (cella=='x')
				s[i][j]=0;
			else
				s[i][j]=cella-'0';
		}
		cella=getchar();
	}
}

// stampa la matrice del sudoku dividendo le celle 3x3
void stampaSudoku(int s[][9])
{
int i, j;
printf("\n");
	for (i=0; i<DIM; i++)
	{
		for (j=0; j<DIM; j++)
		{
			printf(" %d",s[i][j]);
			// barretta divisoria delle celle 3x3
			if (j==2 || j==5)
				printf(" |");
		}
		printf("\n");
		
		
		if (i==2 || i==5)
			{
				 // divisore delle righe
				 for (j=1; j<=DIM*3; j++)
					printf("-");
				 printf("\n");
			}
	}
printf("\n");
}

// verifica se il numero si trova oppure no nella colonna
int inColonna(int k, int c, int s[][DIM])
{
int j;
	for (j=0; j<DIM; j++)
		if (s[j][c]==k) 
			return 0;
return 1;
}

// verifica se il numero si trova oppure no nella riga
int inRiga(int k, int r, int s[][DIM])
{
int j;
	for (j=0; j<DIM; j++)
		if (s[r][j]==k) 
			return 0;
return 1;
}

// verifica se il numero si trova o meno nella cella 3x3
int provaCella(int k, int r, int c, int s[][DIM])
{
int u = r / 3;
int v = c / 3;
int i, j;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			if (s[u*3+i][v*3+j]==k) 
				return 0;
return 1;
}

// verifica che il numero si puo’ mettere nella casella
int numeroPossibile(int k, int r, int c, int s[][DIM])
{
if (inColonna(k,c,s)&&inRiga(k,r,s)&&provaCella(k,r,c,s))
	return 1;
return 0;
}

// il sudoku è completato
int finito(int r, int c)
{
if (r==DIM-1 && c==DIM-1)
	return 1;
return 0;
}

// tenta di mettere un nuovo numero per poi vedere se è compatibile col resto del sudoku
int prova(int x, int y, int s[][DIM])
{
int j;
int u, v;
	//calcola la prossima casella
	if (y==DIM-1) 
	{
		v=0; 
		u=x+1;
	}
	else 
	{
		v=y+1; 
		u=x;
	}
	// casella gia’ occupata, non ci sono scelte da fare
	if (s[x][y])
	{ 
		if (finito(x, y)) 
			return 1;
		return prova(u, v, s);
	}
        //analizza i possibili riempimenti della casella
	for (j=1; j<=9; j++)
	{
		if (numeroPossibile(j,x,y,s))
		{
			tentativo++;
			s[x][y]=j;
			if (finito(x,y))
				return 1;
			if (prova(u, v, s))
				return 1;
			else
				s[x][y]=0;
		}
	}
return 0;
}

// esempio di main
int main()
{
int s[DIM][DIM];
	printf("Inserire le righe della matrice del sudoku da completare\n");
	leggiSudoku(s);
	if (prova(0, 0, s)) 
		stampaSudoku(s);
	else 
		printf("Il problema non ha soluzioni\n"); 
return 1;

}

