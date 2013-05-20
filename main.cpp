#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>

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
    int partes_acertadas;
    int destruido;
    int adicionado;
}NAVIO;

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
                    strcpy(tabuleiro[jogador][linha][coluna].nome_imagem_padrao,"imagens/tabuleiro/q_cl2.bmp");
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

void destruiuNavio(int jogador, char navio[3]){
       int n;
       switch ( navio[1]){
       case '1':n=0;break;
       case '2':n=1;break;
       case'3':n=2;break;
       case'4':n=3;break;
       case'5':n=4;break;

    if(navios[jogador][n].total_partes  == navios[jogador][n].partes_acertadas){
        navios[jogador][n].destruido = 1;
    }
  }
}

int posicionaNavio(int jogador){
    //impedir que o usuário insira um navio duas vezes

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

    for(i=0;i<t_partes;i++){
        if(int_orientacao==1){//vertical
            if(linha_m <=5){
                if(tabuleiro[0][linha_m][coluna].ocupado == 1){
                    validacao +=1;
                }
            }
            linha_m ++;
        }
        else{//horizontal
            if(coluna_m <=8){
                if(tabuleiro[jogador][linha][coluna_m].ocupado == 1){
                    validacao +=1;
                }
            }
            coluna_m  ++;
        }
    }

    if(navios[jogador][n_navio].adicionado == 1){
        validacao = 1;
    }
    linha_m = linha;
    coluna_m = coluna;

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

                linha_m += 1;

                //printf("\ncoluna_m: %d",coluna_m);
                //printf("\nlinha: %d",linha);
                printf("\nimagem: %s",novoNomeImagem);
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

                coluna_m  += 1;

                //printf("\ncoluna_m: %d",coluna_m);
                //printf("\nlinha: %d",linha);
                printf("\nimagem: %s",novoNomeImagem);

           }
        }
        navios[jogador][n_navio].adicionado = 1;
        return 1;
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
                draw_sprite(screen,legenda_l, 25,500);            }
            else{
                imprime_tabuleiro(0,0);
                qtt_navios+=1;
                strcpy(navio,"");
                strcpy(quadrante,"");
                strcpy(orientacao,"");
                draw_sprite(screen,legenda_l, 25,500);
            }
        }
        sair();
    }
}

void posicionarNaviosComputador(){
   //Enquanto todos os navios não forem preenchidos
   //sortear um quadrante e  orientacao
  //verificar se os quadrantes estão ocupados
  //se sim sortear de novo
  //se não posicionar navios: trocar imagens
  //trocar caminho da imagem
  //marcar como ocupado os quadrantes
  //preencher a variavel navios[1] com as informações
    //random(10);
    int nav = 1;
    int add = 0;
    int valida = 0;
    char rand_letras[] = "ABCDEFGH";
    char rand_numeros[] = "12345";
    char rand_orientacao[2][3];
    strcpy(rand_orientacao[0],"01");
    strcpy(rand_orientacao[1],"02");
    while(nav<=5){
        srand(time(NULL));
        int r_l = rand() %8;
        int r_n = rand() %5;
        int r_nav = rand() %2;

        char q[] = {rand_letras[r_l]+'\0', rand_numeros[r_n]+'\0'};
        char n[] = {'n\0',rand_numeros[nav]};
        char o[3];
        //strcpy(o,);

        strcpy(navio,n);
        strcpy(orientacao,rand_orientacao[r_nav]);

        valida = validaQuadrante(q);
        if(valida == 0){
            strcpy(quadrante,q);

            add = posicionaNavio(1);
            if(add == 1){
                nav++;
            }
        }
    }
    strcpy(quadrante,"");
    strcpy(navio,"");
    strcpy(orientacao,"");
}

void tela_ataque(){
    fundo_jogo();
    imprime_tabuleiro(1,1);
    while(!key[KEY_ESC]){

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

void ataque(char quadrante[3]){
    tabuleiro[0][0][0].imagem = load_bitmap("imagens/n1/h/n1_1_cl_h.bmp",NULL);
    strcpy(tabuleiro[0][0][0].nome_imagem,"imagens/n1/h/n1_1_cl_h.bmp");

    int linha, coluna;
    char nomeImagem[60];

    linha = retornaLinha(quadrante);
    coluna = retornaColuna(quadrante);

    if (tabuleiro[0][linha][coluna].ocupado == 1) {
        strcpy(nomeImagem, tabuleiro[0][linha][coluna].nome_imagem);

        char navio[3] = { nomeImagem[13]+'\0', nomeImagem[14]+'\0' };
        char parte[2] = { nomeImagem[16]+'\0' };
        char orientacao[2] = { nomeImagem[21]+'\0' };
        char novoNomeImagem[200];

        snprintf(novoNomeImagem, sizeof novoNomeImagem, "imagens/%s/%s/%s_%s_l_%s.bmp", navio, orientacao, navio, parte, orientacao);

// TODO: Fazer quando tiver pronto o BITMAP do Allegro
        tabuleiro[0][linha][coluna].imagem = load_bitmap(novoNomeImagem,NULL);
        strcpy(tabuleiro[0][linha][coluna].nome_imagem, novoNomeImagem);
        tabuleiro[0][linha][coluna].visivel = 1;

        //textprintf(screen, font, 20, 110, makecol(255, 0, 0), "novoNomeImagem[60]: %s",novoNomeImagem);
        //textprintf(screen, font, 20, 210, makecol(255, 0, 0), "navio: %s",navio);
        //textprintf(screen, font, 20, 310, makecol(255, 0, 0), "orientacao: %s",orientacao);

    }
    else{
// TODO: Fazer quando tiver pronto o BITMAP do Allegro
        tabuleiro[0][linha][coluna].imagem = load_bitmap("imagens/tabuleiro/q_la.bmp",NULL);
        strcpy(tabuleiro[0][linha][coluna].nome_imagem, "imagens/tabuleiro/q_la.bmp");
    }
    //imprime_tabuleiro();
}

void inicia_navio(){
    int i,j;

    for(i=0;i<2;i++){
        for(j=0;j<5;j++){
            strcpy(navios[i][j].nome_navio,nome_navio[j]);
            navios[i][j].partes_acertadas = 0;
            navios[i][j].destruido = 0;
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
    init();
    fundo_jogo();
    strcpy(nome_navio[0],"N1");strcpy(nome_navio[1],"N2");strcpy(nome_navio[2],"N3");strcpy(nome_navio[3],"N4");strcpy(nome_navio[4],"N5");

    //int add;
    tabuleiro_padrao(0); //tabuleiro do jogador
    tabuleiro_padrao(1); //tabuleiro do computador
    inicia_navio();

    //printf("rand_letra %c",rand_letras[r_l]);
    //printf("rand_numero %c",rand_numeros[r_n]);
    //printf("\nr_l %d",r_l);
    //printf("\nr_n %d",r_n);
    //printf("quadrante: %s",quadrante);

    tela_inicial();
    //lendo_string(navio,3,235,510);
    //lendo_string(quadrante,3,235,540);
    //lendo_string(orientacao,3,235,570);
    //add = posicionaNavio();

    //posicionarNaviosComputador();
	deinit();
	return 0;

}
END_OF_MAIN()
