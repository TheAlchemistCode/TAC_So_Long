   typedef struct s_player {
       int x;
       int y;
       int health;
       int attack_cooldown;
       int attack_timer;
       int is_attacking;
       // Add more attributes as needed
   } t_player;

   typedef struct s_enemy {
       int x;
       int y;
       int health;
       int attack_cooldown;
       int attack_timer;
       int is_attacking;
       // Add more attributes as needed
   } t_enemy;