/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alchemist <alchemist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:00:00 by alchemist           #+#    #+#             */
/*   Updated: 2025/10/04 15:00:00 by alchemist          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"
#include "../libft/libft.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void	init_graphics(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
	{
		write(2, "Error: Failed to initialize MLX\n", 33);
		exit(1);
	}
	
	game->win = mlx_new_window(game->mlx, game->map_width * TILE_SIZE, 
		game->map_height * TILE_SIZE, "So Long");
	if (!game->win)
	{
		write(2, "Error: Failed to create window\n", 32);
		exit(1);
	}
	
	printf("Graphics initialized successfully\n");
}

void	init_textures(t_game *game)
{
	int	width;
	int	height;

	game->floor_img = mlx_xpm_file_to_image(game->mlx, "assets/bgtile_walkable.xpm", &width, &height);
	if (!game->floor_img)
	{
		write(2, "Error: Failed to load floor texture\n", 37);
		exit(1);
	}

	game->wall_img = mlx_xpm_file_to_image(game->mlx, "assets/bgtile_bush.xpm", &width, &height);
	if (!game->wall_img)
	{
		write(2, "Error: Failed to load wall texture\n", 36);
		exit(1);
	}

	game->player_img = mlx_xpm_file_to_image(game->mlx, "assets/ninja_idle.xpm", &width, &height);
	if (!game->player_img)
	{
		write(2, "Error: Failed to load player texture\n", 38);
		exit(1);
	}

	game->collectible_img = mlx_xpm_file_to_image(game->mlx, "assets/fish.xpm", &width, &height);
	if (!game->collectible_img)
	{
		write(2, "Error: Failed to load collectible texture\n", 42);
		exit(1);
	}

	game->exit_img = mlx_xpm_file_to_image(game->mlx, "assets/exit.xpm", &width, &height);
	if (!game->exit_img)
	{
		write(2, "Error: Failed to load exit texture\n", 36);
		exit(1);
	}
	
	printf("All textures loaded successfully\n");
}

void	render_map(t_game *game)
{
	int	x;
	int	y;

	mlx_clear_window(game->mlx, game->win);
	
	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			if (game->map[y][x] == '1')
				mlx_put_image_to_window(game->mlx, game->win, game->wall_img, x * TILE_SIZE, y * TILE_SIZE);
			else
				mlx_put_image_to_window(game->mlx, game->win, game->floor_img, x * TILE_SIZE, y * TILE_SIZE);
			
			if (game->map[y][x] == 'P')
				mlx_put_image_to_window(game->mlx, game->win, game->player_img, x * TILE_SIZE, y * TILE_SIZE);
			else if (game->map[y][x] == 'C')
				mlx_put_image_to_window(game->mlx, game->win, game->collectible_img, x * TILE_SIZE, y * TILE_SIZE);
			else if (game->map[y][x] == 'E')
				mlx_put_image_to_window(game->mlx, game->win, game->exit_img, x * TILE_SIZE, y * TILE_SIZE);
			
			x++;
		}
		y++;
	}
}

int	handle_keypress(int keycode, t_game *game)
{
	if (keycode == 65307)
	{
		close_game(game);
	}
	else if (keycode == 119 || keycode == 87)
		move_player(game, game->player_x, game->player_y - 1);
	else if (keycode == 115 || keycode == 83)
		move_player(game, game->player_x, game->player_y + 1);
	else if (keycode == 97 || keycode == 65)
		move_player(game, game->player_x - 1, game->player_y);
	else if (keycode == 100 || keycode == 68)
		move_player(game, game->player_x + 1, game->player_y);
	
	return (0);
}

static t_game	*g_game = NULL;

void	set_game_pointer(t_game *game)
{
	g_game = game;
}

// MiniLibX passes keycode as first parameter even with empty signature
int	key_wrapper(int keycode)
{
	return (handle_keypress(keycode, g_game));
}

int	close_wrapper()
{
	return (close_game(g_game));
}

int	move_player(t_game *game, int new_x, int new_y)
{
	if (new_x < 0 || new_x >= game->map_width || new_y < 0 || new_y >= game->map_height)
		return (0);

	if (game->map[new_y][new_x] == '1')
		return (0);

	if (game->map[new_y][new_x] == 'C')
	{
		game->collectibles--;
		printf("Collectible collected! Remaining: %d\n", game->collectibles);
	}

	if (game->map[new_y][new_x] == 'E' && game->collectibles == 0)
	{
		printf("Level completed!\n");
		load_next_map(game);
		return (1);
	}
	else if (game->map[new_y][new_x] == 'E' && game->collectibles > 0)
	{
		printf("Collect all items first! Remaining: %d\n", game->collectibles);
		return (0);
	}

	game->map[game->player_y][game->player_x] = '0';
	game->player_x = new_x;
	game->player_y = new_y;
	game->map[new_y][new_x] = 'P';
	game->moves++;

	printf("Moves: %d\n", game->moves);
	render_map(game);
	return (1);
}

int	close_game(t_game *game)
{
	if (game->wall_img)
		mlx_destroy_image(game->mlx, game->wall_img);
	if (game->floor_img)
		mlx_destroy_image(game->mlx, game->floor_img);
	if (game->player_img)
		mlx_destroy_image(game->mlx, game->player_img);
	if (game->collectible_img)
		mlx_destroy_image(game->mlx, game->collectible_img);
	if (game->exit_img)
		mlx_destroy_image(game->mlx, game->exit_img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
		mlx_destroy_display(game->mlx);
	
	exit(0);
	return (0);
}

void	display_victory(t_game *game)
{
	printf("🎉 CONGRATULATIONS! 🎉\n");
	printf("Level completed in %d moves!\n", game->moves);
}

void	load_next_map(t_game *game)
{
	printf("Loading next level...\n");
	game->current_map++;
	if (game->current_map > 2)
	{
		printf("🏆 ALL LEVELS COMPLETED! 🏆\n");
		close_game(game);
	}
	else
	{
		printf("No more levels implemented yet.\n");
		close_game(game);
	}
}
