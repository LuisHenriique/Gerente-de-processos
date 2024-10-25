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

/*Protótipos de funções*/
LISTA *lista_criar();
void lista_inserir_processo(celula *, LISTA *);
void lista_imprimir(LISTA *);
void lista_executar_processo(LISTA *);
void lista_destruir(LISTA *);

void QuickSort(LISTA *, int, int, int, int);
int particiona_prior(LISTA *, int, int, int);
int particiona_horario(LISTA *lista, int inicio, int fim, int cres_decres);
int horario_para_segundo(horario);
int busca_binaria(LISTA *, celula *, int);

int main()
{
  LISTA *lista = lista_criar();

  char comando[7];  // Armazena o comando principal
  char comando2[3]; // Armazena o argumento do comando (ex: -p, -t)

  scanf(" %s", comando);

  /* Processamento contínuo até o comando "quit" */
  while (strcmp(comando, "quit") != 0)
  {

    celula *processo = (celula *)malloc(sizeof(celula)); // Aloca espaço para novo processo
    if (processo == NULL)
    {
      fprintf(stderr, "Erro de alocação de memória\n");
      exit(1);
    }

    /* Adicionar novo processo à lista */
    if (strcmp(comando, "add") == 0)
    {
      scanf(" %d %d %d %d %s ", &processo->prior, &processo->chegada.hh, &processo->chegada.mm, &processo->chegada.ss, processo->descricao);
      lista_inserir_processo(processo, lista);
    }
    if (strcmp(comando, "exec") == 0) // executa tarefa, ou seja elimina ela da lista de processos
    {
      scanf(" %s", comando2);

      if (strcmp(comando2, "-p") == 0) // quando for igual a -p ---> executa o de maior prioridade
      {
        QuickSort(lista, lista->inicio, lista->fim - 1, 0, 0); // Ordena a lista em ordem crescente de prioridades
        lista_executar_processo(lista);                        // Elimina o último elemento da lista
      }
      else // -t ---> executa o de menor horário
      {
        QuickSort(lista, lista->inicio, lista->fim - 1, 1, 1); // Ordena a lista em ordem decrescente de horários
        lista_executar_processo(lista);                        // Elimina o último elemento da lista
      }
    }
    if (strcmp(comando, "next") == 0) // Exibe o processo
    {
      scanf(" %s", comando2);
      if (strcmp(comando2, "-p") == 0) // quando for igual a -p ---> exibe o processo de maior prioridade
      {
        QuickSort(lista, lista->inicio, lista->fim - 1, 0, 0); // Ordena a lista em ordem crescente de prioridades
        printf("%02d %02d:%02d:%02d %s ", lista->lista_de_processos[lista->fim - 1]->prior, lista->lista_de_processos[lista->fim - 1]->chegada.hh, lista->lista_de_processos[lista->fim - 1]->chegada.mm, lista->lista_de_processos[lista->fim - 1]->chegada.ss, lista->lista_de_processos[lista->fim - 1]->descricao);
      }
      else // quando for igual a -t ---> exibe o processo de menor horario
      {
        QuickSort(lista, lista->inicio, lista->fim - 1, 1, 1); // Ordena a lista em ordem decrescente de horários
        printf("%02d %02d:%02d:%02d %s ", lista->lista_de_processos[lista->fim - 1]->prior, lista->lista_de_processos[lista->fim - 1]->chegada.hh, lista->lista_de_processos[lista->fim - 1]->chegada.mm, lista->lista_de_processos[lista->fim - 1]->chegada.ss, lista->lista_de_processos[lista->fim - 1]->descricao);
      }
      printf("\n");
      printf("\n");
    }
    if (strcmp(comando, "change") == 0) // Muda os dados do processo
    {
      if (lista != NULL)
      {
        celula *aux = (celula *)malloc(sizeof(celula)); // Aloca um processo auxiliar

        if (aux == NULL)
        {
          fprintf(stderr, "Erro de alocação de memória\n");
          exit(1);
        }

        int pos;

        scanf(" %s", comando2);          // Comando seguinte -p ou -t
        if (strcmp(comando2, "-p") == 0) // quando for igual a -p faz a mudança na prioridade
        {
          scanf("%d", &processo->prior);
          scanf("%d", &aux->prior);

          QuickSort(lista, lista->inicio, lista->fim - 1, 0, 0); // ordena a lista em ordem de prioridade(crescente) antes de realizar a busca binária

          pos = busca_binaria(lista, processo, 1); // Guarda o índice do elemento encontrado na busca binária

          lista->lista_de_processos[pos]->prior = aux->prior; // substitui a nova prioridade na antiga prioridade
        }
        else
        {
          scanf("%d %d %d", &processo->chegada.hh, &processo->chegada.mm, &processo->chegada.ss);
          scanf("%d %d %d", &aux->chegada.hh, &aux->chegada.mm, &aux->chegada.ss);

          QuickSort(lista, lista->inicio, lista->fim - 1, 0, 1); // ordena a lista em ordem de horario (crescente) antes de realizar a busca binária

          pos = busca_binaria(lista, processo, 0); // Guarda o índice do elemento encontrado na busca binária

          // Substitui o novo horário no antigo horário
          lista->lista_de_processos[pos]->chegada.hh = aux->chegada.hh;
          lista->lista_de_processos[pos]->chegada.mm = aux->chegada.mm;
          lista->lista_de_processos[pos]->chegada.ss = aux->chegada.ss;
        }

        // Libera a memória do processo auxiliar
        free(aux);
        aux = NULL;
      }
    }
    if (strcmp(comando, "print") == 0)
    {
      scanf(" %s", comando2);
      if (strcmp(comando2, "-p") == 0) // quando for igual a -p --> imprimi os processos em ordem decrescente de prioridade
      {
        QuickSort(lista, lista->inicio, lista->fim - 1, 1, 0); // Ordena a lista em ordem decrescente de prioridades
        lista_imprimir(lista);
      }
      else
      {
        QuickSort(lista, lista->inicio, lista->fim - 1, 0, 1); // Ordena a lista em ordem crescente de horários
        lista_imprimir(lista);
      }
      printf("\n");
    }

    scanf(" %s", comando);
  };

  // Libera todos os processos alocados e libera a lista
  lista_destruir(lista);

  return 0;
}
/*Funções da estrutura de dados*/
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
  if (lista != NULL)
  {
    int i;

    for (i = 0; i < lista->fim; i++)
    {
      printf("%02d %02d:%02d:%02d %s ", lista->lista_de_processos[i]->prior, lista->lista_de_processos[i]->chegada.hh, lista->lista_de_processos[i]->chegada.mm, lista->lista_de_processos[i]->chegada.ss, lista->lista_de_processos[i]->descricao);
      printf("\n");
    }
  }
}

void lista_executar_processo(LISTA *lista) // remove sempre o último elemento do array
{
  if (lista != NULL && lista->tam != 0)
  {
    // executa processo de maior prioridade ou de menor horário (remove o último elemento do array)

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
void lista_destruir(LISTA *lista)
{
  if (lista != NULL)
  {
    for (int i = 0; i < lista->tam; i++)
    {
      free(lista->lista_de_processos[i]);
      lista->lista_de_processos[i] = NULL;
    }

    free(lista);
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

/*Metodo de busca*/

int busca_binaria(LISTA *lista, celula *processo, int tipo)
{

  if (lista != NULL)
  {
    int inf = lista->inicio;
    int sup = lista->fim - 1;
    int meio;

    while (inf <= sup)
    {
      meio = (inf + sup) / 2;
      if (tipo == 1) // ---> faz busca de acordo com a prioridade
      {
        if (lista->lista_de_processos[meio]->prior == processo->prior)
        {
          return meio;
        }
        if (lista->lista_de_processos[meio]->prior > processo->prior)
        {
          sup = meio - 1;
        }
        else
        {
          inf = meio + 1;
        }
      }
      else // faz busca do horário
      {
        if ((lista->lista_de_processos[meio]->chegada.hh == processo->chegada.hh) && (lista->lista_de_processos[meio]->chegada.mm == processo->chegada.mm) && (lista->lista_de_processos[meio]->chegada.ss == processo->chegada.ss))
        {
          return meio;
        }
        if ((lista->lista_de_processos[meio]->chegada.hh > processo->chegada.hh) ||
            (lista->lista_de_processos[meio]->chegada.hh == processo->chegada.hh && lista->lista_de_processos[meio]->chegada.mm > processo->chegada.mm) ||
            (lista->lista_de_processos[meio]->chegada.hh == processo->chegada.hh && lista->lista_de_processos[meio]->chegada.mm == processo->chegada.mm && lista->lista_de_processos[meio]->chegada.ss > processo->chegada.ss))
        {
          sup = meio - 1;
        }
        else
        {
          inf = meio + 1;
        }
      }
    }
    return inf;
  }
}