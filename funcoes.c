
//
// Created by roberto on 26/07/24.
//
#include "funcoes.h"


int id_atual = 1;

int gerar_id_novo() {
    return id_atual++;
}

void inicializar_lista(ListaTarefas *lista) {
    lista->capacidade = 10;
    lista->tamanho = 0;
    lista->vetor = malloc(lista->capacidade * sizeof(Tarefa));
    if (lista->vetor == NULL) {
        perror("Erro ao alocar memoria para lista de tarefas");
        exit(EXIT_FAILURE);
    }
}

int data_expirada(const char* prazo) {
    struct tm tm_prazo = {0};
    time_t agora;
    struct tm tm_agora = {0};

    sscanf(prazo, "%d-%d-%d", &tm_prazo.tm_year, &tm_prazo.tm_mon, &tm_prazo.tm_mday);
    tm_prazo.tm_year -= 1900;
    tm_prazo.tm_mon -= 1;

    time(&agora);
    tm_agora = *localtime(&agora);

    return mktime(&tm_prazo) < mktime(&tm_agora);
}

void liberar_lista(ListaTarefas *lista) {
    free(lista->vetor);
}

void adicionar_tarefa(ListaTarefas *lista) {
    if (lista->tamanho >= lista->capacidade) {
        lista->capacidade *= 2;
        lista->vetor = realloc(lista->vetor, lista->capacidade * sizeof(Tarefa));
        if (lista->vetor == NULL) {
            perror("Erro ao realocar memoria para lista de tarefas");
            exit(EXIT_FAILURE);
        }
    }

    Tarefa nova_tarefa;
    nova_tarefa.id = gerar_id_novo();

    printf("Digite a descricao da tarefa: ");
    fgets(nova_tarefa.descricao, DESC_SIZE, stdin);
    nova_tarefa.descricao[strcspn(nova_tarefa.descricao, "\n")] = '\0';
    if (strlen(nova_tarefa.descricao) == 0) {
        printf("Descricao nao pode ser vazia.\n");
        return;
    }

    char prioridade_temp[10];
    printf("Digite a prioridade (alta, media, baixa): ");
    fgets(prioridade_temp, sizeof(prioridade_temp), stdin);
    prioridade_temp[strcspn(prioridade_temp, "\n")] = '\0';
    maiuscula_para_minuscula(prioridade_temp);

    if (strcmp(prioridade_temp, "alta") == 0 ||
        strcmp(prioridade_temp, "media") == 0 ||
        strcmp(prioridade_temp, "baixa") == 0) {
        strcpy(nova_tarefa.prioridade, prioridade_temp);
    } else {
        printf("Prioridade invalida. Escolha entre alta, media ou baixa.\n");
        return;
    }

    char prazo_temp[11];
    printf("Digite o prazo (AAAA-MM-DD): ");
    fgets(prazo_temp, sizeof(prazo_temp), stdin);
    prazo_temp[strcspn(prazo_temp, "\n")] = '\0';

    struct tm tm_prazo = {0};
    if (sscanf(prazo_temp, "%d-%d-%d", &tm_prazo.tm_year, &tm_prazo.tm_mon, &tm_prazo.tm_mday) == 3) {
        tm_prazo.tm_year -= 1900;
        tm_prazo.tm_mon -= 1;
        if (tm_prazo.tm_year < 0 || tm_prazo.tm_mon < 0 || tm_prazo.tm_mday < 1 ||
            tm_prazo.tm_mon > 11 || tm_prazo.tm_mday > 31) {
            printf("Formato de data invalido. Use o formato AAAA-MM-DD.\n");
            return;
        }
        strcpy(nova_tarefa.prazo, prazo_temp);
    } else {
        printf("Formato de data invalido. Use o formato AAAA-MM-DD.\n");
        return;
    }

    nova_tarefa.concluida = 0;
    strcpy(nova_tarefa.data_conclusao, "");

    lista->vetor[lista->tamanho++] = nova_tarefa;

    printf("Tarefa adicionada com sucesso!\n");
}


void excluir_tarefa(ListaTarefas *lista) {
    visualizar_tarefas(lista,3);
    int id;
    printf("Digite o ID da tarefa a ser excluida: ");
    scanf("%d", &id);
    getchar();

    int encontrada = 0;
    for (int i = 0; i < lista->tamanho; i++) {
        if (lista->vetor[i].id == id) {
            encontrada = 1;
            for (int j = i; j < lista->tamanho - 1; j++) {
                lista->vetor[j] = lista->vetor[j + 1];
            }
            lista->tamanho--;
            printf("Tarefa excluida com sucesso!\n");
            break;
        }
    }

    if (!encontrada) {
        printf("Tarefa com ID %d nao encontrada!\n", id);
    }
}

void editar_tarefa(ListaTarefas *lista) {
    visualizar_tarefas(lista,3);
    int id;
    printf("Digite o ID da tarefa a ser editada: ");
    if (scanf("%d", &id) != 1) {
        printf("ID inválido.\n");
        while (getchar() != '\n'); // Limpar buffer de entrada
        return;
    }
    while (getchar() != '\n'); // Limpar buffer de entrada

    int indice = -1;
    for (int i = 0; i < lista->tamanho; i++) {
        if (lista->vetor[i].id == id) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Tarefa com ID %d não encontrada.\n", id);
        return;
    }

    Tarefa *tarefa = &lista->vetor[indice];

    printf("Digite a nova descricao da tarefa (max %d caracteres): ", DESC_SIZE - 1);
    fgets(tarefa->descricao, DESC_SIZE, stdin);
    tarefa->descricao[strcspn(tarefa->descricao, "\n")] = '\0';
    if (strlen(tarefa->descricao) == 0) {
        printf("Descricao não pode ser vazia.\n");
        return;
    }

    char prioridade_temp[10];
    printf("Digite a nova prioridade (alta, media, baixa): ");
    fgets(prioridade_temp, sizeof(prioridade_temp), stdin);
    prioridade_temp[strcspn(prioridade_temp, "\n")] = '\0';
    maiuscula_para_minuscula(prioridade_temp);

    if (strcmp(prioridade_temp, "alta") == 0 ||
        strcmp(prioridade_temp, "media") == 0 ||
        strcmp(prioridade_temp, "baixa") == 0) {
        strcpy(tarefa->prioridade, prioridade_temp);
    } else {
        printf("Prioridade invalida. Escolha entre alta, media ou baixa.\n");
        return;
    }

    char prazo_temp[11];
    printf("Digite o novo prazo (AAAA-MM-DD): ");
    fgets(prazo_temp, sizeof(prazo_temp), stdin);
    prazo_temp[strcspn(prazo_temp, "\n")] = '\0';

    struct tm tm_prazo = {0};
    if (sscanf(prazo_temp, "%d-%d-%d", &tm_prazo.tm_year, &tm_prazo.tm_mon, &tm_prazo.tm_mday) == 3) {
        tm_prazo.tm_year -= 1900;
        tm_prazo.tm_mon -= 1;
        if (tm_prazo.tm_year < 0 || tm_prazo.tm_mon < 0 || tm_prazo.tm_mday < 1 ||
            tm_prazo.tm_mon > 11 || tm_prazo.tm_mday > 31) {
            printf("Formato de data invalido. Use o formato AAAA-MM-DD.\n");
            return;
        }
        strcpy(tarefa->prazo, prazo_temp);
    } else {
        printf("Formato de data invalido. Use o formato AAAA-MM-DD.\n");
        return;
    }

    printf("Tarefa editada com sucesso!\n");
}


void concluir_tarefa(ListaTarefas *lista) {
    visualizar_tarefas(lista,3);
    int id;
    printf("Digite o ID da tarefa a ser concluída: ");
    if (scanf("%d", &id) != 1) {
        printf("ID inválido.\n");
        while (getchar() != '\n'); // Limpar buffer de entrada
        return;
    }
    while (getchar() != '\n'); // Limpar buffer de entrada

    int indice = -1;
    for (int i = 0; i < lista->tamanho; i++) {
        if (lista->vetor[i].id == id) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Tarefa com ID %d não encontrada.\n", id);
        return;
    }

    Tarefa *tarefa = &lista->vetor[indice];

    if (tarefa->concluida) {
        printf("Tarefa já está marcada como concluída.\n");
        return;
    }

    tarefa->concluida = 1;

    time_t agora = time(NULL);
    struct tm *tm_agora = localtime(&agora);
    strftime(tarefa->data_conclusao, sizeof(tarefa->data_conclusao), "%Y-%m-%d", tm_agora);

    printf("Tarefa concluída com sucesso!\n");
}


void maiuscula_para_minuscula(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void buscar_tarefas(ListaTarefas *lista) {
    int opcao;
    char busca[DESC_SIZE];

    printf("Buscar tarefa por:\n");
    printf("1. Descricao\n");
    printf("2. Prioridade\n");
    printf("3. Prazo para conclusao\n");
    printf("4. Data de conclusao\n");
    printf("Escolha uma opcão (1-4): ");
    scanf("%d", &opcao);
    getchar();

    printf("Tarefas encontradas:\n");

    switch(opcao) {
        case 1:
            printf("Digite a descricao (completa ou parcial): ");
            fgets(busca, DESC_SIZE, stdin);
            busca[strcspn(busca, "\n")] = '\0';
            maiuscula_para_minuscula(busca);

            for (int i = 0; i < lista->tamanho; i++) {
                char descricao_lower[DESC_SIZE];
                strcpy(descricao_lower, lista->vetor[i].descricao);
                maiuscula_para_minuscula(descricao_lower);

                if (strstr(descricao_lower, busca) != NULL) {
                    printf("ID: %d, Descricao: %s, Prioridade: %s, Prazo: %s, Concluida: %s, Data Conclusao: %s\n",
                           lista->vetor[i].id, lista->vetor[i].descricao, lista->vetor[i].prioridade,
                           lista->vetor[i].prazo, lista->vetor[i].concluida ? "Sim" : "Nao",
                           lista->vetor[i].concluida ? lista->vetor[i].data_conclusao : "Nao definida");
                }
            }
            break;
        case 2:
            printf("Digite a prioridade (alta, media, baixa): ");
            fgets(busca, 10, stdin);
            busca[strcspn(busca, "\n")] = '\0';
            maiuscula_para_minuscula(busca);

            for (int i = 0; i < lista->tamanho; i++) {
                char prioridade_lower[10];
                strcpy(prioridade_lower, lista->vetor[i].prioridade);
                maiuscula_para_minuscula(prioridade_lower);

                if (strcmp(prioridade_lower, busca) == 0) {
                    printf("ID: %d, Descricao: %s, Prioridade: %s, Prazo: %s, Concluida: %s, Data Conclusao: %s\n",
                           lista->vetor[i].id, lista->vetor[i].descricao, lista->vetor[i].prioridade,
                           lista->vetor[i].prazo, lista->vetor[i].concluida ? "Sim" : "Nao",
                           lista->vetor[i].concluida ? lista->vetor[i].data_conclusao : "Nao definida");
                }
            }
            break;
        case 3:
            printf("Digite o prazo (AAAA-MM-DD): ");
            fgets(busca, 11, stdin);
            busca[strcspn(busca, "\n")] = '\0';

            for (int i = 0; i < lista->tamanho; i++) {
                if (strcmp(lista->vetor[i].prazo, busca) == 0) {
                    printf("ID: %d, Descricao: %s, Prioridade: %s, Prazo: %s, Concluida: %s, Data Conclusao: %s\n",
                           lista->vetor[i].id, lista->vetor[i].descricao, lista->vetor[i].prioridade,
                           lista->vetor[i].prazo, lista->vetor[i].concluida ? "Sim" : "Nao",
                           lista->vetor[i].concluida ? lista->vetor[i].data_conclusao : "Nao definida");
                }
            }
            break;
        case 4:
            printf("Digite a data de conclusao (AAAA-MM-DD): ");
            fgets(busca, 11, stdin);
            busca[strcspn(busca, "\n")] = '\0';

            for (int i = 0; i < lista->tamanho; i++) {
                if (strcmp(lista->vetor[i].data_conclusao, busca) == 0) {
                    printf("ID: %d, Descricao: %s, Prioridade: %s, Prazo: %s, Concluida: %s, Data Conclusao: %s\n",
                           lista->vetor[i].id, lista->vetor[i].descricao, lista->vetor[i].prioridade,
                           lista->vetor[i].prazo, lista->vetor[i].concluida ? "Sim" : "Nao",
                           lista->vetor[i].concluida ? lista->vetor[i].data_conclusao : "Nao definida");
                }
            }
            break;
        default:
            printf("Opcão invalida.\n");
            break;
    }
}

void visualizar_tarefas(ListaTarefas *lista, int filtro){
    if (lista->tamanho == 0) {
        printf("Nao ha tarefas para exibir.\n");
        return;
    }

    printf("%-5s | %-50s | %-10s | %-12s | %-10s | %-12s | %-10s\n", "ID", "Descricao", "Prioridade", "Prazo", "Concluida", "Data Conclusao", "Atrasada");
    printf("----------------------------------------------------------------------------------------------------------------------------\n");

    int tarefas_exibidas = 0;

    for (int i = 0; i < lista->tamanho; i++) {
        int exibir = 0;
        switch (filtro) {
            case 1: // Tarefas concluidas
                if (lista->vetor[i].concluida) {
                    exibir = 1;
                }
                break;
            case 2: // Tarefas nao concluidas
                if (!lista->vetor[i].concluida) {
                    exibir = 1;
                }
                break;
            case 3: // Todas as tarefas
                exibir = 1;
                break;
        }

        if (exibir) {
            int atrasada = !lista->vetor[i].concluida && data_expirada(lista->vetor[i].prazo);
            printf("%-5d | %-50s | %-10s | %-12s | %-10s | %-12s | %-10s\n",
                   lista->vetor[i].id,
                   lista->vetor[i].descricao,
                   lista->vetor[i].prioridade,
                   lista->vetor[i].prazo,
                   lista->vetor[i].concluida ? "Sim" : "Nao",
                   lista->vetor[i].data_conclusao,
                   atrasada ? "Sim" : "Nao");
            tarefas_exibidas++;
        }
    }

    if (tarefas_exibidas == 0) {
        printf("Nao ha tarefas para exibir com o filtro selecionado.\n");
    }
}

int converter_prioridade(const char *prioridade) {
    if (strcmp(prioridade, "baixa") == 0) return 1;
    if (strcmp(prioridade, "media") == 0) return 2;
    if (strcmp(prioridade, "alta") == 0) return 3;
    return 0; // valor padrão, caso a prioridade não seja reconhecida
}

void bubble_sort_por_prioridade(Tarefa *vetor, int tamanho, int ordem) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - 1 - i; j++) {
            int prioridade1 = converter_prioridade(vetor[j].prioridade);
            int prioridade2 = converter_prioridade(vetor[j + 1].prioridade);
            if ((ordem == 1 && prioridade1 > prioridade2) ||
                (ordem == 2 && prioridade1 < prioridade2)) {
                Tarefa temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

void bubble_sort_por_prazo(Tarefa *vetor, int tamanho, int ordem) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - 1 - i; j++) {
            if ((ordem == 1 && strcmp(vetor[j].prazo, vetor[j + 1].prazo) > 0) ||
                (ordem == 2 && strcmp(vetor[j].prazo, vetor[j + 1].prazo) < 0)) {
                Tarefa temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

void bubble_sort_por_data_conclusao(Tarefa *vetor, int tamanho, int ordem) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - 1 - i; j++) {
            if ((ordem == 1 && strcmp(vetor[j].data_conclusao, vetor[j + 1].data_conclusao) > 0) ||
                (ordem == 2 && strcmp(vetor[j].data_conclusao, vetor[j + 1].data_conclusao) < 0)) {
                Tarefa temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

void ordenar_tarefas(ListaTarefas *lista) {
    int opcao, ordem;
    printf("Ordenar tarefas por:\n");
    printf("1. Prioridade\n");
    printf("2. Prazo\n");
    printf("3. Data de conclusao\n");
    printf("Escolha uma opcão (1-3): ");
    scanf("%d", &opcao);
    getchar();

    printf("Escolha a ordem de ordenação:\n");
    printf("1. Crescente\n");
    printf("2. Decrescente\n");
    printf("Escolha uma opcão (1-2): ");
    scanf("%d", &ordem);
    getchar();

    switch (opcao) {
        case 1:
            bubble_sort_por_prioridade(lista->vetor, lista->tamanho, ordem);
            break;
        case 2:
            bubble_sort_por_prazo(lista->vetor, lista->tamanho, ordem);
            break;
        case 3:
            bubble_sort_por_data_conclusao(lista->vetor, lista->tamanho, ordem);
            break;
        default:
            printf("Opcão invalida.\n");
            return;
    }

    printf("Tarefas ordenadas com sucesso!\n\n");
    visualizar_tarefas(lista, 3);
}

void salvar_tarefas(ListaTarefas *lista) {
    FILE *arquivo = fopen("tarefas.bin", "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo para salvar tarefas");
        return;
    }

    for (int i = 0; i < lista->tamanho; i++) {
        fwrite(&lista->vetor[i], sizeof(Tarefa), 1, arquivo);
    }
    printf("Arquivo salvo com sucesso.\n");

    fclose(arquivo);
}

void carregar_tarefas(ListaTarefas *lista) {
    FILE *arquivo = fopen("tarefas.bin", "rb");
    if (arquivo == NULL) {
        printf("Arquivo não encontrado. Criando novo arquivo.\n");
        arquivo = fopen("tarefas.bin", "wb");
        if (arquivo == NULL) {
            perror("Erro ao criar o arquivo");
            return;
        }
        fclose(arquivo); // Fecha o arquivo criado
        return;
    }
    printf("\nArquivo de tarefas aberto com sucesso.\n");
    lista->tamanho = 0;
    int maior_id = 0;

    while (fread(&lista->vetor[lista->tamanho], sizeof(Tarefa), 1, arquivo)) {
        if (lista->vetor[lista->tamanho].id > maior_id) {
            maior_id = lista->vetor[lista->tamanho].id;
        }
        lista->tamanho++;
    }

    fclose(arquivo);

    id_atual = maior_id + 1;
}

void menu_iterativo() {
    ListaTarefas lista;
    inicializar_lista(&lista);
    carregar_tarefas(&lista);

    int opcao;

    do {
        printf("\n\n");
        printf("|===============================> LISTA DE TAREFAS <===============================|\n");
        printf("|====================================> MENU <======================================|\n");
        printf("1. Adicionar Tarefa\n");
        printf("2. Excluir\n");
        printf("3. Editar\n");
        printf("4. Concluir/Desmarcar\n");
        printf("5. Buscar\n");
        printf("6. Visualizar\n");
        printf("7. Ordenar\n");
        printf("8. Sair\n");
        printf("Escolha uma opcão (1-8): ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1:
                adicionar_tarefa(&lista);
                break;
            case 2:
                excluir_tarefa(&lista);
                break;
            case 3:
                editar_tarefa(&lista);
                break;
            case 4:
                concluir_tarefa(&lista);
                break;
            case 5:
                buscar_tarefas(&lista);
                break;
            case 6:
                int filtro;
                printf("Escolha o filtro de visualizacao:\n");
                printf("1. Tarefas concluidas | ");
                printf("2. Não concluidas | ");
                printf("3. Todas\n");
                printf("Escolha uma opcão (1-3): ");
                scanf("%d", &filtro);
                if (filtro < 1 || filtro > 3) {
                    printf("Opcão invalida!\n");
                } else {
                    visualizar_tarefas(&lista, filtro);
                }
                break;
            case 7:
                ordenar_tarefas(&lista);
                break;
            case 8:
                salvar_tarefas(&lista);
                liberar_lista(&lista);
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opcão invalida. Tente novamente.\n");
        }
    } while (opcao != 8);
}

