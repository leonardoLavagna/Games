#include <set>

#include "colore.h"
#include "scacchiera.h"

using namespace std;

#define LEGALE 0
#define CASELLA_UGUALE 1
#define COLORE_UGUALE 2
#define NON_STESSA_DIAGONALE 3
#define NON_STESSA_RIGA_O_COLONNA 4
#define CASELLA_VUOTA 5
#define TURNO_ERRATO 6
#define NON_SALTO_CAVALLO 7
#define PEDONE 8
#define LINEA_NON_LIBERA 9
#define NON_STESSA_LINEA 10
#define RE_ILLEGALE 11
#define RE_SOTTO_SCACCO 12
#define SCACCO_MATTO 13
#define SYNTAX_ERROR 14
#define INIZIO_PARTITA 15
#define EN_PASSANT 16
#define DOPPIO_PASSO 17
#define ARROCCO 18
#define FINE 100

const int cavDirX[8]={-2,-2,-1,-1,1,1,2,2};
const int cavDirY[8]={-1,1,-2,2,-2,2,-1,1};

const int alfDirX[4]={-1,-1,1,1};
const int alfDirY[4]={-1,1,-1,1};

const int torreDirX[4]={0,0,1,-1};
const int torreDirY[4]={1,-1,0,0};

class Pezzo {
protected:	
	Colore col;
	Casella pos;
public:
	Pezzo(Colore c, int x, int y);
	Colore colore();
	Casella posizione();
	virtual set<Casella *> allMoves(Scacchiera s);

	int puoiMuovere(Casella a, Scacchiera s);
	virtual int puoiMuovere(Casella da, Casella a, Scacchiera s);
	void move(Casella a);
	/* il metodo print è puramente virtual. Rende Pezzo classe astratta. 
	 * Deve essere implementato nelle classi concrete 
	 */
	virtual void print() = 0;
	void printPos();
	virtual void printFen() = 0;
};

class Alfiere : public Pezzo {

public:
	Alfiere(Colore c, int x, int y) : Pezzo(c,x,y){}
	virtual int puoiMuovere(Casella da, Casella a, Scacchiera s);

	virtual void print();
	virtual void printFen();

	virtual set<Casella *> allMoves(Scacchiera s);
}; // end class Alfiere

class Torre : public Pezzo {
public:
	Torre(Colore c, int x, int y) : Pezzo(c,x,y){}
	virtual int puoiMuovere(Casella da, Casella a, Scacchiera s);
	virtual void print();
	virtual void printFen();

	virtual set<Casella *> allMoves(Scacchiera s);
}; // end class Torre

class Regina : public Torre, Alfiere{
	/* questa classe è una alternativa a Donna che fa vedere l'ereditarietà multipla.
	 * Donna eredita sia da torre che da Alfiere.
	 * Osservare che NON si tratta di una relazione IS-A, quanto di una specializzazione funzionale
	 */
	Regina(Colore c, int x, int y) : Alfiere(c, x, y), Torre(c,x,y){}

	virtual int puoiMuovere(Casella da, Casella a, Scacchiera s);
	virtual void print();
	virtual void printFen();
	virtual set<Casella *> allMoves(Scacchiera s);
}; // end class Donna2

class Donna : public Alfiere {
public:
	Donna(Colore c, int x, int y) : Alfiere(c, x, y){}

	virtual int puoiMuovere(Casella da, Casella a, Scacchiera s);
	virtual void print();
	virtual void printFen();
	virtual set<Casella *> allMoves(Scacchiera s);
}; // end class Donna

class Cavallo : public Pezzo {
public:
	Cavallo(Colore c, int x, int y) : Pezzo(c, x, y){}

	virtual int puoiMuovere(Casella da, Casella a, Scacchiera s);
	virtual void print();
	virtual void printFen();

	virtual set<Casella *> allMoves(Scacchiera s);
}; // fine classe Cavallo

class Re : public Pezzo {
public:
	Re(Colore c, int x, int y) : Pezzo(c, x, y){}

	virtual int puoiMuovere(Casella da, Casella a, Scacchiera s);
	virtual void print();
	virtual void printFen();

	virtual set<Casella *> allMoves(Scacchiera s);
}; // end class Re

class Pedone : public Pezzo {
	int dir;
	int base;
public:
	Pedone(Colore c, int x, int y) : Pezzo(c, x, y){
		if (c.bianco()){
			dir=1;
			base=1;
		}
		else{ 
			dir=-1;
			base=6;
		}
	}

	virtual int puoiMuovere(Casella da, Casella a, Scacchiera s);
	virtual void print();
	virtual void printFen();

	virtual set<Casella *> allMoves(Scacchiera s);
}; // end class Pedone
