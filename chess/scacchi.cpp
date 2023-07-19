#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <set>
#include <vector>
#include <map>
#include <string>

#include "pezzi.h"

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

void printError(int n){
	switch(n){
		case LEGALE: printf("Mossa Legale\n"); break;
		case CASELLA_UGUALE: printf("I pezzi non possono stare fermi!\n"); break;
		case COLORE_UGUALE: printf("Non puoi ammucchiare più pezzi nella stessa casella!\n"); break;
		case NON_STESSA_DIAGONALE: printf("Mossa non in diagonale!\n"); break;
		case NON_STESSA_RIGA_O_COLONNA: printf("Mossa non su riga o traversa!\n"); break;
		case CASELLA_VUOTA: printf("Casella di partenza vuota\n"); break;
		case TURNO_ERRATO: printf("Ha mosso il giocatore sbagliato!\n"); break;
		case NON_SALTO_CAVALLO: printf("Il cavallo si muove a L!\n"); break;
		case PEDONE: printf("Mossa Illegale per il pedone!\n"); break;
		case LINEA_NON_LIBERA: printf("C'è qualcosa in mezzo!\n"); break;
		case NON_STESSA_LINEA: printf("La Donna non può fare tutto!\n"); break;
		case RE_ILLEGALE: printf("Mossa illegale per il Re!\n"); break;
		case RE_SOTTO_SCACCO: printf("Il re non può rimanere sotto scacco\n"); break;
		case SCACCO_MATTO: printf("Scacco Matto!!!!\n"); break;
		case SYNTAX_ERROR: printf("Mossa mal digitata\n"); break;
		case INIZIO_PARTITA: printf("Non si puo' ritirare oltre la mossa\n"); break;
		case EN_PASSANT: printf("Presa al passo!!!\n"); break;
	}
}

const Casella casellaImpossibile = Casella(8,8);

const Colore bianco = Colore(1);
const Colore nero = Colore(0);

extern const int cavDirX[8]; //={-2,-2,-1,-1,1,1,2,2};
extern const int cavDirY[8]; //={-1,1,-2,2,-2,2,-1,1};

extern const int alfDirX[4]; //={-1,-1,1,1};
extern const int alfDirY[4]; //={-1,1,-1,1};

extern const int torreDirX[4]; //={0,0,1,-1};
extern const int torreDirY[4]; //={1,-1,0,0};

class Move{
public:
	Casella da;
	Casella a;

	Move(Casella x, Casella y){
		da=x;
		a=y;
	}

};



class Mossa {
public:	
	Casella da;
	Casella a;
	Pezzo* preso;
	void print(){
		printf("mossa:");
		if (preso!=NULL) preso->printPos();
		da.print();
		a.print();
	}

	void printM(){
		da.print();
		if (preso!=NULL) printf("x"); else printf("-");
		a.print();
		printf(" ");
	}
};

class Partita {
	Scacchiera s;
	Colore turno;

	/* informazione ridondante */
	Colore avv;
	map<Colore, Pezzo *> re;
	map<Colore, set<Pezzo*> > pezzi;
	map<Colore, bool*> arrocco;
	int mosseNeutre;
	int nMosse;
	vector<Mossa> mosse;
	Casella ep;
	int primaMossa;

	void insert(char c, int i, int j){
		/* inserisce nella scacchiera un pezzo rappresentato come un carattere (inglese)
		 * metodo privato ausiliario nel caricamento di una posizione FEN
		 */
			printf("insert: %c %d %d\n",c,i,j);
			Pezzo *p;
			if (c=='K') {
				p = new Re(bianco,i,j);
				pezzi[bianco].insert(p);
				re[bianco]=p;
			}
			if (c=='Q') {
				p = new Donna(bianco,i,j);
				pezzi[bianco].insert(p);
			}
			if (c=='B') {
				p = new Alfiere(bianco,i,j);
				pezzi[bianco].insert(p);
			}
			if (c=='N') {
				p = new Cavallo(bianco,i,j);
				pezzi[bianco].insert(p);
			}
			if (c=='R') {
				p = new Torre(bianco,i,j);
				pezzi[bianco].insert(p);
			}
			if (c=='P') {
				p = new Pedone(bianco,i,j);
				pezzi[bianco].insert(p);
			}
			if (c=='k') {
				p = new Re(nero,i,j);
				pezzi[nero].insert(p);
				re[nero]=p;
			}
			if (c=='q') {
				p = new Donna(nero,i,j);
				pezzi[nero].insert(p);
			}
			if (c=='b') {
				p = new Alfiere(nero,i,j);
				pezzi[nero].insert(p);
			}
			if (c=='n') {
				p = new Cavallo(nero,i,j);
				pezzi[nero].insert(p);
			}
			if (c=='r') {
				p = new Torre(nero,i,j);
				pezzi[nero].insert(p);
			}
			if (c=='p') {
				p = new Pedone(nero,i,j);
				pezzi[nero].insert(p);
			}
			s.add(i,j,p);
		}


	Mossa eseguiMossa(Casella da, Casella a){
			Pezzo *p=s.dammiPezzo(da);
			Pezzo *q=s.dammiPezzo(a);
			Mossa m;
			m.da = da;
			m.a = a;
			m.preso = q;
			s.remove(da);
			s.add(a,p);
			p->move(a);
			if (q!=NULL) pezzi[q->colore()].erase(q);
			return m;
			}


	Mossa eseguiEP(Casella da, Casella a){
			Pezzo *p=s.dammiPezzo(da);
			Pezzo *q=s.dammiPezzo(da.x,a.y);
			Mossa m;
			m.da = da;
			m.a = a;
			m.preso = q;
			s.remove(da);
			s.remove(Casella(da.x,a.y));
			s.add(a,p);
			p->move(a);
			if (q!=NULL) pezzi[q->colore()].erase(q);
			return m;
			}

	void ritiraMossa(Mossa m){
			Pezzo *p=s.dammiPezzo(m.a);
			Pezzo *q=s.dammiPezzo(m.da);
			s.remove(m.a);
			s.add(m.da,p);
			p->move(m.da);
			//if (m.preso->posizione != m.a) ep=m.a;
			if (m.preso!=NULL) {
				s.add(m.preso->posizione(),m.preso);
				pezzi[m.preso->colore()].insert(m.preso);
				}
			}


	
public:	
	Partita(){
		/* inizializza la posizione iniziale degli scacchi, il turno al bianco, etc. 
		 * dopo la definizione di Partita(string), questo costruttore è equivalente 
		 * a Partita("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")	
		 */
		turno = bianco;
		avv = nero;
		Pezzo* p;
		Casella cas;
		ep = casellaImpossibile;

		for(int i=0; i<COL; i++){
			p = new Pedone(nero,6,i); s.add(6, i, p); pezzi[nero].insert(p);
			p = new Pedone(bianco,1,i); s.add(1, i, p); pezzi[bianco].insert(p);
		}

		p = new Torre(bianco,0,0); s.add(0, 0, p); pezzi[bianco].insert(p);
		p = new Torre(bianco,0,7); s.add(0, 7, p); pezzi[bianco].insert(p);
		p = new Torre(nero,7,0); s.add(7, 0, p); pezzi[nero].insert(p);
		p = new Torre(nero,7,7); s.add(7, 7, p); pezzi[nero].insert(p);
		p = new Cavallo(bianco,0,1); s.add(0, 1, p); pezzi[bianco].insert(p);
		p = new Cavallo(bianco,0,6); s.add(0, 6, p); pezzi[bianco].insert(p);
		p = new Cavallo(nero,7,1); s.add(7, 1, p); pezzi[nero].insert(p);
		p = new Cavallo(nero,7,6); s.add(7, 6, p); pezzi[nero].insert(p);
		p = new Alfiere(bianco,0,2); s.add(0, 2, p); pezzi[bianco].insert(p);
		p = new Alfiere(bianco,0,5); s.add(0, 5, p); pezzi[bianco].insert(p);
		p = new Alfiere(nero,7,2); s.add(7, 2, p); pezzi[nero].insert(p);
		p = new Alfiere(nero,7,5); s.add(7, 5, p); pezzi[nero].insert(p);
		p = new Donna(bianco,0,3); s.add(0, 3, p); pezzi[bianco].insert(p);
		p = new Donna(nero,7,3); s.add(7, 3, p); pezzi[nero].insert(p);
		p = new Re(bianco,0,4); s.add(0, 4, p); pezzi[bianco].insert(p);
		re[bianco]=p;
		p = new Re(nero,7,4); s.add(7, 4, p); pezzi[nero].insert(p);
		re[nero]=p;
		nMosse = 0;
		primaMossa = 0;
		mosseNeutre = 0;
		arrocco[bianco]=(bool *) calloc(2,sizeof(bool));
		arrocco[nero]=(bool *) calloc(2,sizeof(bool));
		arrocco[bianco][0]=true; arrocco[bianco][1]=true;
		arrocco[nero][0]=true; arrocco[nero][1]=true;
	}

	Partita(string f){
	/* inizializza una partita partendo da una posizione espressa in notazione FEN
	 * (vedi https://it.wikipedia.org/wiki/Notazione_Forsyth-Edwards)
	 */	
		int i=0;
		int j=0;

		int k=0;
		/* posizione dei pezzi */
		while (f[k]!=' '){
			char c=f[k++];
			if ('0'<=c && c<='8') j+=c - '0';
			else if (c=='R' || c=='r' || c=='K' || c=='k' || c=='B' || c=='b' || c=='Q' || c=='q' || c=='P' || c=='p' 
						|| c=='N' || c=='n')
				insert(c,7-i,j++);
			else if (c == '/'){
				i++;
				j=0;
			}
			else printf("Fen Scorretta!!!\n\n");
		}
		/* turno */
		k++;
		if (f[k]=='w') {
			turno=bianco;
			avv==nero;
		}
		if (f[k]=='b') {
			turno=nero;
			avv==bianco;
		}
		/* arrocco */
		k++;
		arrocco[bianco]=(bool *) calloc(2,sizeof(bool));
		arrocco[nero]=(bool *) calloc(2,sizeof(bool));
		arrocco[bianco][0]=false; 
		arrocco[bianco][1]=false;
		arrocco[nero][0]=false; 
		arrocco[nero][1]=false;
		while(f[k]!=' '){
			if (f[k]='Q') arrocco[bianco][0]=true;
			if (f[k]='q') arrocco[nero][0]=true;
			if (f[k]='K') arrocco[bianco][1]=true;
			if (f[k]='k') arrocco[nero][1]=true;
			}
		/* eventuale presa al passo possibile */
		k++;
		if (f[k]='-') {
				ep=casellaImpossibile;
				k++;
			} else {
				ep=Casella(f[k],f[k+1]);
				k+=2;
			}
		/* numero semimosse dall'ultima cattura o mossa di pedone */
		k++;
		mosseNeutre = 0;	
		while (f[k] && f[k]!=' '){
			if (f[k]>='0' && f[k]<='9') mosseNeutre = mosseNeutre * 10 + f[k] - '0';
			k++;
		}

		/* numero mosse complessive */
		k++;
		nMosse = 0;	
		while (f[k] && f[k]!=' '){
			if (f[k]>='0' && f[k]<='9') nMosse = nMosse * 10 + f[k] - '0';
			k++;
		}
		primaMossa=nMosse;

	}


	void print(){
	/* stampa la posizione corrente: prima le liste dei pezzi, 
	 * poi la scacchiera, infine il turno
	 */	
		printf("\n");
		ep.print();
		printf("\n");		
		printPezzi(pezzi[bianco]);
		printPezzi(pezzi[nero]);
		printf("\n");

		for (int i=RIG-1; i>=0; i--){
			for (int j=0; j<COL; j++){ 
				Pezzo* p = s.dammiPezzo(i,j);
				if (p!=NULL) p->print();
					else printf(" . ");
				}
			printf("\n");	
		}
		turno.print();
	}

	void printPezzi(set<Pezzo *> s){
	/* stampa la lista dei pezzi sulla scacchiera 
	 */	
		for (set<Pezzo *>::iterator it = s.begin(); it!=s.end(); ++it)
			(*it)->printPos();
		printf("\n");
	}

	void printMosse(){
	/* stampa la sequenza delle mosse giocate (a partire dall'esecuzione del costruttore) 
	 */	
		bool semiMossa = false;
		int j=primaMossa;
		for (vector<Mossa>::iterator it = mosse.begin(); it!=mosse.end(); ++it){
			if (!semiMossa) {
				j++;
				printf("%d. ",j);
			}
			(*it).printM();
			if (semiMossa) printf("\n");
			semiMossa = !semiMossa;
		}
		printf("\n");
	}

	void printFEN(){
		int caselleLibere=0;
		for (int i=0; i<RIG; i++){
			if (i>0){ 
				if (caselleLibere>0){
					printf("%1d",caselleLibere);
					caselleLibere=0;
					}
				printf("/");
			}
			for (int j=0; j<COL; j++){
				if (s.dammiPezzo(i,j)==NULL) caselleLibere++;
				else {
					if (caselleLibere>0){
						printf("%1d",caselleLibere);
						caselleLibere=0;
					}
					s.dammiPezzo(i,j)->printFen();
				}
			}
		}	
		printf(" ");
		if (turno==bianco) printf("w");
			else printf("b");
		printf(" ");
		if (arrocco[bianco][0]) printf("K");	
		if (arrocco[bianco][1]) printf("Q");	
		if (arrocco[bianco][0]) printf("k");	
		if (arrocco[bianco][1]) printf("q");
		if (!arrocco[bianco][0] && !arrocco[bianco][1] && !arrocco[nero][0] && !arrocco[nero][1]) printf("-");
		printf(" ");
		if (ep==casellaImpossibile) printf("-");
		   else ep.print();	
		printf(" ");
		printf("%d ",mosseNeutre);   
		printf("%d ",nMosse);
		}   

	bool scaccoMatto(Colore t){
		Casella r = re[t]->posizione();
		Colore a = Colore(1-t.toInt());
		for (set<Pezzo *>::iterator it = pezzi[t].begin(); it!=pezzi[t].end(); ++it){
			set<Casella *> pm = (*it)->allMoves(s);
			for (set<Casella *>::iterator cit=pm.begin(); cit!=pm.end(); ++cit){
				Mossa m=eseguiMossa((*it)->posizione(), **cit);
				bool rss=reSottoScacco(t);
				ritiraMossa(m);
				if (!rss) {
					m.print();
					printf("\n");
					return false;
				}
			}
		}	
		return true;
		}

	int reSottoScacco(Colore t){
		Casella r = re[t]->posizione();
		Colore a = Colore(1-t.toInt());
		for (set<Pezzo *>::iterator it = pezzi[a].begin(); it!=pezzi[a].end(); ++it){
			if ((*it)->puoiMuovere(r,s)==LEGALE) return 1;
			}	
	return 0;	
	}

	int muovi(Casella da, Casella a){
		Mossa m;
		Pezzo *p=s.dammiPezzo(da);
		if (p==NULL) return CASELLA_VUOTA;
		if (p->colore()!=turno) return TURNO_ERRATO;
		int res=p->puoiMuovere(da, a, s);
		printError(res);

		/* analisi mosse speciali 	
		if (res==ARROCCO) && arroccoPossibile()){
			eseguiArrocco(da,a);
			res=LEGALE;
		}
		*/

		if (res==DOPPIO_PASSO) {
				ep=Casella((da.x+a.x)/2,da.y);
				res=LEGALE;
			} else if (res==LEGALE) ep=casellaImpossibile;

		if (res==LEGALE) m = eseguiMossa(da,a);

		if (reSottoScacco(turno)) {
			ritiraMossa(m);
			res=RE_SOTTO_SCACCO;
		}

		if (reSottoScacco(avv)){
			printf("Scacco al Re!!!\n");
			if (scaccoMatto(avv)) res=SCACCO_MATTO;
		}

		if (res==LEGALE) {
			turno.cambiaColore();
			avv.cambiaColore();
			mosse.push_back(m);
			nMosse++;
			mosseNeutre++;
		}
		
		return res;	
/*		if (scacco(turno)) {
			ritira();
			*/
		}

	int muovi(char* c){
	/* interpreta (grossolamente) l'input e lo passa a muovi(da,a)
	 * una mossa è data da casella inizio casella fine (esempio e2e4)
	 * l'arrocco è rappresentato con la mossa di Re (esempio e1g1)
	 * la promozione specifica il pezzo promosso, es. c7c8D significa che 
	 * il P bianco raggiunge l'ottava e promuove a Donna
	 */	
		if (c[0]<'a' || c[0]>'h' || c[2]<'a' || c[2]>'h') return SYNTAX_ERROR;
		if (c[1]<'1' || c[1]>'8' || c[3]<'1' || c[3]>'8') return SYNTAX_ERROR;
		Casella da(c[1]-'1',c[0]-'a');
		Casella a(c[3]-'1', c[2]-'a');
		return muovi(da,a);
	}	
	
	int ritira(){
		printf("%d\n",nMosse);
		if (nMosse==primaMossa) return INIZIO_PARTITA;
		nMosse--;
		mosseNeutre--;
		Mossa m = mosse.back();
		m.print();
		printf("\n");
		mosse.pop_back();
		m.print();
		printf("\n");
		ritiraMossa(m);
		turno.cambiaColore();
		avv.cambiaColore();
	}
}; //end class Partita

int main(){
	//Partita bn("r1bq1rk1/pp3ppp/3n4/2p1N3/2B5/7P/PPP2PP1/R1BQR1K1 w - 25 15");
	Partita bn;
	int res;
	char * mossa = (char *)calloc(6, sizeof(char));
	do {
		bn.print();
		scanf("%s",mossa);
		if (mossa[0]=='q') break;
		else if (mossa[0]=='p') bn.printMosse();	
		else if (mossa[0]=='r') res = bn.ritira();
		else if (mossa[0]=='n') bn.printFEN();
		else {
			res = bn.muovi(mossa);
			if (res == SCACCO_MATTO){
				bn.print();
				printf("ScaccoMatto!!!");
				break;
			}
		}	
//		if (res == PATTA){}
		printError(res);
	} while(true);
	printf("FINE!!");
	return 1;
}


