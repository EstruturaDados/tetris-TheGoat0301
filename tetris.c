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

// Definindo a estrutura da pilha de reserva
typedef struct {
  Peca *pecas;    // Array dinâmico para armazenar as peças reservadas
  int topo;       // Índice do topo da pilha (-1 se vazia)
  int capacidade; // Capacidade máxima da pilha
} Pilha;

// Protótipos das funções
void inicializarFila(Fila *fila, int tamanho);
void liberarFila(Fila *fila);
void inicializarPilha(Pilha *pilha, int capacidade);
void liberarPilha(Pilha *pilha);
Peca gerarPeca(int id);
int enqueue(Fila *fila, Peca peca);
int dequeue(Fila *fila, Peca *peca);
int push(Pilha *pilha, Peca peca);
int pop(Pilha *pilha, Peca *peca);
int trocarPecaAtual(Fila *fila, Pilha *pilha);
int trocaMultipla(Fila *fila, Pilha *pilha);
void exibirEstado(Fila *fila, Pilha *pilha);
void mostrarMenu();

int main() {
  srand(time(NULL)); // Inicializa a semente para números aleatórios
  Fila fila;
  Pilha pilha;
  int tamanhoFila = 5;     // Tamanho fixo da fila
  int capacidadePilha = 3; // Capacidade máxima da pilha

  // Inicializa a fila e a pilha
  inicializarFila(&fila, tamanhoFila);
  inicializarPilha(&pilha, capacidadePilha);

  // Preenche a fila com 5 peças iniciais
  for (int i = 0; i < tamanhoFila; i++) {
    Peca novaPeca = gerarPeca(fila.proximoId++);
    enqueue(&fila, novaPeca);
  }

  int opcao;
  do {
    exibirEstado(&fila, &pilha); // Exibe o estado atual da fila e pilha
    mostrarMenu();               // Mostra o menu de opções
    printf("Digite a opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1: { // Jogar peça (dequeue)
      Peca pecaRemovida;
      if (dequeue(&fila, &pecaRemovida)) {
        printf("Peça jogada: [%c %d]\n", pecaRemovida.nome, pecaRemovida.id);
        // Adiciona nova peça para manter a fila cheia
        Peca novaPeca = gerarPeca(fila.proximoId++);
        enqueue(&fila, novaPeca);
      } else {
        printf("Fila vazia! Não é possível jogar uma peça.\n");
      }
      break;
    }
    case 2: { // Reservar peça (move da fila para a pilha)
      Peca pecaReservada;
      if (dequeue(&fila, &pecaReservada)) {
        if (push(&pilha, pecaReservada)) {
          printf("Peça reservada: [%c %d]\n", pecaReservada.nome,
                 pecaReservada.id);
          // Adiciona nova peça para manter a fila cheia
          Peca novaPeca = gerarPeca(fila.proximoId++);
          enqueue(&fila, novaPeca);
        } else {
          printf("Pilha cheia! Não é possível reservar a peça.\n");
          // Devolve a peça à fila se a pilha estiver cheia
          enqueue(&fila, pecaReservada);
        }
      } else {
        printf("Fila vazia! Não é possível reservar uma peça.\n");
      }
      break;
    }
    case 3: { // Usar peça reservada (pop da pilha)
      Peca pecaUsada;
      if (pop(&pilha, &pecaUsada)) {
        printf("Peça usada da reserva: [%c %d]\n", pecaUsada.nome,
               pecaUsada.id);
        // Adiciona nova peça para manter a fila cheia
        Peca novaPeca = gerarPeca(fila.proximoId++);
        enqueue(&fila, novaPeca);
      } else {
        printf("Pilha vazia! Não há peças reservadas para usar.\n");
      }
      break;
    }
    case 4: { // Trocar peça atual (frente da fila com topo da pilha)
      if (trocarPecaAtual(&fila, &pilha)) {
        printf(
            "Troca realizada: peça da frente da fila com o topo da pilha.\n");
      } else {
        printf("Não foi possível realizar a troca (fila ou pilha vazia).\n");
      }
      break;
    }
    case 5: { // Troca múltipla (3 primeiras peças da fila com as 3 da pilha)
      if (trocaMultipla(&fila, &pilha)) {
        printf("Troca múltipla realizada: 3 primeiras peças da fila com as 3 "
               "da pilha.\n");
      } else {
        printf("Não foi possível realizar a troca múltipla (menos de 3 peças "
               "na fila ou pilha).\n");
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

  // Libera a memória alocada
  liberarFila(&fila);
  liberarPilha(&pilha);
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

// Inicializa a pilha com a capacidade especificada
void inicializarPilha(Pilha *pilha, int capacidade) {
  pilha->pecas = (Peca *)malloc(capacidade * sizeof(Peca));
  pilha->capacidade = capacidade;
  pilha->topo = -1; // Pilha vazia
}

// Libera a memória alocada para a pilha
void liberarPilha(Pilha *pilha) {
  free(pilha->pecas);
  pilha->capacidade = 0;
  pilha->topo = -1;
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

// Insere uma peça no topo da pilha
int push(Pilha *pilha, Peca peca) {
  if (pilha->topo + 1 >= pilha->capacidade) {
    return 0; // Pilha cheia
  }
  pilha->topo++;
  pilha->pecas[pilha->topo] = peca;
  return 1; // Sucesso
}

// Remove uma peça do topo da pilha
int pop(Pilha *pilha, Peca *peca) {
  if (pilha->topo < 0) {
    return 0; // Pilha vazia
  }
  *peca = pilha->pecas[pilha->topo];
  pilha->topo--;
  return 1; // Sucesso
}

// Troca a peça da frente da fila com a peça do topo da pilha
int trocarPecaAtual(Fila *fila, Pilha *pilha) {
  if (fila->contador == 0 || pilha->topo < 0) {
    return 0; // Fila ou pilha vazia
  }
  // Troca direta nos arrays para preservar a ordem da fila
  Peca temp = fila->pecas[fila->frente];
  fila->pecas[fila->frente] = pilha->pecas[pilha->topo];
  pilha->pecas[pilha->topo] = temp;
  return 1; // Sucesso
}

// Troca as 3 primeiras peças da fila com as 3 peças da pilha
int trocaMultipla(Fila *fila, Pilha *pilha) {
  if (fila->contador < 3 || pilha->topo + 1 < 3) {
    return 0; // Menos de 3 peças na fila ou pilha
  }
  // Calcula os índices das 3 primeiras peças da fila (considerando
  // circularidade)
  int idx1 = fila->frente;
  int idx2 = (idx1 + 1) % fila->tamanho;
  int idx3 = (idx2 + 1) % fila->tamanho;

  // Salva as peças da fila
  Peca f1 = fila->pecas[idx1];
  Peca f2 = fila->pecas[idx2];
  Peca f3 = fila->pecas[idx3];

  // Salva as peças da pilha (bottom [0], mid [1], top [2])
  Peca p_bottom = pilha->pecas[0];
  Peca p_mid = pilha->pecas[1];
  Peca p_top = pilha->pecas[2];

  // Atribui à fila: frente = top da pilha, +1 = mid, +2 = bottom
  fila->pecas[idx1] = p_top;
  fila->pecas[idx2] = p_mid;
  fila->pecas[idx3] = p_bottom;

  // Atribui à pilha: bottom = frente da fila, mid = +1, top = +2
  pilha->pecas[0] = f1;
  pilha->pecas[1] = f2;
  pilha->pecas[2] = f3;

  return 1; // Sucesso
}

// Exibe o estado atual da fila e da pilha
void exibirEstado(Fila *fila, Pilha *pilha) {
  printf("\nEstado atual:\n");
  printf("Fila de peças\t");
  if (fila->contador == 0) {
    printf("(vazia)\n");
  } else {
    int indice = fila->frente;
    for (int i = 0; i < fila->contador; i++) {
      printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
      indice = (indice + 1) % fila->tamanho;
    }
    printf("\n");
  }

  printf("Pilha de reserva\t(Topo -> Base): ");
  if (pilha->topo < 0) {
    printf("(vazia)\n");
  } else {
    for (int i = pilha->topo; i >= 0; i--) {
      printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
    }
    printf("\n");
  }
  printf(
      "Tabela: Visualização atual da fila de peças e da pilha de reserva.\n");
  printf("Curadoria de TI.\n");
}

// Mostra o menu de opções
void mostrarMenu() {
  printf("\nOpções disponíveis:\n");
  printf("Código\tAção\n");
  printf("1\tJogar peça da frente da fila\n");
  printf("2\tEnviar peça da fila para a pilha de reserva\n");
  printf("3\tUsar peça da pilha de reserva\n");
  printf("4\tTrocar peça da frente da fila com o topo da pilha\n");
  printf("5\tTrocar os 3 primeiros da fila com as 3 peças da pilha\n");
  printf("0\tSair\n");
  printf("Tabela: Comandos para movimentar peças entre a fila e a pilha de "
         "reserva.\n");
  printf("Curadoria de TI.\n");
}