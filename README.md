Documentação TP Allegro
Visão Geral
O jogo é uma implementação básica do clássico jogo Space Invaders, feito em C, utilizando a biblioteca Allegro 5. O jogador controla uma nave espacial, representada por um triângulo, localizada na parte inferior da tela, movendo-a horizontalmente, da esquerda para a direita, usando as teclas A e D, respectivamente. Na parte superior da tela, cinco linhas e quatro colunas de alienígenas se movimentam horizontalmente de forma coordenada e, ao chegar na borda direita da tela, descem uma linha para baixo. O objetivo do jogador é disparar, usando a tecla SPACE, projéteis em todos os alienígenas. Toda vez que um alienígena é atingido, a pontuação do jogador aumenta em um. O jogador vence quando todos os alienígenas tiverem sido atingidos por projéteis. O jogo pode acabar quando o alienígena da linha mais baixa atingir tanto o chão, quanto a nave. Ao final do jogo, se o jogador ganhou ou perdeu, é calculado, por acesso a um arquivo nomeado highscore.txt, se a pontuação do jogo atual é maior que a retornada pelo arquivo, a pontuação do arquivo é substituída pela atual, se tornando o novo high score. Na tela final, são mostradas informações, como demonstrado abaixo. É possível apertar R para tentar novamente ou ESC para sair do jogo.
Imagens

Imagem da tela durante o jogo.

Imagem da tela final do jogo.
Dependências
O jogo utiliza as seguintes dependências:
stdio.h
stdlib.h
allegro5/allegro.h
allegro5/allegro_font.h
allegro5/allegro_ttf.h
allegro5/allegro_primitives.h
Compilação
Para compilar o invaders.c, você precisará ter o Allegro 5 configurado em seu ambiente de desenvolvimento. Você pode usar o makefile, arquivo de instrução de compilação. Como o jogo foi desenvolvido em Linux, o funcionamento do makefile é garantido APENAS neste Sistema Operacional.
No terminal, digite as linhas a seguir para compilar e abrir o jogo:
make invaders
./invaders
Constantes de jogo
As constantes globais definem parâmetros importantes do jogo, como dimensões da tela, tamanhos de objetos, velocidades e configurações de jogo. Todas elas podem ser alteradas facilmente, e servem para ajustar diversos aspectos do jogo, como dificuldade.
ALIENS_ROWS: Número de linhas de alienígenas.
ALIENS_COLUMNS: Número de colunas de alienígenas.
SCREEN_W: Largura da tela do jogo (960 pixels).
SCREEN_H: Altura da tela do jogo (540 pixels).
FPS: Quadros por segundo (o padrão do jogo é 144.0).
GROUND_H: Altura do "chão" na parte inferior da tela.
GROUND_W: Largura do "chão" (igual à largura da tela).
SHIP_W: Largura da nave do jogador.
SHIP_H: Altura da nave do jogador.
SHIP_SPEED: Velocidade de movimento horizontal da nave.
Y_BASE: Posição Y da base da nave (usada para o triângulo).
BULLET_W: Largura do tiro da nave.
BULLET_H: Altura do tiro da nave.
BULLET_SPEED: Velocidade vertical do tiro.
ALIEN_W: Largura de um alienígena.
ALIEN_H: Altura de um alienígena.
ALIEN_SPACING: Espaçamento entre os alienígenas.
ALIEN_SPEED_X: Velocidade de movimento horizontal dos alienígenas.
ALIEN_SPEED_Y: Distância que os alienígenas descem verticalmente quando atingem a borda da tela.
FONT_SIZE: Tamanho da fonte para o texto do jogo.
FONT_PATH: Caminho para o arquivo da fonte TTF.
GAME_PLAYING: Estado do jogo quando está em andamento (1).
GAME_OVER: Estado do jogo quando a partida terminou (2).
GAME_EXIT: Estado para sair do jogo (0).
Estruturas de dados
O jogo utiliza três estruturas de dados (structs) para representar os objetos na tela.
Ship - representa a nave que o jogador controla.
Alien - representa cada um dos inimigos, de forma individual.
Bullets - representa os projéteis do jogo.
Funções
As funções são agrupadas pelas responsabilidades.
7.1. Inicialização
void init_ship(struct Ship *ship)
Inicializa os atributos da nave: posição inicial (centro da tela), cor, dimensões e velocidade.
void init_aliens(struct Alien aliens[ALIENS_ROWS][ALIENS_COLUMNS])
Inicializa a matriz de alienígenas. Calcula suas posições iniciais para centralizá-los, define a cor, dimensões, velocidades e marca-os como vivos (is_alive = 1).
void init_bullet(struct Bullets* bullet)
Inicializa os atributos de um novo tiro: dimensões, posição inicial (acima da nave), velocidade e cor. Marca-o como ativo (active = 1).
void deactivate_bullet(struct Bullets* bullet)
Desativa um tiro, movendo-o para fora da tela e definindo active = 0. Usado após uma colisão ou quando o tiro sai da tela.
7.2. Desenhar na tela
void draw_scenario()
Desenha elementos do cenário, sendo eles o fundo (todo preto), e o chão.
void draw_score(ALLEGRO_FONT *font, int score)
Desenha a pontuação atual no canto superior esquerdo da tela.
void draw_ship(struct Ship ship)
Desenha a nave do jogador como um triângulo preenchido na tela.
void draw_alien(struct Alien alien)
Desenha um alienígena como um retângulo preenchido, somente se alien.is_alive for 1.
void draw_bullet(struct Bullets bullet)
Desenha o tiro como um retângulo preenchido, somente se bullet.active for 1.
7.3. Movimento
void move_ship(struct Ship *ship)
Atualiza a posição horizontal da nave com base nos valores de ship.right e ship.left, e impede que a nave saia dos limites da tela.
void move_aliens_vertically(struct Alien aliens[ALIENS_ROWS][ALIENS_COLUMNS])
Move todos os alienígenas vivos para baixo pela distância ALIEN_SPEED_Y e inverte a direção horizontal (speed_x) de todos os alienígenas. Esta função é chamada quando qualquer alienígena atinge a borda da tela.
void move_alien(struct Alien *alien)
Move um alienígena individual horizontalmente e verifica se o próximo movimento do alienígena fará com que ele colida com as bordas da tela. Se sim, chama move_aliens_vertically para todos os alienígenas.
void move_bullet(struct Bullets *bullet)
Move o tiro verticalmente para cima e desativa o tiro se ele sair da tela (atingir a parte superior).
7.4. Controle do jogo

int read_high_score()
Abre o arquivo highscore.txt em modo leitura e verifica se ele existe. Se existir, lê o valor dentro dele e define como o current_high_score atual (variável global). Se não existir, cria o arquivo highscore.txt e define o current_high_score como 0.
void update_high_score(int score)
	Compara o score atual (atualizado durante o jogo toda vez que há colisão entre uma bala e um alien) com o high_score retornado da função read_high_score. Se for maior, abre o arquivo highscore.txt e escreve o score atual nele. Se não, nada acontece.
void reset_game()
	Toda vez que o jogador seleciona para começar uma nova partida, a função chama as funções de inicialização em todos os objetos da tela, para restaurá-los aos valores iniciais.
void draw_game_over_screen(ALLEGRO_FONT *font, int final_score, int high_score_val)
	Desenha a tela de game-over. É chamada toda vez que há colisão de um alien com a nave, com o chão, ou quando o jogo acaba (todos os aliens foram derrotados).
int check_all_aliens_dead(struct Alien aliens[ALIENS_ROWS][ALIENS_COLUMNS])
	Verifica o valor is_alive de todos os aliens na matriz. Se um estiver vivo (ou seja, seu is_alive é 1), retorna 0, já que há um alien vivo e o jogo não acabou. Caso nenhum esteja vivo, retorna 1, e o jogo acaba.

