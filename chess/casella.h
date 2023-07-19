#include<stdio.h>

class Casella {

public:
	int x;
	int y;

Casella(){
	x=0;
	y=0;
}	

Casella(char * c){
	x=c[0]-'a';
	y=c[1]-'0';
}

Casella(char c0, char c1){
	x=c0-'a';
	y=c1-'0';
}

Casella(int a, int b){
		x=a;
		y=b;
	}

Casella(const Casella& c){
		x=c.x;
		y=c.y;
	}

void move(int dx, int dy){
	x=x+dx;
	y=y+dy;
}

bool operator==(Casella c){
	return x==c.x && y==c.y;
}	

bool operator!=(Casella c){
	return x!=c.x || y!=c.y;
}	

void print(){
	printf("%1c%1d",'a'+y,x+1);
}

void printCoord(){
	printf("(%1d,%1d)\n",x,y);
}

}; //end class Casella