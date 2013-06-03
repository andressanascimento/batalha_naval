#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>

int eu = 0;
int pc = 0;

typedef struct
{
    BITMAP *imagem;
    char nome_imagem[100];
    int ocupado;
    int visivel;
    char nome_imagem_padrao[100];
    int x;
    int y;
}TABULEIRO;

typedef struct
{
    char nome_navio[3];
    int total_partes;
    int adicionado;
}NAVIO;

void Randomize(void)
{
    srand((int) time(NULL));
}

int RandomInteger(int low, int high)
{
    int k;
    double d;

    d = (double) rand() / ((double) RAND_MAX + 1);
    k = (int) (d * (high - low + 1));
    return (low + k);
}

int cont_q;
int vez = 0;
int telaAtaque = 0;

//variaveis globais
BITMAP *fundo;
BITMAP *buffer;
BITMAP *q_es;
BITMAP *q_cl;
BITMAP *fundo_letras;
BITMAP *legenda_letras;

//VARIVEL PRINCIPAL
TABULEIRO tabuleiro[2][5][8];
NAVIO navios[2][5];
//int tabuleiro[5][8][2];
//BITMAP *tabuleiro_imagem[5][7];

void ataque(char quadrante[3],int jogador);
int ataqueComputador();

char navio[3],orientacao[3],quadrante[3];
char nome_navio[5][3];

inline void init()
{
    allegro_init();
	set_color_depth(16);
	set_gfx_mode(GFX_AUTODETECT, 800, 600, 0, 0);

	install_timer();
	install_keyboard();
	install_mouse();
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	set_window_title("Jogo Batalha Naval");
	buffer = create_bitmap(SCREEN_W, SCREEN_H);
}

inline void deinit()
{
	clear_keybuf();
	allegro_exit();
}

int retornaColuna(char quadrante[3]){
    int posicao,i;
    char letras_ma[] = "ABCDEFGHN";
    char letras_mi[] = "abcdefghn";

    for(i=0;i<8;i++){
        if( (quadrante[0] == letras_ma[i]) || (quadrante[0] == letras_mi[i]) ){
            posicao = i;
            //printf("Posicao: %d",posicao);
        }
    }
    return posicao;
}

int retornaLinha(char quadrante[3]){
    char numero[] = "12345";
    int i,posicao;
    for(i=0;i<5;i++){
        if(quadrante[1]==numero[i]){
            posicao = i;
        }
    }
    return posicao;
}

void randomQuadrante(){
    Randomize();
    int random_letras[100] = {
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),
                            RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7),RandomInteger(0,7)
                        };

    int random_numeros[100] = {
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),
                            RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4),RandomInteger(0,4)
                        };

    char rand_letras[] = "ABCDEFGH";
    char rand_numeros[] = "12345";
    int r_l = random_letras[random_letras[cont_q]];
    int r_n= random_numeros[random_numeros[cont_q]];
    if(cont_q>95){
        cont_q=0;
    }
    else{
        cont_q++;
    }

    //printf("\nletra :%d ",r_l);
    //printf("\nnumero: %d ",r_n);
    char q[5] = {rand_letras[r_l]+'\0', rand_numeros[r_n]+'\0'};
    strcpy(quadrante,q);
}

//funcoes complexas para o jogo
void tabuleiro_padrao(int jogador){
    //fundo_jogo();
    int x = 44;
    int y = 171;
    q_es = load_bitmap("imagens/tabuleiro/q_es2.bmp", NULL);
    q_cl = load_bitmap("imagens/tabuleiro/q_cl2.bmp", NULL);

    for(int linha=0; linha<5;linha++){
        for(int coluna=0;coluna<8;coluna++){
            tabuleiro[jogador][linha][coluna].ocupado = 0;
            tabuleiro[jogador][linha][coluna].visivel = 0;
            //Se a linha for par
            if(linha%2==0){
                //se a coluna for par
                if(coluna%2==0){
                    //insere a imagem escura
                    //draw_sprite(screen, q_es, x,y);
                    tabuleiro[jogador][linha][coluna].x = x;
                    tabuleiro[jogador][linha][coluna].y = y;

                    //strcpy(tabuleiro_imagem[linha][coluna],"q_es");

                    tabuleiro[jogador][linha][coluna].imagem = load_bitmap("imagens/tabuleiro/q_es2.bmp", NULL);
                    strcpy(tabuleiro[jogador][linha][coluna].nome_imagem,"imagens/tabuleiro/q_es2.bmp");
                    strcpy(tabuleiro[jogador][linha][coluna].nome_imagem_padrao,"imagens/tabuleiro/q_es2.bmp");
                    x = x+72;
                }
                else{
                    //se a coluna for impar
                    //insere a imagem clara
                    //draw_sprite(screen, q_cl, x,y);
                    tabuleiro[jogador][linha][coluna].x = x;
                    tabuleiro[jogador][linha][coluna].y = y;

                    //strcpy(tabuleiro_imagem[linha][coluna],"q_cl");
                    tabuleiro[jogador][linha][coluna].imagem = load_bitmap("imagens/tabuleiro/q_cl2.bmp", NULL);
                    strcpy(tabuleiro[jogador][linha][coluna].nome_imagem,"imagens/tabuleiro/q_cl2.bmp");
                    strcpy(tabuleiro[jogador][linha][coluna].nome_imagem_padrao,"imagens/tabuleiro/q_cl2.bmp");
                    x = x+72;
                }
            }
            else{
                //Se a linha for impar
                //Se a coluna for par
                if(coluna%2==0){
                    //insere a imagem clara
                    //draw_sprite(screen, q_cl, x,y);
                    tabuleiro[jogador][linha][coluna].x = x;
                    tabuleiro[jogador][linha][coluna].y = y;

                    //strcpy(tabuleiro_imagem[linha][coluna],"q_cl");
                    tabuleiro[jogador][linha][coluna].imagem = load_bitmap("imagens/tabuleiro/q_cl2.bmp", NULL);
                    strcpy(tabuleiro[jogador][linha][coluna].nome_imagem,"imagens/tabuleiro/q_cl2.bmp");
                    strcpy(tabuleiro[jogador][linha][coluna].nome_imagem_padrao,"imagens/tabuleiro/q_cl2.bmp");
                    x = x+72;
                }
                else{
                    //insere a imagem escura
                    //draw_sprite(screen, q_es, x,y);
                    tabuleiro[jogador][linha][coluna].x = x;
                    tabuleiro[jogador][linha][coluna].y = y;
                    //strcpy(tabuleiro_imagem[linha][coluna],"q_es");
                    tabuleiro[jogador][linha][coluna].imagem = load_bitmap("imagens/tabuleiro/q_es2.bmp", NULL);
                    strcpy(tabuleiro[jogador][linha][coluna].nome_imagem,"imagens/tabuleiro/q_es2.bmp");
                    strcpy(tabuleiro[jogador][linha][coluna].nome_imagem_padrao,"imagens/tabuleiro/q_es2.bmp");
                    x = x+72;
                }
            }
            //se o último quadrado da linha for inserido então o x volta a posição original
            if(coluna==7){
                x = 44;
            }
        }
        //aqui adiciona para criar uma nova linha
        y = y+61;
    }
}

void imprime_tabuleiro(int jogador,int ataque){
    BITMAP *legenda_letras;
    legenda_letras = load_bitmap("imagens/legenda_letras.bmp", NULL);
    draw_sprite(screen, legenda_letras, 44,150);

    BITMAP *legenda_numeros;
    legenda_numeros = load_bitmap("imagens/legenda_numeros.bmp", NULL);
    draw_sprite(screen, legenda_numeros,14,185);

    for(int linha=0; linha<5;linha++){
        for(int coluna=0;coluna<8;coluna++){
                if(ataque == 0){
                    draw_sprite(screen, tabuleiro[jogador][linha][coluna].imagem, tabuleiro[jogador][linha][coluna].x,tabuleiro[jogador][linha][coluna].y);
                }
                else{
                    if(tabuleiro[jogador][linha][coluna].visivel == 1){
                        draw_sprite(screen, tabuleiro[jogador][linha][coluna].imagem, tabuleiro[jogador][linha][coluna].x,tabuleiro[jogador][linha][coluna].y);
                    }
                    else{
                        draw_sprite(screen, load_bitmap(tabuleiro[jogador][linha][coluna].nome_imagem_padrao,NULL), tabuleiro[jogador][linha][coluna].x,tabuleiro[jogador][linha][coluna].y);
                    }
                }
        }
    }

}

void fundo_jogo(){
    fundo = load_bitmap("imagens/inicio.bmp", NULL);
    draw_sprite(buffer,fundo, 0, 0);
    draw_sprite(screen,buffer, 0, 0);
    //blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
}

void imprime_letras(char vetor[3],int tamanho,int x, int y){
    char letras_ma[] = "ABCDEFGHN";
    char letras_mi[] = "abcdefghn";
    char numeros[] = "012345";
    char nome_imagem_letras[9][10];
    char nome_imagem_numeros[6][10];
    strcpy(nome_imagem_letras[0],"A.bmp");strcpy(nome_imagem_letras[1],"B.bmp");strcpy(nome_imagem_letras[2],"C.bmp");
    strcpy(nome_imagem_letras[3],"D.bmp");strcpy(nome_imagem_letras[4],"E.bmp");strcpy(nome_imagem_letras[5],"F.bmp");
    strcpy(nome_imagem_letras[6],"G.bmp");strcpy(nome_imagem_letras[7],"H.bmp");strcpy(nome_imagem_letras[8],"N.bmp");
    strcpy(nome_imagem_numeros[0],"0.bmp");strcpy(nome_imagem_numeros[1],"1.bmp");strcpy(nome_imagem_numeros[2],"2.bmp");
    strcpy(nome_imagem_numeros[3],"3.bmp");strcpy(nome_imagem_numeros[4],"4.bmp");strcpy(nome_imagem_numeros[5],"5.bmp");


    int j,i;
    BITMAP *letras;
    for(j = 0;j<tamanho;j++){
        for(i = 0; i<9;i++){
            if(i<6){
                if(vetor[j] == numeros[i]){
                    letras = load_bitmap(nome_imagem_numeros[i], NULL);
                    draw_sprite(screen,letras,x,y);
                    //draw_sprite(screen,legenda_letras,0,0);
                    x = x+15;
                }
            }
            if( (vetor[j] == letras_ma[i]) || (vetor[j] == letras_mi[i]) ){
                letras = load_bitmap(nome_imagem_letras[i], NULL);
                draw_sprite(screen,letras,x,y);
                //draw_sprite(screen,legenda_letras,0,0);
                x = x+15;
            }

        }
    }

}

void legenda(){

    if(telaAtaque == 1){
        BITMAP *legenda_l;
        legenda_l = load_bitmap("imagens/legenda_ataque.bmp", NULL);
        draw_sprite(screen,legenda_l, 25,500);
    }
    else{
        BITMAP *legenda;
        legenda = load_bitmap("imagens/legenda.bmp", NULL);
        draw_sprite(screen, legenda, 25,500);

        if(navio != NULL){
            imprime_letras(navio,3,235,510);
        }
        if(quadrante!=NULL){
            imprime_letras(quadrante,3,235,540);
        }
    }

}

void lendo_string(char *vetor, int tamanho, int x_ini, int y_ini)
{
        int x=x_ini;
        int y=y_ini;
        int cur_pos = 0; //posição inicial da string a ser lida
        int tecla = 0;  // armazena cada tecla a ser pressionada
        int i;         // variável auxiliar

        char letras_ma[] = "ABCDEFGHN";
        char letras_mi[] = "abcdefghn";
        char numeros[] = "012345";
        char nome_imagem_letras[9][10];
        char nome_imagem_numeros[6][10];
        strcpy(nome_imagem_letras[0],"A.bmp");strcpy(nome_imagem_letras[1],"B.bmp");strcpy(nome_imagem_letras[2],"C.bmp");
        strcpy(nome_imagem_letras[3],"D.bmp");strcpy(nome_imagem_letras[4],"E.bmp");strcpy(nome_imagem_letras[5],"F.bmp");
        strcpy(nome_imagem_letras[6],"G.bmp");strcpy(nome_imagem_letras[7],"H.bmp");strcpy(nome_imagem_letras[8],"N.bmp");
        strcpy(nome_imagem_numeros[0],"0.bmp");strcpy(nome_imagem_numeros[1],"1.bmp");strcpy(nome_imagem_numeros[2],"2.bmp");
        strcpy(nome_imagem_numeros[3],"3.bmp");strcpy(nome_imagem_numeros[4],"4.bmp");strcpy(nome_imagem_numeros[5],"5.bmp");
        for (i = 0; i < tamanho; i++) vetor[i] = '\0'; // preenchendo o vetor com \O que indica o fim da string

        BITMAP *letras;
        while (tecla>>8 != KEY_ENTER)  // laço até ser pressionada  a tecla enter, >>8 rotaciona 8 bits à esquerda
        {
            tecla = readkey();
            if( ((tamanho-1) > cur_pos) ||  (tecla >> 8 == KEY_BACKSPACE) ){

                if(tecla >> 8 == KEY_BACKSPACE) //efeito de deletar um caracter
                {
                    //n_teclas = 0;
                    x = x_ini;
                    i = 0;

                    legenda();

                    cur_pos--;
                    if(cur_pos<0)cur_pos = 0;
                    vetor[cur_pos] = '\0';

                    //cur_pos--;
                    imprime_letras(vetor,3,x,y);

                }
                else{
                //clear(screen);// limpa tela

                    for(i = 0; i<9;i++){
                        if(((tecla & 0xff) == letras_ma[i]) || ((tecla & 0xff) == letras_mi[i])){
                            vetor[cur_pos] = tecla;
                            cur_pos++;
                            letras = load_bitmap(nome_imagem_letras[i], NULL);
                            draw_sprite(screen,letras,x,y);
                            //draw_sprite(screen,legenda_letras,0,0);
                            x = x+15;
                        }
                    }
                    for(i = 0; i<6;i++){
                        if((tecla & 0xff) == numeros[i]){
                            vetor[cur_pos] = tecla;
                            cur_pos++;
                            letras = load_bitmap(nome_imagem_numeros[i], NULL);
                            draw_sprite(screen,letras,x,y);
                            //draw_sprite(screen,legenda_letras,0,0);
                            x = x+15;
                        }
                    }
                }
            }
                //clear(screen);
                //textprintf(screen, font, x, y, makecol(255, 0, 0), vetor);
                //textout_ex(screen,font, vetor, x,y, makecol(255, 0, 0),-1); //vai escrevendo na tela o que está sendo digitado, na posição e cor indicados
        }
}

void sair(){
    if((mouse_x>= 675)&&(mouse_x <= 753)&&(mouse_y <= 95)&&(mouse_y >= 54))
    //if(mouse_x>= 675)
    {
        if(mouse_b & 1)
            {
                exit(0); //textprintf(screen, font, 100, 200, makecol(255, 0, 0), "oiiii!");
            }
    }
}

void tela_inicial();

void menu()
{
    select_mouse_cursor(1); //cursor do mouse
    show_mouse(screen); // mostra o cursor

    while(!key[KEY_ESC]){
        BITMAP *menu;
        //textprintf(screen, font, 20, 110, makecol(255, 0, 0), "X: %d Y: %d", mouse_x,mouse_y);
        menu = load_bitmap("imagens/voltar.bmp", NULL);
        draw_sprite(screen,menu, 650,496);
        destroy_bitmap(menu);
        //if((mouse_x>= 675)&&(mouse_x <=753)&&(mouse_y <=150)&&(mouse_y >=347))
        if((mouse_x >= 600)and (mouse_x<=800)and (mouse_y>=496)and (mouse_y<=590))
        {
            if(mouse_b & 1)
            {
                tela_inicial();
            }
        }
    }
}

//Telas mais simples
void tela_instrucoes(){
    fundo_jogo();
    while(!key[KEY_ESC]){
        BITMAP *instrucoes_tela;
        instrucoes_tela = load_bitmap("imagens/instrucoes_tela.bmp", NULL);
        draw_sprite(screen, instrucoes_tela, 137,158);
        destroy_bitmap(instrucoes_tela);
        menu();
        sair();
    }
}

void tela_creditos(){

    fundo_jogo();
    while(!key[KEY_ESC]){
        sair();
        BITMAP *credito_jogo;
        credito_jogo = load_bitmap("imagens/credito/credito_jogo.bmp", NULL);
        draw_sprite(screen, credito_jogo, 137,158);
        destroy_bitmap(credito_jogo);
        menu();
    }
}

int validaNavio(char barco[3]){
    char vetor[]="N1 N2 N3 N4 N5 n1 n2 n3 n4 n5";  //POSSIBILIDADE DE BARCO , AS PEGUEI E TRANSFORMEI EM UMA STRING
    if(strstr(vetor,barco))   // STRSTR UMA FUNCAO QUE COMPARA DUAS STRING
        return 1;
    return 0;
}

int validaQuadrante(char quadrante[3]){
    char vetor[]="a1 a2 a3 a4 a5 b1 b2 b3 b4 b5 c1 c2 c3 c4 c5 d1 d2 d3 d4 d5 e1 e2 e3 e4 e5 f1 f2 f3 f4 f5 g1 g2 g3 g4 g5 h1 h2 h3 h4 h5 A1 A2 A3 A4 A5 B1 B2 B3 B4 B5 C1 C2 C3 C4 C5  D1 D2 D3 D4 D5 E1 E2 E3 E4 E5 F1 F2 F3 F4 F5 G1 G2 G3 G4 G5  H1 H2 H3 H4 H5 ";
    if(strstr(vetor,quadrante)){
        return 1;
    }
    return 0;
}

int validaOrientacao(char orientacao[3]){
    char vetor[]="01 02 03 04 05";
    if(strstr(vetor,orientacao)){
        return 1;
    }
    return 0;
}

int posicionaNavio(int jogador){
    int linha,coluna,t_partes,i,j,validacao,coluna_m,linha_m,int_orientacao;
    char numero[] = {"0123456789"};
    char parte[2],l;
    char text_orientacao[2];
    int n_navio;
    char nome_do_navio[100];
    strcpy(quadrante,strupr(quadrante));

    linha = retornaLinha(quadrante);
    coluna = retornaColuna(quadrante);
    validacao = 0;

    linha_m = linha;
    coluna_m = coluna;
    validacao = 0;
    char novoNomeImagem[100];
    t_partes = 0;

    strcpy(navio,strupr(navio));
    for(j=0;j<5;j++){
        if(	strcmp(navio,nome_navio[j]) == 0){
            t_partes = navios[jogador][j].total_partes;
            n_navio = j;
        }
    }

    int_orientacao = orientacao[1]-48;
    i = 0;
    while(i<t_partes){
        if(int_orientacao==1){//vertical
            if(linha_m < 5){
                if(tabuleiro[jogador][linha_m][coluna].ocupado == 1){
                    validacao++;
                }
                linha_m++;
            }
            else{
                validacao++;
            }
        }
        else{//horizontal
            if(coluna_m <8){
                if(tabuleiro[jogador][linha][coluna_m].ocupado == 1){
                    validacao++;
                }
                coluna_m++;
            }
            else{
                validacao++;
            }
        }
        i++;
    }

    if(navios[jogador][n_navio].adicionado == 1){
        validacao = 1;
    }

    if(int_orientacao == 1){
        strcpy(text_orientacao,"v");
    }
    else{
        strcpy(text_orientacao,"h");
    }

    if(validacao > 0){
        return 0;
    }
    else{
       linha_m = linha;
       coluna_m = coluna;
       for(i=0;i<t_partes;i++){
            if(int_orientacao==1){//vertical
                strcpy(parte,itoa(i+1,parte,10));
                navio[0] = tolower(navio[0]);

                char nomeImagem[100];
                strcpy(nomeImagem,tabuleiro[jogador][linha_m][coluna].nome_imagem);

                snprintf(novoNomeImagem, sizeof novoNomeImagem, "imagens/%s/%s/%s_%s_%c_%s.bmp", navio, text_orientacao, navio, parte,nomeImagem[20],text_orientacao);
                tabuleiro[jogador][linha_m][coluna].imagem = load_bitmap(novoNomeImagem,NULL);
                strcpy(tabuleiro[jogador][linha_m][coluna].nome_imagem, novoNomeImagem);
                tabuleiro[jogador][linha_m][coluna].ocupado = 1;

                //printf("\n\norientacao: %d",int_orientacao);
                //printf("\ncoluna_m: %d",coluna_m);
                //printf("\nlinha: %d",linha);
                //printf("\nquadrante: %s",quadrante);
                //printf("\nimagem: %s",novoNomeImagem);

                linha_m += 1;

            }
            else{//horizontal
                strcpy(parte,itoa(i+1,parte,10));
                navio[0] = tolower(navio[0]);

                char nomeImagem[100];
                strcpy(nomeImagem,tabuleiro[jogador][linha][coluna_m].nome_imagem);

                snprintf(novoNomeImagem, sizeof novoNomeImagem, "imagens/%s/%s/%s_%s_%c_%s.bmp", navio,text_orientacao, navio, parte,nomeImagem[20],text_orientacao);
                tabuleiro[jogador][linha][coluna_m].imagem = load_bitmap(novoNomeImagem,NULL);
                strcpy(tabuleiro[jogador][linha][coluna_m].nome_imagem,novoNomeImagem);
                tabuleiro[jogador][linha][coluna_m].ocupado =1;

                //printf("\n\norientacao: %d",int_orientacao);
                //printf("\ncoluna_m: %d",coluna_m);
                //printf("\nlinha: %d",linha);
                //printf("\nquadrante: %s",quadrante);
                //printf("\nimagem: %s",novoNomeImagem);

                coluna_m  += 1;

           }
        }
        navios[jogador][n_navio].adicionado = 1;
        return 1;
    }
}

void telaGanhou(int jogador){
    fundo_jogo();
	BITMAP *imagem;

    //supondo que retorna o ganhador
    while(!key[KEY_ESC]){

        if(jogador == 0){
            imagem = load_bitmap("imagens/voce_ganhou.bmp",NULL);
        }
        else{
            imagem = load_bitmap("imagens/voce_perdeu.bmp",NULL);
        }

        draw_sprite(screen,imagem,200,200);
    }
	exit(0);
}

void tela_ataque(){
    fundo_jogo();
    telaAtaque = 1;

    BITMAP *legenda_l;
    BITMAP *vez_imagem;
    legenda_l = load_bitmap("imagens/legenda_ataque.bmp", NULL);

    while(!key[KEY_ESC]){
        sair();
        imprime_tabuleiro(vez,1);
        //vez do jogador
        if(vez == 0){
            vez_imagem = load_bitmap("imagens/sua_vez.bmp", NULL);
            draw_sprite(screen,vez_imagem, 670,155);

            imprime_tabuleiro(1,1);

            draw_sprite(screen,legenda_l, 25,500);
            lendo_string(quadrante,3,240,513);
            while(validaQuadrante(quadrante)!=1){
               lendo_string(quadrante,3,240,513);
            }
            int linha,coluna;
            linha = retornaLinha(quadrante);
            coluna = retornaColuna(quadrante);

            if(tabuleiro[1][linha][coluna].visivel==0){
                ataque(quadrante,1);
                imprime_tabuleiro(1,1);
                if(eu == 9){
                    telaGanhou(0);
                }
                else{
                    vez = 1;
                }
                rest(400);
            }
            else{
                lendo_string(quadrante,3,240,513);
                while(validaQuadrante(quadrante)!=1){
                    lendo_string(quadrante,3,240,513);
                }
            }
        }
        else{
            fundo_jogo();
            vez_imagem = load_bitmap("imagens/computador.bmp", NULL);
            draw_sprite(screen,vez_imagem, 670,155);
            imprime_tabuleiro(0,1);
            rest(1800);
            ataqueComputador();
            imprime_tabuleiro(0,1);

            if(pc==9){
                telaGanhou(1);
            }
            else{
                vez = 0;
            }
            rest(1800);
        }
    }
}

void tela_escolhe_navios(){

    int navios;
    navios = 0;

    legenda_letras = create_bitmap(SCREEN_W, SCREEN_H);
    fundo_jogo();
    BITMAP *legenda_l;
    legenda_l = load_bitmap("imagens/legenda.bmp", NULL);
    draw_sprite(screen,legenda_l, 25,500);


    imprime_tabuleiro(0,0);
    BITMAP *legenda_navio;
    legenda_navio = load_bitmap("imagens/navios_legenda.bmp", NULL);
    draw_sprite(screen, legenda_navio, 630,120);
    int qtt_navios,add;
    qtt_navios = 0;
    while(!key[KEY_ESC]){
        sair();
        while(qtt_navios < 5){
            strcpy(navio,"");
            strcpy(quadrante,"");
            strcpy(orientacao,"");

            lendo_string(navio,3,235,510);
            while(validaNavio(navio)!=1){
               textprintf(screen, font, 150, 510, makecol(255, 0, 0), "%s","O navio digitado não existe!");
               strcpy(navio,"");
               legenda();
               lendo_string(navio,3,235,510);
            }

            lendo_string(quadrante,3,235,540);
            while(validaQuadrante(quadrante)!=1){
                textprintf(screen, font, 150, 510, makecol(255, 0, 0), "%s","O quadrante digitado não existe!");
                strcpy(quadrante,"");
                legenda();
                lendo_string(quadrante,3,235,540);
            }

            lendo_string(orientacao,3,235,570);
            while(validaOrientacao(orientacao)!=1){
                textprintf(screen, font, 150, 510, makecol(255, 0, 0), "%s","Digite um código válido!");
                strcpy(quadrante,"");
                legenda();
                lendo_string(orientacao,3,235,570);
            }

            add = posicionaNavio(0);
            //textprintf(screen, font, 150, 510, makecol(255, 0, 0), "Add: %i",add);

            if(add == 0){
                textprintf(screen, font, 150, 510, makecol(255, 0, 0), "%s","O navio não pode ser inserido");
                strcpy(navio,"");
                strcpy(quadrante,"");
                strcpy(orientacao,"");
            }
            else{
                imprime_tabuleiro(0,0);
                BITMAP *colocou;
                if(navio[1]=='1'){
                    colocou = load_bitmap("imagens/colocou_n1.bmp", NULL);
                    draw_sprite(screen,colocou,670,155);
                }
                if(navio[1]=='2'){
                    colocou = load_bitmap("imagens/colocou_n2.bmp", NULL);
                    draw_sprite(screen,colocou,680,210);
                }
                if(navio[1]=='3'){
                    colocou = load_bitmap("imagens/colocou_n3.bmp", NULL);
                    draw_sprite(screen,colocou,685,270);
                }
                if(navio[1]=='4'){
                    colocou = load_bitmap("imagens/colocou_n4.bmp", NULL);
                    draw_sprite(screen,colocou,680,330);
                }
                if(navio[1]=='5'){
                    colocou = load_bitmap("imagens/colocou_n5.bmp", NULL);
                    draw_sprite(screen,colocou,660,400);
                }





                qtt_navios+=1;
                strcpy(navio,"");
                strcpy(quadrante,"");
                strcpy(orientacao,"");
                draw_sprite(screen,legenda_l, 25,500);
            }
        }
        printf("%d",qtt_navios);
        rest(600);
        //break;
        tela_ataque();
        sair();
    }
}

void posicionarNaviosComputador(){

    strcpy(quadrante,"");
    strcpy(navio,"");
    strcpy(orientacao,"");
    int r_nav;
    int nav = 0;
    int add = 0;
    int valida = 0;
    char o[3];
    char rand_orientacao[2][3];
    char rand_numeros[] = {"12345"};
    strcpy(rand_orientacao[0],"01");
    strcpy(rand_orientacao[1],"02");
    while(nav<5){
        r_nav = 0;
        strcpy(navio,"");
        strcpy(orientacao,"");
        strcpy(quadrante,"");
        r_nav = rand() %2;

        char n[5] = {'n'+'\0',rand_numeros[nav]+'\0'};
        char o[3];
        //strcpy(o,);

        strcpy(navio,n);
        strcpy(orientacao,rand_orientacao[r_nav]);

        randomQuadrante();
        valida = validaQuadrante(quadrante);
        if(valida == 1){

            printf("\nadd antes: %d",add);
            add = posicionaNavio(1);
            printf("\nadd despois: %d",add);

            if(add == 1){
                nav++;
                add=0;
            }
        }
    }
}

void tela_inicial(){

    fundo_jogo();
    sair();
    BITMAP *novo_jogo;
    novo_jogo = load_bitmap("imagens/novo_jogo.bmp", NULL);
    draw_sprite(screen, novo_jogo, 307,203);
    destroy_bitmap(novo_jogo);

    BITMAP *instrucoes;
    instrucoes = load_bitmap("imagens/instrucoes.bmp", NULL);
    draw_sprite(screen, instrucoes, 307,276);
    destroy_bitmap(instrucoes);

    BITMAP *creditos;
    creditos = load_bitmap("imagens/creditos.bmp", NULL);
    draw_sprite(screen, creditos, 307,347);
    destroy_bitmap(creditos);

    select_mouse_cursor(1); //cursor do mouse
    show_mouse(screen); // mostra o cursor

    while (!key[KEY_ESC]){
      //textprintf(screen, font, 20, 110, makecol(255, 0, 0), "X: %d Y: %d", mouse_x,mouse_y);
      if((mouse_x >= 315) &&(mouse_x <=485) &&(mouse_y >=277) &&(mouse_y <= 427))
          {
          if (mouse_b & 1)
            {
                rest(1000);
                clear(screen);
                tela_instrucoes();
                break;
            }
          }
      if((mouse_x>= 315)and (mouse_x<=485) and (mouse_y >= 350 ) and (mouse_y <= 400))
      {
          if (mouse_b & 1)
            {
                rest(1000);
                clear(screen);
                tela_creditos();
                break;
            }
      }
      if((mouse_x>= 315)and (mouse_x<=485) and (mouse_y >=200) and (mouse_y <= 250)){
          if (mouse_b & 1)
            {
                rest(1000);
                clear(screen);
                tela_escolhe_navios();
                break;
            }
      }
      sair();
    }
}

void ataque(char quadrante[3],int jogador){
    //tabuleiro[0][0][0].imagem = load_bitmap("imagens/n1/h/n1_1_cl_h.bmp",NULL);
    //strcpy(tabuleiro[0][0][0].nome_imagem,"imagens/n1/h/n1_1_cl_h.bmp");

    int linha, coluna;
    char nomeImagem[60],orientacao[3];

    linha = retornaLinha(quadrante);
    coluna = retornaColuna(quadrante);

    if (tabuleiro[jogador][linha][coluna].ocupado == 1) {
        strcpy(nomeImagem, tabuleiro[jogador][linha][coluna].nome_imagem);

        char navio[3] = { nomeImagem[13]+'\0', nomeImagem[14]+'\0' };
        char parte[2] = { nomeImagem[16]+'\0' };
        char orientacao[2] = { nomeImagem[20]+'\0' };
        char novoNomeImagem[200];

        snprintf(novoNomeImagem, sizeof novoNomeImagem, "imagens/%s/%s/%s_%s_l_%s.bmp", navio, orientacao, navio, parte, orientacao);


        tabuleiro[jogador][linha][coluna].imagem = load_bitmap(novoNomeImagem,NULL);
        strcpy(tabuleiro[jogador][linha][coluna].nome_imagem, novoNomeImagem);
        tabuleiro[jogador][linha][coluna].visivel = 1;

        int n;
        switch ( navio[1]){
            case '1':n=0;break;
            case '2':n=1;break;
            case '3':n=2;break;
            case '4':n=3;break;
            case '5':n=4;break;
        }

        if(vez==0){
            eu++;
        }
        else{
            pc++;
        }
        //printf("\njogador: %d",vez);
        //printf("\nnavio: %d",n);
        //printf("\npartes acertadas: %d",navios[vez][n].partes_acertadas);
        //printf("\nimagem: %s\n",novoNomeImagem);

    }
    else{
        tabuleiro[jogador][linha][coluna].imagem = load_bitmap("imagens/tabuleiro/q_la2.bmp",NULL);
        strcpy(tabuleiro[jogador][linha][coluna].nome_imagem, "imagens/tabuleiro/q_la2.bmp");
        tabuleiro[jogador][linha][coluna].visivel = 1;
        //printf("imagens/tabuleiro/q_la2.bmp");
    }
    //imprime_tabuleiro();
}


int ataqueComputador(){
    int valida,linha,coluna,at;
    at = 0;
    while(at == 0){
        randomQuadrante();
        //printf("%s",quadrante);
        valida = validaQuadrante(quadrante);

        if(valida == 1){
            linha = retornaLinha(quadrante);
            coluna = retornaColuna(quadrante);
            if(tabuleiro[0][linha][coluna].visivel==0){
                ataque(quadrante,0);
                at = 1;
            }
        }
    }
}

void inicia_navio(){
    int i,j;

    for(i=0;i<2;i++){
        for(j=0;j<5;j++){
            strcpy(navios[i][j].nome_navio,nome_navio[j]);
            navios[i][j].adicionado = 0;
        }
    }
    navios[0][0].total_partes = 1; navios[1][0].total_partes = 1;
    navios[0][1].total_partes = 2; navios[1][1].total_partes = 2;
    navios[0][2].total_partes = 1; navios[1][2].total_partes = 1;
    navios[0][3].total_partes = 2; navios[1][3].total_partes = 2;
    navios[0][4].total_partes = 3; navios[1][4].total_partes = 3;
}
int main()
{
    cont_q = 0;
    init();
    fundo_jogo();
    strcpy(nome_navio[0],"N1");strcpy(nome_navio[1],"N2");strcpy(nome_navio[2],"N3");strcpy(nome_navio[3],"N4");strcpy(nome_navio[4],"N5");

    //int add;
    tabuleiro_padrao(0); //tabuleiro do jogador
    tabuleiro_padrao(1); //tabuleiro do computador

    inicia_navio();
    posicionarNaviosComputador();
    //imprime_tabuleiro(1,0);
    //rest(5000);
    //tela_ataque();
    //char q[3];
    //randomQuadrante();
    //printf("%s",quadrante);
    //randomQuadrante();
    //printf("\n%s",quadrante);
    //randomQuadrante();
    //printf("\n%s",quadrante);
    //randomQuadrante();
    //printf("\n%s",quadrante);
    //randomQuadrante();
    //printf("\n%s",quadrante);
    tela_inicial();

    //tela_ataque();
	deinit();
	return 0;

}
END_OF_MAIN()
