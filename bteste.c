#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <unistd.h>

const int INT_MIN = -2147483647;

/*************************************
 * Fila para navegacao por niveis
 *************************************/
TAILQ_HEAD(tailhead, entry) head;

struct entry {
  struct noBst *raiz;
  TAILQ_ENTRY(entry) entries;
};

void add_to_queue(struct noBst *raiz) {
  struct entry *elem = (struct entry *)malloc(sizeof(struct entry));
  if (elem)
    elem->raiz = raiz;
  TAILQ_INSERT_TAIL(&head, elem, entries);
}
/*************************************/

struct noBst {
  struct noBst *esq;
  int val;
  struct noBst *dir;
};

void imprimir(struct noBst *raiz) {
  if (raiz != NULL) {
    imprimir(raiz->esq);
    printf("%d, ", raiz->val);
    imprimir(raiz->dir);
  }
}

/**
 * Funcao que aloca um novo noBst.
 **/
struct noBst *alocarNo(int val) {
  struct noBst *no = (struct noBst *)malloc(sizeof(struct noBst));
  no->esq = NULL;
  no->val = val;
  no->dir = NULL;
  return no;
}

/**
 * Funcao recursiva para inserir novo no.
 **/
void inserir(struct noBst **raiz, int val) {
  if ((*raiz) == NULL)
    (*raiz) = alocarNo(val);
  else {
    if (val > (*raiz)->val)
      inserir(&(*raiz)->dir, val);
    else
      inserir(&(*raiz)->esq, val);
  }
}

/**
 * Funcao recursiva para buscar valor.
 **/
bool buscar(struct noBst *raiz, int val) {
  if (raiz == NULL) {
    return false;
  } else {
    if (raiz->val == val)
      return true;
    else if (raiz->val > val)
      return buscar(raiz->esq, val);
    else
      return buscar(raiz->dir, val);
  }
}

int minIt(struct noBst *raiz) { // raiz eh local
  if (raiz == NULL)
    return INT_MIN;
  while (raiz->esq != NULL)
    raiz = raiz->esq;
  return raiz->val;
}

int minRec(struct noBst *raiz) { // raiz eh local
  if (raiz == NULL)
    return INT_MIN;
  else if (raiz->esq != NULL)
    return minRec(raiz->esq);
  return raiz->val;
}

int maxBST(struct noBst *raiz) { // raiz eh local
  if (raiz == NULL)
    return INT_MIN;
  else if (raiz->dir != NULL)
    return maxBST(raiz->dir);
  return raiz->val;
}

int max(int a, int b) {
  if (a > b)
    return a;
  else
    return b;
}

int altura(struct noBst *raiz) {
  if (raiz == NULL)
    return -1;
  else {
    return 1 + max(altura(raiz->esq), altura(raiz->dir));
  }
}

void navNivel(struct noBst *raiz) {
  TAILQ_INIT(&head);
  // add a raiz na fila
  add_to_queue(raiz);

  struct entry *elem = head.tqh_first;
  // enquanto a fila não estiver vazia
  while (elem != NULL) {
    struct noBst *noVisitado = elem->raiz;
    // imprimir/processar o elemento na frente da fila
    printf("%d, ", noVisitado->val);
    // adicionar as sub-árvores à fila
    if (noVisitado->esq != NULL)
      add_to_queue(noVisitado->esq);
    if (noVisitado->dir != NULL)
      add_to_queue(noVisitado->dir);

    // remover o nó que está na frente da fila
    TAILQ_REMOVE(&head, elem, entries);
    free(elem);
    elem = head.tqh_first;
  }
}

void navPreOrdem(struct noBst *raiz) {
  if (raiz != NULL) {
    printf("%d, ", raiz->val);
    navPreOrdem(raiz->esq);
    navPreOrdem(raiz->dir);
  }
}

void navEmOrdem(struct noBst *raiz) {
  if (raiz != NULL) {
    navEmOrdem(raiz->esq);
    printf("%d, ", raiz->val);
    navEmOrdem(raiz->dir);
  }
}

void navPosOrdem(struct noBst *raiz) {
  if (raiz != NULL) {
    navPosOrdem(raiz->esq);
    navPosOrdem(raiz->dir);
    printf("%d, ", raiz->val);
  }
}

struct noBst *remover(struct noBst *raiz, int val) {
  if (raiz == NULL)
    return raiz;
  else if (val < raiz->val)
    raiz->esq = remover(raiz->esq, val);
  else if (val > raiz->val)
    raiz->dir = remover(raiz->dir, val);
  else { // val == raiz->val
    if (raiz->esq == NULL && raiz->dir == NULL) {
      free(raiz);
      raiz = NULL;
    } else if (raiz->dir == NULL) {
      struct noBst *noASerRemovido = raiz;
      raiz = raiz->esq;
      free(noASerRemovido);
    } else if (raiz->esq == NULL) {
      struct noBst *noASerRemovido = raiz;
      raiz = raiz->dir;
      free(noASerRemovido);
    } else { // possui dois filhos
      int maxEsq = maxBST(raiz->esq);
      raiz->val = maxEsq;
      raiz->esq = remover(raiz->esq, maxEsq);
    }
  }
  return raiz;
}

int main() {

  // Árvore a ser criada
  //        12
  //       /  \
  //      8    14
  //     / \     \
  //    4   10    18
  //   / \       /  \
  //  2   6     16   20

  // Pré-ordem: 12, 8, 4, 2, 6, 10, 14, 18, 16, 20
  // Pós-ordem: 2, 6, 4, 10, 8, 16, 20, 18, 14, 12

  struct noBst *raiz = NULL;
  inserir(&raiz, 12);
  inserir(&raiz, 8);
  inserir(&raiz, 14);
  inserir(&raiz, 4);
  inserir(&raiz, 10);
  inserir(&raiz, 18);
  inserir(&raiz, 2);
  inserir(&raiz, 6);
  inserir(&raiz, 16);
  inserir(&raiz, 20);

  // imprimir(raiz);

  // BUSCA
  // printf("\n%d\n", buscar(raiz, 40));
  // printf("%d\n", buscar(raiz, 20));
  // printf("%d\n", buscar(raiz, 2));

  // MIN-MAX
  // printf("\n%d\n", minIt(raiz));
  // printf("%d\n", minRec(raiz));
  // printf("\n%d\n", maxIt(raiz));
  // printf("%d\n", maxRec(raiz));

  // ALTURA
  // printf("\nAltura: %d\n", altura(raiz));

  // NAVEGAÇÃO
  // navNivel(raiz);
  // navPreOrdem(raiz);
  // navEmOrdem(raiz);
  // navPosOrdem(raiz);

  // REMOÇÃO
  imprimir(raiz);
  printf("\n");
  raiz = remover(raiz, 6);
  imprimir(raiz);
  printf("\n");
  raiz = remover(raiz, 4);
  imprimir(raiz);
  printf("\n");
  raiz = remover(raiz, 12);
  imprimir(raiz);
  printf("\n");
  raiz = remover(raiz, 18);
  imprimir(raiz);
  printf("\n");
  
}
