cmake_minimum_required(VERSION 3.21)
project(projeto_lista_tarefas C)

set(CMAKE_C_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        libprg
        GIT_REPOSITORY https://github.com/roberto1929/libprg
        GIT_TAG origin/main
        # GIT_TAG 0.0.1
        # You can reference a local directory instead
        # URL file:///${CMAKE_CURRENT_SOURCE_DIR}/../libprg
)
FetchContent_MakeAvailable(libprg)

add_executable(projeto_lista_tarefas main.c
        funcoes.c
        funcoes.h
)

# linking libprg
target_link_libraries(projeto_lista_tarefas PUBLIC libprg)