#include <stdio.h>
#define N_DISCHI 4 

void PrintHanoi(int a[N_DISCHI][3]){

	int i, j;
	char * pattern[6];

	pattern[0] = "    |    ";
	pattern[1] = "    *    ";
	pattern[2] = "   ***   ";
	pattern[3] = "  *****  ";
	pattern[4] = " ******* ";
	pattern[5] = "*********";

	for (i=0; i<N_DISCHI; i++) {
		for (j=0; j<3; j++){
			printf("%s ", pattern[a[i][j]]);
		}
		printf("\n");
	}
	printf("-----------------------------\n");
	printf("\n");
}

void Move(int a[N_DISCHI][3], int source, int dest){

	int temp = 0;
	
	int riga_source = 0;
	int riga_dest = N_DISCHI - 1;

	while (a[riga_source][source] == 0) riga_source++;
	while (a[riga_dest][dest] != 0) riga_dest--;
	temp = a[riga_source][source];
	a[riga_source][source] = a[riga_dest][dest];
	a[riga_dest][dest] = temp;
	PrintHanoi(a);
}

void Hanoi(int a[N_DISCHI][3], int n, int source, int dest){

	int temp = 0;
	int dest2 = 3 - source - dest;
	
	if (n == 0){
		return;
	}
	else{
		Hanoi(a, n - 1, source, dest2);
		Move(a, source, dest);
		Hanoi(a, n - 1, dest2, dest);
	}
}

int main()
{

	int h[N_DISCHI][3];

	int i, j;

	for (i=0; i<N_DISCHI; i++) {
			h[i][0] = i+1;
	}

	for (i=0; i<N_DISCHI; i++) {
		for (j=1; j<3; j++){
			h[i][j] = 0;
		}
	}
	PrintHanoi(h);
	Hanoi(h, N_DISCHI, 0, 2);
}
