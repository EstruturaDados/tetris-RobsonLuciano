#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ============================================================
   DEFINIÇÃO DA ESTRUTURA PEÇA
   ============================================================ */

typedef struct {
    char nome;   // Tipo da peça ('I', 'O', 'T', 'L')
    int id;      // Identificador único
} Peca;

/* ============================================================
   FILA CIRCULAR
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
   CONTROLE GLOBAL
   ============================================================ */

int proximoId = 0;

/* ============================================================
   GERAÇÃO AUTOMÁTICA
   ============================================================ */

Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = proximoId++;
    return nova;
}

/* ============================================================
   FUNÇÕES FILA
   ============================================================ */

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaVazia(Fila *f) { return f->total == 0; }
int filaCheia(Fila *f) { return f->total == MAX_FILA; }

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

/* Acessa elemento por posição lógica (0 = frente) */
Peca* acessarFila(Fila *f, int posicao) {
    int indice = (f->inicio + posicao) % MAX_FILA;
    return &f->itens[indice];
}

/* ============================================================
   FUNÇÕES PILHA
   ============================================================ */

void inicializarPilha(Pilha *p) { p->topo = -1; }

int pilhaVazia(Pilha *p) { return p->topo == -1; }
int pilhaCheia(Pilha *p) { return p->topo == MAX_PILHA - 1; }

int push(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return 0;
    p->itens[++p->topo] = nova;
    return 1;
}

int pop(Pilha *p, Peca *removida) {
    if (pilhaVazia(p)) return 0;
    *removida = p->itens[p->topo--];
    return 1;
}

/* ============================================================
   EXIBIÇÃO
   ============================================================ */

void mostrarEstado(Fila *fila, Pilha *pilha) {

    printf("\n=========================================\n");
    printf("Fila de peças: ");

    for (int i = 0; i < fila->total; i++) {
        Peca *p = acessarFila(fila, i);
        printf("[%c %d] ", p->nome, p->id);
    }

    printf("\nPilha de reserva (Topo -> Base): ");

    if (pilhaVazia(pilha)) {
        printf("(vazia)");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->itens[i].nome, pilha->itens[i].id);
        }
    }

    printf("\n=========================================\n");
}

/* ============================================================
   OPERAÇÕES AVANÇADAS
   ============================================================ */

/* Troca frente da fila com topo da pilha */
void trocarSimples(Fila *fila, Pilha *pilha) {

    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("Troca impossível: fila ou pilha vazia.\n");
        return;
    }

    Peca *frente = acessarFila(fila, 0);
    Peca *topo = &pilha->itens[pilha->topo];

    Peca temp = *frente;
    *frente = *topo;
    *topo = temp;

    printf("Troca simples realizada.\n");
}

/* Troca os 3 primeiros da fila com os 3 da pilha */
void trocaMultipla(Fila *fila, Pilha *pilha) {

    if (fila->total < 3 || pilha->topo < 2) {
        printf("Troca múltipla impossível: quantidade insuficiente.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {

        Peca *filaPeca = acessarFila(fila, i);
        Peca *pilhaPeca = &pilha->itens[pilha->topo - i];

        Peca temp = *filaPeca;
        *filaPeca = *pilhaPeca;
        *pilhaPeca = temp;
    }

    printf("Troca múltipla realizada entre 3 peças.\n");
}

/* ============================================================
   MAIN
   ============================================================ */

int main() {

    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    /* Inicializa fila cheia */
    for (int i = 0; i < MAX_FILA; i++)
        enqueue(&fila, gerarPeca());

    int opcao;
    Peca temp;

    do {

        mostrarEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente\n");
        printf("2 - Enviar peça para pilha\n");
        printf("3 - Usar peça da pilha\n");
        printf("4 - Trocar frente com topo\n");
        printf("5 - Troca múltipla (3 peças)\n");
        printf("0 - Sair\n");
        printf("Opção escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1:
                if (dequeue(&fila, &temp)) {
                    printf("Peça jogada: [%c %d]\n", temp.nome, temp.id);
                    enqueue(&fila, gerarPeca());
                }
                break;

            case 2:
                if (!pilhaCheia(&pilha) && dequeue(&fila, &temp)) {
                    push(&pilha, temp);
                    printf("Peça enviada para pilha.\n");
                    enqueue(&fila, gerarPeca());
                } else {
                    printf("Não foi possível enviar para pilha.\n");
                }
                break;

            case 3:
                if (pop(&pilha, &temp))
                    printf("Peça usada da pilha: [%c %d]\n", temp.nome, temp.id);
                else
                    printf("Pilha vazia.\n");
                break;

            case 4:
                trocarSimples(&fila, &pilha);
                break;

            case 5:
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("Encerrando sistema...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}
