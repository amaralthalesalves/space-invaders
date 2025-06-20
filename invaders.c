#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

// -------------------- CONSTANTES DE JOGO --------------------
#define ALIENS_ROWS      4
#define ALIENS_COLUMNS   5

const int SCREEN_W = 960;
const int SCREEN_H = 540;
const float FPS = 144.0;

// Chão
const int GROUND_H = 80;
const int GROUND_W = 960; // Pode usar SCREEN_W diretamente

// Ship
const int SHIP_W = 80;
const int SHIP_H = 40;
const float SHIP_SPEED = 1.5;
const int Y_BASE = (SCREEN_H - GROUND_H / 2);


// Bullet
const int BULLET_W = 5;
const int BULLET_H = 5;
const float BULLET_SPEED = 1.5;

// Aliens
const int ALIEN_W = 40;
const int ALIEN_H = 40;
const int ALIEN_SPACING = 20;
const float ALIEN_SPEED_X = 2.5;
const float ALIEN_SPEED_Y = 40.0; // Igual à altura do alien

// Fonte
const int FONT_SIZE = 24;
const char* FONT_PATH = "spaceage.ttf"; // Certifique-se que esta fonte existe ou use al_create_builtin_font()

// Game states (CORRIGIDO: valores únicos para cada estado)
const int GAME_PLAYING = 1;
const int GAME_OVER = 2;
const int GAME_EXIT = 0;

//-------------------------- ESTRUTURAS -------------------
// Estrutura para a ship
struct Ship{
    float x; // Posição da ship
    ALLEGRO_COLOR color; // Cor da ship
    float width, height; // Dimensões da ship
    float speed; // Velocidade da ship
    int left, right; // Direção da ship
};
struct Ship ship;

void init_ship(struct Ship *ship){
    ship->x = SCREEN_W / 2; // Posição inicial da ship no centro da tela
    ship->color = al_map_rgb(200, 200, 200); // Cor branca para a ship
    ship->width = SHIP_W; // Largura da ship
    ship->height = SHIP_H; // Altura da ship
    ship->speed = SHIP_SPEED; // Velocidade da ship
    ship->left = 0; // Direção inicial da ship (esquerda)
    ship->right = 0; // Direção inicial da ship (direita)
}

// Estrutura para os aliens
struct Alien{
    float x, y; // Posição do alien
    ALLEGRO_COLOR color; // Cor do alien
    float width, height; // Dimensões do alien
    float speed_x, speed_y; // Velocidade do alien
    int is_alive;
};
struct Alien aliens[ALIENS_ROWS][ALIENS_COLUMNS];

void init_aliens(struct Alien aliens[ALIENS_ROWS][ALIENS_COLUMNS]) {
    // Ajuste para centralizar os aliens na tela
    float total_aliens_width = ALIENS_COLUMNS * ALIEN_W + (ALIENS_COLUMNS - 1) * ALIEN_SPACING;
    float start_x_offset = (SCREEN_W - total_aliens_width) / 2.0;

    for(int i = 0; i < ALIENS_ROWS; i++){
        for (int j = 0; j < ALIENS_COLUMNS; j++){
            aliens[i][j].x = start_x_offset + j * (ALIEN_W + ALIEN_SPACING);
            aliens[i][j].y = 50 + i * (ALIEN_H + ALIEN_SPACING); // Começa um pouco abaixo do topo
            aliens[i][j].color = al_map_rgb(0, 255, 0);
            aliens[i][j].width = ALIEN_W;
            aliens[i][j].height = ALIEN_H;
            aliens[i][j].speed_x = ALIEN_SPEED_X;
            aliens[i][j].speed_y = ALIEN_SPEED_Y;
            aliens[i][j].is_alive = 1;
        }
    }
}

// Estrutura dos tiros
struct Bullets{
    int width;
    int height;
    int x;
    int y;
    int active;
    float d_y;
    ALLEGRO_COLOR color;
};
struct Bullets bullet;

void init_bullet(struct Bullets* bullet){
    bullet->width = BULLET_W;
    bullet->height = BULLET_H;
    bullet->x = ship.x - bullet->width / 2;
    bullet->y = SCREEN_H - GROUND_H - ship.height;
    bullet->d_y = BULLET_SPEED;
    bullet->color = al_map_rgb(200, 200, 200); // Cor branca para a bala
    bullet->active = 1;
}

// Desativa a bala, movendo-a para fora da tela
void deactivate_bullet(struct Bullets* bullet) {
    bullet->active = 0;
    bullet->x = -BULLET_W;
    bullet->y = -BULLET_H;
}

// --------------------------- FUNÇÕES -------------------
// Desenha o cenário do jogo
void draw_scenario(){
    al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa a tela com a cor preta
    // Desenha o chão
    al_draw_filled_rectangle(0, SCREEN_H - GROUND_H, GROUND_W, SCREEN_H, al_map_rgb(100, 100, 100)); // Desenha o chão
}
//Desenha o score
void draw_score(ALLEGRO_FONT *font, int score) {
    al_draw_textf(
        font, al_map_rgb(255, 255, 255), 
        10, 10,                          
        0,                               
        "Score: %d", score              
    );
}

// Desenha a ship
void draw_ship(struct Ship ship){
    al_draw_filled_triangle(
        ship.x - (ship.width / 2), Y_BASE - ship.height, // Ponto inferior esquerdo
        ship.x + ship.width / 2, Y_BASE - ship.height, // Ponto inferior direito
        ship.x, Y_BASE - ship.height * 2, // Ponto superior
        ship.color // Cor da ship
    );
}

// Atualiza a posição da ship com base na direção
void move_ship(struct Ship *ship) {
    if (ship->left) {
        ship->x -= ship->speed; // Move a ship para a esquerda
        if (ship->x < ship->width / 2) {
            ship->x = ship->width / 2; // Limita a posição mínima da ship
        }
    }
    if (ship->right) {
        ship->x += ship->speed; // Move a ship para a direita
        if (ship->x > SCREEN_W - ship->width / 2) {
            ship->x = SCREEN_W - ship->width / 2; // Limita a posição máxima da ship
        }
    }
}

// Desenha o alien
void draw_alien(struct Alien alien) {
    if(!alien.is_alive) {
        return; // Se o alien não está vivo, não desenha
    }
    al_draw_filled_rectangle(
        alien.x, alien.y, alien.x + alien.width, alien.y + alien.height, // Define os pontos do retângulo
        alien.color // Cor do alien
    );
}

// Movimenta os aliens verticalmente (quando atingem borda horizontal)
void move_aliens_vertically(struct Alien aliens[ALIENS_ROWS][ALIENS_COLUMNS]){
    for(int i = 0; i < ALIENS_ROWS; i++){
        for(int j = 0; j < ALIENS_COLUMNS; j++){
            // Apenas move aliens que estão vivos
            if (aliens[i][j].is_alive) {
                aliens[i][j].y += aliens[i][j].speed_y;
            }
        }
    }
    // Inverte a direção horizontal de TODOS os aliens após uma mudança de linha
    for(int i = 0; i < ALIENS_ROWS; i++){
        for(int j = 0; j < ALIENS_COLUMNS; j++){
            aliens[i][j].speed_x *= -1; // Inverte a direção horizontal
        }
    }
}

// Move um alien individualmente, tratando colisão com bordas para mover todos verticalmente
void move_alien(struct Alien *alien) {
    if (!alien->is_alive) { // Se o alien não está vivo, não o mova
        return;
    }
    // Verifica se o próximo movimento fará o alien colidir com a borda
    if(alien->x + alien->width + alien->speed_x > SCREEN_W || alien->x + alien->speed_x < 0) {
        // Se houver colisão com a borda, todos os aliens se movem verticalmente e invertem a direção horizontal
        move_aliens_vertically(aliens);
    }
    alien->x += alien->speed_x; // Move o alien horizontalmente
}

// Desenha a bala
void draw_bullet(struct Bullets bullet){
    if(!bullet.active){
        return;
    }
    al_draw_filled_rectangle(
        bullet.x, bullet.y, bullet.x + bullet.width, bullet.y + bullet.height,
        bullet.color
    );
}

// Move a bala para cima
void move_bullet(struct Bullets *bullet){
    if (!bullet->active) {
        return;
    }
    bullet->y -= bullet->d_y;
    // Desativa a bala se ela sair da tela
    if (bullet->y < 0) {
        bullet->active = 0;
    }
}

//VERIFICAR COLISÕES

// Verificar colisão do alien com o chão
int alien_ground_collision(struct Alien *alien) {
    if (alien->is_alive && alien->y + alien->height >= SCREEN_H - GROUND_H) {
        return 0; // Retorna 0 se o alien atingir o chão
    }
    return 1; // Retorna 1 se o alien não atingir o chão
}

//Verificar colisão da bala com o alien
void alien_bullet_colision(struct Alien *alien, struct Bullets *bullet, int* score){
    if(alien->is_alive && bullet->active &&
    // Verifica se o retângulo da bala se sobrepõe ao retângulo do alien
    bullet->x < alien->x + alien->width &&
    bullet->x + bullet->width > alien->x &&
    bullet->y < alien->y + alien->height &&
    bullet->y + bullet->height > alien->y){
        (*score)++; // CORREÇÃO APLICADA AQUI
        alien->is_alive = 0;
        bullet->active = 0; // Desativa a bala
    }
}

//Colisão entre o alien e a ship
int alien_ship_colision(struct Alien *alien, struct Ship ship){
    if(alien->is_alive &&
    // Verifica se os retângulos se sobrepõem 
    alien->x < ship.x + ship.width / 2 &&
    alien->x + alien->width > ship.x - ship.width / 2 &&
    alien->y < Y_BASE - ship.height &&
    alien->y + alien->height > Y_BASE - ship.height * 2) {
        return 0; // colisão ocorreu
    }
    return 1; // sem colisão
}

// --- FUNÇÕES DE HIGH SCORE E GAME OVER ---

// Lê o high score atual do arquivo
int read_high_score() {
    FILE* highscore_file = fopen("highscore.txt", "r");
    int current_high_score = 0;
    if (highscore_file) {
        fscanf(highscore_file, "%d", &current_high_score);
        fclose(highscore_file);
    } else {
        // Se o arquivo não existe, cria um e escreve 0 como high score inicial
        highscore_file = fopen("highscore.txt", "w");
        if (highscore_file) {
            fprintf(highscore_file, "0");
            fclose(highscore_file);
        }
    }
    return current_high_score;
}

// Armazena o high score no arquivo, se a pontuação atual for maior
void update_high_score(int score){
    int current_high_score = read_high_score(); 
    if(score > current_high_score){
        FILE* highscore_file = fopen("highscore.txt", "w");
        if (highscore_file) {
            fprintf(highscore_file, "%d", score);
            fclose(highscore_file);
        } else {
            fprintf(stderr, "Falha ao abrir o arquivo para salvar o high score!\n");
        }
    }
}

// Função para reiniciar o jogo
void reset_game(){
    init_ship(&ship); // Reinicializa a nave
    init_aliens(aliens); // Reinicializa os aliens
    deactivate_bullet(&bullet); // Garante que a bala está inativa
}


// Desenha a tela de "Game Over"
void draw_game_over_screen(ALLEGRO_FONT *font, int final_score, int high_score_val){ // Adicionado high_score_val
    al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa a tela
    al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_W / 2, SCREEN_H / 2 - 80, ALLEGRO_ALIGN_CENTER, "GAME OVER!");
    al_draw_textf(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 - 30, ALLEGRO_ALIGN_CENTER, "Sua Pontuacao: %d", final_score);
    al_draw_textf(font, al_map_rgb(255, 255, 0), SCREEN_W / 2, SCREEN_H / 2 + 10, ALLEGRO_ALIGN_CENTER, "High Score: %d", high_score_val); // Exibe o high score
    al_draw_text(font, al_map_rgb(0, 255, 0), SCREEN_W / 2, SCREEN_H / 2 + 60, ALLEGRO_ALIGN_CENTER, "Pressione 'R' para Tentar Novamente");
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 100, ALLEGRO_ALIGN_CENTER, "Pressione 'ESC' para Sair");
}

// Checa se todos os aliens estão mortos
int check_all_aliens_dead(struct Alien aliens[ALIENS_ROWS][ALIENS_COLUMNS]){
    for(int i = 0; i < ALIENS_ROWS; i++){
        for(int j = 0; j < ALIENS_COLUMNS; j++){
            if(aliens[i][j].is_alive == 1){
                return 0; // Ainda há aliens vivos
            }
        }
    }
    return 1; // Todos os aliens foram eliminados
}

int main(void){
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font = NULL;

    // ------------------------- INICIALIZADORES -------------------
    // Inicializador Allegro
    if(!al_init()){
        fprintf(stderr, "Falha ao inicializar Allegro\n");
        return -1;
    }

    // Inicializador do timer
    timer = al_create_timer(1.0/FPS);
    if(!timer){
        fprintf(stderr, "Falha ao inicializar timer\n");
        return -1;
    }

    // Inicializador do addon de primitivas
    if(!al_init_primitives_addon()) {
        fprintf(stderr, "Falha ao inicializar addon de primitivas!\n");
        al_destroy_timer(timer);
        return -1;
    }

    // Inicializador do addon de fontes
    if(!al_init_font_addon()) {
        fprintf(stderr, "Falha ao inicializar addon de fonte!\n");
        al_destroy_timer(timer);
        al_shutdown_primitives_addon();
        return -1;
    }

    // Inicializador do display
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display){
        fprintf(stderr, "Falha ao inicializar display\n");
        al_destroy_timer(timer);
        al_shutdown_primitives_addon();
        al_shutdown_font_addon();
        return -1;
    }

    // Inicializador da fonte (usando uma fonte embutida para evitar problemas de caminho de arquivo)
    // Se quiser usar 'spaceage.ttf', certifique-se de que o arquivo está no diretório correto e use al_load_ttf_font
    font = al_create_builtin_font(); 
    if (!font) {
        fprintf(stderr, "Failed to load font! Attempting to load TTF font.\n");
        font = al_load_ttf_font(FONT_PATH, FONT_SIZE, 0); // Tenta carregar sua fonte TTF
        if (!font) {
            fprintf(stderr, "Failed to load TTF font and builtin font not available!\n");
            al_destroy_display(display);
            al_destroy_timer(timer);
            al_shutdown_primitives_addon();
            al_shutdown_font_addon();
            return -1;
        }
    }

    // Inicializador da fila de eventos
    event_queue = al_create_event_queue();
    if(!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_font(font);
        al_shutdown_primitives_addon();
        al_shutdown_font_addon();
        return -1;
    }

    // Inicializador do teclado
    if(!al_install_keyboard()) {
        fprintf(stderr, "failed to install keyboard!\n");
        al_destroy_event_queue(event_queue);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_font(font);
        al_shutdown_primitives_addon();
        al_shutdown_font_addon();
        return -1;
    }

    // Inicializador do mouse (não usado para o botão "Tentar Novamente" por enquanto, mas bom ter)
    if(!al_install_mouse()) {
        fprintf(stderr, "failed to initialize mouse!\n");
        al_uninstall_keyboard();
        al_destroy_event_queue(event_queue);
        al_destroy_display(display);
        al_destroy_timer(timer);
        al_destroy_font(font);
        al_shutdown_primitives_addon();
        al_shutdown_font_addon();
        return -1;
    }

    // ------------------------- REGISTRADORES -------------------
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // ------------------------- LOOP PRINCIPAL -------------------

    int current_game_state = GAME_PLAYING; 
    int redraw = 1;
    int score = 0; // Pontuação da partida atual
    int high_score = read_high_score(); // Lê o high score no início do jogo

    al_start_timer(timer);

    // Reseta o jogo para o seu estado padrão
    reset_game(); 

    while(current_game_state != GAME_EXIT){
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev); // Espera por um evento para economizar CPU

        if(ev.type == ALLEGRO_EVENT_TIMER){
            redraw = 1;
            
            // Lógica de atualização do jogo SOMENTE se estiver jogando
            if (current_game_state == GAME_PLAYING) {
                move_ship(&ship);
                move_bullet(&bullet);

                // Controlar todos os aliens e verificar fim de jogo por colisão
                int game_ended_by_collision = 0;
                for(int i = 0; i < ALIENS_ROWS; i++){
                    for(int j = 0; j < ALIENS_COLUMNS; j++){
                        if (aliens[i][j].is_alive) { // Move apenas aliens vivos
                            move_alien(&aliens[i][j]);
                            alien_bullet_colision(&aliens[i][j], &bullet, &score); // Passando &score
                            if(!alien_ground_collision(&aliens[i][j]) || !alien_ship_colision(&aliens[i][j], ship)) {
                                game_ended_by_collision = 1; // Marca que uma colisão fatal ocorreu
                            }
                        }
                    }
                }
                
                // Se o jogo terminou por colisão ou todos os aliens morreram, muda para GAME_OVER
                if (game_ended_by_collision || check_all_aliens_dead(aliens)) {
                    update_high_score(score); // Atualiza o high score antes de ir para a tela de game over
                    high_score = read_high_score(); // Lê o high score atualizado para exibir na tela
                    current_game_state = GAME_OVER;
                }
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_ESCAPE:
                    current_game_state = GAME_EXIT; // Sai do loop principal
                    break;
                case ALLEGRO_KEY_SPACE:
                    // Se estiver jogando e não houver bala ativa, atira
                    if (current_game_state == GAME_PLAYING && !bullet.active) {
                        init_bullet(&bullet);
                    }
                    break;
                case ALLEGRO_KEY_R:
                    // Se estiver na tela de Game Over e 'R' for pressionado, reinicia
                    if (current_game_state == GAME_OVER) {
                        reset_game(); // Reinicializa posições e estados
                        score = 0; // Zera a pontuação para a nova partida
                        current_game_state = GAME_PLAYING; // Volta para o estado de jogo
                    }
                    break;
                case ALLEGRO_KEY_A:
                    if (current_game_state == GAME_PLAYING) {
                        ship.left = 1;
                    }
                    break;
                case ALLEGRO_KEY_D:
                    if (current_game_state == GAME_PLAYING) {
                        ship.right = 1;
                    }
                    break;
            }
        } 
        else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_A:
                    ship.left = 0;
                    break;
                case ALLEGRO_KEY_D:
                    ship.right = 0;
                    break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            current_game_state = GAME_EXIT;
        }

        // --- SEÇÃO DE DESENHO ---
        // Desenha apenas se 'redraw' for verdadeiro e a fila de eventos estiver vazia para evitar flickr
        if(redraw && al_is_event_queue_empty(event_queue)) { 
            redraw = 0;
            
            if (current_game_state == GAME_PLAYING) {
                draw_scenario();
                draw_ship(ship);
                draw_bullet(bullet);
                draw_score(font, score);
                for(int i = 0; i < ALIENS_ROWS; i++){
                    for(int j = 0; j < ALIENS_COLUMNS; j++){
                        draw_alien(aliens[i][j]);
                    }
                }
            } else if (current_game_state == GAME_OVER) {
                // Passa a pontuação final e o high score para a tela de Game Over
                draw_game_over_screen(font, score, high_score);
            }
            al_flip_display(); // Atualiza a tela UMA VEZ por frame
        }
    }

    // ------------------------- FINALIZADORES -------------------
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    // Não é necessário fechar high_score_file aqui, pois ele é fechado nas funções read/update

    return 0;
}
