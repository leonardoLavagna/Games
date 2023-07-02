#include  <stdio.h>
 
int giocatore(int n, int scelta)
{
	if (scelta < 1 || scelta > 3)
	{
		printf("\nLa scelta di elementi da togliere\n");
		printf("deve essereo 1 o 2 o 3\n");
		return n;
	}
	int restanti = n - scelta;
 
	printf("Il giocatore ha scelto di togliere %i elementi.\n", scelta);
	printf("rimangono %i elementi.\n", restanti);
 
	return restanti;
}
 
int computer(int n)
{
	int scelta = n % 4;
	int restanti = n - scelta;
 
	printf("\nIl computer toglie %u elementi.\n", scelta);
	printf("rimangono %i elementi\n", restanti);
 
	return restanti;
}


int main()
{
	printf("\nGioco Nim\n");
 
	int elementi = 12;
 
	while(elementi > 0)
	{
		printf("\nQuanti elementi vuoi togliere?: ");
 
		int uin;
		scanf("%i", &uin);
 
		int prossimo = giocatore(elementi, uin);
 
		if (prossimo == elementi)
		{
			continue;
		}
 
		elementi = prossimo;
 
		elementi = computer(elementi);
	}
	printf("\nHa vinto il computer!\n");
 
	return 0;
}
 
