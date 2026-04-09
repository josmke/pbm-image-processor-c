#include <stdio.h>
#include <stdlib.h> // Para exit()
#include <string.h> // Para strcmp()
#include <stdbool.h> // Para o tipo 'bool' usado na sua função
#include <ctype.h> // Para isspace() na leitura do PBM

// --- Constantes Baseadas no Enunciado ---

// Limite máximo de tamanho da imagem [cite: 166]
#define MAX_ALTURA 768
#define MAX_LARGURA 1024

// --- Variável Global para a Imagem ---

// Usamos um array global para facilitar a passagem entre as funções
// de leitura e a sua função de codificação.
// A sua função já espera [][1024], então definimos a largura aqui
int imagem[MAX_ALTURA][MAX_LARGURA];

// --- Protótipos das Funções ---

// Sua função de codificação (mantida 100% fiel, com correção do nome da chamada)
void decodificar_imagem(int imagem_analise[][MAX_LARGURA], int linha_inicial, int coluna_inicial, int altura, int largura);

// Funções auxiliares para implementar os requisitos do projeto
void exibir_ajuda(char* nome_programa);
void processar_manual(int *altura_ptr, int *largura_ptr);
void processar_arquivo(char* nome_arquivo, int *altura_ptr, int *largura_ptr);


// --- Função Principal (main) ---
// Responsável por processar os argumentos da linha de comando
int main(int argc, char *argv[]) {
    int altura = 0, largura = 0;

    // Se nenhum argumento for fornecido, exibe a ajuda [cite: 126]
    if (argc == 1) {
        exibir_ajuda(argv[0]);
        return 0;
    }

    // Processa o argumento fornecido
    if (strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "--help") == 0) {
        // Argumento de ajuda [cite: 133]
        exibir_ajuda(argv[0]);
    } else if (strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "--manual") == 0) {
        // Argumento de entrada manual [cite: 134, 145]
        processar_manual(&altura, &largura);
        // Chama sua função para codificar a imagem lida
        decodificar_imagem(imagem, 0, 0, altura, largura);
        printf("\n"); // Adiciona uma nova linha no final
    } else if (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--file") == 0) {
        // Argumento de entrada por arquivo [cite: 135]
        if (argc < 3) {
            // Verifica se o nome do arquivo foi fornecido [cite: 138]
            printf("Erro: Nome do arquivo nao especificado apos %s.\n\n", argv[1]);
            exibir_ajuda(argv[0]);
            return 1; // Retorna erro
        }
        processar_arquivo(argv[2], &altura, &largura);
        // Chama sua função para codificar a imagem lida
        decodificar_imagem(imagem, 0, 0, altura, largura);
        printf("\n"); // Adiciona uma nova linha no final
    } else {
        // Argumento desconhecido
        printf("Erro: Argumento invalido '%s'.\n\n", argv[1]);
        exibir_ajuda(argv[0]);
        return 1; // Retorna erro
    }

    return 0;
}

// --- Implementação das Funções Auxiliares ---

/**
 * @brief Exibe as informações de uso do programa (help).
 *
 * @param nome_programa O nome do executável (argv[0]).
 */
void exibir_ajuda(char* nome_programa) {
    // Texto baseado na Figura 10 [cite: 128, 129, 130, 131, 132, 133, 134, 135]
    printf("Uso: %s [-? | -m | -f ARQ]\n", nome_programa);
    printf("Codifica imagens binarias dadas em arquivos PBM ou por dados informados\n");
    printf("manualmente.\n");
    printf("Argumentos:\n");
    printf(" -?, --help : apresenta essa orientacao na tela.\n");
    printf(" -m, --manual: ativa o modo de entrada manual, em que o usuario fornece\n");
    printf("               todos os dados da imagem informando-os atraves do teclado.\n");
    printf(" -f, --file : considera a imagem representada no arquivo PBM (Portable\n");
    printf("               bitmap).\n");
}

/**
 * @brief Processa a entrada de dados manual do usuário.
 *
 * @param altura_ptr Ponteiro para armazenar a altura da imagem.
 * @param largura_ptr Ponteiro para armazenar a largura da imagem.
 */
void processar_manual(int *altura_ptr, int *largura_ptr) {
    // Solicita as dimensões [cite: 146]
    printf("Modo de entrada manual ativado.\n");
    printf("Informe a largura (max %d): ", MAX_LARGURA);
    scanf("%d", largura_ptr);
    printf("Informe a altura (max %d): ", MAX_ALTURA);
    scanf("%d", altura_ptr);

    // Validação das dimensões
    if (*largura_ptr <= 0 || *largura_ptr > MAX_LARGURA || *altura_ptr <= 0 || *altura_ptr > MAX_ALTURA) {
        printf("Erro: Dimensoes invalidas. Limites sao %dx%d.\n", MAX_LARGURA, MAX_ALTURA); // [cite: 166]
        exit(1); // Encerra o programa com erro
    }

    // Solicita os pixels [cite: 146]
    // O enunciado diz 0 (branco) ou 1 (preto) [cite: 100]
    printf("Informe os %d pixels (0 para branco, 1 para preto):\n", (*altura_ptr) * (*largura_ptr));
    for (int i = 0; i < *altura_ptr; i++) {
        for (int j = 0; j < *largura_ptr; j++) {
            scanf("%d", &imagem[i][j]);
        }
    }
    printf("Leitura manual concluida. Codigo gerado:\n");
}

/**
 * @brief Processa a entrada de dados via arquivo PBM.
 *
 * @param nome_arquivo O nome do arquivo a ser lido.
 * @param altura_ptr Ponteiro para armazenar a altura da imagem.
 * @param largura_ptr Ponteiro para armazenar a largura da imagem.
 */
void processar_arquivo(char* nome_arquivo, int *altura_ptr, int *largura_ptr) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo '%s'.\n", nome_arquivo);
        exit(1);
    }

    char buffer[256];
    int c;

    // 1. Ler o "magic number" P1 [cite: 98]
    fscanf(arquivo, "%s", buffer);
    if (strcmp(buffer, "P1") != 0) {
        printf("Erro: O arquivo '%s' nao e um formato PBM (P1).\n", nome_arquivo);
        fclose(arquivo);
        exit(1);
    }

    // 2. Ler dimensões, pulando comentários [cite: 103]
    do {
        // Pula espaços em branco e novas linhas
        c = fgetc(arquivo);
        while (isspace(c)) {
            c = fgetc(arquivo);
        }

        // Se for um comentário ('#'), pula a linha inteira [cite: 103]
        if (c == '#') {
            while (c != '\n' && c != EOF) {
                c = fgetc(arquivo);
            }
        }
    } while (c == '#' || isspace(c));

    // Devolve o caractere que não era espaço/comentário para o buffer
    ungetc(c, arquivo);

    // Lê as dimensões (largura e altura) [cite: 99]
    if (fscanf(arquivo, "%d %d", largura_ptr, altura_ptr) != 2) {
        printf("Erro: Nao foi possivel ler as dimensoes do arquivo PBM.\n");
        fclose(arquivo);
        exit(1);
    }

    // Validação das dimensões
    if (*largura_ptr <= 0 || *largura_ptr > MAX_LARGURA || *altura_ptr <= 0 || *altura_ptr > MAX_ALTURA) {
        printf("Erro: Dimensoes invalidas (%dx%d) no arquivo. Limites sao %dx%d.\n", *largura_ptr, *altura_ptr, MAX_LARGURA, MAX_ALTURA); // [cite: 166]
        fclose(arquivo);
        exit(1);
    }

    // 3. Ler os dados dos pixels (0 para branco, 1 para preto) [cite: 100]
    for (int i = 0; i < *altura_ptr; i++) {
        for (int j = 0; j < *largura_ptr; j++) {
            if (fscanf(arquivo, "%d", &imagem[i][j]) != 1) {
                printf("Erro: Falha ao ler dados de pixel no arquivo (posicao %d, %d).\n", i, j);
                fclose(arquivo);
                exit(1);
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' (%dx%d) lido com sucesso. Codigo gerado:\n", nome_arquivo, *largura_ptr, *altura_ptr);
}


// --- SUA FUNÇÃO DE CODIFICAÇÃO ---
// (Lógica e comentários 100% preservados, conforme solicitado)
//  para permitir que a recursão funcione corretamente)

void decodificar_imagem(int imagem_analise[][MAX_LARGURA], int linha_inicial, int coluna_inicial, int altura, int largura){
    //Caso Base
    //Guarda o primeiro pixel da imagem ou quadrante analisado
    int pixel_referencia = imagem_analise[linha_inicial][coluna_inicial];
    //Variavel para guardar se é uma sequência uniforme
    bool ehuniforme = true;
    //Percorrer a matriz(imagem original)
    //Percorrer as linhas da matriz 
    for (int i = linha_inicial; i < linha_inicial + altura; i++){
        //Percorrer as colunas da matriz
        for (int j = coluna_inicial; j < coluna_inicial + largura; j++){
            //Se a sequência não for uniforme
            if (pixel_referencia != imagem_analise[i][j]){
                ehuniforme = false;
                break; //Para o loop j ao encontrar um pixel diferente
            }
        }
        //Se encontrou um pixel diferente
        if (ehuniforme == false){
            break; //Para o loop i ao encontrar um pixel diferente
        }
    }
    //Se a sequência não for uniforme
    if (ehuniforme == false){
        //Imprime 'X' sequência mista
        printf("X");
        //Dividir a matriz em 4 quadrantes (matrizes menores) utilizando  recursão
        //declarar as alturas e larguras
        //altura para Q1 e Q2, superiores
        int a1 = (altura + 1) / 2; //Se altura for impar, deixa a maior parte para cima 
        //altura para Q3 e Q4, inferiores
        int a2 = altura / 2; //Pega o inteiro que sobra de a1
        //largura para Q1 e Q3, esquerdos
        int l1 = (largura + 1) / 2; //Se largura for impar, deixa a maior parte para esquerda 
        //largura para Q2 e Q4, direitos
        int l2 = largura / 2;

        //chamadas recursivas para todos os quadrantes

        //Verificar se as variaveis de altura e largura são maiores do que 0, para cada quadrante 

        if (a1 > 0 && l1 > 0){ //se a altura superior e a largura esquerda forem maiores do que 0
            //Quadrante 1 (superior esquerdo), começa da linha e coluna inicial, até a1 e l1 (altura superior e largura esquerda)
            decodificar_imagem(imagem_analise, linha_inicial, coluna_inicial, a1, l1); // 

        if (a1 > 0 && l2 > 0){ //se a altura superior e a largura direita forem maiores do que 0
            //Quadrante 2 (superior direito), começa da linha inicial e coluna inicial + l1 (largura da esquerda), até a1 e l2 (altura superior e largura direita)
            decodificar_imagem(imagem_analise, linha_inicial, coluna_inicial + l1, a1, l2); // 
        }
        if (a2 > 0 && l1 > 0){ //se a altura inferior e a largura esquerda forem maiores do que 0
            //Quadrante 3 (inferior esquerdo), começa da linha inicial + a1 (altura superior) e coluna inicial, até a2 e l1 (altura inferior e largura esquerda)
            decodificar_imagem(imagem_analise,linha_inicial + a1, coluna_inicial, a2, l1); // 
        }
        if (a2 > 0 && l2 > 0){ //se a altura inferior e a largura direita forem maiores do que 0
            //Quadrante 4 (inferior direito), começa da linha inicial + a1 e coluna inicial + l1, até a2 e l2 (altura inferior e largura direita)
            decodificar_imagem(imagem_analise, linha_inicial + a1, coluna_inicial + l1, a2, l2); 
        }
    }
    //Se a sequência for uniforme
    else{
        if (pixel_referencia == 0){ //Se for somente 0 (branco, conforme PBM 
            printf("B"); //Imprime branco 
        }
        else{ //Se for somente 1 (preto, conforme PBM 
            printf("P"); //Imprime preto 
        }
    }
}