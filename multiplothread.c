#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TAMANHO_LISTA 10

// Estrutura para armazenar informações passadas para cada thread
struct ThreadData {
    int *lista;
    int tamanho;
};

// Função executada por cada thread classificador
void *funcao_classificador(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;
    
    // Implementação do bubble sort (pode ser substituído por outro algoritmo)
    for (int i = 0; i < data->tamanho - 1; ++i) {
        for (int j = 0; j < data->tamanho - i - 1; ++j) {
            if (data->lista[j] > data->lista[j + 1]) {
                // Troca os elementos se estiverem fora de ordem
                int temp = data->lista[j];
                data->lista[j] = data->lista[j + 1];
                data->lista[j + 1] = temp;
            }
        }
    }

    return NULL;
}

// Função executada pelo thread de mesclagem
void *funcao_mesclagem(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;
    int meio = data->tamanho / 2;
    int *sublista1 = data->lista;
    int *sublista2 = data->lista + meio;

    // Mescla as duas sublistas ordenadas
    int *lista_temp = (int *)malloc(data->tamanho * sizeof(int));
    int i = 0, j = 0, k = 0;

    while (i < meio && j < meio) {
        if (sublista1[i] < sublista2[j]) {
            lista_temp[k++] = sublista1[i++];
        } else {
            lista_temp[k++] = sublista2[j++];
        }
    }

    while (i < meio) {
        lista_temp[k++] = sublista1[i++];
    }

    while (j < meio) {
        lista_temp[k++] = sublista2[j++];
    }

    // Copia a lista mesclada de volta para a lista original
    for (i = 0; i < data->tamanho; ++i) {
        data->lista[i] = lista_temp[i];
    }

    free(lista_temp);
    return NULL;
}

int main() {
    int lista[TAMANHO_LISTA];

    // Solicita ao usuário para inserir números
    printf("Digite %d numeros:\n", TAMANHO_LISTA);
    for (int i = 0; i < TAMANHO_LISTA; ++i) {
        scanf("%d", &lista[i]);
    }

    pthread_t threads[3];
    struct ThreadData thread_data[3];

    // Divide a lista entre os threads
    int elementos_por_thread = TAMANHO_LISTA / 2;
    for (int i = 0; i < 2; ++i) {
        thread_data[i].lista = lista + i * elementos_por_thread;
        thread_data[i].tamanho = elementos_por_thread;
        pthread_create(&threads[i], NULL, funcao_classificador, (void *)&thread_data[i]);
    }

    // Aguarde até que os threads classificadores tenham concluído
    for (int i = 0; i < 2; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Execute o thread de mesclagem
    thread_data[2].lista = lista;
    thread_data[2].tamanho = TAMANHO_LISTA;
    pthread_create(&threads[2], NULL, funcao_mesclagem, (void *)&thread_data[2]);

    // Aguarde até que o thread de mesclagem tenha concluído
    pthread_join(threads[2], NULL);

    // Lista final ordenada
    printf("Lista ordenada: ");
    for (int i = 0; i < TAMANHO_LISTA; ++i) {
        printf("%d ", lista[i]);
    }
    printf("\n");

    return 0;
}
