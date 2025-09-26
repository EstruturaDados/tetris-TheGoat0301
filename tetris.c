#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definindo a estrutura da peça
typedef struct {
  char nome; // Tipo da peça ('I', 'O', 'T', 'L')
  int id;    // Identificador único
} Peca;

// Definindo a estrutura da fila circular
typedef struct {
  Peca *pecas;   // Array dinâmico para armazenar as peças
  int frente;    // Índice da frente da fila
  int traseira;  // Índice da traseira da fila
  int tamanho;   // Tamanho máximo da fila
  int contador;  // Número atual de peças na fila
  int proximoId; // Próximo ID a ser atribuído
} Fila;

// Protótipos das funções
void inicializarFila(Fila *fila, int tamanho);
void liberarFila(Fila *fila);
Peca gerarPeca(int id);
int enqueue(Fila *fila, Peca peca);
int dequeue(Fila *fila, Peca *peca);
void exibirFila(Fila *fila);
void mostrarMenu();

int main() {
  srand(time(NULL)); // Inicializa a semente para números aleatórios
  Fila fila;
  int tamanhoFila = 5; // Tamanho fixo da fila
  inicializarFila(&fila, tamanhoFila);

  // Preenche a fila com 5 peças iniciais
  for (int i = 0; i < tamanhoFila; i++) {
    Peca novaPeca = gerarPeca(fila.proximoId++);
    enqueue(&fila, novaPeca);
  }

  int opcao;
  do {
    exibirFila(&fila);
    mostrarMenu();
    printf("Digite a opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1: { // Jogar peça (dequeue)
      Peca pecaRemovida;
      if (dequeue(&fila, &pecaRemovida)) {
        printf("Peça jogada: [%c %d]\n", pecaRemovida.nome, pecaRemovida.id);
      } else {
        printf("Fila vazia! Não é possível jogar uma peça.\n");
      }
      break;
    }
    case 2: { // Inserir nova peça (enqueue)
      Peca novaPeca = gerarPeca(fila.proximoId++);
      if (enqueue(&fila, novaPeca)) {
        printf("Peça inserida: [%c %d]\n", novaPeca.nome, novaPeca.id);
      } else {
        printf("Fila cheia! Não é possível inserir uma nova peça.\n");
      }
      break;
    }
    case 0: // Sair
      printf("Saindo do programa.\n");
      break;
    default:
      printf("Opção inválida! Tente novamente.\n");
    }
  } while (opcao != 0);

  liberarFila(&fila);
  return 0;
}

// Inicializa a fila com o tamanho especificado
void inicializarFila(Fila *fila, int tamanho) {
  fila->pecas = (Peca *)malloc(tamanho * sizeof(Peca));
  fila->tamanho = tamanho;
  fila->frente = 0;
  fila->traseira = -1;
  fila->contador = 0;
  fila->proximoId = 0;
}

// Libera a memória alocada para a fila
void liberarFila(Fila *fila) {
  free(fila->pecas);
  fila->tamanho = 0;
  fila->frente = 0;
  fila->traseira = -1;
  fila->contador = 0;
}

// Gera uma nova peça com nome aleatório e ID fornecido
Peca gerarPeca(int id) {
  Peca peca;
  char tipos[] = {'I', 'O', 'T', 'L'};
  peca.nome = tipos[rand() % 4]; // Escolhe um tipo aleatoriamente
  peca.id = id;
  return peca;
}

// Insere uma peça no final da fila
int enqueue(Fila *fila, Peca peca) {
  if (fila->contador >= fila->tamanho) {
    return 0; // Fila cheia
  }
  fila->traseira = (fila->traseira + 1) % fila->tamanho;
  fila->pecas[fila->traseira] = peca;
  fila->contador++;
  return 1; // Sucesso
}

// Remove uma peça da frente da fila
int dequeue(Fila *fila, Peca *peca) {
  if (fila->contador == 0) {
    return 0; // Fila vazia
  }
  *peca = fila->pecas[fila->frente];
  fila->frente = (fila->frente + 1) % fila->tamanho;
  fila->contador--;
  return 1; // Sucesso
}

// Exibe o estado atual da fila
void exibirFila(Fila *fila) {
  printf("\nFila de peças\n");
  if (fila->contador == 0) {
    printf("Fila vazia.\n");
    return;
  }
  int indice = fila->frente;
  for (int i = 0; i < fila->contador; i++) {
    printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
    indice = (indice + 1) % fila->tamanho;
  }
  printf("\n");
}

// Mostra o menu de opções
void mostrarMenu() {
  printf("\nOpções de ação:\n");
  printf("Código\tAção\n");
  printf("1\tJogar peça (dequeue)\n");
  printf("2\tInserir nova peça (enqueue)\n");
  printf("0\tSair\n");
}