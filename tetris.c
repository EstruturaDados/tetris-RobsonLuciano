#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ============================================================
   DEFINIÇÃO DA ESTRUTURA PEÇA
   ============================================================ */

typedef struct {
    char nome;   // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;      // Identificador único da peça
} Peca;

/* ============================================================
   DEFINIÇÃO DA FILA CIRCULAR
   ============================================================ */

#define MAX 5   // Tamanho fixo da fila

typedef struct {
    Peca itens[MAX]; // Array de peças
    int inicio;      // Índice do início da fila
    int fim;         // Índice do final da fila
    int total;       // Quantidade atual de elementos
} Fila;

/* ============================================================
   CONTROLE DE ID GLOBAL
   ============================================================ */

int proximoId = 0;  // Controla a geração automática dos IDs

/* ============================================================
   FUNÇÃO PARA GERAR PEÇAS AUTOMATICAMENTE
   ============================================================ */

Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;

    nova.nome = tipos[rand() % 4];  // Sorteia tipo
    nova.id = proximoId++;          // Gera ID único

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

int filaCheia(Fila *f) {
    return f->total == MAX;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

/* Enqueue */
void inserir(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("⚠ Fila cheia! Não é possível inserir nova peça.\n");
        return;
    }

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX;
    f->total++;
}

/* Dequeue */
void jogarPeca(Fila *f) {
    if (filaVazia(f)) {
        printf("⚠ Fila vazia! Não há peça para jogar.\n");
        return;
    }

    Peca removida = f->itens[f->inicio];
    printf("Peça jogada: [%c %d]\n", removida.nome, removida.id);

    f->inicio = (f->inicio + 1) % MAX;
    f->total--;
}

/* Exibir fila */
void mostrarFila(Fila *f) {
    printf("\n=============================\n");
    printf("Fila de peças:\n");

    if (filaVazia(f)) {
        printf("(Fila vazia)\n");
    } else {
        for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
            printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        }
        printf("\n");
    }

    printf("=============================\n");
}

/* ============================================================
   PROGRAMA PRINCIPAL
   ============================================================ */

int main() {

    srand(time(NULL));  // Inicializa gerador aleatório

    Fila fila;
    inicializarFila(&fila);

    /* Inicializa a fila com 5 peças */
    for (int i = 0; i < MAX; i++) {
        inserir(&fila, gerarPeca());
    }

    int opcao;

    do {
        mostrarFila(&fila);

        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;

            case 2:
                inserir(&fila, gerarPeca());
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
