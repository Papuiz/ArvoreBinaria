#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do nó da árvore binária
typedef struct Node {
    int chave;
    struct Node* pai;
    struct Node* direita;
    struct Node* esquerda;
} Nodo;

// Função que cria um novo nó e retorna um ponteiro para ele
Nodo* criarNodo(int chave) {
    Nodo* nodo = (Nodo*)malloc(sizeof(Nodo)); // Aloca dinamicamente um novo nó
    if (nodo) {
        nodo->chave = chave; // Define a chave do novo nó
        nodo->esquerda = NULL; // Inicializa as posições esquerda, direita e pai como nulo
        nodo->direita = NULL;
        nodo->pai = NULL;
    }
    return nodo;
}

// Função que insere um nó na árvore
Nodo* inserir(Nodo* raiz, int chave) {
    if (!raiz) {
        return criarNodo(chave); // Se a raiz for nula, cria um novo nó e o torna a raiz
    }
    Nodo* atual = raiz; // Cria um nó auxiliar que será a referência atual
    Nodo* pai = NULL; // Cria um nó para o pai
    while (atual) { // Enquanto a referência atual não for nula, percorre a árvore
        pai = atual; // Atualiza o nó pai com o valor atual
        if (chave <= atual->chave) { // Modificando para permitir chaves duplicadas
            atual = atual->esquerda; // Move a referência atual para a esquerda
        } else {
            atual = atual->direita; // Move a referência atual para a direita
        }
    }
    Nodo* novo = criarNodo(chave); // Cria um novo nó com a chave fornecida
    if (!novo) { // Verifica se a alocação foi bem-sucedida
        return raiz;
    }
    novo->pai = pai; // Define o pai do novo nó como o pai encontrado durante a busca
    if (chave <= pai->chave) { // Modificando para permitir chaves duplicadas
        pai->esquerda = novo; // Sempre inserir à esquerda ou à direita, dependendo do seu critério
    } else {
        pai->direita = novo;
    }
    return raiz; // Retorna a raiz da árvore
}

// Função para encontrar o nó com a menor chave na árvore
Nodo* encontrarMenor(Nodo* nodo) {
    while (nodo->esquerda) { // Percorre a árvore pela esquerda até encontrar o menor valor
        nodo = nodo->esquerda;
    }
    return nodo; // Retorna o nó com a menor chave
}

// Função para remover um nó da árvore
Nodo* remover(Nodo* raiz, int chave) {
    if (!raiz) {
        return raiz; // Se a raiz for nula, retorna a árvore inalterada
    }
    
    Nodo* atual = raiz; // Define um nó atual para percorrer a árvore
    Nodo* pai = NULL; // Cria um nó para o pai
    
    while (atual) {
        if (chave < atual->chave) {
            pai = atual;
            atual = atual->esquerda; // Move para a esquerda se a chave for menor
        } else if (chave > atual->chave) {
            pai = atual;
            atual = atual->direita; // Move para a direita se a chave for maior
        } else {
            if (atual->esquerda == NULL) { // Caso o nó a ser removido não tenha filho à esquerda
                Nodo* NodoAuxiliar = atual->direita; // Armazena a subárvore direita em um nó auxiliar
                free(atual); // Libera o nó a ser removido
                
                if (!pai) {
                    return NodoAuxiliar; // Se não há pai, o nó removido era a raiz
                } else if (pai->esquerda == atual) {
                    pai->esquerda = NodoAuxiliar; // Atualiza o filho esquerdo do pai
                } else {
                    pai->direita = NodoAuxiliar; // Atualiza o filho direito do pai
                }
            } else if (atual->direita == NULL) { // Caso o nó a ser removido não tenha filho à direita
                Nodo* NodoAuxiliar = atual->esquerda; // Armazena a subárvore esquerda em um nó auxiliar
                free(atual); // Libera o nó a ser removido
                
                if (!pai) {
                    return NodoAuxiliar; // Se não há pai, o nó removido era a raiz
                } else if (pai->esquerda == atual) {
                    pai->esquerda = NodoAuxiliar; // Atualiza o filho esquerdo do pai
                } else {
                    pai->direita = NodoAuxiliar; // Atualiza o filho direito do pai
                }
            } else {
                Nodo* NodoAuxiliar = encontrarMenor(atual->direita); // Encontra o nó com a menor chave na subárvore direita
                atual->chave = NodoAuxiliar->chave; // Copia a chave do nó auxiliar para o nó atual
                atual->direita = remover(atual->direita, NodoAuxiliar->chave); // Remove o nó auxiliar da subárvore direita
            }
            break; // Sai do loop após a remoção
        }
    }
    
    return raiz; // Retorna a raiz da árvore
}

// Função para imprimir a árvore em ordem
void imprimirEmOrdem(Nodo* raiz, FILE* arquivo) {
    if (raiz) {
        imprimirEmOrdem(raiz->esquerda, arquivo); // Imprime os nós à esquerda
        fprintf(arquivo, "%d ", raiz->chave); // Imprime a chave do nó atual no arquivo
        imprimirEmOrdem(raiz->direita, arquivo); // Imprime os nós à direita
    }
}

int main() {
    Nodo* raiz = NULL; // Inicializa a raiz como nula
    FILE* arquivo_entrada;
    FILE* arquivo_saida;
    char operacao;
    int chave;
    arquivo_entrada = fopen("in.txt", "r"); // Abre o arquivo de entrada para leitura
    arquivo_saida = fopen("out.txt", "w"); // Abre o arquivo de saída para escrita
    while (fscanf(arquivo_entrada, " %c %d", &operacao, &chave) != EOF) { // Lê as operações e chaves do arquivo de entrada
        if (operacao == 'i') {
            raiz = inserir(raiz, chave); // Insere um nó com a chave lida
        } else if (operacao == 'r') {
            raiz = remover(raiz, chave); // Remove um nó com a chave lida
        }
    }
    imprimirEmOrdem(raiz, arquivo_saida); // Imprime a árvore em ordem no arquivo de saída
    fclose(arquivo_entrada); // Fecha o arquivo de entrada
    fclose(arquivo_saida); // Fecha o arquivo de saída
    return 0;
}
