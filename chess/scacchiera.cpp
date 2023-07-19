#include "scacchiera.h"

int Scacchiera::calcolaDirezione(int n, int m){
		/* metodo privato */
		if (n==m) return 0;
		if (n<m) return 1;
		return -1;
	}

Scacchiera::Scacchiera(){
	/* alloca la scacchiera vuota */
		s=(Pezzo ***) calloc(RIG,sizeof(Pezzo**));
		for (int i=0; i<RIG; i++){
			s[i]=(Pezzo**) calloc(COL, sizeof(Pezzo*));
			for (int j=0; j<COL; j++)
				s[i][j]=NULL;
		}
	}

void Scacchiera::print(){
	/* stampa la scacchiera. Non conoscendo i pezzi, stampa le caselle occupate con '*' */
		for (int i=0; i<RIG; i++){
			for (int j=0; j<COL; j++)
				if(s[i][j]==NULL) printf(".");
					else printf("*");
			printf("\n");
			}		
	}

void Scacchiera::add(int x, int y, Pezzo *p){
	/* aggiunge un pezzo, dando le coordinate */
		s[x][y]=p;
	}

void Scacchiera::add(Casella in, Pezzo *p){
	/* metodo add overloaded, che prende in input una casella */
		s[in.x][in.y]=p;
	}

Pezzo* Scacchiera::remove(Casella da){
		Pezzo* p = s[da.x][da.y];
		s[da.x][da.y] = NULL;
		return p;
	}

Pezzo* Scacchiera::dammiPezzo(Casella in){
	/* restituisce il pezzo contenuto in una casella */
		return s[in.x][in.y];
	}

Pezzo* Scacchiera::dammiPezzo(int x, int y){
	/* metodo overloaded con le coordinate. Qual è il metodo dammiPezzo più "riusabile"? */
		return s[x][y];
	}

bool Scacchiera::lineaLibera(Casella da, Casella a){
		/* PREC: assume da e a stiano su stessa colonna, traversa o diagonale 
		 * POST: restituice true se la linea tra da e a non contiene ostacoli. 
		 */	
		int dirx = calcolaDirezione(da.x, a.x);
		int diry = calcolaDirezione(da.y, a.y);

		Casella c(da.x+dirx, da.y+diry);
		while (c!=a){
			if (dammiPezzo(c)!=NULL) return false;
			c.move(dirx, diry);
		}
		return true;
	}
