#include <stdio.h>
#include <iostream>
#include <vector>
#include <set>
#include <string.h>

using namespace std;
// Qual a ideia principal do algoritmo?

// Usar UnionFind para separar os pixels em pontos "pais"

// Com isso, é fácil identificar agrupamentos de pixels 1 e de pixel 0


// Código da classe UnionFind inspirado no código https://www.geeksforgeeks.org/dsa/introduction-to-disjoint-set-data-structure-or-union-find-algorithm/

class UnionFind {
    std::vector<int> pais;
public:

    UnionFind(int size) {

        pais.resize(size);

        for (int i = 0; i < size; i++) {
            pais[i] = i;
        }
    }

    // Função criada para pegar o vetor "pais"
    std::vector<int> get(){

        return pais;
    }

    int find(int i) {
      
        if (pais[i] == i) {
            return i;
        }
        
        // Função recursiva para achar o pai do outro nó até achar o verdadeiro pai
        return find(pais[i]);
    }


    void unite(int i, int j) {
      
        int i_pai = find(i);
        int j_pai = find(j);

        pais[i_pai] = j_pai;
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

// Função que dada uma matriz e a posição do pixel, descobre o valor do pixel do lado esquerdo
int vizinho_esquerda(const std::vector<std::vector<int>>& matriz, int posicao_x_pixel, int posicao_y_pixel){

    int pixel_lado_esquerdo = matriz[posicao_x_pixel][posicao_y_pixel - 1];
    return pixel_lado_esquerdo;
}

// Função que dada uma matriz e a posição do pixel, descobre o valor do pixel de cima
int vizinho_acima(const std::vector<std::vector<int>>& matriz, int posicao_x_pixel, int posicao_y_pixel){

    int pixel_acima = matriz[posicao_x_pixel-1][posicao_y_pixel];
    return pixel_acima;
}

// Função que dada uma matriz e a posição do pixel, descobre o valor do pixel da diagonal superior esquerdo
int vizinho_diagona_esquerda(const std::vector<std::vector<int>>& matriz, int posicao_x_pixel, int posicao_y_pixel){

    int pixel_diagonal_esquerda = matriz[posicao_x_pixel-1][posicao_y_pixel-1];
    return pixel_diagonal_esquerda;
}

// Criar função que descobre se o pixel é vizinho 8

int main () {

    FILE * in;

    char endereco[1000];

    printf("Digite o endereco da imagems: ");
    cin >> endereco;
    //scanf(" %[^\n]", endereco);

    // Abre a imagem
    in = fopen(endereco, "rt");

    if(in == NULL){
        printf("Erro ao abrir a imagem. Tente novamente");
        return 1;
    }

    // Valor padrão da linha do arquivo
    char line[1024];
    char tipo[3];

    // Lê o header (P1)
    fscanf(in, "%s", tipo);

    // Lê a largura e altura
    int coluna, linha;
    fscanf(in, "%d %d", &coluna, &linha);
    //cout << coluna << " x " << coluna << endl;
    //printf("%d %d \n", coluna, linha);


    // Matriz da imagem como um vetor de vetores
    std::vector<std::vector<int>> imagem(linha, std::vector<int>(coluna));

    // Matriz como vetor
    std::vector<int> imagem_array;
    imagem_array.resize(linha*coluna);


    // Função que passa por todos os pixeis e coloca em uma matriz
    // Como usaremos union find precisa usar array onde cada posição diz qual o pai daquele pixel
    for(int i = 0; i < linha; i ++){
        for(int j = 0; j < coluna; j++){

            char pixel_lido;
            int pixel_lido_int;
            fscanf(in, " %c", &pixel_lido);
            pixel_lido_int = pixel_lido - '0';
            imagem[i][j] = pixel_lido_int;
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


            // Se o pixel estiver na ponta superior a esquerda da imagem, não faz nada
            if (i == 0 && j == 0){
                continue;
            }

            // Se estiver na extremidade de cima da imagem, pega só o pixel do lado
            if (i == 0){

                int posicao_pixel_lado = (coluna*i)+ (j-1);
                int pixel_lado = vizinho_esquerda(imagem, i, j);

                if(pixel == pixel_lado){
                    unionFind.unite(posicao_pixel_atual, posicao_pixel_lado);
                }

                continue;

            }

            // Se o pixel estiver na extremidade esquerda da matriz, pega só o vizinho de cima
            if(j == 0){
                int posicao_pixel_acima = (coluna*(i-1)) + j;

                int pixel_acima = vizinho_acima(imagem, i, j);

                if(pixel == pixel_acima){
                    unionFind.unite(posicao_pixel_atual, posicao_pixel_acima);
                }

                continue;
            }

            // Aqui o pixel está no meio da imagem. Sem nenhum implicação
            int posicao_pixel_acima = (coluna*(i-1)) + j;
            int posicao_pixel_lado = (coluna*i)+ (j-1);
            int posicao_diagonal_esquerda = (coluna * (i - 1)) + (j - 1);

            int pixel_acima = vizinho_acima(imagem, i, j);
            int pixel_lado = vizinho_esquerda(imagem, i, j);
            int pixel_diagonal_esquerda = vizinho_diagona_esquerda(imagem,i,j);

            if(pixel == pixel_acima){
                    unionFind.unite(posicao_pixel_atual, posicao_pixel_acima);
                }

            if(pixel == pixel_lado){
                    unionFind.unite(posicao_pixel_atual, posicao_pixel_lado);
                }

            if (pixel == pixel_diagonal_esquerda) {
                    unionFind.unite(posicao_pixel_atual, posicao_diagonal_esquerda);
                }

            // Caso dê algum problema, pode ser por causa da verificação da diagonal, em algum momento o valor de 2 diagonais podem se sobrepor e acabar interferindo no cálculo da raiz do fundo
        }
    }

    set<int> ids_fundo_externo;

    // Esse primeiro for aninhado serve basicamente para identificar o fundo (os zeros que circulam os objetos)
    // Verifica as bordas da matriz e busca o pai do 0 atual, caso seja o mesmo que o anterior, esse zero pertence ao fundo
    // Caso o 0 não tenha o mesmo pai em comum com os demais, ele não pertence ao fundo, portanto é um buraco
    // O Union Find que é o responsável por fazer essa separação dos pais
    for(int i = 0; i < linha; i++){
        for(int j = 0; j < coluna; j++){
            if(i == 0 || i == linha-1 || j == 0 || j == coluna-1){ // Borda
                if(imagem[i][j] == 0){
                    int raiz = unionFind.find((coluna * i) + j);
                    ids_fundo_externo.insert(raiz);
                }
            }
        }
    }

    set<int> objetos_com_buraco;
    set<int> todos_objetos;

    // Esse outro for aninhado verifica as vizinhanças entre 1 e 0 internos
    // Basicamente ele faz a checagem assim: 
    // Estou em um objeto (pixel = 1). Ao lado tem um pixel 0. Esse zero é filho do pai do fundo?
    // Caso não, então é um buraco.
    for(int i = 0; i < linha; i++){
        for(int j = 0; j < coluna; j++){

            if(imagem[i][j] == 1){ // Aqui é caso ache parte de um objeto
                int idx_atual = (coluna * i) + j;
                int raiz_objeto = unionFind.find(idx_atual);
                todos_objetos.insert(raiz_objeto);

                // Verifica os vizinhos em busca de zeros que não são fundo
                int vizinhos_x[] = {i-1, i+1, i, i};
                int vizinhos_y[] = {j, j, j-1, j+1};

                for(int k = 0; k < 4; k++){
                    int ni = vizinhos_x[k];
                    int nj = vizinhos_y[k];

                    // se for válido 
                    if(ni >= 0 && ni < linha && nj >= 0 && nj < coluna){
                        if(imagem[ni][nj] == 0){ // e zero
                            int raiz_zero = unionFind.find((coluna * ni) + nj);

                            // e não for um fundo exerto, então é um buraco 
                            if(ids_fundo_externo.find(raiz_zero) == ids_fundo_externo.end()){
                                objetos_com_buraco.insert(raiz_objeto);
                            }
                        }
                    }
                }
            }
        }
    }

    int total_objetos = todos_objetos.size();
    int qtd_com_buraco = objetos_com_buraco.size();
    int qtd_sem_buraco = total_objetos - qtd_com_buraco;

    // Pega os valores do UnionFind
    std::vector<int> parentes = unionFind.get();

    int grupos_pixels_0 = 0;
    int grupos_pixels_1 = 0;

    // // Conta os parentes
    // for(int i = 0; i < coluna*linha; i++){

    //     if(unionFind.find(i) == i){

    //         if(imagem_array[i] == 0){
    //             grupos_pixels_0++;
    //         }

    //         if(imagem_array[i] == 1){
    //             grupos_pixels_1++;
    //         }

    //     }
    // }

    cout << "Numero de objetos: " << total_objetos << endl;
    cout << "Objetos com buracos: " << qtd_com_buraco << endl;
    cout << "Objetos sem buracos: " << qtd_sem_buraco << endl;

    // printf("Numero de objetos: %d \n", grupos_pixels_1);
    // printf("Numero de buracos: %d", grupos_pixels_0 - 1);
};