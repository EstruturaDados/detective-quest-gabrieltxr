#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Sistema completo com árvore binária, BST de pistas e tabela hash de suspeitos

// ============================================
// STRUCTS E DEFINIÇÕES
// ============================================

// 🌱 Nível Novato: Struct para o mapa da mansão
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
    bool temPista;           // Indica se há pista nesta sala
    char textoPista[100];    // Texto da pista encontrada
} Sala;

// 🔍 Nível Aventureiro: Struct para árvore de busca de pistas
typedef struct NoPista {
    char texto[100];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

// 🧠 Nível Mestre: Struct para lista de pistas de um suspeito
typedef struct ItemPista {
    char pista[100];
    struct ItemPista *proximo;
} ItemPista;

// 🧠 Nível Mestre: Struct para suspeitos
typedef struct Suspeito {
    char nome[50];
    int contador;              // Quantas vezes foi citado
    ItemPista *pistas;         // Lista encadeada de pistas
    struct Suspeito *proximo;  // Para tratar colisões (encadeamento)
} Suspeito;

// Tabela Hash
#define TAM_HASH 10
Suspeito *tabelaHash[TAM_HASH];

// ============================================
// PROTÓTIPOS DAS FUNÇÕES
// ============================================

// Nível Novato - Mapa da Mansão
Sala* criarSala(const char *nome, bool temPista, const char *pista);
void conectarSalas(Sala *pai, Sala *esq, Sala *dir);
void explorarSalas(Sala *salaAtual, NoPista **raizPistas);
void liberarMapa(Sala *raiz);

// Nível Aventureiro - Árvore de Pistas
NoPista* inserirBST(NoPista *raiz, const char *texto);
void listarPistas(NoPista *raiz);
void liberarBST(NoPista *raiz);

// Nível Mestre - Tabela Hash de Suspeitos
int funcaoHash(const char *nome);
void inicializarHash();
void inserirHash(const char *pista, const char *suspeito);
void buscarSuspeito(const char *nome);
void listarAssociacoes();
void encontrarSuspeitoMaisProvavel();
void liberarHash();

// Utilitários
void exibirMenu();
void exibirMenuPrincipal();
void limparTela();

// ============================================
// FUNÇÃO PRINCIPAL
// ============================================

int main() {
    NoPista *raizPistas = NULL;
    int opcao;
    
    printf("===========================================\n");
    printf("      DETECTIVE QUEST - ENIGMA STUDIOS\n");
    printf("   Arvores e Tabela Hash - Nivel Mestre\n");
    printf("===========================================\n\n");
    
    // 🌱 Nível Novato: Criação do mapa da mansão (árvore binária fixa)
    printf("Carregando mapa da mansao...\n");
    
    Sala *hall = criarSala("Hall de Entrada", false, "");
    Sala *biblioteca = criarSala("Biblioteca", true, "Livro com pagina marcada sobre venenos");
    Sala *cozinha = criarSala("Cozinha", true, "Faca ensanguentada escondida");
    Sala *sotao = criarSala("Sotao", true, "Carta amorosa assinada por Carlos");
    Sala *jardim = criarSala("Jardim", true, "Pegadas levam ao portao dos fundos");
    Sala *escritorio = criarSala("Escritorio", true, "Documento financeiro adulterado por Marina");
    Sala *quarto = criarSala("Quarto Principal", true, "Relogio parado as 23h15");
    Sala *garagem = criarSala("Garagem", true, "Pneu furado recentemente");
    
    // Montagem da árvore
    conectarSalas(hall, biblioteca, cozinha);
    conectarSalas(biblioteca, sotao, jardim);
    conectarSalas(cozinha, escritorio, quarto);
    conectarSalas(jardim, NULL, garagem);
    
    // 🧠 Nível Mestre: Inicializa tabela hash
    inicializarHash();
    
    // Pré-carrega algumas associações pista-suspeito
    inserirHash("venenos", "Dr. Silva");
    inserirHash("Faca ensanguentada", "Carlos");
    inserirHash("Carta amorosa", "Carlos");
    inserirHash("adulterado por Marina", "Marina");
    inserirHash("Pegadas", "Dr. Silva");
    inserirHash("Relogio parado", "Marina");
    inserirHash("Pneu furado", "Carlos");
    
    printf("Mapa carregado! Sistema de investigacao pronto.\n\n");
    
    // Menu principal
    do {
        exibirMenuPrincipal();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa buffer
        
        limparTela();
        
        switch(opcao) {
            case 1:
                // 🌱 Explorar mansão
                printf("=== EXPLORAR MANSAO ===\n\n");
                explorarSalas(hall, &raizPistas);
                break;
                
            case 2:
                // 🔍 Listar pistas coletadas
                printf("=== PISTAS COLETADAS (em ordem alfabetica) ===\n");
                if(raizPistas == NULL) {
                    printf("Nenhuma pista coletada ainda.\n\n");
                } else {
                    listarPistas(raizPistas);
                    printf("\n");
                }
                break;
                
            case 3:
                // 🧠 Buscar suspeito específico
                printf("=== BUSCAR SUSPEITO ===\n");
                char nomeBusca[50];
                printf("Nome do suspeito: ");
                fgets(nomeBusca, 50, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
                buscarSuspeito(nomeBusca);
                break;
                
            case 4:
                // 🧠 Listar todos os suspeitos
                printf("=== TODOS OS SUSPEITOS E PISTAS ===\n");
                listarAssociacoes();
                break;
                
            case 5:
                // 🧠 Encontrar suspeito mais provável
                printf("=== ANALISE FINAL ===\n");
                encontrarSuspeitoMaisProvavel();
                break;
                
            case 0:
                printf("Encerrando investigacao...\n");
                printf("Caso encerrado!\n");
                break;
                
            default:
                printf("Opcao invalida!\n\n");
        }
        
    } while(opcao != 0);
    
    // Liberação de memória
    liberarMapa(hall);
    liberarBST(raizPistas);
    liberarHash();
    
    return 0;
}

// ============================================
// NÍVEL NOVATO - MAPA DA MANSÃO
// ============================================

// criarSala():
// Cria uma nova sala com nome, indica se tem pista e armazena texto da pista
Sala* criarSala(const char *nome, bool temPista, const char *pista) {
    Sala *nova = (Sala*)malloc(sizeof(Sala));
    if(nova == NULL) {
        printf("ERRO: Falha na alocacao de memoria!\n");
        exit(1);
    }
    
    strcpy(nova->nome, nome);
    nova->temPista = temPista;
    if(temPista) {
        strcpy(nova->textoPista, pista);
    }
    nova->esquerda = NULL;
    nova->direita = NULL;
    
    return nova;
}

// conectarSalas():
// Conecta uma sala pai com suas salas filhas (esquerda e direita)
void conectarSalas(Sala *pai, Sala *esq, Sala *dir) {
    if(pai != NULL) {
        pai->esquerda = esq;
        pai->direita = dir;
    }
}

// explorarSalas():
// Permite navegação interativa pela mansão
// Coleta pistas automaticamente e insere na BST
void explorarSalas(Sala *salaAtual, NoPista **raizPistas) {
    char escolha;
    
    while(salaAtual != NULL) {
        printf("===========================================\n");
        printf("Voce esta no(a): %s\n", salaAtual->nome);
        printf("===========================================\n");
        
        // 🔍 Verifica se há pista nesta sala
        if(salaAtual->temPista) {
            printf("\n*** PISTA ENCONTRADA ***\n");
            printf("Pista: %s\n", salaAtual->textoPista);
            printf("(Pista adicionada ao caderno de investigacao)\n\n");
            
            // Insere na árvore de busca de pistas
            *raizPistas = inserirBST(*raizPistas, salaAtual->textoPista);
        }
        
        // Verifica se é nó-folha
        if(salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("\nEste comodo nao possui mais caminhos!\n");
            printf("Voltando ao menu principal...\n\n");
            break;
        }
        
        // Exibe caminhos disponíveis
        printf("Caminhos disponiveis:\n");
        if(salaAtual->esquerda != NULL) {
            printf("  [E] Esquerda -> %s\n", salaAtual->esquerda->nome);
        }
        if(salaAtual->direita != NULL) {
            printf("  [D] Direita  -> %s\n", salaAtual->direita->nome);
        }
        printf("  [S] Sair da exploracao\n");
        
        exibirMenu();
        
        printf("Sua escolha: ");
        scanf(" %c", &escolha);
        getchar();
        
        printf("\n");
        
        switch(escolha) {
            case 'e':
            case 'E':
                if(salaAtual->esquerda != NULL) {
                    printf("Seguindo para a esquerda...\n\n");
                    salaAtual = salaAtual->esquerda;
                } else {
                    printf("ERRO: Nao ha caminho a esquerda!\n\n");
                }
                break;
                
            case 'd':
            case 'D':
                if(salaAtual->direita != NULL) {
                    printf("Seguindo para a direita...\n\n");
                    salaAtual = salaAtual->direita;
                } else {
                    printf("ERRO: Nao ha caminho a direita!\n\n");
                }
                break;
                
            case 's':
            case 'S':
                printf("Saindo da exploracao...\n\n");
                return;
                
            default:
                printf("Opcao invalida!\n\n");
        }
    }
}

// liberarMapa():
// Libera memória da árvore de salas (pós-ordem)
void liberarMapa(Sala *raiz) {
    if(raiz == NULL) return;
    liberarMapa(raiz->esquerda);
    liberarMapa(raiz->direita);
    free(raiz);
}

// ============================================
// NÍVEL AVENTUREIRO - ÁRVORE DE PISTAS (BST)
// ============================================

// inserirBST():
// Insere uma pista na árvore de busca binária (ordenação alfabética)
NoPista* inserirBST(NoPista *raiz, const char *texto) {
    // Caso base: cria novo nó
    if(raiz == NULL) {
        NoPista *novo = (NoPista*)malloc(sizeof(NoPista));
        strcpy(novo->texto, texto);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    
    // Comparação para decidir posição
    int comp = strcmp(texto, raiz->texto);
    
    if(comp < 0) {
        raiz->esquerda = inserirBST(raiz->esquerda, texto);
    } else if(comp > 0) {
        raiz->direita = inserirBST(raiz->direita, texto);
    }
    // Se comp == 0, pista já existe (não insere duplicata)
    
    return raiz;
}

// listarPistas():
// Exibe pistas em ordem alfabética (percurso in-order)
void listarPistas(NoPista *raiz) {
    if(raiz == NULL) return;
    
    listarPistas(raiz->esquerda);
    printf("  - %s\n", raiz->texto);
    listarPistas(raiz->direita);
}

// liberarBST():
// Libera memória da BST de pistas
void liberarBST(NoPista *raiz) {
    if(raiz == NULL) return;
    liberarBST(raiz->esquerda);
    liberarBST(raiz->direita);
    free(raiz);
}

// ============================================
// NÍVEL MESTRE - TABELA HASH DE SUSPEITOS
// ============================================

// funcaoHash():
// Calcula índice hash baseado na soma dos valores ASCII do nome
int funcaoHash(const char *nome) {
    int soma = 0;
    for(int i = 0; nome[i] != '\0'; i++) {
        soma += nome[i];
    }
    return soma % TAM_HASH;
}

// inicializarHash():
// Inicializa todos os slots da tabela como NULL
void inicializarHash() {
    for(int i = 0; i < TAM_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

// inserirHash():
// Associa uma pista a um suspeito na tabela hash
// Trata colisões com lista encadeada
void inserirHash(const char *pista, const char *suspeito) {
    int indice = funcaoHash(suspeito);
    
    // Busca se o suspeito já existe na lista encadeada
    Suspeito *atual = tabelaHash[indice];
    while(atual != NULL) {
        if(strcmp(atual->nome, suspeito) == 0) {
            // Suspeito já existe, adiciona pista à lista
            ItemPista *novaPista = (ItemPista*)malloc(sizeof(ItemPista));
            strcpy(novaPista->pista, pista);
            novaPista->proximo = atual->pistas;
            atual->pistas = novaPista;
            atual->contador++;
            return;
        }
        atual = atual->proximo;
    }
    
    // Suspeito não existe, cria novo
    Suspeito *novo = (Suspeito*)malloc(sizeof(Suspeito));
    strcpy(novo->nome, suspeito);
    novo->contador = 1;
    
    // Cria primeira pista
    novo->pistas = (ItemPista*)malloc(sizeof(ItemPista));
    strcpy(novo->pistas->pista, pista);
    novo->pistas->proximo = NULL;
    
    // Insere no início da lista (tratamento de colisão)
    novo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

// buscarSuspeito():
// Busca e exibe informações de um suspeito específico
void buscarSuspeito(const char *nome) {
    int indice = funcaoHash(nome);
    Suspeito *atual = tabelaHash[indice];
    
    while(atual != NULL) {
        if(strcmp(atual->nome, nome) == 0) {
            printf("\nSuspeito encontrado: %s\n", atual->nome);
            printf("Numero de pistas relacionadas: %d\n", atual->contador);
            printf("Pistas:\n");
            
            ItemPista *pista = atual->pistas;
            while(pista != NULL) {
                printf("  - %s\n", pista->pista);
                pista = pista->proximo;
            }
            printf("\n");
            return;
        }
        atual = atual->proximo;
    }
    
    printf("\nSuspeito '%s' nao encontrado no sistema.\n\n", nome);
}

// listarAssociacoes():
// Lista todos os suspeitos e suas pistas
void listarAssociacoes() {
    printf("===========================================\n");
    bool encontrouAlgum = false;
    
    for(int i = 0; i < TAM_HASH; i++) {
        Suspeito *atual = tabelaHash[i];
        while(atual != NULL) {
            encontrouAlgum = true;
            printf("\nSuspeito: %s (%d pista(s))\n", atual->nome, atual->contador);
            
            ItemPista *pista = atual->pistas;
            while(pista != NULL) {
                printf("  - %s\n", pista->pista);
                pista = pista->proximo;
            }
            
            atual = atual->proximo;
        }
    }
    
    if(!encontrouAlgum) {
        printf("Nenhum suspeito cadastrado ainda.\n");
    }
    printf("===========================================\n\n");
}

// encontrarSuspeitoMaisProvavel():
// Determina o suspeito com mais pistas relacionadas
void encontrarSuspeitoMaisProvavel() {
    char nomeMaisProvavel[50] = "";
    int maxPistas = 0;
    
    for(int i = 0; i < TAM_HASH; i++) {
        Suspeito *atual = tabelaHash[i];
        while(atual != NULL) {
            if(atual->contador > maxPistas) {
                maxPistas = atual->contador;
                strcpy(nomeMaisProvavel, atual->nome);
            }
            atual = atual->proximo;
        }
    }
    
    if(maxPistas > 0) {
        printf("===========================================\n");
        printf("    SUSPEITO MAIS PROVAVEL\n");
        printf("===========================================\n");
        printf("Nome: %s\n", nomeMaisProvavel);
        printf("Total de pistas relacionadas: %d\n", maxPistas);
        printf("===========================================\n\n");
    } else {
        printf("Nenhum suspeito cadastrado para analise.\n\n");
    }
}

// liberarHash():
// Libera toda memória da tabela hash
void liberarHash() {
    for(int i = 0; i < TAM_HASH; i++) {
        Suspeito *atual = tabelaHash[i];
        while(atual != NULL) {
            Suspeito *temp = atual;
            
            // Libera lista de pistas
            ItemPista *pista = atual->pistas;
            while(pista != NULL) {
                ItemPista *tempPista = pista;
                pista = pista->proximo;
                free(tempPista);
            }
            
            atual = atual->proximo;
            free(temp);
        }
    }
}

// ============================================
// FUNÇÕES UTILITÁRIAS
// ============================================

void exibirMenu() {
    printf("-------------------------------------------\n");
    printf("E - Esquerda | D - Direita | S - Sair\n");
    printf("-------------------------------------------\n");
}

void exibirMenuPrincipal() {
    printf("===========================================\n");
    printf("MENU PRINCIPAL - DETECTIVE QUEST\n");
    printf("===========================================\n");
    printf("1 - Explorar mansao (arvore binaria)\n");
    printf("2 - Listar pistas coletadas (BST)\n");
    printf("3 - Buscar suspeito especifico (Hash)\n");
    printf("4 - Listar todos os suspeitos (Hash)\n");
    printf("5 - Encontrar suspeito mais provavel\n");
    printf("0 - Sair\n");
    printf("===========================================\n");
}

void limparTela() {
    printf("\n\n");
}
