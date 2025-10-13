/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alchemist <alchemist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:00:00 by alchemist           #+#    #+#             */
/*   Updated: 2025/10/04 15:00:00 by alchemist          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <unistd.h>
# include <stdlib.h>
# include "../minilibx-linux/mlx.h"

# define TILE_SIZE 64
# define ATTACK_COOLDOWN_MS 1000
# define PLAYER_ATTACK_COOLDOWN_MS 300
# define ATTACK_ANIM_DURATION_MS 200
# define DEATH_ANIM_DURATION_MS 300
# define PLAYER_MAX_HEALTH 100
# define ENEMY_ATTACK_DAMAGE 20
# define PLAYER_ATTACK_DAMAGE 50

// Enemy structure
typedef struct s_enemy
{
	int				x;
	int				y;
	int				health;
	int				alive;
	int				is_attacking;
	int				attack_frame_index;
	unsigned long	last_attack_time_ms;
	unsigned long	attack_anim_start_ms;
	void			*idle_img;
	void			*attack_frames[2];
	void			*dead_img;
	int				is_dying;
	unsigned long	death_start_ms;
	int				frame_w;
	int				frame_h;
}	t_enemy;

// Define your game structures here, for example:
typedef struct s_game
{
	void	*mlx;
	void	*win;
	char	**map;
	int		map_width;
	int		map_height;
	int		collectibles;
	int		collected;
	int		moves;
	int		player_x;
	int		player_y;
	int		player_health;
	unsigned long	player_last_attack_ms;
	int		exit_x;
	int		exit_y;
	int		victory;
	int		current_map;
	// Graphics textures
	void	*wall_img;
	void	*floor_img;
	void	*collectible_img;
	void	*exit_img;
	void	*player_img;
	// Enemy system
	t_enemy	*enemies;
	int		enemy_count;
}	t_game;

// Function prototypes
void	parse_map(char *filename, t_game *game);
void	validate_map(t_game *game);
void	validate_path(t_game *game);
// Graphics functions
void	init_graphics(t_game *game);
void	init_textures(t_game *game);
void	render_map(t_game *game);
int		move_player(t_game *game, int new_x, int new_y);
int	handle_keypress(int keycode, t_game *game);
int	close_game(t_game *game);
void	cleanup_level(t_game *game);
void	display_victory(t_game *game);
void	load_next_map(t_game *game);
void	set_game_pointer(t_game *game);
int	key_press_handler(int keycode, void *param);
int	close_wrapper();

// Enemy functions
void	init_enemies(t_game *game);
void	update_enemies(t_game *game);
void	render_enemies(t_game *game);
void	enemy_attack_player(t_game *game, t_enemy *enemy);
void	free_enemies(t_game *game);

// Combat functions
int		is_adjacent(int x1, int y1, int x2, int y2);
void	player_attack(t_game *game);
void	check_enemy_attacks(t_game *game);
int		has_living_enemy_at(t_game *game, int x, int y);

// Animation functions
void	update_enemy_animation(t_enemy *enemy, unsigned long current_time);
void	load_enemy_textures(t_game *game, t_enemy *enemy);

// Utility functions
unsigned long	get_time_ms(void);

#endif
