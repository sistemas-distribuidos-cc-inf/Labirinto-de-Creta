#include <jni.h>
#include <stdio.h>
#include "ServerImplements.h"

/**
 * Universidade Federal de Goiás(UFG)
 * Instituto de Informática(INF)
 * Curso: Ciência da Computação
 *
 * Atividade da disciplina de IA
 * Prof.
 *
 * Matricula    Nome
 * 201300779    Warley Gonçalves dos Santos
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAMANHO        20   /**< Quantidade de movimentos pre processados.    */
#define POPULACAOsize 400   /**< Quantidade de cromossomas de uma populacao.  */
#define GERACOES      200   /**< Quantidade de vezes que a reproducao ocorre. */
	#define THRESHOLDcross 10  /**< Ponto de corte para selecao de individuos.   */
#define SHIFTcross    200   /**< Escolhe os casais. Metade de POPULACAOsize.  */
#define TXMutacao      40   /**< Taxa de mutacao ao gerar filhos.             */

#define X 21    /**< Tamanho do mapa em relacao ao eixo X.                    */
#define Y 11    /**< Tamanho do mapa em relacao ao eixo Y.                    */

#define CIMA     119    /**< Para cima (w).                                   */
#define BAIXO    115    /**< Para baixo (s).                                  */
#define ESQUERDA  97    /**< Para esquerda (a).                               */
#define DIREITA  100    /**< Para direita (d).                                */
#define PARADO     0    /**< Fica no mesmo lugar.                             */

#define PAREDE 35   /**< ID para PAREDE (#).                                  */
#define VAZIO  32   /**< ID para espaço VAZIO (ESPACE).                       */

#define MODULO(x) (x >= 0?  x: -(x)) /**< Macro pra calcular modulo.          */

/**
 * Abstração do mapa. Matriz de caracteres de tamanho X, Y.
 */
typedef char Labirinto[Y][X];

    static Labirinto mapa =
    {
        "####################",
        "#    #        #    #",
        "# ## # ###### # ## #",
        "# #              # #",
        "# # ## ##  ## ## # #",
        "#      #    #      #",
        "# # ## ###### ## # #",
        "# #              # #",
        "# ## # ###### # ## #",
        "#    #        #    #",
        "####################"
    };

/**
 * Abstração de uma cromossoma, que simboliza a sequencias de jogadas
 * predefinidas para um determinado jogador. Vetor de caracteres de tamanho
 * TAMANHO.
 */
typedef char Cromossoma[TAMANHO];

/**
 * Estrutura que armazena a possicao de um jogador no mapa.
 */
typedef struct Posicao
{
    int x;  /**< Posicao em relacao ao eixo x. */
    int y;  /**< Posicao em relacao ao eixo y. */
} Posicao;

/**
 * Abstracao para um jogador, pode ser o jogador01 ou um minotauro.
 */
typedef struct Jogador
{
    char id;               /**< Caracter que simboliza o jogador no mapa.     */
    Posicao posicao;       /**< Posicao onde o jogador se encontra.           */
    Cromossoma cromossoma; /**< Proximos passos a ser executado pelo jogador. */
} Jogador;

/******************************************************************************/
int distanceManhattan(Posicao, Posicao);
int simulaDesloca(Jogador, Posicao);
void desloca(Jogador *, int);
void geraIndividuo(Cromossoma *);
Cromossoma *geraPopulacao(int );
void melhorIndividuo(Jogador *, Posicao);
void swap(int *, int *);
Cromossoma *crossover(Cromossoma *);
void mutacao(Cromossoma *);
void selectionSort(Cromossoma **, int *, int);
/******************************************************************************/

/**
 * Funcao que calcula a distancia entre dois pontos usando o algoritimo da
 * distancia de Manhattan (Distancia do Taxi).
 * @param p1 Primeiro ponto para calcular a distancia.
 * @param p2 Segundo ponto para calcular a distancia.
 * @return Disntancia calculada entre os dois pontos.
 */
int distanceManhattan(Posicao p1, Posicao p2)
{
    return (MODULO(p1.x - p2.x) + MODULO(p1.y - p2.y));
}

/**
 * Simula deslocamento.
 * Funcao destinada a calcular a eficacia de um Individuo.
 * Calcula qual sera a distancia final entre o minotauro e a posicao do jogador01
 * apos a execucao de todos os passos representados por uma cromossoma.
 * @param mapa Matriz de char que representa o mapa.
 * @param minotauro Estrutura que representa o minotauro.
 * @param posicao Posicao onde o PacMan se encontra no mapa.
 * @return Distancia entre o minotauro e o PacMan apos a execucao de todos os
 * movimentos ja pre processados.
 */
int simulaDesloca(Jogador minotauro, Posicao posicao)
{
    int i;

    Labirinto copiaLabirinto;
    Jogador copia = minotauro;
    memcpy(copiaLabirinto, mapa, sizeof(char) * (X * Y) );

    for(i = 0; i < TAMANHO; ++i)
    {
        desloca(&copia, i);
        if(copia.posicao.x == posicao.x && copia.posicao.y == posicao.y)
            return 0;
    }
    return distanceManhattan(copia.posicao, posicao);
}

/**
 * Essa função reposiciona o jogador no mapa.
 * O jogador pode ir para Cima, Baixa, Esquerda e Direita.
 * @param mapa Matriz de TAMANHO X, Y que representa o mapa e os jogadores em
 * suas respectivas posições.
 * @param jogador Representa o jogador que sera reposicionado.
 * @param indice Indice da cromossoma que sera usada para reposicionamento.
 */
void desloca(Jogador *jogador, int indice)
{
    switch (jogador->cromossoma[indice])
    {
    case CIMA:
        if(mapa[(jogador->posicao.y) - 1][jogador->posicao.x] != PAREDE)
        {
            mapa[jogador->posicao.y][jogador->posicao.x] = VAZIO;
            mapa[jogador->posicao.y - 1][jogador->posicao.x] = jogador->id;
            --jogador->posicao.y;
        }
        break;
    case BAIXO:
        if(mapa[jogador->posicao.y + 1][jogador->posicao.x] != PAREDE)
        {
            mapa[jogador->posicao.y][jogador->posicao.x] = VAZIO;
            mapa[jogador->posicao.y + 1][jogador->posicao.x] = jogador->id;
            ++jogador->posicao.y;
        }
        break;
    case ESQUERDA:
        if(mapa[jogador->posicao.y][jogador->posicao.x - 1] != PAREDE)
        {
            mapa[jogador->posicao.y][jogador->posicao.x] = VAZIO;
            mapa[jogador->posicao.y][jogador->posicao.x - 1] = jogador->id;
            --jogador->posicao.x;
        }
        break;
    case DIREITA:
        if(mapa[jogador->posicao.y][jogador->posicao.x + 1] != PAREDE)
        {
            mapa[jogador->posicao.y][jogador->posicao.x] = VAZIO;
            mapa[jogador->posicao.y][jogador->posicao.x + 1] = jogador->id;
            ++jogador->posicao.x;
        }
        break;
    default:
        mapa[jogador->posicao.y][jogador->posicao.x] = jogador->id;
    }
}

/**
 * Funcao que gera os valores para uma cromossoma de forma aleatoria.
 * @param cromossoma Proximos passos a ser executado por um jogador.
 */
void geraIndividuo(Cromossoma *cromossoma)
{
    int i;
    char posibilidades[] = {BAIXO, CIMA, ESQUERDA, DIREITA};

    for(i = 0; i < TAMANHO; ++i)
    {
        (*cromossoma)[i] = posibilidades[rand() % 4];
    }
}

/**
 * Cria uma populacao de forma aleatoria.
 * @param size Tamanho da populacao.
 * @return Ponteiro para a populacao gerada.
 */
Cromossoma *geraPopulacao(int size)
{
    int i;
    Cromossoma *populacao = (Cromossoma *) malloc(sizeof(Cromossoma) * size);

    if(!populacao)
    {
        perror("Memoria insuficiente!\n");
        exit(-1);
    }
    for(i = 0; i < size; ++i)
    {
        geraIndividuo(&(populacao[i]));
    }
    return populacao;
}

/**
 * Troca dois inteiros de variavel. Auxiliar de selectionSort.
 * @param a Ponteiro para variavel tipo inteiro A.
 * @param b Ponteiro para variaval tipo inteiro B.
 */
void swap(int *a, int *b)
{
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * Algoritimo de ordenacao por selecao para colocar os cromossomas de uma
 * populacao em ordem crescente.
 * @param populacao Ponteiro para ponteiro da populacao.
 * @param aptidao Vetor de inteiros com a aptidao gerada pela simulacao de
 * deslocamento para cada cromossoma, em sua respectiva posicao.
 * @param size Quantidade de cromossomas na populacao.
 */
void selectionSort(Cromossoma **populacao, int aptidao[], int size)
{
    int i,
        j,
        least;
    Cromossoma tmp;

    for (i = 0; i < size - 1; ++i)
    {
        for (j = i + 1, least = i; j < size; ++j)
            if (aptidao[j] < aptidao[least])
                least = j;
        if (i != least)
        {
            swap(&aptidao[least], &aptidao[i]);
            memcpy(&tmp,(*populacao)[least], sizeof(Cromossoma));
            memcpy((*populacao)[least], (*populacao)[i], sizeof(Cromossoma));
            memcpy((*populacao)[i], &tmp, sizeof(Cromossoma));
        }
    }
}

/**
 * Funcao que realiza a selecao e reproducao de individuos, ordenado do mais
 * apto ao menos apto.
 * Os pais mais aptos se reproduzem. Cada casal geral dois filhos.
 * @param pais Ponteiro para o vetor de Cromossomas dos pais.
 * @see THRESHOLDcross Ponto de corte para a quantidade de pais aptos.
 * @see SHIFTcross Deslocamento entre os vetores de pais e maes.
 */
Cromossoma *crossover(Cromossoma *pais)
{
    int i,
        j;
    Cromossoma *filhos;
    int THRESHOLDcrosds = (rand() % (TAMANHO /2)) + 1;

    filhos  = (Cromossoma *) malloc(sizeof(Cromossoma) * POPULACAOsize);
    for(i = 0; i < (SHIFTcross / 2) + 1; ++i)
    {
        for(j = 0; j < THRESHOLDcross; ++j)
        {
            filhos[i][j] = pais[i][j];
            filhos[i + SHIFTcross][j] = pais[i + (SHIFTcross / 2) ][j];
        }
        for(j = THRESHOLDcross; j < TAMANHO; ++j)
        {
            filhos[i][j] = pais[(SHIFTcross / 2) + i][j];
            filhos[i + SHIFTcross][j] = pais[i][j];
        }
    }
    for( ; i < SHIFTcross; ++i)
    {
        for(j = 0; j < THRESHOLDcross; ++j)
        {
            filhos[i][j] = pais[i][j];
            filhos[i + SHIFTcross][j] = pais[i - (SHIFTcross / 2) - 1][j];
        }
        for(j = THRESHOLDcross; j < TAMANHO; ++j)
        {
            filhos[i][j] = pais[i - (SHIFTcross / 2) - 1][j];
            filhos[i + SHIFTcross][j] = pais[i][j];
        }
    }
    return filhos;
}
/**
 * Funcao que realiza a mutacao de um gene de forma aleatoria.
 * A mutacao pode ocorrer ou nao de acordo com a taxa de mutação.
 * @param cromossoma Cromossoma que pode sofre a mutação.
 * @see TXMutacao Taxa de Mutacao
 */
void mutacao(Cromossoma *cromossoma)
{
    char possibilidades[] = {BAIXO, CIMA, ESQUERDA, DIREITA};
    char tmp;
    int gene;

    if((rand() % 100) < TXMutacao)
    {
        do
        {
            tmp = possibilidades[rand() % 4];
            gene = rand() % TAMANHO;

        }
        while(tmp == *(cromossoma)[gene]);
        (*cromossoma)[gene] = tmp;
    }
}

/**
 * Funcao responsavel pela IA.
 * @param minotauro Ponteiro para o minotauro que deseja traçar rota.
 * @param posicao Posicao de destino.
 * @param mapa Labirinto do jogo.
 */
void melhorIndividuo(Jogador *minotauro, Posicao posicao)
{
    int i,
        j,
        *aptidao,
        aptidaoMelhorPai;
    Jogador copiaFantasma;
    Cromossoma *tmpPopulacao,
               *populacao,
               melhorPai;

    /**
     * Gera populacao inicial e cria uma copia do Fantasma.
     */
    populacao = geraPopulacao(POPULACAOsize);
    memcpy(&copiaFantasma, minotauro, sizeof(Jogador));

    /**
     * Calcula a aptidao da populacao inicial, e ordena.
     */
    aptidao = (int*) malloc(sizeof(int) * POPULACAOsize);
    for(i = 0; i < POPULACAOsize; ++i)
    {
        memcpy(&(copiaFantasma.cromossoma), &(populacao[i]),
               sizeof(Cromossoma));
        aptidao[i] = simulaDesloca(copiaFantasma, posicao);
    }
    selectionSort(&populacao, aptidao, POPULACAOsize);

    for(i = 0; i < GERACOES; ++i)
    {
        /**
         * Mantem o historico de melhor pai.
         */
        aptidaoMelhorPai = aptidao[0];
        memcpy(&melhorPai, populacao[0], sizeof(Cromossoma));

        /**
         * Selecao e Reproducao.
         */
        tmpPopulacao = populacao;
        populacao = crossover(populacao);
        free(tmpPopulacao);

        /**
         * Mutacao.
         */
        for(j = 0; j < POPULACAOsize; ++j)
        {
            mutacao(&(populacao[j]));
        }

        /**
         * Calculo de aptidao.
         */
        for(j = 0; j < POPULACAOsize; ++j)
        {
            memcpy(&(copiaFantasma.cromossoma), &(populacao[j]),
                   sizeof(Cromossoma));
            aptidao[j] = simulaDesloca(copiaFantasma, posicao);
        }

        /**
         * Restaura o pai mais apto.
         */
        selectionSort(&populacao, aptidao, POPULACAOsize);
        aptidao[POPULACAOsize - 1] = aptidaoMelhorPai;
        memcpy(populacao[POPULACAOsize - 1], &melhorPai, sizeof(Cromossoma));

        /**
         * Ordena e registra log da nova populacao.
         */
        selectionSort(&populacao, aptidao, POPULACAOsize);
    }
    /**
     * Escolha do individuo.
     */
    memcpy(minotauro->cromossoma, populacao[0], sizeof(Cromossoma));

    /**
     * Libera memoria.
     */
    free(populacao);
    free(aptidao);
}

JNIEXPORT jstring JNICALL Java_ServerImplements_sayHello(JNIEnv *env, jobject thisObj, jstring inJNIStr) {
   // Step 1: Convert the JNI String (jstring) into C-String (char*)
   const char *inCStr = (*env)->GetStringUTFChars(env, inJNIStr, NULL);
 
    static Jogador minotauro = {'U', 10, 5, PARADO}; /**< Jogador PC.     */
    static Jogador jogador01 = {'0', 9, 9, PARADO};     /**< Jogador humano. */
    if (NULL == inCStr) return NULL;
        jogador01.cromossoma[0] = inCStr[0];
    (*env)->ReleaseStringUTFChars(env, inJNIStr, inCStr);  // release resources

    char outCStr [] =
    {
        "####################\n"
        "#    #        #    #\n"
        "# ## # ###### # ## #\n"
        "# #              # #\n"
        "# # ## ##  ## ## # #\n"
        "#      #    #      #\n"
        "# # ## ###### ## # #\n"
        "# #              # #\n"
        "# ## # ###### # ## #\n"
        "#    #        #    #\n"
        "####################"
    };
    
    desloca(&jogador01, 0);

    /**
     * Verifica se o deslocamento do jogador Humano ocasionou o fim de
     * jogo.
     */
    if(jogador01.posicao.x == minotauro.posicao.x &&
            jogador01.posicao.y == minotauro.posicao.y)
    {

    }
    melhorIndividuo(&minotauro, jogador01.posicao);
    desloca(&minotauro, 0);
    /**
     * Verifica se o deslocamento do jogador PC ocasionou o fim de jogo.
     */
    if(jogador01.posicao.x == minotauro.posicao.x &&
            jogador01.posicao.y == minotauro.posicao.y)
    {

    }
    outCStr[(jogador01.posicao.y * 21) + jogador01.posicao.x] = jogador01.id;
    outCStr[(minotauro.posicao.y * 21) + minotauro.posicao.x] = minotauro.id;
   // Step 3: Convert the C-string (char*) into JNI String (jstring) and return
   return (*env)->NewStringUTF(env, outCStr);
}
