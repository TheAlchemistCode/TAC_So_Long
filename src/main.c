/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
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

int main(int argc, char **argv)
{
	t_game	game;

	// Initialize game structure to zero
	ft_bzero(&game, sizeof(t_game));
	
	// Seed random number generator for wall randomization
	srand(time(NULL));

	if (argc != 2)
	{
		write(2, "Error: Invalid number of arguments\n", 35);
		return (1);
	}
	
	printf("=== SO LONG - GRAPHICS DEMO ===\n");
	
	// Step 1: Parse and validate map
	printf("1. Parsing map: %s\n", argv[1]);
	parse_map(argv[1], &game);
	validate_map(&game);
	game.moves = 0;  // Initialize movement counter
	game.victory = 0;  // Initialize victory flag
	game.player_health = PLAYER_MAX_HEALTH;  // Initialize player health
	game.player_last_attack_ms = 0;  // Initialize attack cooldown
	
	// Determine current map number from filename
	if (strstr(argv[1], "map1.ber"))
		game.current_map = 1;
	else if (strstr(argv[1], "map2.ber"))
		game.current_map = 2;
	else
		game.current_map = 1; // Default to map 1
	
	printf("✓ Map loaded: %dx%d tiles\n", game.map_width, game.map_height);
	
	// Step 2: Initialize graphics
	printf("2. Initializing graphics...\n");
	init_graphics(&game);
	
	// Step 3: Load textures
	printf("3. Loading textures...\n");
	init_textures(&game);
	
	// Step 4: Initialize enemies
	printf("4. Initializing enemies...\n");
	init_enemies(&game);
	
	// Step 5: Render the map
	printf("5. Rendering initial map...\n");
	render_map(&game);
	render_enemies(&game);
	
	// Step 6: Set up event handlers
	printf("6. Setting up event handlers...\n");
	set_game_pointer(&game);
	mlx_hook(game.win, 17, 1L<<17, close_wrapper, &game);          // Window close button
	mlx_hook(game.win, 2, 1L<<0, key_press_handler, &game);        // KeyPress events
	
	// Step 7: Start the game loop
	printf("7. Starting game loop... (Press ESC to quit, SPACE to attack)\n");
	printf("Player Health: %d/%d\n", game.player_health, PLAYER_MAX_HEALTH);
	printf("====================================\n");
	mlx_loop(game.mlx);
	
	return (0);
}
