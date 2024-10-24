#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DESCR 50
#define TAM_MAX 1000

/* Armazena um horário de chegada */
typedef struct
{
  int hh;
  int mm;
  int ss;
} horario;

/* Armazena informações de um processo */
typedef struct
{
  int prior;
  horario chegada;
  char descricao[MAX_DESCR];
} celula;

/*Minha strutura de dados : Lista sequencial*/
typedef struct lista
{
  int tam;
  celula *lista_de_processos[TAM_MAX];
  int inicio;
  int fim;
} LISTA;

/*protóripos de funções*/
LISTA *lista_criar();
void lista_inserir_processo(celula *, LISTA *);
void lista_imprimir(LISTA *);
void lista_executar_processo(LISTA *);
void QuickSort(LISTA *, int, int, int, int);
int particiona_prior(LISTA *, int, int, int);
int particiona_horario(LISTA *lista, int inicio, int fim, int cres_decres);
int horario_para_segundo(horario);

int main()
{
  LISTA *lista = lista_criar();

  char comando[7];
  char comando2[3];

  scanf(" %s", comando);

  while (strcmp(comando, "quit"))
  {

    celula *processo = (celula *)malloc(sizeof(celula));

    if (!(strcmp(comando, "add")))
    {
      scanf(" %d %d %d %d %s ", &processo->prior, &processo->chegada.hh, &processo->chegada.mm, &processo->chegada.ss, processo->descricao);
      lista_inserir_processo(processo, lista);
    }
    if (!(strcmp(comando, "exec"))) // executa tarefa, ou seja elimina ela da lista de processos
    {
      scanf(" %s", comando2);

      if (!(strcmp(comando2, "-p"))) // quando for igual a -p ---> executa com maior prioridade
      {
        QuickSort(lista, lista->inicio, lista->fim - 1, 0, 0);
        lista_executar_processo(lista);
      }
      else // -t ---> executa com o menor tempo
      {
        QuickSort(lista, lista->inicio, lista->fim - 1, 1, 1);
        lista_executar_processo(lista);
      }
    }
    if (!(strcmp(comando, "next")))
    {
      scanf(" %s", comando2);
      if (!(strcmp(comando2, "-p"))) // quando for igual a -p ---> exibe o processo de maior prioridade
      {
        QuickSort(lista, lista->inicio, lista->fim - 1, 0, 0);
        printf("%d %02d:%02d:%02d %s ", lista->lista_de_processos[lista->fim - 1]->prior, lista->lista_de_processos[lista->fim - 1]->chegada.hh, lista->lista_de_processos[lista->fim - 1]->chegada.mm, lista->lista_de_processos[lista->fim - 1]->chegada.ss, lista->lista_de_processos[lista->fim - 1]->descricao);
      }
      else // quando for igual a -t ---> exibe o processo de menor horario
      {
        QuickSort(lista, lista->inicio, lista->fim - 1, 1, 1);
        printf("%d %02d:%02d:%02d %s ", lista->lista_de_processos[lista->fim - 1]->prior, lista->lista_de_processos[lista->fim - 1]->chegada.hh, lista->lista_de_processos[lista->fim - 1]->chegada.mm, lista->lista_de_processos[lista->fim - 1]->chegada.ss, lista->lista_de_processos[lista->fim - 1]->descricao);
      }
      printf("\n\n");
    }
    if (!(strcmp(comando, "chance")))
    {
      scanf(" %s", comando2);
      if (!(strcmp(comando2, "-p"))) // quando for igual a -p
      {
      }
      else
      {
      }
    }
    if (!(strcmp(comando, "print")))
    {
      scanf(" %s", comando2);
      if (!(strcmp(comando2, "-p"))) // quando for igual a -p --> imprimi os processos em ordem decrescente de prioridade
      {
        QuickSort(lista, lista->inicio, lista->fim - 1, 1, 0);
        lista_imprimir(lista);
      }
      else
      {
        QuickSort(lista, lista->inicio, lista->fim - 1, 0, 1);
        lista_imprimir(lista);
      }
      printf("\n\n");
    }

    scanf(" %s", comando);
  };
  /*printf("\nNÃO ORDENADO\n");
  lista_imprimir(lista);
  printf("\nGerenciador de processos finalizado\n");

  printf("\nORDENADO\n");
  QuickSort(lista, lista->inicio, lista->fim - 1, 1, 1);
  lista_imprimir(lista);
  printf("\nGerenciador de processos finalizado\n");*/
  return 0;
}
/*Funções da estrutura de dado*/
LISTA *lista_criar()
{
  LISTA *lista = (LISTA *)malloc(sizeof(LISTA));
  if (lista != NULL)
  {
    lista->tam = 0;
    lista->inicio = 0;
    lista->fim = 0;
    return lista;
  }

  return NULL;
}

// função de inserir processo na lista de processos(array)
void lista_inserir_processo(celula *processo, LISTA *lista)
{
  if (lista != NULL && lista->tam != TAM_MAX)
  {
    lista->lista_de_processos[lista->fim] = processo;
    lista->tam++;
    lista->fim++;
  }
}

void lista_imprimir(LISTA *lista)
{
  if (lista != NULL && !lista->tam == 0)
  {
    int i;

    for (i = 0; i < lista->fim; i++)
    {
      printf("%d %02d:%02d:%02d %s ", lista->lista_de_processos[i]->prior, lista->lista_de_processos[i]->chegada.hh, lista->lista_de_processos[i]->chegada.mm, lista->lista_de_processos[i]->chegada.ss, lista->lista_de_processos[i]->descricao);
      printf("\n");
    }
  }
}

void lista_executar_processo(LISTA *lista) // remove sempre o último elemento do array
{
  if (lista != NULL && lista->tam != 0)
  {
    // executa processo de maior prioridade (remove o último elemento)

    if (lista->tam == TAM_MAX)
    {
      free(lista->lista_de_processos[lista->fim]);
      lista->lista_de_processos[lista->fim] = NULL;
    }
    else
    {
      free(lista->lista_de_processos[lista->fim - 1]);
      lista->lista_de_processos[lista->fim - 1] = NULL;
    }
    lista->tam--;
    lista->fim--;
  }
}

/*Funções de ordenação -> prioridade  */

int particiona_prior(LISTA *lista, int inicio, int fim, int cres_decres)
{

  int meio = (inicio + fim) / 2;

  int pivo = ((lista->lista_de_processos[inicio]->prior) + (lista->lista_de_processos[fim]->prior) + (lista->lista_de_processos[meio]->prior)) / 3; // Elemento pivô(prioridades)

  while (inicio < fim)
  {
    if (cres_decres == 0) // ordena em ordem crescente si --> cres_decres = 0
    {
      while (inicio < fim && lista->lista_de_processos[inicio]->prior <= pivo)
        inicio++;

      while (inicio < fim && lista->lista_de_processos[fim]->prior > pivo)
        fim--;
    }
    else // ordena em ordem decrescente si --> cres_decres = 1
    {
      while (inicio < fim && lista->lista_de_processos[inicio]->prior > pivo)
        inicio++;

      while (inicio < fim && lista->lista_de_processos[fim]->prior <= pivo)
        fim--;
    }
    celula *aux = lista->lista_de_processos[inicio];
    lista->lista_de_processos[inicio] = lista->lista_de_processos[fim];
    lista->lista_de_processos[fim] = aux;
  }

  return inicio;
}

void QuickSort(LISTA *lista, int inicio, int fim, int cres_decres, int tipo)
{

  if (inicio < fim)
  {
    int pos;
    if (tipo == 0) // particiona_prioridade
    {
      pos = particiona_prior(lista, inicio, fim, cres_decres);
    }
    else // particiona_horario
    {

      pos = particiona_horario(lista, inicio, fim, cres_decres);
    }
    QuickSort(lista, inicio, pos - 1, cres_decres, tipo);
    QuickSort(lista, pos, fim, cres_decres, tipo);
  }
}

/*Funções de ordenação -> horário  */

int horario_para_segundo(horario h)
{
  return (h.hh * 3600 + h.mm * 60 + h.ss);
}

int particiona_horario(LISTA *lista, int inicio, int fim, int cres_decres)
{
  int meio = (inicio + fim) / 2;
  int pivo = ((horario_para_segundo(lista->lista_de_processos[inicio]->chegada)) + (horario_para_segundo(lista->lista_de_processos[meio]->chegada)) + (horario_para_segundo(lista->lista_de_processos[fim]->chegada))) / 3;

  while (inicio < fim)
  {
    if (cres_decres == 0) // Ordena em ordem crescente
    {
      while (inicio < fim && horario_para_segundo(lista->lista_de_processos[inicio]->chegada) <= pivo)
        inicio++;

      while (inicio < fim && horario_para_segundo(lista->lista_de_processos[fim]->chegada) > pivo)
        fim--;
    }
    else // Ordena em ordem decrescente
    {
      while (inicio < fim && horario_para_segundo(lista->lista_de_processos[inicio]->chegada) > pivo)
        inicio++;

      while (inicio < fim && horario_para_segundo(lista->lista_de_processos[fim]->chegada) <= pivo)
        fim--;
    }

    celula *aux = lista->lista_de_processos[inicio];
    lista->lista_de_processos[inicio] = lista->lista_de_processos[fim];
    lista->lista_de_processos[fim] = aux;
  }

  return inicio;
}