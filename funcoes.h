//
// Created by roberto on 26/07/24.
//

#ifndef PROJETO_2_ROBERTO1929_FUNCOES_H
#define PROJETO_2_ROBERTO1929_FUNCOES_H

#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include <ctype.h>

#define DESC_SIZE 100

typedef struct {
    int id;
    char descricao[DESC_SIZE];
    char prioridade[10];
    char prazo[11];
    int concluida;
    char data_conclusao[11];
} Tarefa;

typedef struct {
    Tarefa *vetor;
    int capacidade;
    int tamanho;
} ListaTarefas;

void maiuscula_para_minuscula(char *str);
int gerar_id_novo();
void inicializar_lista(ListaTarefas *lista);
void liberar_lista(ListaTarefas *lista);
void adicionar_tarefa(ListaTarefas *lista);
void excluir_tarefa(ListaTarefas *lista);
void editar_tarefa(ListaTarefas *lista);
void concluir_tarefa(ListaTarefas *lista);
void buscar_tarefas(ListaTarefas *lista);
void visualizar_tarefas(ListaTarefas *lista, int filtro);
void ordenar_tarefas(ListaTarefas *lista);
void salvar_tarefas(ListaTarefas *lista);
void carregar_tarefas(ListaTarefas *lista);
int converter_prioridade(const char *prioridade);
int data_expirada(const char* prazo);
void menu_iterativo();

#endif // FUNCOES_H
#endif //PROJETO_2_ROBERTO1929_FUNCOES_H
