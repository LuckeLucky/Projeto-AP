#define _CRT_SECURE_NO_WARNINGS
//Include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Define
#define STRING char *
#define MAX_LINHA_FICHEIRO 150
//Estruturas
typedef char LinhaTexto[MAX_LINHA_FICHEIRO];
typedef struct
{
	int  ocupado;
	char id_contrato[10], nome[70], id_local[27], potencia[6];
}CONTRATO;
typedef struct
{
	int ocupado;
	char id_local[27], id_freguesia[7];
}LOCAIS;
typedef struct
{
	int contagem;
	char id_contrato[10], ano[5], mes[3], dia[3];
}CONTAGEM;
typedef struct
{
	char *distrito, *concelho, *freguesia,id_freguesia[7];
}FREGUESIA;
LinhaTexto LT;
//Prototipo das funções
STRING *Read_Split_Line_File(FILE *f, int n_campos_max, int *n_campos_lidos, char *separadores);
int contalinhas(char *nome);
int menu();
void inserir(CONTRATO *u, LOCAIS *t, int *n);
void apagacontrato(CONTRATO *u, LOCAIS *t, int n,int tipo);
void inserircontagem(CONTRATO *u, CONTAGEM *p, int *k);
int menu2();
int encontralocais(LOCAIS *t, char *compara, int n);
int encontracontrato(CONTRATO *c, char *compara, int n);
void mostracontagens(CONTAGEM *p, int *k,char *id);
void pesquisarmostrar(CONTRATO *u, LOCAIS *t, CONTAGEM *k, int n,int *conta);
void troca(char *a, char *b);
int ordenarapelido(char *a, char *b);
void ordena(CONTRATO *u, LOCAIS *t,int n, int tipo);
void mostra(CONTRATO *u, LOCAIS *t, CONTAGEM *k, int n, int *conta,int tipo);
//Main
void main()
{
	int n_campos_max = 20, n_campos_lidos;
	int opcao;
	int lcontrato = contalinhas("contratos.txt"), lcontagem = contalinhas("contagens.txt");//Guarda em uma variavel as linhas do contrato e na outra as linhas das contagens
	//Declarar os ponteiros para estruturas e posteriormente alocar espaço para eles
	//+20 sempre que for preciso adicionar...
	CONTRATO *c =(CONTRATO*)malloc((lcontrato + 20) * sizeof(CONTRATO));
	LOCAIS *l= (LOCAIS*)malloc((lcontrato + 20) * sizeof(LOCAIS));
	CONTAGEM *q=(CONTAGEM*)malloc((lcontagem+20)* sizeof(CONTAGEM));
	FREGUESIA *f= (FREGUESIA*)malloc(2882 * sizeof(FREGUESIA));//Tem sempre o mesmo número de linhas
	//Abrir os ficheiros em modo leitura
	FILE *contrato = fopen("contratos.txt", "r");
	FILE *locais= fopen("locais.txt", "r"); 
	FILE *contagem= fopen("contagens.txt", "r"); 
	FILE *freguesia= fopen("freguesias.txt", "r");
	if (!contrato || !locais || !contagem || !freguesia) 
	{
		printf("\n\n\tImpossivel abrir um dos Ficheiros \n\n");
	}
	//Ler os ficheiros para as respetivas estruturas
	while (!feof(contrato))
	{
		STRING *V = Read_Split_Line_File(contrato, n_campos_max, &n_campos_lidos, "\t\r\n");
		strcpy(c->id_contrato, V[0]);
		strcpy(c->nome, V[1]);
		strcpy(c->id_local, V[2]);
		strcpy(c->potencia, V[3]);
		c->ocupado = 1;
		c++;
		for (int i = 0; i < n_campos_lidos; i++)
			free(V[i]);
		free(V);
	}
	while (!feof(locais))
	{
		STRING *V = Read_Split_Line_File(locais, n_campos_max, &n_campos_lidos, "\t\r\n");
		strcpy(l->id_local, V[0]);
		strcpy(l->id_freguesia, V[1]);
		l->ocupado = 1;
		l++;
		for (int i = 0; i < n_campos_lidos; i++)
			free(V[i]);
		free(V);
	}
	while (!feof(contagem))
	{
		STRING *V = Read_Split_Line_File(contagem, n_campos_max, &n_campos_lidos, "\t.\r\n");
		strcpy(q->id_contrato, V[0]);
		q->contagem = atoi(V[1]);
		strcpy(q->ano, V[2]);
		strcpy(q->mes, V[3]);
		strcpy(q->dia, V[4]);
		q++;
		for (int i = 0; i < n_campos_lidos; i++)
			free(V[i]);
		free(V);
	}
	while (!feof(freguesia))
	{
		STRING *V = Read_Split_Line_File(freguesia, n_campos_max, &n_campos_lidos, "\t\r\n");
		strcpy(f->id_freguesia, V[0]);
		f->distrito = V[1];
		f->concelho = V[2];
		f->freguesia = V[3];
		f++;
	}
	c -= lcontrato;
	l -= lcontrato;
	q -= lcontagem;
	f -= 2882;
	//MENU_________________
	do
	{
		opcao = menu();
		system("cls");
		switch (opcao)
		{
		case 1: inserir(c, l,&lcontrato);
			break;
		case 2:apagacontrato(c, l,lcontrato,1);
			break;
		case 3:pesquisarmostrar(c, l, q, lcontrato, &lcontagem);
			break;
		case 4:ordena(c, l, lcontrato, 1); mostra(c, l, q, lcontrato, &lcontagem, 1);
			break;
		case 5:ordena(c, l, lcontrato, 2); mostra(c, l, q, lcontrato, &lcontagem, 1);
			break;
		case 6:ordena(c, l, lcontrato, 3); mostra(c, l, q, lcontrato, &lcontagem, 1);
			break;
		case 7:ordena(c, l, lcontrato, 4); mostra(c, l, q, lcontrato, &lcontagem, 1);
			break;
		case 8:ordena(c, l, lcontrato, 5); mostra(c, l, q, lcontrato, &lcontagem, 1);
			break;
		case 9:ordena(c, l, lcontrato, 6); mostra(c, l, q, lcontrato, &lcontagem, 2);
			break;
		case 0:  break;
		}

	} while (opcao != 0);
	//Fechar os ficheiros
	fclose(contrato);
	fclose(locais);
	fclose(contagem);
	fclose(freguesia);
	//Guardar ...
}
//Funções
STRING *Read_Split_Line_File(FILE *f, int n_campos_max, int *n_campos_lidos, char *separadores)
{
	*n_campos_lidos = 0;
	if (!f) return NULL;
	if (fgets(LT, MAX_LINHA_FICHEIRO, f) != NULL)	// fgets lê uma linha do ficheiro de texto para a string LT
	{
		// "partir" a linha lida, usando os separadores definidos
		STRING *Res = (STRING *)malloc(n_campos_max * sizeof(STRING));  // alocação de um array com n_campos_max ponteiros para STRING
		char *pch = strtok(LT, separadores);
		int cont = 0;
		while (pch != NULL)
		{
			Res[cont] = (char *)malloc((strlen(pch) + 1) * sizeof(char*)); // alocação do espaço necessário para guardar a string correspondente ao campo
			strcpy(Res[cont++], pch);
			pch = strtok(NULL, separadores);
		}
		*n_campos_lidos = cont;
		return Res;
	}
	return NULL;
}
int contalinhas(char *nome)
{
	int ch = 0, linha=1;
	FILE *f = fopen(nome, "r");
	if (f == NULL)
	{
		printf("Impossivel Abrir ficheiro para contar as linhas");
		return 0;
	}
	while (!feof(f))//Conta o numero de linhas do ficheiro
	{
		ch = fgetc(f);//Pega char a char e se for \n "mudança de linha" linha+1
		if (ch == '\n')
		{
			linha++;
		}
	}
	fclose(f);
	return linha;
}
int menu()
{
	int op;
	printf("\t\t\tMENU");
	printf("\n___________________Gestao De Clientes________________");
	printf("\n|Inserir Contrato++++++++++++++++++++++++++++++++(1)|");
	printf("\n|Apagar Contrato---------------------------------(2)|");
	printf("\n|Pesquisar por parte do nome+++++++++++++++++++++(3)|");
	printf("\n_____________________Ordenar________________________");
	printf("\n|Id_Contrato-------------------------------------(4)|");
	printf("\n|Nome++++++++++++++++++++++++++++++++++++++++++++(5)|");
	printf("\n|Apelido-----------------------------------------(6)|");
	printf("\n|Id_Local++++++++++++++++++++++++++++++++++++++++(7)|");
	printf("\n|Potencia----------------------------------------(8)|");
	printf("\n|Id_Freguesia++++++++++++++++++++++++++++++++++++(9)|");
	printf("\nSair_____________________________________________(0)");
	do
	{
		printf("Opcao?");
		scanf("%d", &op);
	} while (op < 0 || op>9);
	return op;
}
void inserir(CONTRATO *u, LOCAIS *t, int *n)
{
	for (int i = 0; i < *n; i++)//Procura por um espaço livre na estrutura
	{
		if (u->ocupado == 1)
			u++;
		if (t->ocupado == 1)
			t++;
	}
	printf("\t\t\t\t\t\t*INSERIR CONTRATO*");
	fseek(stdin, 0, SEEK_END);
	printf("\nid_contrato(9):");
	gets_s(u->id_contrato, 10);
	fseek(stdin, 0, SEEK_END);
	printf("\nNome do Cliente:");
	gets_s(u->nome, 50);
	fseek(stdin, 0, SEEK_END);
	printf("\nid_local(PT..):");
	gets_s(u->id_local, 27);
	strcpy(t->id_local, u->id_local);
	fseek(stdin, 0, SEEK_END);
	printf("\nid_freguesia(6):");
	gets_s(t->id_freguesia, 7);
	fseek(stdin, 0, SEEK_END);
	printf("\nPotencia:");
	gets_s(u->potencia, 6);
	u->ocupado = 1;
	t->ocupado = 1;
	(*n)++;//Como inserimos 1 contrato agora a linha do contrato vai ser +1
	printf("\n\tInserido!\n");
}
void apagacontrato(CONTRATO *u, LOCAIS *t, int n,int tipo)
{
	char id[10], cp1=1,cp2=1;
	printf("\t\t\t\t\t\t*APAGAR CONTRATO*");
	if (tipo == 1)
	{
		fseek(stdin, 0, SEEK_END);
		printf("\nid_contrato:");
		gets_s(id, 10);

		for (int i = 0; i < n &&cp1 != 0; i++)//Econtra-se na estrutura aquele que tem o mesmo id_contrato 
		{
			cp1 = strcmp(u->id_contrato, id);
			if (cp1 < 0 || cp1>0)
				u++;
		}
	}
	u->ocupado = 0;
	for (int i = 0; i < n && cp2 != 0; i++)//Já decoberto podemos comparar o id_local da estrutura dos contratos com o id_local da estrutura dos locais
	{
		cp2 = strcmp(u->id_local, t->id_local);
		if (cp2 < 0 || cp2>0)
			t++;
	}
	t->ocupado = 0;
	printf("\n%s\t%s\t%s\t%s", u->id_contrato, u->nome, u->id_local, u->potencia);
	printf("\n\tApagado!\n");
}
void inserircontagem(CONTRATO *u, CONTAGEM *p, int *k)
{
	int b = 0;
	while (b<*k)
	{
		p++;
		b++;
	}
	fseek(stdin, 0, SEEK_END);
	printf("\nContagem:");
	scanf("%d", &p->contagem);
	fseek(stdin, 0, SEEK_END);
	printf("\nAno:");
	gets_s(p->ano, 5);
	fseek(stdin, 0, SEEK_END);
	printf("\nMes:");
	gets_s(p->mes, 3);
	fseek(stdin, 0, SEEK_END);
	printf("\nDia:");
	gets_s(p->dia, 3);
	strcpy(p->id_contrato, u->id_contrato);
	(*k)++;
	printf("\nContagem Adicionada");
}
int menu2()
{
	int op;
	printf("\n1->Adicionar Contagem");
	printf("\n2->Mostra Contagens");
	printf("\n3->Remover Contrato");
	printf("\n0->Regressar ao Menu Principal");
	do
	{
		printf("\nOpcao?");
		scanf("%d", &op);
	} while (op < 0 || op>9);
	return op;
}
int encontralocais(LOCAIS *t, char *compara, int n)
{
	//Função recebe sempre o id local
	int cmp, i = 0;
	for (; i < n; i++)
	{
		cmp = strcmp(compara, t[i].id_local);
		if (cmp == 0)
			return i;
	}
	return NULL;
}
int encontracontrato(CONTRATO *c, char *compara, int n)
{
	//Função recebe sempre o id local 
	int cmp, i = 0;
	for (; i < n; i++)
	{
		cmp = strcmp(compara, c[i].id_local);
		if (cmp == 0)
			return i;
	}
	return NULL;
}
void mostracontagens(CONTAGEM *p, int *k,char *id)
{
	int compara = 1;
	for (int i = 0; i < *k; i++)
	{
		compara = strcmp(id, p->id_contrato);
		if (compara == 0)
		{
			printf("%d\t%s.%s.%s\n", p->contagem, p->ano, p->mes, p->dia);
			p++;
		}
		else
			p++;
	}
}
void pesquisarmostrar(CONTRATO *u, LOCAIS *t, CONTAGEM *k, int n, int *conta)
{
	printf("\t\t\t\t\t\t*PESQUISAR POR PARTE DO NOME*");
	char pnome[40];
	int i = 0;
	char *esta = NULL;
	char Y = 'N';
	int op, b = 0;
	do {//Procura se a parte inserida no nome se encontra num dos nomes dentro da estrutura
		fseek(stdin, 0, SEEK_END);
		printf("\n Parte do Nome:");
		gets_s(pnome, 40);
		for (; i < n&& Y=='N'; i++)
		{
			esta = strstr(u->nome, pnome);//strstr devolve NULL se nao aparecer a string na outra string maior
			if (esta == NULL)
				u++;
			if (esta != NULL && u->ocupado==1)//Quando encontramos uma possivel hipotese está tem tem que estar ocupada
			{
				printf("%s\t%s\t%s\t%s\n", u->id_contrato, u->nome, u->id_local, u->potencia);
				printf("Era este contrato que procurava ..S..N..)");
				scanf(" %c", &Y);
				if (Y == 'N')
					u++;
			}
		}
		if (i == n && Y=='N')//Caso se precorra o ciclo for completamente e não seja encontrada a parte do nome
		{
			printf("Impossivel Encontrar!\n");
			u -= n;
			i = 0;
		}//Vai ser pedida novamente uma parte do nome
	} while (Y=='N');
	do
	{
		op = menu2();
		system("cls");
		switch (op)
		{
		case 1:inserircontagem(u, k, conta);
			break;
		case 2:mostracontagens(k, conta, u->id_contrato);
			break;
		case 3:apagacontrato(u, t, n, 0); op = 0;//Se apagarmos o contrato já não é útil usar mais nada deste menu
			break;
		case 0:  break;
		}

	} while (op >0&&op<4);
}
void troca(char *a, char *b)
{
	char troca[70];
	strcpy(troca, a);
	strcpy(a, b);
	strcpy(b, troca);
}
int ordenarapelido(char *a, char *b)
{
	int ta = strlen(a), tb = strlen(b);
	int n;
	if (ta >= tb)
		n = ta;
	else
		n = tb;
	while (a[ta] != ' ')
		ta--;
	while (b[tb] != ' ')
		tb--;
	do
	{
		if (a[ta] > b[tb])
			return 1;
		if (a[ta] < b[tb])
			return -1;
		ta++;
		tb++;
	} while (a[ta] != '\0' || b[tb] != '\0');
	return 0;
	
}
void ordena(CONTRATO *u, LOCAIS *t,int n, int tipo)
{
	int res;
	ordenarapelido(u[0].nome, u[1].nome);
	for (int i=0; i < n - 1; i++)//n-1 ja que não vai ser necessario ordenar o ultimo
	{
		int aux;
		for (int j =0; j < n-1; j++)//procura	mínimo	entre	o	índice	i	e	o	fim
		{
			switch (tipo)
			{
			case 1: res = strcmp(u[j].id_contrato, u[j + 1].id_contrato); break;
			case 2: res = strcmp(u[j].nome, u[j + 1].nome); break;
			case 3: res = ordenarapelido(u[j].nome, u[j + 1].nome); break;
			case 4:	res = strcmp(u[j].id_local, u[j + 1].id_local); break;
			case 5:	res = strcmp(u[j].potencia, u[j + 1].potencia); break;
			case 6: res = strcmp(t[j].id_freguesia, t[j + 1].id_freguesia); break;
			}
			if (res > 0)//O que significa que no strcmp a s1 é maior que a s2 tendo que ser feita a troca
			{
				if (tipo != 6)
				{
					troca(u[j].id_contrato, u[j + 1].id_contrato);
					troca(u[j].nome, u[j + 1].nome);
					troca(u[j].id_local, u[j + 1].id_local);
					troca(u[j].potencia, u[j + 1].potencia);
					aux = u[j].ocupado;
					u[j].ocupado = u[j + 1].ocupado;
					u[j + 1].ocupado = aux;
				}
				else
				{
					troca(t[j].id_local, t[j + 1].id_local);
					troca(t[j].id_freguesia, t[j + 1].id_freguesia);
					aux = t[j].ocupado;
					t[j].ocupado = t[j + 1].ocupado;
					t[j + 1].ocupado = aux;
				}
			}
		}
	}
}
int menu3()
{
	int op;
	printf("\n1->Adicionar Contagem");
	printf("\n2->Mostra Contagens");
	printf("\n3->Remover Contrato");
	printf("\n5->Avancar");
	printf("\n6->Recuar");
	printf("\n0->Regressar ao Menu Principal");
	do
	{
		printf("\nOpcao?");
		scanf("%d", &op);
	} while (op < 0 || op>9);
	return op;
}
void mostra(CONTRATO *u, LOCAIS *t, CONTAGEM *k, int n, int *conta,int tipo)
{
	int op, res, pl,pc;
	do {
		if (tipo == 1)//Contrato
		{
			pc = 0;
			pl = encontralocais(t, u[pc].id_local, n);
		}
		if (tipo == 2)//Locais
			{
			pl = 0;
			pc = encontracontrato(u, t[pl].id_local, n);
			if (pc == NULL)
			{
				printf("Não existe esse id_local nos locais.txt");
			}
			}
		printf("%s\t%s\t%s\t%s\t%s", u[pc].id_contrato, u[pc].nome, u[pc].id_local, u[pc].potencia, t[pl].id_freguesia);
		do
		{
			op = menu3();
			system("cls");
			switch (op)
			{
			case 1:inserircontagem(u, k, conta);
				break;
			case 2:mostracontagens(k, conta, u->id_contrato);
				break;
			case 3:apagacontrato(u, t, n, 0); op = 0;//Se apagarmos o contrato já não é útil usar mais nada deste menu
				break;
			case 0:op = 0;  break;
			}
		} while (op > 0 && op <4 );


	}while (op!=0);
}