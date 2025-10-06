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
	game->player_img = game->floor_img; // Player shows on floor for now
	
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
				// Collectible - floor background + collectible image (centered if too big)
				mlx_put_image_to_window(game->mlx, game->win, 
					game->floor_img, pixel_x, pixel_y);
				// Center large fish sprite within the tile
				int offset_x = (310 > TILE_SIZE) ? -(310 - TILE_SIZE) / 2 : 0;
				int offset_y = (310 > TILE_SIZE) ? -(310 - TILE_SIZE) / 2 : 0;
				mlx_put_image_to_window(game->mlx, game->win, 
					game->collectible_img, pixel_x + offset_x, pixel_y + offset_y);
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
				// Player - draw a solid red square (guaranteed visible)
				for (int i = 0; i < TILE_SIZE; i++)
				{
					for (int j = 0; j < TILE_SIZE; j++)
					{
						mlx_pixel_put(game->mlx, game->win, pixel_x + i, pixel_y + j, 0xFF0000); // Red
					}
				}
				printf("P at (%d,%d) rendered as RED SQUARE at pixel (%d,%d) ", x, y, pixel_x, pixel_y);
			}
			x++;
		}
		y++;
	}
	printf("\n✓ Map rendered successfully with XPM textures!\n");
}

int	handle_keypress(int keycode, t_game *game)
{
	printf("Key pressed: %d\n", keycode);
	
	// ESC key closes the game
	if (keycode == 65307) // ESC key code
	{
		close_game(game);
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