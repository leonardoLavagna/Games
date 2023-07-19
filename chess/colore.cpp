	#include "colore.h"

	Colore::Colore(){
		c=0;
	}

	Colore::Colore(int x){
		c=x;
	}

	Colore::Colore(const Colore& col){
		c=col.c;
	}

	void Colore::cambiaColore(){
		c = 1-c;
	}

	void Colore::print(){
		switch(c){
			case 0: printf("Nero\n");
			break;
			case 1: printf("Bianco\n");
			break;
			case 2: printf("Blue\n");
			break;
			case 3: printf("Rosso\n");
			break;
			case 4: printf("Verde\n");
			break;
			default: printf("Incolore\n");
		}
	}

	bool Colore::bianco(){
		return c==1;
	}

	bool Colore::nero(){
		return c==0;
	}
	bool Colore::operator ==(Colore col){
		return c==col.c;
	}

	bool Colore::operator !=(Colore col){
		if (c==col.c) return false;
			else return true;
	}

	bool Colore::operator <(Colore col) const {
		if (c<col.c) return true;
			else return false;
	}

	bool Colore::operator <=(Colore col) const {
		if (c<=col.c) return true;
			else return false;
	}

	int Colore::toInt() const {return c;}


