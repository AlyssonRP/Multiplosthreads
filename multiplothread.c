#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// struct ThreadData: Armazena informações necessárias para cada thread, como ponteiro para a lista e seu tamanho.

struct ThreadData
{
    int *lista;
    int tamanho;
};

// funcao_classificador: Implementa o algoritmo de ordenação de bolha para classificar uma parte da lista.

void *funcao_classificador(void *arg)
{
    struct ThreadData *data = (struct ThreadData *)arg;

    // Implementação do bubble sort
    for (int i = 0; i < data->tamanho - 1; ++i)
    {
        for (int j = 0; j < data->tamanho - i - 1; ++j)
        {
            if (data->lista[j] > data->lista[j + 1])
            {
                // Troca os elementos se estiverem fora de ordem
                int temp = data->lista[j];
                data->lista[j] = data->lista[j + 1];
                data->lista[j + 1] = temp;
            }
        }
    }

    return NULL;
}

// funcao_mesclagem: Mescla duas sublistas ordenadas em uma única lista ordenada.

void *funcao_mesclagem(void *arg)
{
    struct ThreadData *data = (struct ThreadData *)arg;
    int meio = data->tamanho / 2;
    int *sublista1 = data->lista;
    int *sublista2 = data->lista + meio;

    // Mescla as duas sublistas ordenadas
    int *lista_temp = (int *)malloc(data->tamanho * sizeof(int));
    int i = 0, j = 0, k = 0;

    while (i < meio && j < meio)
    {
        if (sublista1[i] < sublista2[j])
        {
            lista_temp[k++] = sublista1[i++];
        }
        else
        {
            lista_temp[k++] = sublista2[j++];
        }
    }

    while (i < meio)
    {
        lista_temp[k++] = sublista1[i++];
    }

    while (j < meio)
    {
        lista_temp[k++] = sublista2[j++];
    }

    // Copia a lista mesclada de volta para a lista original
    for (i = 0; i < data->tamanho; ++i)
    {
        data->lista[i] = lista_temp[i];
    }

    free(lista_temp);
    return NULL;
}

// Alocação dinâmica de memória para a lista com base nos argumentos fornecidos na linha de comando.
// Preenchimento da lista com números fornecidos como argumentos.
// Criação de threads classificadores e execução da função funcao_classificador para cada parte da lista.
// Aguardo até que os threads classificadores tenham concluído.
// Criação de um thread de mesclagem e execução da função funcao_mesclagem para mesclar as sublistas.
// Aguardo até que o thread de mesclagem tenha concluído.
// Impressão da lista final ordenada.
// Liberação da memória alocada dinamicamente.

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Digite pelo menos 1 numero como argumento na linha de comando.\n");
        return 1;
    }

    int *lista;
    int total_elementos = argc - 1;

    // Aloca espaço para a lista dinamicamente
    lista = (int *)malloc(total_elementos * sizeof(int));

    // Preenche a lista com os números fornecidos como argumentos
    for (int i = 0; i < total_elementos; ++i)
    {
        lista[i] = atoi(argv[i + 1]); // Converte a string para inteiro
    }

    //     Verificação do número correto de argumentos na linha de comando.
    //     Conversão de strings para números inteiros usando atoi.
    //     Alocacão dinâmica de memória usando malloc.
    //     Liberação de memória alocada dinamicamente usando free.

    pthread_t threads[3];
    struct ThreadData thread_data[3];

    int elementos_por_thread = total_elementos / 2;
    for (int i = 0; i < 2; ++i)
    {
        thread_data[i].lista = lista + i * elementos_por_thread;
        thread_data[i].tamanho = elementos_por_thread;
        pthread_create(&threads[i], NULL, funcao_classificador, (void *)&thread_data[i]);
    }

    for (int i = 0; i < 2; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    thread_data[2].lista = lista;
    thread_data[2].tamanho = total_elementos;
    pthread_create(&threads[2], NULL, funcao_mesclagem, (void *)&thread_data[2]);

    pthread_join(threads[2], NULL);

    printf("Lista ordenada: ");
    for (int i = 0; i < total_elementos; ++i)
    {
        printf("%d ", lista[i]);
    }
    printf("\n");

    // Libera a memória alocada dinamicamente
    free(lista);

    return 0;
}
