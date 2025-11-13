// Trabalho Feito por : Gabriel Henrique Maciel e Kaue de Oliveira

#include <stdio.h>    // printf e scanf — nossos melhores amigos
#include <stdlib.h>   // system() pra limpar a tela e mudar cor
#include <ctype.h>    // toupper() — porque o jogador pode digitar em minúsculo
#include <windows.h>  // Beep() — somzinho retrô do sucesso (ou do fracasso)
#include <time.h>     // clock() — pra contar quanto tempo tu demora pra fugir do labirinto
#include <conio.h>

#define N 10 // Tamanho do labirinto — 10x10 de pura emoção

// Função para mostrar o labirinto na tela (printa com emojis)=
void mostrarLabirinto(int labirinto[N][N], int x, int y)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == x && j == y)
                printf("🧍"); // jogador — o herói da jornada
            else if (labirinto[i][j] == 1)
                printf("🌳"); // arvores — vai que acha algo que nao deve
            else if (labirinto[i][j] == -1)
                printf("👸"); // princesa — a luz no fim da prisao (literalmente)
            else if (labirinto[i][j] == 2)
                printf("🕳️"); // armadilha — tipo piso molhado no shopping
            else if (labirinto[i][j] == 3)
                printf("🐎"); // cavalo — te faz ganhar passos
            else if (labirinto[i][j] == 4)
                printf("🗡️"); // espada sagrada — item obrigatório pra não passar vergonha
            else if (labirinto[i][j] == 5)
                printf("🐉"); // dragao que mantem a princesa trancada
            else
                printf("🟢"); // caminho livre — siga, aventureiro
        }
        printf("\n");
    }
}

// Verifica se o movimento é válido — evita que o jogador faça besteira
int validarMovimento(int labirinto[N][N], int novoX, int novoY, int *temChave, int *pontos)
{
    // Tentou sair do mapa? parabéns, a princesa morreu bobao.
    if (novoX < 0 || novoX >= N || novoY < 0 || novoY >= N)
    {
        printf("❌ Ta indo pra onde covarde? A princesa esta em perigo!!\n");
        return 0;
    }

    // caso tente atravessar pode acabar encontrando coisas (tipo um esquilo)
    if (labirinto[novoX][novoY] == 1)
    {
        printf("🌳 Arvores! nao se sabe oque tem na floresta!\n");
        (*pontos) -= 10;
        return 0;
    }

    // Cadeado sem chave — parece minha vida financeira
    if (labirinto[novoX][novoY] == 5 && !(*temChave))
    {
        printf("🐉 fraco de mais! Volta lá e pega a espada, bobao acha que ganha de um dragao na porrada?.\n");
        (*pontos) -= 5;
        return 0;
    }

    // Cadeado destrancado — parabéns, agora sim!
    if (labirinto[novoX][novoY] == 5 && *temChave)
    {
        printf("🐉 Você matou o dragao! agora so falta salvar a princesa!\n");
        Beep(600, 200);
        labirinto[novoX][novoY] = 0;
        *temChave = 0;
        return 1;
    }

    return 1; // Tudo certo, pode andar sem medo
}

// Função pra tocar sons — tipo trilha sonora de 8 bits 🎵
void tocarSom(int tipo)
{
    switch (tipo)
    {
    case 0: Beep(400, 300); break; // erro — som da vergonha
    case 1: Beep(750, 100); break; // movimento — suave e elegante
    case 2: Beep(1000, 150); Beep(1200, 150); break; // item — sucesso momentâneo
    case 3: Beep(500, 300); Beep(450, 300); Beep(400, 500); break; // armadilha — faz o L
    case 4: Beep(1000, 300); Beep(1500, 300); Beep(2000, 500); break; // vitória — fogos imaginários
    case 5: Beep(400, 500); Beep(400, 500); Beep(400, 500); break; // game over — F
    }
}

// Função pra limpar a tela
void limparTela()
{
    system("cls");
}

// Programa principal — onde o herói nasce e (talvez) vence
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    system("chcp 65001 >nul");
    system("color 9");

    // Nosso glorioso mapa — cheio de armadilhas e sonhos quebrados
    int labirinto[N][N] = {
        {0, 1, 0, 3, 0, 0, 0, 0, 0, 1},
        {0, 1, 0, 1, 0, 1, 1, 1, 0, 0},
        {0, 0, 0, 2, 0, 1, 0, 0, 2, 0},
        {1, 0, 1, 1, 1, 1, 1, 0, 0, 3},
        {0, 3, 0, 1, -1, 1, 1, 0, 1, 1},
        {0, 1, 0, 1, 5, 1, 0, 0, 0, 1},
        {1, 1, 0, 1, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 1, 1, 2, 0},
        {0, 1, 2, 0, 1, 0, 0, 3, 0, 0},
        {4, 0, 0, 0, 1, 0, 1, 0, 1, 1}
    };

    // Variáveis básicas — o kit de sobrevivência basico
    int x = 0, y = 0;
    int passos = 40;
    int pontos = 0;
    int temChave = 0;
    int contadorPassos = 0;
    char comando;
    int jogando = 1;

    // 🕒 Controle de tempo — pra saber quanto tempo tu ficou preso kk
    clock_t inicio, fim;
    double tempoTotal;
    inicio = clock();

    while (jogando)
    {
        limparTela();

        // Atualiza tempo, que nem a previsão do tempo
        fim = clock();
        tempoTotal = (double)(fim - inicio) / CLOCKS_PER_SEC;

        printf("==== JOGO DO LABIRINTO EM FORMA DE RPG ====\n");
        printf("Use W, A, S, D para mover o heroi.\n");
        printf("Pegue a espada do sagrada (🗡 ), e mate o dragao (🐉) e salve a princesa aprisionada (👸)\n\n");

        // Caiu em armadilha — cuidado onde pisa
        if (labirinto[x][y] == 2)
        {
            printf("\n⚠ Caiu na armadilha! -5 passos e -5 de dignidade.\n");
            tocarSom(3);
            passos -= 5;
            labirinto[x][y] = 0;
        }

        // Mostra o mapa bonitinho
        mostrarLabirinto(labirinto, x, y);

        // Status do jogador — tipo HUD mas raiz
        printf("\nPassos restantes: %d", passos);
        printf("\nPontos: %d", pontos);
        printf("\nPosição: (%d, %d)", x + 1, y + 1);
        printf("\n⏱️ Tempo: %.1f segundos\n", tempoTotal);

        // 🎯 Vitória — o momento épico em que o heroi salvou a princesa
        if (labirinto[x][y] == -1)
        {
            fim = clock();
            tempoTotal = (double)(fim - inicio) / CLOCKS_PER_SEC;
            pontos += 200;
            printf("\n🎉 Parabéns! Você salvou a princesa do dragao!\n");
            printf("Pontos finais: %d\n", pontos);
            printf("⏱️ Tempo total: %.2f segundos\n", tempoTotal);
            printf("🕹️ E o prêmio é... nota 10 pro trabalho!\n");
            tocarSom(4);
            break;
        }

        //  Comeu um cavalo — energia +10
        if (labirinto[x][y] == 3)
        {
            printf("\nVocê comeu um cavalo (+10 pontos, +5 passos)\n");
            pontos += 10;
            passos += 5;
            tocarSom(2);
            labirinto[x][y] = 0;
        }

        // 🗡 Pegou a espada — agora é o heroi do labirinto
        if (labirinto[x][y] == 4)
        {
            printf("\nVocê pegou a espada sagrada! 🗡  Agora mate o dragao branco de olhos azuis.\n");
            pontos += 10;
            temChave = 1;
            tocarSom(2);
            labirinto[x][y] = 0;
        }

        // Movimento do jogador
        printf("\nDigite seu movimento (W/A/S/D): ");
        comando = getch();
        comando = toupper(comando); // aceita maiúsculo, minúsculo, caixa alta, caixa baixa e sla oq

        int novoX = x, novoY = y;

        // Direções — bem intuitivas, espero 😅
        if (comando == 'W') novoX--;
        else if (comando == 'S') novoX++;
        else if (comando == 'A') novoY--;
        else if (comando == 'D') novoY++;
        else
        {
            printf("Comando inválido! (para de espamar seu but) \n");
            continue;
        }

        contadorPassos++;
        passos--;
        pontos += 5;

        int resultado = validarMovimento(labirinto, novoX, novoY, &temChave, &pontos);

        if (resultado == 1)
        {
            x = novoX;
            y = novoY;
            tocarSom(1);
        }
        else
        {
            tocarSom(0);
        }

        // 😭 Fim dos passos — acabou o fôlego
        if (passos < 0)
        {
            fim = clock();
            tempoTotal = (double)(fim - inicio) / CLOCKS_PER_SEC;
            printf("\n💀 Fim de jogo! Seus passos acabaram e outro heroi salvou a princesa.\n");
            printf("⏱️ Tempo total: %.2f segundos\n", tempoTotal);
            tocarSom(5);
            break;
        }
    }

    return 0;
}
