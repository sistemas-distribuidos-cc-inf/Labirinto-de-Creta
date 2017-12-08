/**
 * Universidade Federal de Goiás(UFG)
 * Instituto de Informática(INF)
 * Curso: Ciência da Computação
 *
 * Atividade da disciplina de IA
 * Prof.
 *
 * Matricula    Nome Celso
 * 201300779    Warley Gonçalves dos Santos
 */

#include <jni.h>
#include "ServerImplements.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAMANHO        15   /**< Quantidade de movimentos pre processados.    */
#define POPULACAOsize 100   /**< Quantidade de cromossomas de uma populacao.  */
#define GERACOES      50   /**< Quantidade de vezes que a reproducao ocorre. */
#define THRESHOLDcross 2  /**< Ponto de corte para selecao de individuos.   */
#define SHIFTcross    50   /**< Escolhe os casais. Metade de POPULACAOsize.  */
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
void geraIndividuo(Cromossoma *, Posicao);
Cromossoma *geraPopulacao(int, Posicao);
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


int contaPassos(Labirinto mapa, Jogador fantasma, Posicao posicao)
{
    int i;

    Labirinto copiaMapa;
    Jogador copia = fantasma;
    memcpy(copiaMapa, mapa, sizeof(char) * (X * Y) );

    for(i = 0; i < TAMANHO; ++i)
    {
        desloca(&copia, i);
        if(copia.posicao.x == posicao.x && copia.posicao.y == posicao.y)
            return i;
    }
    return i -1;
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
void geraIndividuo(Cromossoma *cromossoma, Posicao p)
{
    Posicao q = p;
    static char temp[11][21] =
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

    int i, newCroma, repet, lest = 0, evitaVoltar;
    char posibilidades[] = {BAIXO, CIMA, ESQUERDA, DIREITA};

    for(i = 0; i < TAMANHO; ++i)
    {
        evitaVoltar = 0;
        do
        {
            repet = 0;
            newCroma = posibilidades[rand() % 4];
            if(newCroma == BAIXO)
            {
                if (lest == CIMA)
                {
                    evitaVoltar = 1;
                }
                if(temp[p.y + 1][p.x] == '#')
                    repet = 1;
                else
                    ++p.y;
            }
            else if(newCroma == CIMA)
            {
                if (lest == BAIXO)
                {
                    evitaVoltar = 1;
                }
                if(temp[p.y - 1][p.x] == '#')
                    repet = 1;
                else
                    --p.y;
            }
            else if(newCroma == ESQUERDA)
            {
                if (lest == DIREITA)
                {
                    evitaVoltar = 1;
                }
                if(temp[p.y][p.x - 1] == '#')
                    repet = 1;
                else
                    --p.x;
            }
            else if(newCroma == DIREITA)
            {
                if (lest == ESQUERDA)
                {
                    evitaVoltar = 1;
                }
                if(temp[p.y][p.x + 1] == '#')
                    repet = 1;
                else
                    ++p.x;
            }
        }
        while(repet);

        if(evitaVoltar)
        {
            i = -1;
            p = q;
            continue;
        }
        lest = newCroma;
        (*cromossoma)[i] = newCroma;
    }
}

/**
 * Cria uma populacao de forma aleatoria.
 * @param size Tamanho da populacao.
 * @return Ponteiro para a populacao gerada.
 */
Cromossoma *geraPopulacao(int size, Posicao p)
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
        geraIndividuo(&(populacao[i]), p);
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

        int passosMelhorPai;
        int passosAtual;

    /**
     * Gera populacao inicial e cria uma copia do Fantasma.
     */
        populacao = geraPopulacao(POPULACAOsize, minotauro->posicao);
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
            memcpy(&(copiaFantasma.cromossoma), &(populacao[0]), sizeof(Cromossoma));
            passosMelhorPai = contaPassos(mapa, copiaFantasma, posicao);
            aptidaoMelhorPai = aptidao[0];
            memcpy(&melhorPai, populacao[0], sizeof(Cromossoma));

            for(j = 1; j < POPULACAOsize; ++j)
            {
                memcpy(&(copiaFantasma.cromossoma), &(populacao[j]),
                 sizeof(Cromossoma));

                passosAtual = contaPassos(mapa, copiaFantasma, posicao);
                if( passosAtual < passosMelhorPai)
                {
                    passosMelhorPai = passosAtual;
                    aptidaoMelhorPai = aptidao[j];
                    memcpy(&melhorPai, populacao[j], sizeof(Cromossoma));
                }
            }

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
        for(j = 0; j < POPULACAOsize; ++j)
        {
            memcpy(&(copiaFantasma.cromossoma), &(populacao[j]),
             sizeof(Cromossoma));

            passosAtual = contaPassos(mapa, copiaFantasma, posicao);
            if( passosAtual < passosMelhorPai)
            {
                passosMelhorPai = passosAtual;
                aptidaoMelhorPai = aptidao[j];
                memcpy(&melhorPai, populacao[j], sizeof(Cromossoma));
            }
        }
        memcpy(minotauro->cromossoma, melhorPai, sizeof(Cromossoma));

    /**
     * Libera memoria.
     */
        free(populacao);
        free(aptidao);
    }

    JNIEXPORT jstring JNICALL Java_ServerImplements_sayHello(JNIEnv *env, jobject thisObj, jstring inJNIStr)
    {
     const char *inCStr = (*env)->GetStringUTFChars(env, inJNIStr, NULL);
     
    static Jogador minotauro = {'U', 9, 5, DIREITA}; /**< Jogador PC.     */
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
        "####################+"
    };
    
    desloca(&jogador01, 0);

    /**
     * Verifica se o deslocamento do jogador Humano ocasionou o fim de
     * jogo.
     */
    if(jogador01.posicao.x == minotauro.posicao.x &&
        jogador01.posicao.y == minotauro.posicao.y)
    {
        outCStr[231] = '-';
    }
    melhorIndividuo(&minotauro, jogador01.posicao);
    desloca(&minotauro, 0);

    /**
     * Verifica se o deslocamento do jogador PC ocasionou o fim de jogo.
     */
    if(jogador01.posicao.x == minotauro.posicao.x &&
        jogador01.posicao.y == minotauro.posicao.y)
    {
        outCStr[231] = '-';
    }
    outCStr[(jogador01.posicao.y * 21) + jogador01.posicao.x] = jogador01.id;
    outCStr[(minotauro.posicao.y * 21) + minotauro.posicao.x] = minotauro.id;

    return (*env)->NewStringUTF(env, outCStr);
}
