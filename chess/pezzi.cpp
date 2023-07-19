#include "pezzi.h"


	/* costruttori dei pezzi */
	Pezzo::Pezzo(Colore c, int x, int y){col=c; pos.x = x, pos.y=y;}

	/* metodi osservatori */
	Colore Pezzo::colore(){return col;}
	Casella Pezzo::posizione(){return pos;} 

	/* il metodo puoiMuovere è il cuore dell'applicazione e implementa, 
	 * per ciascun pezzo, le suoe regole di movimento. Esiste in due versioni con 
	 * diverso numero di parametri. Quello "vero" è quello ridefinito su ogni 
	 * classe che rappresenta un pezzo
	 */
	int Pezzo::puoiMuovere(Casella a, Scacchiera s){
		return puoiMuovere(pos,a,s);
	}

	int Pezzo::puoiMuovere(Casella da, Casella a, Scacchiera s){
		if (da==a) return CASELLA_UGUALE;
		Pezzo *p=s.dammiPezzo(a);
		if (p!=NULL && p->colore()==col) return COLORE_UGUALE;
		return LEGALE;
	}

 	int Alfiere::puoiMuovere(Casella da, Casella a, Scacchiera s){
		int res = Pezzo::puoiMuovere(da,a,s);
		if (res != LEGALE) return res;
		if (da.x+da.y!=a.x+a.y && da.x-da.y!=a.x-a.y) return NON_STESSA_DIAGONALE;
		if (s.lineaLibera(da,a)) return LEGALE;
			else return LINEA_NON_LIBERA;
	}

	int Torre::puoiMuovere(Casella da, Casella a, Scacchiera s){
		int res = Pezzo::puoiMuovere(da,a,s);
		if (res != LEGALE) return res;
		if (da.x!=a.x && da.y!=a.y) return NON_STESSA_RIGA_O_COLONNA;
		if (s.lineaLibera(da,a)) return LEGALE;
			else return LINEA_NON_LIBERA;
	}

	int Regina::puoiMuovere(Casella da, Casella a, Scacchiera s){
		int res = Alfiere::puoiMuovere(da,a,s); 
		if (res==LEGALE) res=Torre::puoiMuovere(da,a,s);
		return res;	
	}

	int Donna::puoiMuovere(Casella da, Casella a, Scacchiera s){
		int res = Alfiere::puoiMuovere(da,a,s); 
		if (res==CASELLA_UGUALE || res == COLORE_UGUALE || res == LINEA_NON_LIBERA || res == LEGALE)
			return res;			
		if (da.x!=a.x && da.y!=a.y && da.x!=a.x && da.y!=a.y) 
			return NON_STESSA_LINEA;
		if (s.lineaLibera(da,a)) return LEGALE;
			else return LINEA_NON_LIBERA;
		}

	int Cavallo::puoiMuovere(Casella da, Casella a, Scacchiera s){
		int res = Pezzo::puoiMuovere(da,a,s);
		if (res != LEGALE) return res;
		for(int i=0; i<8; i++){
			if (a.x==da.x+cavDirX[i] && a.y==da.y+cavDirY[i]) return LEGALE;
		}
		return NON_SALTO_CAVALLO; 	
	}

	int Re::puoiMuovere(Casella da, Casella a, Scacchiera s){
		int res = Pezzo::puoiMuovere(da,a,s); 
		if (res!= LEGALE) return res;
		if (abs(da.x-a.x)<=1 && abs(da.y-a.y)<=1) return LEGALE;
		/* verifica arrocco */
		if (da.x==a.x && abs(da.y-a.y)==2) return ARROCCO;
		return RE_ILLEGALE; 
	}

	int Pedone::puoiMuovere(Casella da, Casella a, Scacchiera s){
		int res = Pezzo::puoiMuovere(da,a,s);
		if (res != LEGALE) return res;
		Pezzo *p=s.dammiPezzo(a);
		/* verifica se la mossa e' un passo */		
		if (p==NULL && a.y==da.y && (a.x-da.x==dir))
			return LEGALE;
		/* verifica se la mossa e' di due passi dalla casella di partenza */		
		Pezzo *p2=s.dammiPezzo(da.x+dir, da.y);
		if (p==NULL && p2==NULL && a.y==da.y && da.x==base && a.x-da.x==2*dir)
			return DOPPIO_PASSO;			
		/* verifica se la mossa e' una presa */		
		if (p!=NULL && (p->colore()!=col && (a.y-da.y==1 || a.y-da.y==-1)) && a.x-da.x==dir)
			return LEGALE;
		/* verifica se la mossa e' una presa en-passant 	
		if (p==NULL && (a.y-da.y==1 || a.y-da.y==-1) && a.x-da.x==dir)
			return EN_PASSANT;
		*/	
		return PEDONE;	
	}


	/* il metodo allMoves restituisce la lista di caselle dove si può muovere 
	 * un pezzo. Osservare che il metodo di Pezzo funziona correttamente su ogni 
	 * pezzo, in quanto è parametrico rispetto al metodo virtual puoiMuovere
	 * allMoves viene ridefinito su ogni singolo pezzo per ragioni di efficienza
	 */	
	set<Casella *> Pezzo::allMoves(Scacchiera s){
		set<Casella *> sm;
		for(int i=0; i<RIG; i++)
			for(int j=0; j<COL; j++){
				Casella *a = new Casella(i,j);
				if (puoiMuovere(*a,s)==LEGALE) sm.insert(a);
			}
		return sm;
	}

	set<Casella *> Alfiere::allMoves(Scacchiera s){
		set<Casella *> sm;
		for(int i=0; i<4; i++){
			int x=pos.x+alfDirX[i]; 
			int y=pos.y+alfDirY[i];
			while (x>=0 && x<RIG && y>=0 && y<COL){
				Pezzo *p = s.dammiPezzo(x,y);
				if (p==NULL || p->colore()!=col) sm.insert(new Casella(x,y));
				if (p!=NULL) break;
				x=x+alfDirX[i];
				y=y+alfDirY[i];
				}
			}	
			return sm;
		}

	set<Casella *> Torre::allMoves(Scacchiera s){
		set<Casella *> sm;
		for(int i=0; i<4; i++){
			int x=pos.x+torreDirX[i]; 
			int y=pos.y+torreDirY[i];
			while (x>=0 && x<RIG && y>=0 && y<COL){
				Pezzo *p = s.dammiPezzo(x,y);
				if (p==NULL || p->colore()!=col) sm.insert(new Casella(x,y));
				if (p!=NULL) break;
				x=x+torreDirX[i];
				y=y+torreDirY[i];
				}
			}	
			return sm;
		}

	set<Casella *> Regina::allMoves(Scacchiera s){
		set<Casella *> am = Alfiere::allMoves(s);
		set<Casella *> tm = Torre::allMoves(s);
		/* insert di set<T> permette di inserire un range di elementi, in questo caso, tutti */
		am.insert(tm.begin(),tm.end());
		return am;
	}

	set<Casella *> Donna::allMoves(Scacchiera s){
		set<Casella *> sm = Alfiere::allMoves(s);
		for(int i=0; i<4; i++){
			int x=pos.x+torreDirX[i]; 
			int y=pos.y+torreDirY[i];
			while (x>=0 && x<RIG && y>=0 && y<COL){
				Pezzo *p = s.dammiPezzo(x,y);
				if (p==NULL || p->colore()!=col) sm.insert(new Casella(x,y));
				if (p!=NULL) break;
				x=x+torreDirX[i];
				y=y+torreDirY[i];
				}
			}	
			return sm;
		}

	set<Casella *> Cavallo::allMoves(Scacchiera s){
		set<Casella *> sm;
		for(int i=0; i<8; i++){
			int x=pos.x+cavDirX[i]; 
			int y=pos.y+cavDirY[i];
			if (x>=0 && x<RIG && y>=0 && y<COL){
				Pezzo *p = s.dammiPezzo(x,y);
				if (p==NULL || p->colore()!= col) sm.insert(new Casella(x,y));
			}
		}
		return sm;
	}

	set<Casella *> Re::allMoves(Scacchiera s){
		set<Casella *> sm;
		for(int x=pos.x-1; x<=pos.x+1; x++){
			for (int y=pos.y-1; y<=pos.y+1; y++)
			if (x>=0 && x<RIG && y>=0 && y<COL){
				Pezzo *p = s.dammiPezzo(x,y);
				if (p==NULL || p->colore()!= col) sm.insert(new Casella(x,y));
			}
		}
		return sm;
	}

	set<Casella *> Pedone::allMoves(Scacchiera s){
		set<Casella *> sm;
		Pezzo* p;
		int x=pos.x+dir;
		int y=pos.y;
		// mossa di un passo 
		if (s.dammiPezzo(x,y)==NULL) sm.insert(new Casella(x,y));
		// prese 
		y=pos.y-1;
		if (y>=0){
			p=s.dammiPezzo(x,y);
			if (p!=NULL && p->colore()!=col) sm.insert(new Casella(x,y));
		}
		y=pos.y+1;
		if (y<COL){
			p=s.dammiPezzo(x,y);
			if (p!=NULL && p->colore()!=col) sm.insert(new Casella(x,y));
		}
		// mossa di due passi 
		if (pos.x==base){
			x=pos.x+2*dir;
			y=pos.y;
			if (s.dammiPezzo(x,y)==NULL) sm.insert(new Casella(x,y));
		}
		return sm;
	}


	void Pezzo::move(Casella a){
		pos.x=a.x;
		pos.y=a.y;
	}

/* metodi per visualizzare i pezzi sulla scacchiera
 * o nella lista di pezzi
 */

	void Alfiere::print(){
			if (col.bianco()) printf(" A ");
				else printf(" a ");
		}

	void Torre::print(){
		if (col.bianco()) printf(" T ");
			else printf(" t ");
		}

	void Regina::print(){
	/* da osservare che col è "ambiguo" per il compilatore! Infatti la Donna ha due copie di pezzo */
		if (Alfiere::col.bianco()) printf(" D ");
			else printf(" d ");
		}

	void Donna::print(){
		if (col.bianco()) printf(" D ");
			else printf(" d ");
		}

	void Cavallo::print(){
		if (col.bianco()) printf(" C ");
			else printf(" c ");
		}

	void Re::print(){
		if (col.bianco()) printf(" R ");
			else printf(" r ");
		}
	
	void Pedone::print(){
		if (col.bianco()) printf(" P ");
			else printf(" p ");
		}
	
/* stampa per la notazione FEN */
			void Alfiere::printFen(){
			if (col.bianco()) printf("B");
				else printf("b");
		}

	void Torre::printFen(){
		if (col.bianco()) printf("R");
			else printf("r");
		}

	void Regina::printFen(){
	/* da osservare che col è "ambiguo" per il compilatore! Infatti la Donna ha due copie di pezzo */
		if (Alfiere::col.bianco()) printf("Q");
			else printf("q");
		}

	void Donna::printFen(){
		if (col.bianco()) printf("Q");
			else printf("q");
		}

	void Cavallo::printFen(){
		if (col.bianco()) printf("N");
			else printf("n");
		}

	void Re::printFen(){
		if (col.bianco()) printf("K");
			else printf("k");
		}
	
	void Pedone::printFen(){
		if (col.bianco()) printf("P");
			else printf("p");
		}

	void Pezzo::printPos(){
		print();
		pos.print();
	}
