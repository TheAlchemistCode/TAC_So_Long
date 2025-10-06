/* *******	// Load exit (watertile.xpm - 64x64)
	game->exit_img = mlx_xpm_file_to_image(game->mlx, 
		"assets/watertile.xpm", &width, &height);
	if (!game->exit_img)
	{
		printf("Error: Failed to load watertile.xpm for exit\n");
		exit(1);
	}
	printf("✓ Loaded exit texture: watertile.xmp (%dx%d)\n", width, height);*********************************************************	// Load collectible (fish.xpm)
	game->collectible_img = mlx_xpm_file_to_image(game->mlx, 
		"assets/fish.xpm", &width, &height);
	if (!game->collectible_img)
	{
		printf("Error: Failed to load fish.xpm for collectibles\n");
		exit(1);
	}
	printf("✓ Loaded collectible texture: fish.xpm (%dx%d)\n", width, height);
	
	// Load exit (green_tile.xpm)
	game->exit_img = mlx_xpm_file_to_image(game->mlx, 
		"assets/green_tile.xpm", &width, &height);
	if (!game->exit_img)
	{
		printf("Error: Failed to load green_tile.xpm for exit\n");
		exit(1);
	}
	printf("✓ Loaded exit texture: green_tile.xpm (%dx%d)\n", width, height);
	
	// Load player (use green_tile.xpm - more visible)
	game->player_img = mlx_xpm_file_to_image(game->mlx, 
		"assets/green_tile.xpm", &width, &height);
	if (!game->player_img)
	{
		printf("Error: Failed to load green_tile.xpm for player\n");
		exit(1);
	}
	printf("✓ Loaded player texture: green_tile.xpm (%dx%d) - TEMP PLAYER\n", width, height);*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alchemist <alchemist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 00:00:00 by alchemist           #+#    #+#             */
/*   Updated: 2025/10/06 00:00:00 by alchemist          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"
#include <stdio.h>

/* 
** MiniLibX Tutorial:
** 1. mlx_init() - Creates connection to X server
** 2. mlx_new_window() - Creates a window
** 3. mlx_xpm_file_to_image() - Loads image files
** 4. mlx_put_image_to_window() - Draws images to window
** 5. mlx_loop() - Keeps window open and handles events
*/

void	init_graphics(t_game *game)
{
	// Step 1: Initialize MiniLibX connection
	game->mlx = mlx_init();
	if (!game->mlx)
	{
		printf("Error: Failed to initialize MiniLibX\n");
		exit(1);
	}
	
	// Step 2: Create window (size = map_width * TILE_SIZE, map_height * TILE_SIZE)
	game->win = mlx_new_window(game->mlx, 
		game->map_width * TILE_SIZE, 
		game->map_height * TILE_SIZE, 
		"So Long - TAC Edition");
	if (!game->win)
	{
		printf("Error: Failed to create window\n");
		exit(1);
	}
	
	printf("✓ Graphics initialized: %dx%d window\n", 
		game->map_width * TILE_SIZE, 
		game->map_height * TILE_SIZE);
}

void	init_textures(t_game *game)
{
	int	width, height;
	
	printf("Loading XPM textures...\n");
	
	// Load wall texture (flor.xpm for '1')
	game->wall_img = mlx_xpm_file_to_image(game->mlx, 
		"assets/flor.xpm", &width, &height);
	if (!game->wall_img)
	{
		printf("Error: Failed to load flor.xmp for walls\n");
		exit(1);
	}
	printf("✓ Loaded wall texture: flor.xpm (%dx%d)\n", width, height);
	
	// Load floor texture (tile.xpm for '0')
	game->floor_img = mlx_xpm_file_to_image(game->mlx, 
		"assets/tile.xpm", &width, &height);
	if (!game->floor_img)
	{
		printf("Error: Failed to load tile.xmp for floors\n");
		exit(1);
	}
		printf("✓ Loaded floor texture: tile.xmp (%dx%d)\n", width, height);
	
	// Load collectible (fish.xpm)
	
	// Use rock.xpm for collectible, watertile.xpm for exit (temporary)
	game->collectible_img = mlx_xpm_file_to_image(game->mlx, 
		"assets/rock.xpm", &width, &height);
	game->exit_img = mlx_xpm_file_to_image(game->mlx, 
		"assets/watertile.xpm", &width, &height);
	
	// Load ninja player sprite
	game->player_img = mlx_xpm_file_to_image(game->mlx, 
		"assets/ninja_idle.xpm", &width, &height);
	if (!game->player_img)
	{
		printf("Error: Failed to load ninja_idle.xpm for player\n");
		exit(1);
	}
	printf("✓ Loaded player texture: ninja_idle.xpm (%dx%d)\n", width, height);
	
	printf("✓ All XPM textures loaded successfully!\n");
}

void	render_map(t_game *game)
{
	int	x, y;
	int	pixel_x, pixel_y;
	
	printf("Rendering map with XPM textures...\n");
	
	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			// Calculate pixel position for this tile
			pixel_x = x * TILE_SIZE;
			pixel_y = y * TILE_SIZE;
			
			// Draw based on map character using XPM images
			if (game->map[y][x] == '1')
			{
				// Wall - use flor.xpm
				mlx_put_image_to_window(game->mlx, game->win, 
					game->wall_img, pixel_x, pixel_y);
			}
			else if (game->map[y][x] == '0')
			{
				// Floor - use tile.xpm
				mlx_put_image_to_window(game->mlx, game->win, 
					game->floor_img, pixel_x, pixel_y);
			}
			else if (game->map[y][x] == 'C')
			{
				// Collectible - floor background + yellow square
				mlx_put_image_to_window(game->mlx, game->win, 
					game->floor_img, pixel_x, pixel_y);
				// Draw yellow square for collectible (with 8-pixel border)
				for (int i = 8; i < TILE_SIZE - 8; i++)
				{
					for (int j = 8; j < TILE_SIZE - 8; j++)
					{
						mlx_pixel_put(game->mlx, game->win, pixel_x + i, pixel_y + j, 0xFFFF00); // Yellow
					}
				}
				printf("C at (%d,%d) ", x, y);
			}
			else if (game->map[y][x] == 'E')
			{
				// Exit - use watertile.xpm (blue water exit)
				mlx_put_image_to_window(game->mlx, game->win, 
					game->exit_img, pixel_x, pixel_y);
				printf("E at (%d,%d) ", x, y);
			}
			else if (game->map[y][x] == 'P')
			{
				// Player - floor background + ninja sprite
				mlx_put_image_to_window(game->mlx, game->win, 
					game->floor_img, pixel_x, pixel_y);
				mlx_put_image_to_window(game->mlx, game->win, 
					game->player_img, pixel_x, pixel_y);
				printf("P at (%d,%d) rendered as NINJA at pixel (%d,%d) ", x, y, pixel_x, pixel_y);
			}
			x++;
		}
		y++;
	}
	printf("\n✓ Map rendered successfully with XPM textures!\n");
}

int	move_player(t_game *game, int new_x, int new_y)
{
	// Check bounds
	if (new_x < 0 || new_x >= game->map_width || new_y < 0 || new_y >= game->map_height)
		return (0);
	
	// Check collision with walls
	if (game->map[new_y][new_x] == '1')
		return (0);
	
	// Valid move - update player position
	game->map[game->player_y][game->player_x] = '0'; // Clear old position
	game->player_x = new_x;
	game->player_y = new_y;
	game->map[new_y][new_x] = 'P'; // Set new position
	
	// Re-render the map
	render_map(game);
	
	printf("Player moved to (%d,%d)\n", new_x, new_y);
	return (1);
}

int	handle_keypress(int keycode, t_game *game)
{
	printf("Key pressed: %d\n", keycode);
	
	// ESC key closes the game
	if (keycode == 65307) // ESC key code
	{
		close_game(game);
	}
	
	// WASD Movement
	if (keycode == 119 || keycode == 87) // W key (up)
	{
		move_player(game, game->player_x, game->player_y - 1);
	}
	else if (keycode == 115 || keycode == 83) // S key (down)
	{
		move_player(game, game->player_x, game->player_y + 1);
	}
	else if (keycode == 97 || keycode == 65) // A key (left)
	{
		move_player(game, game->player_x - 1, game->player_y);
	}
	else if (keycode == 100 || keycode == 68) // D key (right)
	{
		move_player(game, game->player_x + 1, game->player_y);
	}
	
	return (0);
}

int	close_game(t_game *game)
{
	printf("Closing game...\n");
	
	// Clean up MiniLibX resources
	if (game->wall_img)
		mlx_destroy_image(game->mlx, game->wall_img);
	if (game->floor_img)
		mlx_destroy_image(game->mlx, game->floor_img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
	
	// Clean up map memory
	if (game->map)
	{
		for (int i = 0; i < game->map_height; i++)
			free(game->map[i]);
		free(game->map);
	}
	
	exit(0);
}