#include <stdio.h>

// Criar função que descobre se o pixel é vizinho 4

// Criar função que descobre se o pixel é vizinho 8

int main () {

    FILE * in;

    char endereco[10000];

    printf("Digite o endereço da imagems: ");
    scanf(" %[^\n]", endereco);

    // Abre a imagem
    in = fopen(endereco, "rt");

    if(in == NULL){
        printf("Erro ao abrir a imagem. Tente novamente");
        return 1;
    }

    // Valor padrão da linha do arquivo
    char line[1024];
    char tipo[3];

    // Lê o header (P2)
    fscanf(in, "%s", tipo);
    printf("Tipo: %s\n", tipo);

    // Lê a largura e altura
    int coluna, linha;
    fscanf(in, "%d %d", &coluna, &linha);
    printf("%d %d \n", coluna, linha);


    // Lê o valor máximo da intensidade
    int intensidade;
    fscanf(in, "%d", &intensidade);
    printf("Intensidade: %d \n", intensidade);


    // Função que passa por todos os pixeis
    for(int i = 0; i < linha; i ++){
        for(int j = 0; j < coluna; j++){

            int pixel;
            fscanf(in, "%d", &pixel);

        }
    }

};