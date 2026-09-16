/*
  Trabalho M1 - Algoritmos e Programacao II
  Labirinto Giratorio - Windows

  Integrantes:
  Lucas Simas
  Felipe
  Jair Machado

  Baseado no codigo de movimentacao passado pelo professor.
  Adaptacao somente das funcoes de teclado, cursor e cor para Windows.
*/

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#define TAM 11

void PosicionaCursor(int XPos, int YPos) {
    COORD coord;
    coord.X = XPos;
    coord.Y = YPos;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void EscondeCursor() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);

    cursorInfo.bVisible = false;

    SetConsoleCursorInfo(console, &cursorInfo);
}

void FundoAzul() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(console, BACKGROUND_BLUE);
}

void CorNormal() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(console, FOREGROUND_RED |
                                      FOREGROUND_GREEN |
                                      FOREGROUND_BLUE);
}

bool portaFechada(int celula, int orientacao) {
    if (celula == 6 && (orientacao == 0 || orientacao == 180))
        return true;

    if (celula == 7 && (orientacao == 90 || orientacao == 270))
        return true;

    return false;
}

bool sustentaBloco(int celula, int orientacao) {
    if (celula == 1)
        return true;

    if (portaFechada(celula, orientacao))
        return true;

    return false;
}

bool podeAndar(int m[][TAM], int blocos[][TAM],
               int x, int y, int orientacao) {

    if (x < 0 || x >= TAM || y < 0 || y >= TAM)
        return false;

    if (m[x][y] == 1)
        return false;

    if (blocos[x][y] == 1)
        return false;

    if (portaFechada(m[x][y], orientacao))
        return false;

    return true;
}

bool estaNaAlavanca(int m[][TAM], int px, int py) {
    return m[px][py] == 4;
}

bool chegouNaSaida(int m[][TAM], int px, int py) {
    return m[px][py] == 5;
}

void desenha(int m[][TAM], int blocos[][TAM],
             int px, int py, int orientacao,
             int movimentos, int rotacoes, int numeroMapa) {

    PosicionaCursor(0, 0);

    cout << "LABIRINTO GIRATORIO                         " << endl;
    cout << "Mapa: " << numeroMapa << "                                  " << endl;
    cout << "Orientacao: " << orientacao << " graus                    " << endl;
    cout << "Movimentos: " << movimentos
         << "   Rotacoes: " << rotacoes << "                    " << endl;
    cout << "WASD mover | Q/E girar | R reiniciar | ESC menu          " << endl;
    cout << endl;

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {

            if (i == px && j == py) {
                cout << "@";
            }
            else if (blocos[i][j] == 1) {
                cout << "O";
            }
            else {
                switch (m[i][j]) {
                    case 0:
                        cout << " ";
                        break;

                    case 1:
                        FundoAzul();
                        cout << " ";
                        CorNormal();
                        break;

                    case 4:
                        cout << "A";
                        break;

                    case 5:
                        cout << "S";
                        break;

                    case 6:
                        if (portaFechada(6, orientacao))
                            cout << "=";
                        else
                            cout << ":";
                        break;

                    case 7:
                        if (portaFechada(7, orientacao))
                            cout << "|";
                        else
                            cout << ";";
                        break;

                    default:
                        cout << " ";
                }
            }
        }
        cout << endl;
    }

    cout << endl;

    if (estaNaAlavanca(m, px, py))
        cout << "Voce esta em uma alavanca. Q e E liberados.             " << endl;
    else
        cout << "Procure uma alavanca para girar o mapa.                  " << endl;
}

void moveJogador(int m[][TAM], int blocos[][TAM],
                 int &px, int &py, int orientacao,
                 char tecla, int &movimentos) {

    int x = px;
    int y = py;

    if (tecla == 'w' || tecla == 'W')
        x--;
    else if (tecla == 's' || tecla == 'S')
        x++;
    else if (tecla == 'a' || tecla == 'A')
        y--;
    else if (tecla == 'd' || tecla == 'D')
        y++;
    else
        return;

    if (podeAndar(m, blocos, x, y, orientacao)) {
        px = x;
        py = y;
        movimentos++;
    }
}

void copiaMatriz(int origem[][TAM], int destino[][TAM]) {
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            destino[i][j] = origem[i][j];
}

void giraDireita(int origem[][TAM], int destino[][TAM]) {
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            destino[j][TAM - 1 - i] = origem[i][j];
}

void giraEsquerda(int origem[][TAM], int destino[][TAM]) {
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            destino[TAM - 1 - j][i] = origem[i][j];
}

void rotacionaDireita(int m[][TAM], int blocos[][TAM],
                      int &px, int &py) {

    int auxM[TAM][TAM];
    int auxB[TAM][TAM];

    giraDireita(m, auxM);
    giraDireita(blocos, auxB);

    copiaMatriz(auxM, m);
    copiaMatriz(auxB, blocos);

    int x = px;
    int y = py;

    px = y;
    py = TAM - 1 - x;
}

void rotacionaEsquerda(int m[][TAM], int blocos[][TAM],
                       int &px, int &py) {

    int auxM[TAM][TAM];
    int auxB[TAM][TAM];

    giraEsquerda(m, auxM);
    giraEsquerda(blocos, auxB);

    copiaMatriz(auxM, m);
    copiaMatriz(auxB, blocos);

    int x = px;
    int y = py;

    px = TAM - 1 - y;
    py = x;
}

void verificaEsmagamento(int m[][TAM], int blocos[][TAM],
                         int px, int py, int orientacao,
                         bool &perdeu) {

    perdeu = false;

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {

            if (portaFechada(m[i][j], orientacao)) {

                if (blocos[i][j] == 1)
                    blocos[i][j] = 0;

                if (i == px && j == py)
                    perdeu = true;
            }
        }
    }
}

void gravidade(int m[][TAM], int blocos[][TAM], int orientacao) {
    bool caiu = true;

    while (caiu) {
        caiu = false;

        for (int i = TAM - 2; i >= 0; i--) {
            for (int j = 0; j < TAM; j++) {

                if (blocos[i][j] == 1) {

                    if (blocos[i + 1][j] == 0 &&
                        !sustentaBloco(m[i + 1][j], orientacao)) {

                        blocos[i][j] = 0;
                        blocos[i + 1][j] = 1;
                        caiu = true;
                    }
                }
            }
        }
    }
}

void limpaBlocos(int blocos[][TAM]) {
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            blocos[i][j] = 0;
}

void mapa1(int m[][TAM], int blocos[][TAM], int &px, int &py) {
    int aux[TAM][TAM] = {
        {1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,1,0,0,0,0,1},
        {1,0,0,0,0,1,0,0,0,0,1},
        {1,0,0,0,0,1,0,0,0,0,1},
        {1,0,0,0,0,1,0,0,0,0,1},
        {1,0,4,0,0,0,0,0,5,0,1},
        {1,0,0,0,0,1,0,0,0,0,1},
        {1,0,0,0,0,1,0,0,0,0,1},
        {1,0,0,0,0,1,0,0,0,0,1},
        {1,0,0,0,0,1,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1}
    };

    copiaMatriz(aux, m);
    limpaBlocos(blocos);

    blocos[5][5] = 1;

    px = 5;
    py = 2;
}

void mapa2(int m[][TAM], int blocos[][TAM], int &px, int &py) {
    int aux[TAM][TAM] = {
        {1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,1,0,5,0,1},
        {1,0,0,0,0,0,6,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,1},
        {1,0,4,0,0,0,1,0,0,0,1},
        {1,0,0,0,0,0,1,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1}
    };

    copiaMatriz(aux, m);
    limpaBlocos(blocos);

    px = 8;
    py = 2;
}

void mapa3(int m[][TAM], int blocos[][TAM], int &px, int &py) {
    int aux[TAM][TAM] = {
        {1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,1,0,0,1,0,0,1},
        {1,0,0,0,1,0,0,1,0,0,1},
        {1,0,0,0,1,0,0,1,0,0,1},
        {1,0,0,0,1,0,0,1,0,0,1},
        {1,0,4,0,6,0,4,7,0,5,1},
        {1,0,0,0,1,0,0,1,0,0,1},
        {1,0,0,0,1,0,0,1,0,0,1},
        {1,0,0,0,1,0,0,1,0,0,1},
        {1,0,0,0,1,0,0,1,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1}
    };

    copiaMatriz(aux, m);
    limpaBlocos(blocos);

    px = 5;
    py = 2;
}

void carregaMapa(int numero, int m[][TAM],
                 int blocos[][TAM], int &px, int &py) {

    if (numero == 1)
        mapa1(m, blocos, px, py);
    else if (numero == 2)
        mapa2(m, blocos, px, py);
    else
        mapa3(m, blocos, px, py);
}

int mapaAleatorio() {
    srand(time(0));
    return (rand() % 3) + 1;
}

int escolheMapa() {
    int op;

    system("cls");

    cout << "ESCOLHA O MAPA" << endl << endl;
    cout << "1 - Mapa 1" << endl;
    cout << "2 - Mapa 2" << endl;
    cout << "3 - Mapa 3" << endl << endl;
    cout << "Opcao: ";

    cin >> op;

    while (op < 1 || op > 3) {
        cout << "Digite 1, 2 ou 3: ";
        cin >> op;
    }

    return op;
}

int novoJogo() {
    int op;

    system("cls");

    cout << "NOVO JOGO" << endl << endl;
    cout << "1 - Escolher mapa" << endl;
    cout << "2 - Mapa aleatorio" << endl << endl;
    cout << "Opcao: ";

    cin >> op;

    while (op < 1 || op > 2) {
        cout << "Digite 1 ou 2: ";
        cin >> op;
    }

    if (op == 1)
        return escolheMapa();

    return mapaAleatorio();
}

void sobre() {
    system("cls");

    cout << "LABIRINTO GIRATORIO" << endl << endl;
    cout << "Algoritmos e Programacao II" << endl;
    cout << "09/2026" << endl;
    cout << "Professor: Thiago Felski Pereira" << endl << endl;

    cout << "Equipe:" << endl;
    cout << "Lucas Simas" << endl;
    cout << "Felipe" << endl;
    cout << "Jair Machado" << endl << endl;

    cout << "Objetivo: chegar ate a saida do labirinto." << endl;
    cout << "O mapa so pode ser girado quando o jogador" << endl;
    cout << "estiver sobre uma alavanca." << endl;
    cout << "Depois da rotacao, os blocos sofrem a gravidade." << endl << endl;

    cout << "W A S D - mover" << endl;
    cout << "Q - girar para esquerda" << endl;
    cout << "E - girar para direita" << endl;
    cout << "R - reiniciar fase" << endl;
    cout << "ESC - voltar ao menu" << endl << endl;

    cout << "Pressione uma tecla para voltar.";
    getch();
}

int menu(bool jogoEmAndamento) {
    int op;

    system("cls");

    cout << "LABIRINTO GIRATORIO" << endl << endl;
    cout << "1 - Novo jogo" << endl;

    if (jogoEmAndamento)
        cout << "2 - Continuar" << endl;
    else
        cout << "2 - Continuar (indisponivel)" << endl;

    cout << "3 - Sobre" << endl;
    cout << "4 - Fim" << endl << endl;
    cout << "Opcao: ";

    cin >> op;

    while (op < 1 || op > 4) {
        cout << "Digite uma opcao de 1 a 4: ";
        cin >> op;
    }

    return op;
}

void reiniciaMapa(int numeroMapa, int m[][TAM], int blocos[][TAM],
                  int &px, int &py, int &orientacao,
                  int &movimentos, int &rotacoes) {

    carregaMapa(numeroMapa, m, blocos, px, py);
    orientacao = 0;
    movimentos = 0;
    rotacoes = 0;
}

void jogar(int m[][TAM], int blocos[][TAM],
           int &px, int &py, int &orientacao,
           int &movimentos, int &rotacoes,
           int numeroMapa, bool &jogoEmAndamento) {

    bool voltar = false;
    bool perdeu = false;

    while (!voltar) {

        desenha(m, blocos, px, py, orientacao,
                movimentos, rotacoes, numeroMapa);

        char tecla = getch();

        if (tecla == 27) {
            voltar = true;
        }
        else if (tecla == 'r' || tecla == 'R') {
            reiniciaMapa(numeroMapa, m, blocos, px, py,
                          orientacao, movimentos, rotacoes);
        }
        else if (tecla == 'q' || tecla == 'Q') {

            if (estaNaAlavanca(m, px, py)) {

                rotacionaEsquerda(m, blocos, px, py);

                orientacao -= 90;
                if (orientacao < 0)
                    orientacao = 270;

                rotacoes++;

                verificaEsmagamento(m, blocos, px, py,
                                    orientacao, perdeu);

                if (!perdeu)
                    gravidade(m, blocos, orientacao);
            }
        }
        else if (tecla == 'e' || tecla == 'E') {

            if (estaNaAlavanca(m, px, py)) {

                rotacionaDireita(m, blocos, px, py);

                orientacao += 90;
                if (orientacao == 360)
                    orientacao = 0;

                rotacoes++;

                verificaEsmagamento(m, blocos, px, py,
                                    orientacao, perdeu);

                if (!perdeu)
                    gravidade(m, blocos, orientacao);
            }
        }
        else {
            moveJogador(m, blocos, px, py,
                        orientacao, tecla, movimentos);
        }

        if (perdeu) {
            system("cls");
            cout << "Voce foi esmagado por uma porta." << endl;
            cout << "Pressione uma tecla para reiniciar.";
            getch();

            reiniciaMapa(numeroMapa, m, blocos, px, py,
                          orientacao, movimentos, rotacoes);

            perdeu = false;
        }

        if (chegouNaSaida(m, px, py)) {
            system("cls");

            cout << "Mapa concluido!" << endl << endl;
            cout << "Mapa: " << numeroMapa << endl;
            cout << "Movimentos: " << movimentos << endl;
            cout << "Rotacoes: " << rotacoes << endl << endl;
            cout << "Pressione uma tecla para voltar ao menu.";

            getch();

            jogoEmAndamento = false;
            voltar = true;
        }
    }
}

int main() {
    EscondeCursor();

    int m[TAM][TAM];
    int blocos[TAM][TAM];

    int px = 0;
    int py = 0;
    int orientacao = 0;
    int movimentos = 0;
    int rotacoes = 0;
    int numeroMapa = 1;

    bool jogoEmAndamento = false;
    bool fim = false;

    while (!fim) {

        int op = menu(jogoEmAndamento);

        if (op == 1) {

            numeroMapa = novoJogo();

            reiniciaMapa(numeroMapa, m, blocos, px, py,
                          orientacao, movimentos, rotacoes);

            jogoEmAndamento = true;

            system("cls");

            jogar(m, blocos, px, py, orientacao,
                  movimentos, rotacoes, numeroMapa,
                  jogoEmAndamento);
        }
        else if (op == 2) {

            if (jogoEmAndamento) {
                system("cls");

                jogar(m, blocos, px, py, orientacao,
                      movimentos, rotacoes, numeroMapa,
                      jogoEmAndamento);
            }
        }
        else if (op == 3) {
            sobre();
        }
        else if (op == 4) {
            fim = true;
        }
    }

    system("cls");
    cout << "Fim do programa." << endl;

    return 0;
}
