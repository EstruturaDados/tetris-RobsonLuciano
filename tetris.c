#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ============================================================
   DEFINIÇÃO DA ESTRUTURA PEÇA
   ============================================================ */

typedef struct {
    char nome;   // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;      // Identificador único
} Peca;

/* ============================================================
   FILA CIRCULAR (PEÇAS FUTURAS)
   ============================================================ */

#define MAX_FILA 5

typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int total;
} Fila;

/* ============================================================
   PILHA (RESERVA)
   ============================================================ */

#define MAX_PILHA 3

typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

/* ============================================================
   CONTROLE GLOBAL DE IDs
   ============================================================ */

int proximoId = 0;

/* ============================================================
   GERAÇÃO AUTOMÁTICA DE PEÇAS
   ============================================================ */

Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;

    nova.nome = tipos[rand() % 4];
    nova.id = proximoId++;

    return nova;
}

/* ============================================================
   FUNÇÕES DA FILA
   ============================================================ */

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

int filaCheia(Fila *f) {
    return f->total == MAX_FILA;
}

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}

int dequeue(Fila *f, Peca *removida) {
    if (filaVazia(f)) return 0;

    *removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return 1;
}

/* ============================================================
   FUNÇÕES DA PILHA
   ============================================================ */

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

int push(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return 0;

    p->topo++;
    p->itens[p->topo] = nova;
    return 1;
}

int pop(Pilha *p, Peca *removida) {
    if (pilhaVazia(p)) return 0;

    *removida = p->itens[p->topo];
    p->topo--;
    return 1;
}

/* ============================================================
   EXIBIÇÃO DO ESTADO DO JOGO
   ============================================================ */

void mostrarEstado(Fila *fila, Pilha *pilha) {

    printf("\n=====================================\n");
    printf("Fila de peças: ");

    for (int i = 0, idx = fila->inicio; i < fila->total; i++, idx = (idx + 1) % MAX_FILA) {
        printf("[%c %d] ", fila->itens[idx].nome, fila->itens[idx].id);
    }

    printf("\nPilha de reserva (Topo -> Base): ");

    if (pilhaVazia(pilha)) {
        printf("(vazia)");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->itens[i].nome, pilha->itens[i].id);
        }
    }

    printf("\n=====================================\n");
}

/* ============================================================
   PROGRAMA PRINCIPAL
   ============================================================ */

int main() {

    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    /* Inicializa fila sempre cheia */
    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;
    Peca temp;

    do {

        mostrarEstado(&fila, &pilha);

        printf("\nOpções de Ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1: // Jogar peça
                if (dequeue(&fila, &temp)) {
                    printf("Peça jogada: [%c %d]\n", temp.nome, temp.id);
                    enqueue(&fila, gerarPeca()); // mantém fila cheia
                } else {
                    printf("Fila vazia!\n");
                }
                break;

            case 2: // Reservar peça
                if (pilhaCheia(&pilha)) {
                    printf("Pilha cheia! Não é possível reservar.\n");
                } else if (dequeue(&fila, &temp)) {
                    push(&pilha, temp);
                    printf("Peça reservada: [%c %d]\n", temp.nome, temp.id);
                    enqueue(&fila, gerarPeca()); // mantém fila cheia
                }
                break;

            case 3: // Usar peça reservada
                if (pop(&pilha, &temp)) {
                    printf("Peça usada da reserva: [%c %d]\n", temp.nome, temp.id);
                } else {
                    printf("Pilha vazia!\n");
                }
                break;

            case 0:
                printf("Encerrando Tetris Stack...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
