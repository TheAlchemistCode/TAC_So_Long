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
#include <fcntl.h>
#include <unistd.h>
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

	game->player_img = mlx_xpm_file_to_image(game->mlx, "assets/player/ninja_idle.xpm", &width, &height);
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
			else if (game->map[y][x] == 'B')
			{
				// Enemy position - will be rendered by render_enemies()
				// Just draw floor underneath for now
			}
			
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
	else if (keycode == 32)
	{
		// Spacebar - player attacks
		player_attack(game);
		render_map(game);
		render_enemies(game);
	}
	
	return (0);
}

static t_game	*g_game = NULL;

void	set_game_pointer(t_game *game)
{
	g_game = game;
}

// Key event handler for mlx_hook - gets keycode and processes input
int	key_press_handler(int keycode, void *param)
{
	t_game *game = (t_game *)param;
	return (handle_keypress(keycode, game));
}

int	close_wrapper()
{
	return (close_game(g_game));
}

int	move_player(t_game *game, int new_x, int new_y)
{
	int	i;

	if (new_x < 0 || new_x >= game->map_width || new_y < 0 || new_y >= game->map_height)
		return (0);

	if (game->map[new_y][new_x] == '1')
		return (0);

	// Check if there's a living (non-dying) enemy at the destination
	if (has_living_enemy_at(game, new_x, new_y))
	{
		printf("⚠️ Cannot walk through living enemy! Attack it first (SPACE)!\n");
		return (0);
	}

	// Check for 'B' in map (backup check for enemy tiles)
	if (game->map[new_y][new_x] == 'B')
	{
		// Check if there's a dead enemy here
		int enemy_alive = 0;
		i = 0;
		while (i < game->enemy_count)
		{
			if (game->enemies[i].x == new_x && 
				game->enemies[i].y == new_y && 
				game->enemies[i].alive)
			{
				enemy_alive = 1;
				break;
			}
			i++;
		}
		
		if (enemy_alive)
		{
			printf("⚠️ Cannot walk through living enemy! Attack it first (SPACE)!\n");
			return (0);
		}
		// If enemy is dead, allow movement and clear the 'B' tile
		game->map[new_y][new_x] = '0';
		printf("Walking over defeated enemy remains...\n");
	}

	if (game->map[new_y][new_x] == 'C')
	{
		game->collectibles--;
		printf("Collectible collected! Remaining: %d\n", game->collectibles);
	}

	if (game->map[new_y][new_x] == 'E' && game->collectibles == 0)
	{
		printf("\n🎉 Level %d completed!\n", game->current_map);
		printf("Moving to next level...\n");
		
		// Move player to exit position first
		game->map[game->player_y][game->player_x] = '0';
		game->player_x = new_x;
		game->player_y = new_y;
		game->map[new_y][new_x] = 'P';
		game->moves++;
		
		// Render the final state
		render_map(game);
		render_enemies(game);
		
		// Brief pause to show completion
		usleep(1000000); // 1 second
		
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

	printf("Moves: %d | Health: %d/%d\n", game->moves, game->player_health, PLAYER_MAX_HEALTH);
	
	// Update enemies after player moves
	update_enemies(game);
	
	// Render everything
	render_map(game);
	render_enemies(game);
	
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
	
	// Free enemy resources
	free_enemies(game);
	
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

void	cleanup_level(t_game *game)
{
	int i;
	
	printf("Cleaning up current level...\n");
	
	// Free current map memory
	if (game->map)
	{
		for (i = 0; i < game->map_height; i++)
		{
			if (game->map[i])
				free(game->map[i]);
		}
		free(game->map);
		game->map = NULL;
	}
	
	// Free enemy resources
	free_enemies(game);
	
	// Destroy window (will recreate with new dimensions)
	if (game->win)
	{
		mlx_destroy_window(game->mlx, game->win);
		game->win = NULL;
	}
	
	// Destroy textures (will reload fresh)
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
	
	game->wall_img = NULL;
	game->floor_img = NULL;
	game->player_img = NULL;
	game->collectible_img = NULL;
	game->exit_img = NULL;
	
	printf("✓ Level cleaned up successfully\n");
}

void	load_next_map(t_game *game)
{
	char	map_path[256];
	int		fd;
	
	printf("\n=== LOADING NEXT LEVEL ===\n");
	printf("Current level: %d\n", game->current_map);
	
	// Increment level counter
	game->current_map++;
	
	// Construct map filename
	sprintf(map_path, "maps/map%d.ber", game->current_map);
	printf("Attempting to load: %s\n", map_path);
	
	// Check if map file exists
	fd = open(map_path, O_RDONLY);
	if (fd < 0)
	{
		printf("\n🏆 ALL LEVELS COMPLETED! 🏆\n");
		printf("Total moves: %d\n", game->moves);
		printf("Thanks for playing!\n");
		usleep(3000000); // 3 seconds
		close_game(game);
		return;
	}
	close(fd);
	
	// Display victory message for previous level
	display_victory(game);
	usleep(2000000); // 2 seconds
	
	// Clean up current level
	printf("Cleaning up current level...\n");
	cleanup_level(game);
	
	// Reset game state  
	printf("Resetting game state...\n");
	game->collectibles = 0;
	game->player_health = PLAYER_MAX_HEALTH;
	game->player_last_attack_ms = 0;
	game->enemy_count = 0;
	// Don't reset moves - keep total count
	
	// Parse the new map
	printf("Parsing new map: %s\n", map_path);
	parse_map(map_path, game);
	
	// Validate the new map
	printf("Validating new map...\n");
	validate_map(game);
	
	// CRITICAL: Ensure 'P' is in the map array at player position
	if (game->map[game->player_y][game->player_x] != 'P')
	{
		printf("WARNING: Fixing player position in map array\n");
		game->map[game->player_y][game->player_x] = 'P';
	}
	
	// Create new window with correct dimensions
	printf("Creating window: %dx%d pixels\n", 
		game->map_width * TILE_SIZE, game->map_height * TILE_SIZE);
	
	game->win = mlx_new_window(game->mlx, 
		game->map_width * TILE_SIZE,
		game->map_height * TILE_SIZE,
		"So Long - Next Level");
	
	if (!game->win)
	{
		printf("Error: Failed to create window for new level\n");
		exit(1);
	}
	printf("✓ Window created successfully\n");
	
	// Reload all textures (fresh load to prevent corruption)
	printf("Loading textures...\n");
	init_textures(game);
	printf("✓ Textures loaded\n");
	
	// Initialize enemies for new level
	printf("Initializing enemies...\n");
	init_enemies(game);
	printf("✓ Enemies initialized: %d found\n", game->enemy_count);
	
	// Re-register event handlers for new window
	printf("Registering event handlers...\n");
	mlx_hook(game->win, 2, 1L<<0, key_press_handler, game);
	mlx_hook(game->win, 17, 1L<<17, close_wrapper, game);
	set_game_pointer(game);
	printf("✓ Event handlers registered\n");
	
	// Initial render of the new level
	printf("Rendering new level...\n");
	render_map(game);
	render_enemies(game);
	
	printf("\n✅ Level %d loaded successfully!\n", game->current_map);
	printf("Map size: %dx%d\n", game->map_width, game->map_height);
	printf("Player position: (%d, %d)\n", game->player_x, game->player_y);
	printf("Collectibles to find: %d\n", game->collectibles);
	printf("Enemies to defeat: %d\n", game->enemy_count);
	printf("Good luck!\n\n");
}
