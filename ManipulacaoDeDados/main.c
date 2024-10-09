#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>

#define MAX 50
#define MAX_NOME 60
#define MAX_SIGLA 10
#define MAX_CPF 15


typedef struct Pessoa {
    char nome[MAX_NOME];
    int idade;
} Pessoa;

typedef struct Professor {
    Pessoa pes;
    int codigo;
    char depto[MAX_SIGLA];
} Professor;

typedef struct Aluno {
    Pessoa pes;
    int matricula;
    int ano;
    char depto[MAX_SIGLA];
} Aluno;

typedef struct TC {
    int codigo;
    int autor; 
    int orientador; 
    char titulo[MAX_NOME];
    int qtdeVotos; 
} TC;

typedef struct Eleitor {
    char cpf[MAX_CPF];
    bool votou; 
    int codigoTC;
} Eleitor;


Professor docentes[MAX];
int qtdeDocentes = 0;

Aluno formandos[MAX];
int qtdeFormandos = 0;

TC listaTCs[MAX];
int qtdeTCs = 0;

Eleitor comissao[MAX];
int qtdeEleitores = 0;

void lerProfessores(const char* nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        exit(1);
    }

    fscanf(file, "%d", &qtdeDocentes); 
    for (int i = 0; i < qtdeDocentes; i++) {
        fscanf(file, "%d %s %d %[^\n]",
               &docentes[i].codigo,
               docentes[i].depto,
               &docentes[i].pes.idade,
               docentes[i].pes.nome);
    }
    fclose(file);
}

void lerAlunos(const char* nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        exit(1);
    }

    fscanf(file, "%d", &qtdeFormandos); 
    for (int i = 0; i < qtdeFormandos; i++) {
        fscanf(file, "%d %d %s %d %[^\n]",
               &formandos[i].matricula,
               &formandos[i].ano,
               formandos[i].depto,
               &formandos[i].pes.idade,
               formandos[i].pes.nome);
    }
    fclose(file);
}

void lerTCs(const char* nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        exit(1);
    }

    fscanf(file, "%d", &qtdeTCs); 
    for (int i = 0; i < qtdeTCs; i++) {
        fscanf(file, "%d %d %d %[^\n]",
               &listaTCs[i].codigo,
               &listaTCs[i].autor,
               &listaTCs[i].orientador,
               listaTCs[i].titulo);
        listaTCs[i].qtdeVotos = 0; 
    }
    fclose(file);
}

void lerEleitores(const char* nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        exit(1);
    }

    fscanf(file, "%d", &qtdeEleitores); 
    for (int i = 0; i < qtdeEleitores; i++) {
        fscanf(file, "%s", comissao[i].cpf);
        comissao[i].votou = false; 
        comissao[i].codigoTC = -1; 
    }
    fclose(file);  
}


bool validarCPF(const char* cpf) {
    
    if (strlen(cpf) != 14 || cpf[3] != '.' || cpf[7] != '.' || cpf[11] != '-') {
        return false;
    }
    
    
    return true; 
}

void mostrarMenu() {
    printf("MENU\n");
    printf("a) Iniciar nova votação\n");
    printf("b) Continuar votação gravada\n");
}

void mostrarMenuVotacao() {
    printf("MENU DE VOTAÇÃO\n");
    printf("a) Entrar com voto\n");
    printf("b) Suspender votação\n");
    printf("c) Concluir votação\n");
}

void gerarRelatorioVotos(const char* nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "w");
    if (file == NULL) {
        printf("Erro ao criar o arquivo %s.\n", nomeArquivo);
        return;
    }

    int totalVotantes = 0;

    
    for (int i = 0; i < qtdeEleitores; i++) {
        if (comissao[i].votou) {
            totalVotantes++;
        }
    }

    
    fprintf(file, "%d\n", totalVotantes);

    
    for (int i = 0; i < qtdeEleitores; i++) {
        if (comissao[i].votou) {
            fprintf(file, "%s %d\n", comissao[i].cpf, comissao[i].codigoTC);
        }
    }

    fclose(file);
    printf("Relatório de votos gerado com sucesso: %s\n", nomeArquivo);
}

void entrarVoto() {
    char cpf[MAX_CPF];
    printf("Digite o CPF do eleitor: ");
    scanf("%s", cpf);

    if (!validarCPF(cpf)) {
        printf("CPF inválido!\n");
        return; 
    }

   
    bool eleitorEncontrado = false;
    for (int i = 0; i < qtdeEleitores; i++) {
        if (strcmp(comissao[i].cpf, cpf) == 0) {
            eleitorEncontrado = true;

            if (comissao[i].votou) {
                printf("Erro: Este CPF já votou.\n");
                return; 
            }

            
            int codigoTC;
            printf("Digite o código do TC para votar: ");
            scanf("%d", &codigoTC);

            
            bool tcValido = false;
            for (int j = 0; j < qtdeTCs; j++) {
                if (listaTCs[j].codigo == codigoTC) {
                    listaTCs[j].qtdeVotos++;
                    comissao[i].votou = true; 
                    comissao[i].codigoTC = codigoTC; 
                    printf("Voto computado com sucesso!\n");
                    tcValido = true;
                    break;
                }
            }

            if (!tcValido) {
                printf("Erro: Código do TC inválido.\n");
            }
            return; 
        }
    }

    if (!eleitorEncontrado) {
        printf("Erro: CPF não encontrado na comissão.\n");
    }
}

void gerarResultado(const char* nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "w");
    if (file == NULL) {
        printf("Erro ao criar o arquivo %s.\n", nomeArquivo);
        return;
    }

    fprintf(file, "TC vencedor(s)\n");

    int maxVotos = 0;
    for (int i = 0; i < qtdeTCs; i++) {
        if (listaTCs[i].qtdeVotos > maxVotos) {
            maxVotos = listaTCs[i].qtdeVotos;
        }
    }

    for (int i = 0; i < qtdeTCs; i++) {
        if (listaTCs[i].qtdeVotos == maxVotos) {
            fprintf(file, "Codigo: %d\n", listaTCs[i].codigo);
            fprintf(file, "Titulo: %s\n", listaTCs[i].titulo);
           
            for (int j = 0; j < qtdeFormandos; j++) {
                if (formandos[j].matricula == listaTCs[i].autor) {
                    fprintf(file, "Aluno: %s\n", formandos[j].pes.nome);
                    fprintf(file, "Depto aluno: %s\n", formandos[j].depto);
                    break;
                }
            }
           
            for (int j = 0; j < qtdeDocentes; j++) {
                if (docentes[j].codigo == listaTCs[i].orientador) {
                    fprintf(file, "Orientador: %s\n", docentes[j].pes.nome);
                    fprintf(file, "Depto orientador: %s\n", docentes[j].depto);
                    break;
                }
            }
            fprintf(file, "\n");
        }
    }

    fprintf(file, "Eleitores que votaram\n");
    for (int i = 0; i < qtdeEleitores; i++) {
        if (comissao[i].votou) {
            fprintf(file, "%s %d\n", comissao[i].cpf, comissao[i].codigoTC);
        }
    }

    fprintf(file, "\nEleitores que nao votaram\n");
    for (int i = 0; i < qtdeEleitores; i++) {
        if (!comissao[i].votou) {
            fprintf(file, "%s\n", comissao[i].cpf);
        }
    }

    fclose(file);
    printf("Resultados gerados com sucesso: %s\n", nomeArquivo);
}

void continuarVotacao(const char* nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        printf("Arquivo %s não encontrado. Retornando ao menu principal.\n", nomeArquivo);
        return;
    }

    int votosExistentes;
    fscanf(file, "%d", &votosExistentes);
    for (int i = 0; i < votosExistentes; i++) {
        char cpf[MAX_CPF];
        int codigoTC;
        fscanf(file, "%s %d", cpf, &codigoTC);
        
       
        for (int j = 0; j < qtdeEleitores; j++) {
            if (strcmp(comissao[j].cpf, cpf) == 0) {
                comissao[j].votou = true;
                comissao[j].codigoTC = codigoTC;
                for (int k = 0; k < qtdeTCs; k++) {
                    if (listaTCs[k].codigo == codigoTC) {
                        listaTCs[k].qtdeVotos++;
                        break;
                    }
                }
                break;
            }
        }
    }

    fclose(file);
    printf("Votação continuada com sucesso.\n");
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    
    
    lerProfessores("professor.txt");
    lerAlunos("aluno.txt");
    lerTCs("TC_BCC.txt");
    lerEleitores("comissao.txt");

  
    char opcao;
    mostrarMenu();
    printf("Escolha uma opção: ");
    scanf(" %c", &opcao);

    if (opcao == 'a' || opcao == 'A') {
        while (true) {
            mostrarMenuVotacao();
            printf("Escolha uma opção: ");
            char opcaoVotacao;
            scanf(" %c", &opcaoVotacao);

            if (opcaoVotacao == 'a' || opcaoVotacao == 'A') {
                entrarVoto();
            } else if (opcaoVotacao == 'b' || opcaoVotacao == 'B') {
                printf("Votação suspensa.\n");
                gerarRelatorioVotos("parcial.txt");
                break; 
            } else if (opcaoVotacao == 'c' || opcaoVotacao == 'C') {
                printf("Votação concluída.\n");
                gerarResultado("resultado.txt"); 
                break; 
            } else {
                printf("Opção inválida. Tente novamente.\n");
            }
        }
    } else if (opcao == 'b' || opcao == 'B') {
        continuarVotacao("parcial.txt");
        while (true) {
            mostrarMenuVotacao();
            printf("Escolha uma opção: ");
            char opcaoVotacao;
            scanf(" %c", &opcaoVotacao);

            if (opcaoVotacao == 'a' || opcaoVotacao == 'A') {
                entrarVoto();
            } else if (opcaoVotacao == 'b' || opcaoVotacao == 'B') {
                printf("Votação suspensa.\n");
                gerarRelatorioVotos("parcial.txt"); 
                break; 
            } else if (opcaoVotacao == 'c' || opcaoVotacao == 'C') {
                printf("Votação concluída.\n");
                gerarResultado("resultado.txt"); 
                break; 
            } else {
                printf("Opção inválida. Tente novamente.\n");
            }
        }
    } else {
        printf("Opção inválida. O programa será encerrado.\n");
    }

    return 0;
}
