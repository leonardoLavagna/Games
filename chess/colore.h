#include <stdio.h>

class Colore{
	int c;
public:
	Colore();
	Colore(int x);
	Colore(const Colore& col);
	void cambiaColore();
	void print();
	bool bianco();
	bool nero();
	bool operator ==(Colore col);
	bool operator !=(Colore col);
	bool operator <(Colore col) const;
	bool operator <=(Colore col) const;
	int toInt () const;

}; //end class Colore
