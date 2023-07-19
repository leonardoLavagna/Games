#include <stdlib.h>
#include "casella.h"

#define RIG 8 
#define COL 8


class Pezzo;

class Scacchiera{
	Pezzo*** s;
	int calcolaDirezione(int n, int m);
public:
	Scacchiera();
	void print();
	void add(int x, int y, Pezzo *p);
	void add(Casella in, Pezzo *p);
	Pezzo* remove(Casella da);
	Pezzo* dammiPezzo(Casella in);
	Pezzo* dammiPezzo(int x, int y);
	bool lineaLibera(Casella da, Casella a);	
};

