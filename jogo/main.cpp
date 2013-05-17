#include <allegro.h>
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>

typedef struct
{
    BITMAP *imagem;
    char nome_imagem[60];
    int x;
    int y;
}TABULEIRO;

//variaveis globais
BITMAP *fundo;
BITMAP *buffer;
BITMAP *q_es;
BITMAP *q_cl;

//VARIVEL PRINCIPAL

TABULEIRO tabuleiro[5][8];
//int tabuleiro[5][8][2];
//BITMAP *tabuleiro_imagem[5][7];

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
}

inline void deinit()
{
	clear_keybuf();
	allegro_exit();
}

int retornaLinha(char quadrante[2]){
    int posicao;

    char letras[] = "ABCDEFGH";
    int i;
    for(i=0;i<8;i++){
        if( ((int)quadrante[0]) == ((int)letras[i])){
            posicao = i;
            printf("Posicao: %d",posicao);
        }
    }
    return posicao;
}

int retornaColuna(char quadrante[2]){
    char numero[] = "12345";
    int i,posicao;
    for(i=0;i<8;i++){
        if(quadrante[1]==numero[i]){
            posicao = i;
        }
    }
    return posicao;
}
//funcoes complexas para o jogo
void tabuleiro_padrao(){
    //fundo_jogo();
    int x = 44;
    int y = 171;
    q_es = load_bitmap("imagens/tabuleiro/q_es2.bmp", NULL);
    q_cl = load_bitmap("imagens/tabuleiro/q_cl2.bmp", NULL);

    for(int linha=0; linha<5;linha++){
        for(int coluna=0;coluna<8;coluna++){
            //Se a linha for par
            if(linha%2==0){
                //se a coluna for par
                if(coluna%2==0){
                    //insere a imagem escura
                    //draw_sprite(screen, q_es, x,y);
                    tabuleiro[linha][coluna].x = x;
                    tabuleiro[linha][coluna].y = y;
                    //strcpy(tabuleiro_imagem[linha][coluna],"q_es");

                    tabuleiro[linha][coluna].imagem = load_bitmap("imagens/tabuleiro/q_es2.bmp", NULL);
                    strcpy(tabuleiro[linha][coluna].nome_imagem,"imagens/tabuleiro/q_es2.bmp");
                    x = x+72;
                }
                else{
                    //se a coluna for impar
                    //insere a imagem clara
                    //draw_sprite(screen, q_cl, x,y);
                    tabuleiro[linha][coluna].x = x;
                    tabuleiro[linha][coluna].y = y;
                    //strcpy(tabuleiro_imagem[linha][coluna],"q_cl");
                    tabuleiro[linha][coluna].imagem = load_bitmap("imagens/tabuleiro/q_cl2.bmp", NULL);
                    strcpy(tabuleiro[linha][coluna].nome_imagem,"imagens/tabuleiro/q_cl2.bmp");
                    x = x+72;
                }
            }
            else{
                //Se a linha for impar
                //Se a coluna for par
                if(coluna%2==0){
                    //insere a imagem clara
                    //draw_sprite(screen, q_cl, x,y);
                    tabuleiro[linha][coluna].x = x;
                    tabuleiro[linha][coluna].y = y;
                    //strcpy(tabuleiro_imagem[linha][coluna],"q_cl");
                    tabuleiro[linha][coluna].imagem = load_bitmap("imagens/tabuleiro/q_cl2.bmp", NULL);
                    strcpy(tabuleiro[linha][coluna].nome_imagem,"imagens/tabuleiro/q_cl2.bmp");
                    x = x+72;
                }
                else{
                    //insere a imagem escura
                    //draw_sprite(screen, q_es, x,y);
                    tabuleiro[linha][coluna].x = x;
                    tabuleiro[linha][coluna].y = y;
                    //strcpy(tabuleiro_imagem[linha][coluna],"q_es");
                    tabuleiro[linha][coluna].imagem = load_bitmap("imagens/tabuleiro/q_es2.bmp", NULL);
                    strcpy(tabuleiro[linha][coluna].nome_imagem,"imagens/tabuleiro/q_es2.bmp");
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

void imprime_tabuleiro(){
    BITMAP *legenda_letras;
    legenda_letras = load_bitmap("imagens/legenda_letras.bmp", NULL);
    draw_sprite(screen, legenda_letras, 44,150);

    BITMAP *legenda_numeros;
    legenda_numeros = load_bitmap("imagens/legenda_numeros.bmp", NULL);
    draw_sprite(screen, legenda_numeros,14,185);

    for(int linha=0; linha<5;linha++){
        for(int coluna=0;coluna<8;coluna++){
                draw_sprite(screen, tabuleiro[linha][coluna].imagem, tabuleiro[linha][coluna].x,tabuleiro[linha][coluna].y);
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
        for (i = 0; i < tamanho; i++) vetor[i] = '\0'; // preenchendo o vetor com \O que indica o fim da string
        while ((tecla>>8 != KEY_ENTER)||(i<tamanho))  // laço até ser pressionada  a tecla enter, >>8 rotaciona 8 bits à esquerda
        {
                tecla = readkey();
                if (tecla >> 8 == KEY_BACKSPACE) //efeito de deletar um caracter
                {
                        vetor[cur_pos] = '\0'; // preenche com a terminação
                        cur_pos --;
                        if (cur_pos < 0) cur_pos = 0; // string começa na posição zero
                }

                BITMAP *letras;
                //clear(screen);// limpa tela

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
                int i;
                for(i = 0; i<9;i++){
                    if(((tecla & 0xff) == letras_ma[i]) || ((tecla & 0xff) == letras_mi[i])){
                        letras = load_bitmap(nome_imagem_letras[i], NULL);
                        draw_sprite(screen,letras,x,y);
                        i++;
                    }
                }
                for(i = 0; i<6;i++){
                    if((tecla & 0xff) == numeros[i]){
                        letras = load_bitmap(nome_imagem_numeros[i], NULL);
                        draw_sprite(screen,letras,x,y);
                        i++;
                    }
                }
                //clear(letras);
                //textprintf(screen, font, x, y, makecol(255, 0, 0), vetor);
                x = x+20;
                tamanho += 1;

                //textout_ex(screen,font, vetor, x,y, makecol(255, 0, 0),-1); //vai escrevendo na tela o que está sendo digitado, na posição e cor indicados
        }
}
     //allegro_message(vetor); // caixa mensagem com dados finais

//Funcoes de imagem para todas as telas
void fundo_jogo(){
    fundo = load_bitmap("imagens/inicio.bmp", NULL);
    buffer = create_bitmap(SCREEN_W, SCREEN_H);
    draw_sprite(buffer,fundo, 0, 0);
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
}
void sair(){
    if((mouse_x>= 675)&&(mouse_x <= 753)&&(mouse_y <= 95)&&(mouse_y >= 54))
    {
        if(mouse_b & 1)
            {
                exit(0); //textprintf(screen, font, 100, 200, makecol(255, 0, 0), "oiiii!");
            }
    }
}

void menu()
{
    while(!key[KEY_ESC]){
        BITMAP *menu;
        textprintf(screen, font, 20, 110, makecol(255, 0, 0), "X: %d Y: %d", mouse_x,mouse_y);
        menu = load_bitmap("imagens/voltar.bmp", NULL);
        draw_sprite(screen,menu, 300,496);
        destroy_bitmap(menu);
        //if((mouse_x>= 675)&&(mouse_x <=753)&&(mouse_y <=150)&&(mouse_y >=347))
        if(mouse_x > 400)
        {
            if(mouse_b & 1)
            {
                exit(0);
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
        BITMAP *credito_jogo;
        credito_jogo = load_bitmap("imagens/credito/credito_jogo.bmp", NULL);
        draw_sprite(screen, credito_jogo, 137,158);
        destroy_bitmap(credito_jogo);
        menu();
        sair();
    }
}

void tela_escolhe_navios(){
    fundo_jogo();
    imprime_tabuleiro();
    int navios;
    navios = 0;

    while(!key[KEY_ESC]){
        BITMAP *legenda_navio;
        legenda_navio = load_bitmap("imagens/navios_legenda.bmp", NULL);
        draw_sprite(screen, legenda_navio, 630,120);
        destroy_bitmap(legenda_navio);

        char n1[3];
        char quadrante[3];
        char orientacao[3];
        strcpy(n1,"");
        strcpy(quadrante,"");
        strcpy(orientacao,"");

        while(navios<5){
            BITMAP *legenda;
            legenda = load_bitmap("imagens/legenda.bmp", NULL);
            draw_sprite(screen, legenda, 25,500);


            //lendo_string(n1,5,60,570);

            //textprintf(screen, font, 20, 110, makecol(255, 0, 0), "Entre com o quadrante: ");

            lendo_string(quadrante,3,75,500);

            //textprintf(screen, font, 20, 110, makecol(255, 0, 0), "Entre com a orientação: ");

            //lendo_string(orientacao,2);

            navios++;
        }
        sair();
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
      textprintf(screen, font, 20, 110, makecol(255, 0, 0), "X: %d Y: %d", mouse_x,mouse_y);
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
                tela_instrucoes();
                break;
            }
      }
      if((mouse_x>= 315)and (mouse_x<=485) and (mouse_y >=200) and (mouse_y <= 390)){
          if (mouse_b & 1)
            {
                rest(1000);
                clear(screen);
                tela_escolhe_navios();
                break;
            }
      }
    }
}

int main()
{
    init();
    tabuleiro_padrao();
    tela_inicial();

	deinit();
	return 0;

}
END_OF_MAIN()
