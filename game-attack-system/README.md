### Project Structure

1. **Project Setup**
   - Create a new directory for your project.
   - Initialize a Git repository (optional).
   - Create the following directory structure:
     ```
     /my_game
     ├── src/
     │   ├── main.c
     │   ├── player.c
     │   ├── enemy.c
     │   ├── attack.c
     │   └── utils.c
     ├── include/
     │   ├── game.h
     ├── assets/
     │   ├── badsprite_attack.xpm
     │   └── player_attack.xpm
     ├── Makefile
     └── README.md
     ```

2. **Dependencies**
   - Ensure you have MiniLibX installed. You can find it [here](https://github.com/42School/minilibx-linux).
   - Link against the necessary libraries in your Makefile.

### Code Implementation

#### 1. `game.h` (Header File)

```c
#ifndef GAME_H
#define GAME_H

#include <mlx.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct s_player {
    int x;
    int y;
    int attack_cooldown;
    bool is_attacking;
} t_player;

typedef struct s_enemy {
    int x;
    int y;
    int attack_cooldown;
    bool is_attacking;
} t_enemy;

typedef struct s_game {
    void *mlx;
    void *win;
    t_player player;
    t_enemy enemy;
} t_game;

void init_game(t_game *game);
void render(t_game *game);
void handle_input(t_game *game);
void update_game(t_game *game);
bool check_adjacency(t_player *player, t_enemy *enemy);
void player_attack(t_game *game);
void enemy_attack(t_game *game);

#endif
```

#### 2. `main.c`

```c
#include "game.h"

int main(void) {
    t_game game;

    init_game(&game);
    mlx_loop(game.mlx);
    return (0);
}
```

#### 3. `player.c`

```c
#include "game.h"

void init_player(t_player *player) {
    player->x = 100;
    player->y = 100;
    player->attack_cooldown = 0;
    player->is_attacking = false;
}

void player_attack(t_game *game) {
    if (game->player.attack_cooldown == 0) {
        game->player.is_attacking = true;
        game->player.attack_cooldown = 100; // Cooldown in frames
        // Trigger attack animation here
    }
}
```

#### 4. `enemy.c`

```c
#include "game.h"

void init_enemy(t_enemy *enemy) {
    enemy->x = 300;
    enemy->y = 100;
    enemy->attack_cooldown = 0;
    enemy->is_attacking = false;
}

void enemy_attack(t_game *game) {
    if (game->enemy.attack_cooldown == 0 && check_adjacency(&game->player, &game->enemy)) {
        game->enemy.is_attacking = true;
        game->enemy.attack_cooldown = 100; // Cooldown in frames
        // Trigger attack animation here
    }
}
```

#### 5. `attack.c`

```c
#include "game.h"

bool check_adjacency(t_player *player, t_enemy *enemy) {
    // Check if the player and enemy are adjacent (e.g., within 50 pixels)
    return (abs(player->x - enemy->x) < 50 && abs(player->y - enemy->y) < 50);
}

void update_game(t_game *game) {
    if (game->player.attack_cooldown > 0) {
        game->player.attack_cooldown--;
    }
    if (game->enemy.attack_cooldown > 0) {
        game->enemy.attack_cooldown--;
    }
    // Update animations and states here
}
```

#### 6. `utils.c`

```c
#include "game.h"

void init_game(t_game *game) {
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
    init_player(&game->player);
    init_enemy(&game->enemy);
    // Load assets here
}

void render(t_game *game) {
    // Clear window and draw player and enemy
    mlx_clear_window(game->mlx, game->win);
    // Draw player and enemy based on their states
    // If attacking, draw attack animation
}
```

#### 7. `Makefile`

```makefile
NAME = my_game

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I include -L /path/to/minilibx -lmlx -framework OpenGL -framework AppKit

SRC = src/main.c src/player.c src/enemy.c src/attack.c src/utils.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(CFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
```

### Running the Project

1. **Compile the Project**
   ```bash
   make
   ```

2. **Run the Game**
   ```bash
   ./my_game
   ```

### Additional Features

- **Animations**: Implement a function to handle animations for both player and enemy attacks.
- **Health System**: Add health attributes to both player and enemy, and implement damage calculations.
- **Input Handling**: Use `mlx_key_hook` to handle player input for attacking and moving.
- **Game Loop**: Implement a game loop that continuously updates the game state and renders the graphics.

This structure provides a solid foundation for implementing attack mechanics in your game using MLX and XPM. You can expand upon this by adding more features, improving graphics, and refining gameplay mechanics.