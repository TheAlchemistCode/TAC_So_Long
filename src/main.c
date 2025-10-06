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
#include <stdio.h>

int main(int argc, char **argv)
{
	t_game	game;

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
	printf("✓ Map loaded: %dx%d tiles\n", game.map_width, game.map_height);
	
	// Step 2: Initialize graphics
	printf("2. Initializing graphics...\n");
	init_graphics(&game);
	
	// Step 3: Load textures
	printf("3. Loading textures...\n");
	init_textures(&game);
	
	// Step 4: Render the map
	printf("4. Rendering initial map...\n");
	render_map(&game);
	
	// Step 5: Set up event handlers
	printf("5. Setting up event handlers...\n");
	mlx_hook(game.win, 17, 0, close_game, &game);  // Window close button
	mlx_key_hook(game.win, handle_keypress, &game); // Keyboard input
	
	// Step 6: Start the game loop
	printf("6. Starting game loop... (Press ESC to quit)\n");
	printf("====================================\n");
	mlx_loop(game.mlx);
	
	return (0);
}
