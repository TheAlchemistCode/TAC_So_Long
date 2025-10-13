   // game.h
   #ifndef GAME_H
   #define GAME_H

   #include <mlx.h>
   #include <stdbool.h>

   #define WINDOW_WIDTH 800
   #define WINDOW_HEIGHT 600
   #define PLAYER_ATTACK_COOLDOWN 500 // milliseconds
   #define ENEMY_ATTACK_COOLDOWN 1000 // milliseconds

   typedef struct s_player {
       int x;
       int y;
       int health;
       bool is_attacking;
       long last_attack_time;
   } t_player;

   typedef struct s_enemy {
       int x;
       int y;
       int health;
       bool is_attacking;
       long last_attack_time;
   } t_enemy;

   typedef struct s_game {
       void *mlx;
       void *win;
       t_player player;
       t_enemy enemy;
       // Add other game state variables as needed
   } t_game;

   void init_game(t_game *game);
   void render(t_game *game);
   void handle_input(t_game *game);
   void update_game(t_game *game);
   void attack(t_player *player, t_enemy *enemy);
   bool check_adjacency(t_player *player, t_enemy *enemy);

   #endif