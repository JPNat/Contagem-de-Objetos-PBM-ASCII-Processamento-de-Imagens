#include <stdio.h>
#include <iostream>
#include <vector>


// Qual a ideia principal do algoritmo?

// Ser recursivo

// Quando examinar uma parte, tirar esses pixels do problema


// Código pego em https://www.geeksforgeeks.org/dsa/introduction-to-disjoint-set-data-structure-or-union-find-algorithm/

class UnionFind {
    std::vector<int> parent;
public:
    UnionFind(int size) {
      
        parent.resize(size);
      
        // Initialize the parent array with each 
        // element as its own representative
        for (int i = 0; i < size; i++) {
            parent[i] = i;
        }
    }

    std::vector<int> get(){

        return parent;
    }

    // Find the representative (root) of the
    // set that includes element i
    int find(int i) {
      
        // If i itself is root or representative
        if (parent[i] == i) {
            return i;
        }
      
        // Else recursively find the representative 
        // of the parent
        return find(parent[i]);
    }

    // Unite (merge) the set that includes element 
    // i and the set that includes element j
    void unite(int i, int j) {
      
        // Representative of set containing i
        int irep = find(i);
      
        // Representative of set containing j
        int jrep = find(j);
       
        // Make the representative of i's set
        // be the representative of j's set
        parent[irep] = jrep;
    }
};

// ------------------------------------------------------------------------------------------------------

// Criar função que descobre se o pixel é vizinho de 4 pontos pretos "1"
int vizinhos_quatro_eh_1(const std::vector<std::vector<int>>& matriz, int posicao_x_pixel, int posicao_y_pixel){

    int pixel_acima = matriz[posicao_x_pixel-1][posicao_y_pixel];
    int pixel_embaixo = matriz[posicao_x_pixel+1][posicao_y_pixel];
    int pixel_lado_esquerdo = matriz[posicao_x_pixel][posicao_y_pixel - 1];
    int pixel_lado_direito = matriz[posicao_x_pixel][posicao_y_pixel+1];

    
    if( pixel_acima == 1 && 
        pixel_embaixo == 1 &&
        pixel_lado_direito == 1 && 
        pixel_lado_esquerdo == 1)
        
        {
            return 1;
        }
    return 0;
    
}

int vizinho_esquerda(const std::vector<std::vector<int>>& matriz, int posicao_x_pixel, int posicao_y_pixel){

    int pixel_lado_esquerdo = matriz[posicao_x_pixel][posicao_y_pixel - 1];
    return pixel_lado_esquerdo;
}

int vizinho_acima(const std::vector<std::vector<int>>& matriz, int posicao_x_pixel, int posicao_y_pixel){

    int pixel_acima = matriz[posicao_x_pixel-1][posicao_y_pixel];
    return pixel_acima;
}

// Criar função que descobre se o pixel é vizinho 8

int main () {

    printf("Programa iniciado\n");

    FILE * in;

    char endereco[1000];

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


    // Matriz da imagem como um vetor de vetores
    std::vector<std::vector<int>> imagem(linha, std::vector<int>(coluna));

    // Matriz como vetor
    std::vector<int> imagem_array;
    imagem_array.resize(linha*coluna);


    // Função que passa por todos os pixeis e coloca em uma matriz
    // Como usaremos union find precisa usar array
    for(int i = 0; i < linha; i ++){
        for(int j = 0; j < coluna; j++){

            int pixel_lido;
            fscanf(in, "%d", &pixel_lido);
            imagem[i][j] = pixel_lido;
            imagem_array[(coluna * i) + j] = imagem[i][j];
        }
    }

    //Inicializando o Union Find
    UnionFind unionFind(imagem_array.size());


    //Agora iremos juntar tudo com Union Find
    for(int i = 0; i < linha; i ++){
        for(int j = 0; j < coluna; j++){


            int pixel = imagem_array[(coluna * i) + j];

            int posicao_pixel_atual = (coluna*i) + j;


            // Se o pixel estiver na ponta, não faz nada
            if (i == 0 && j == 0){
                continue;
            }

            // Se estiver na extremidade de Y, pega só o pixel do lado
            if (i == 0){

                int posicao_pixel_lado = (coluna*i)+ (j-1);
                int pixel_lado = vizinho_esquerda(imagem, i, j);

                if(pixel == pixel_lado){
                    unionFind.unite(posicao_pixel_lado, posicao_pixel_atual);
                }

                continue;
                
            }

            // Se tiver no topo da matriz, pega só o vizinho de cima
            if(j == 0){
                int posicao_pixel_acima = (coluna*(i-1)) + j;

                int pixel_acima = vizinho_acima(imagem, i, j);

                if(pixel == pixel_acima){
                    unionFind.unite(posicao_pixel_acima, posicao_pixel_atual);
                }
                continue;
            }

            // Aqui o pixel está no meio da imagem. Sem nenhum implicação
            int posicao_pixel_acima = (coluna*(i-1)) + j;
            int posicao_pixel_lado = (coluna*i)+ (j-1);

            int pixel_acima = vizinho_acima(imagem, i, j);
            int pixel_lado = vizinho_esquerda(imagem, i, j);
            
            if(pixel == pixel_acima){
                    unionFind.unite(posicao_pixel_acima, posicao_pixel_atual);
                }

            if(pixel == pixel_lado){
                    unionFind.unite(posicao_pixel_lado, posicao_pixel_atual);
                }
        }
    }

    std::vector<int> parentes = unionFind.get();

    int grupos_pixels_0 = 0;
    int grupos_pixels_1 = 0;
    int buracos = 0;

    // Conta os parentes
    for(int i = 0; i < coluna*linha; i++){

        if(unionFind.find(i) == i){

            if(imagem_array[i] == 0){

                grupos_pixels_0++;
            }
            
            if(imagem_array[i] == 1 || imagem_array[i] == 255){
                grupos_pixels_1++;
            }
        
        }
    }

    printf("Número de objetos: %d", grupos_pixels_0);
};