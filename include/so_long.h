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

// Define your game structures here, for example:
typedef struct s_game
{
	void	*mlx;
	void	*win;
	char	**map;
	int		map_width;
	int		map_height;
	int		collectibles;
	int		player_x;
	int		player_y;
	int		exit_x;
	int		exit_y;
	// Graphics textures
	void	*wall_img;		// watertile.png (walls = '1')
	void	*floor_img;		// tile.png (floor = '0')
	void	*collectible_img;
	void	*exit_img;
	void	*player_img;
}	t_game;

// Function prototypes
void	parse_map(char *filename, t_game *game);
void	validate_map(t_game *game);
// Graphics functions
void	init_graphics(t_game *game);
void	init_textures(t_game *game);
void	render_map(t_game *game);
int		move_player(t_game *game, int new_x, int new_y);
int		handle_keypress(int keycode, t_game *game);
int		close_game(t_game *game);
#endif
