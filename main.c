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

void merge(LISTA *, int, int, int, int, int);
void MergeSort(LISTA *, int, int, int, int);
int horario_para_segundo(horario);
int busca_binaria(LISTA *, celula *, int);

int main()
{
  LISTA *lista = lista_criar();
  if (lista == NULL)
  {
    fprintf(stderr, "Erro ao criar lista.\n");
    return 1;
  }

  char comando[7];  // Armazena o comando principal
  char comando2[3]; // Armazena o argumento do comando (ex: -p, -t)

  while (1)
  {
    scanf(" %6s", comando); // Limita a leitura para evitar overflow
    if (strcmp(comando, "quit") == 0)
      break; // Verifica se o comando é "quit"

    /* Adicionar novo processo à lista */
    if (strcmp(comando, "add") == 0)
    {
      celula *processo = (celula *)malloc(sizeof(celula)); // Aloca espaço para novo processo
      if (processo == NULL)
      {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
      }
      scanf(" %d %d %d %d %49s", &processo->prior, &processo->chegada.hh, &processo->chegada.mm, &processo->chegada.ss, processo->descricao);
      lista_inserir_processo(processo, lista);
    }

    if (strcmp(comando, "exec") == 0)
    { // Executa tarefa, ou seja, elimina ela da lista de processos
      scanf(" %s", comando2);
      if (strcmp(comando2, "-p") == 0)                         // Executa o de maior prioridade
        MergeSort(lista, lista->inicio, lista->fim - 1, 0, 0); // Ordena por prioridade
      else                                                     // -t ---> executa o de menor horário
        MergeSort(lista, lista->inicio, lista->fim - 1, 1, 1); // Ordena por horário

      lista_executar_processo(lista); // Elimina o último elemento da lista
    }

    if (strcmp(comando, "next") == 0)
    { // Exibe o processo
      if (lista->tam > 0)
      {
        scanf(" %s", comando2);
        if (strcmp(comando2, "-p") == 0)                         // Exibe o processo de maior prioridade
          MergeSort(lista, lista->inicio, lista->fim - 1, 0, 0); // Ordena por prioridade
        else                                                     // Exibe o processo de menor horário
          MergeSort(lista, lista->inicio, lista->fim - 1, 1, 1); // Ordena por horário

        printf("%02d %02d:%02d:%02d %s\n", lista->lista_de_processos[lista->fim - 1]->prior, lista->lista_de_processos[lista->fim - 1]->chegada.hh, lista->lista_de_processos[lista->fim - 1]->chegada.mm, lista->lista_de_processos[lista->fim - 1]->chegada.ss, lista->lista_de_processos[lista->fim - 1]->descricao);
      }
      printf("\n");
    }

    if (strcmp(comando, "change") == 0)
    { // Muda os dados do processo
      if (lista->tam > 0)
      {
        scanf(" %s", comando2); // Comando seguinte -p ou -t
        int pos, novo_valor;
        celula proc;

        if (strcmp(comando2, "-p") == 0)
        { // Muda a prioridade
          scanf("%d", &proc.prior);
          scanf("%d", &novo_valor);
          MergeSort(lista, lista->inicio, lista->fim - 1, 0, 0); // Ordena por prioridade
          pos = busca_binaria(lista, &proc, 1);                  // Busca pelo índice
          if (pos != -1)
          {
            lista->lista_de_processos[pos]->prior = novo_valor; // Atualiza a prioridade
          }
        }
        else
        { // Muda o horário
          horario novo_horario;
          scanf("%d %d %d", &proc.chegada.hh, &proc.chegada.mm, &proc.chegada.ss);
          scanf("%d %d %d", &novo_horario.hh, &novo_horario.mm, &novo_horario.ss);

          MergeSort(lista, lista->inicio, lista->fim - 1, 1, 0); // Ordena por horário
          pos = busca_binaria(lista, &proc, 0);                  // Busca pelo índice
          if (pos != -1)
          {
            lista->lista_de_processos[pos]->chegada = novo_horario; // Atualiza o horário
          }
        }
      }
    }

    if (strcmp(comando, "print") == 0)
    {
      scanf(" %s", comando2);
      if (strcmp(comando2, "-p") == 0)                         // Imprime processos em ordem de prioridade
        MergeSort(lista, lista->inicio, lista->fim - 1, 0, 1); // Ordena por prioridade
      else                                                     // Imprime processos em ordem de horário
        MergeSort(lista, lista->inicio, lista->fim - 1, 1, 0); // Ordena por horário

      lista_imprimir(lista);
      printf("\n");
    }
  }

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
    free(lista->lista_de_processos[lista->fim - 1]);
    lista->lista_de_processos[lista->fim - 1] = NULL;
    lista->tam--;
    lista->fim--;
  }
}
void lista_destruir(LISTA *lista)
{
  if (lista != NULL)
  {
    for (int i = 0; i < lista->fim; i++)
    {
      if (lista->lista_de_processos[i] != NULL) // Verifica se o ponteiro não é NULL antes de liberar
      {
        free(lista->lista_de_processos[i]);
        lista->lista_de_processos[i] = NULL;
      }
    }

    free(lista);
  }
}
/*Funções de ordenação   */

void merge(LISTA *lista, int inicio, int meio, int fim, int tipo, int cres_decres)
{
  int tam_esq = meio - inicio + 1;
  int tam_dir = fim - meio;

  celula *esquerda[tam_esq];
  celula *direita[tam_dir];

  for (int i = 0; i < tam_esq; i++)
  {
    esquerda[i] = lista->lista_de_processos[inicio + i];
  }
  for (int j = 0; j < tam_dir; j++)
  {
    direita[j] = lista->lista_de_processos[meio + 1 + j];
  }

  int i = 0, j = 0, k = inicio;

  while (i < tam_esq && j < tam_dir)
  {
    int comparar;
    if (tipo == 0)
    { // Ordenar por prioridade
      comparar = (cres_decres == 0) ? (esquerda[i]->prior <= direita[j]->prior) : (esquerda[i]->prior > direita[j]->prior);
    }
    else
    { // Ordenar por horário
      int hora_esquerda = horario_para_segundo(esquerda[i]->chegada);
      int hora_direita = horario_para_segundo(direita[j]->chegada);
      comparar = (cres_decres == 0) ? (hora_esquerda <= hora_direita) : (hora_esquerda > hora_direita);
    }

    if (comparar)
    {
      lista->lista_de_processos[k] = esquerda[i];
      i++;
    }
    else
    {
      lista->lista_de_processos[k] = direita[j];
      j++;
    }
    k++;
  }

  while (i < tam_esq)
  {
    lista->lista_de_processos[k] = esquerda[i];
    i++;
    k++;
  }

  while (j < tam_dir)
  {
    lista->lista_de_processos[k] = direita[j];
    j++;
    k++;
  }
}
void MergeSort(LISTA *lista, int inicio, int fim, int tipo, int cres_decres)
{
  if (inicio < fim)
  {
    int meio = inicio + (fim - inicio) / 2;

    MergeSort(lista, inicio, meio, tipo, cres_decres);
    MergeSort(lista, meio + 1, fim, tipo, cres_decres);

    merge(lista, inicio, meio, fim, tipo, cres_decres);
  }
}

/*Funções de ordenação -> horário  */

int horario_para_segundo(horario h)
{
  return (h.hh * 3600 + h.mm * 60 + h.ss);
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