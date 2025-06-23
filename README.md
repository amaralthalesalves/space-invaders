# Space Invaders (Allegro 5)

Este é um projeto de uma implementação básica do clássico jogo Space Invaders, desenvolvido em C utilizando a biblioteca Allegro 5.

## Visão Geral

O jogador controla uma nave espacial, representada por um triângulo, localizada na parte inferior da tela. A nave pode ser movida horizontalmente para a esquerda usando a tecla `A` e para a direita usando a tecla `D`.

Na parte superior da tela, uma formação de 5 linhas e 4 colunas de alienígenas se movimenta horizontalmente de forma coordenada. Quando os alienígenas chegam à borda direita da tela, eles descem uma linha para baixo e invertem a direção horizontal.

O objetivo principal do jogador é disparar projéteis, usando a tecla `ESPAÇO`, em todos os alienígenas. Cada vez que um alienígena é atingido, a pontuação do jogador aumenta em um ponto. O jogador vence ao atingir todos os alienígenas.

O jogo pode terminar de duas formas:
* Um alienígena da linha mais baixa atinge o chão.
* Um alienígena colide com a nave do jogador.

Ao final de cada partida, a pontuação atual é comparada com o `high score` salvo em um arquivo chamado `highscore.txt`. Se a pontuação atual for maior, ela substitui o `high score` anterior, tornando-se o novo recorde.

Uma tela final é exibida, mostrando informações relevantes sobre o desfecho do jogo. Nesta tela, o jogador tem as seguintes opções:
* Pressionar `R` para tentar novamente.
* Pressionar `ESC` para sair do jogo.

## Imagens

### Tela durante o jogo
![Imagem da tela durante o jogo.](Documento%20sem%20t%C3%ADtulo.pdf#page=1&zoom=auto,-0.279,720.72)

### Tela final do jogo
![Imagem da tela final do jogo.](Documento%20sem%20t%C3%ADtulo.pdf#page=2&zoom=auto,-0.279,720.72)

## Dependências

O jogo utiliza as seguintes bibliotecas e cabeçalhos padrão do C e da Allegro 5:

* `stdio.h`
* `stdlib.h`
* `allegro5/allegro.h`
* `allegro5/allegro_font.h`
* `allegro5/allegro_ttf.h`
* `allegro5/allegro_primitives.h`

## Compilação

Para compilar o `invaders.c`, é necessário ter a Allegro 5 configurada em seu ambiente de desenvolvimento. O projeto inclui um `Makefile`, que facilita o processo de compilação.

**Nota:** O `Makefile` foi desenvolvido e testado em ambiente Linux, portanto, seu funcionamento é garantido **APENAS** neste sistema operacional.

No terminal, siga os comandos abaixo para compilar e executar o jogo:

```bash
make invaders
./invaders
