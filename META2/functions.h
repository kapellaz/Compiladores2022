 //Rui Santos 2020225542
 //Bruno Sequeira 2020235721

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef struct node * no;


void AdicionaIrmao(no nod,no irmao);
void Arvore(no raiz, int pontos);
no CriaNo(char *valor, char *s_type);
void AdicionaNo(no pai,no novo);
int conta_irmaos(no raiz);


typedef struct node {
	char *valor;
	char * s_type;
	no pai;
	no filho;
	no irmao;
	int num_node;
}node;








