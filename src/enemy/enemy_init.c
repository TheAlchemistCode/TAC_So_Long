/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alchemist <alchemist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 00:00:00 by alchemist           #+#    #+#             */
/*   Updated: 2025/10/13 00:00:00 by alchemist          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/so_long.h"
#include <stdio.h>
#include <stdlib.h>

static int	count_enemies(t_game *game)
{
	int	count;
	int	y;
	int	x;

	count = 0;
	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			if (game->map[y][x] == 'B')
				count++;
			x++;
		}
		y++;
	}
	return (count);
}

void	init_enemies(t_game *game)
{
	int	y;
	int	x;
	int	index;

	game->enemy_count = count_enemies(game);
	
	if (game->enemy_count == 0)
	{
		game->enemies = NULL;
		printf("No enemies found in map\n");
		return;
	}
	
	game->enemies = malloc(sizeof(t_enemy) * game->enemy_count);
	if (!game->enemies)
	{
		printf("Error: Failed to allocate memory for enemies\n");
		exit(1);
	}
	
	index = 0;
	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			if (game->map[y][x] == 'B')
			{
				game->enemies[index].x = x;
				game->enemies[index].y = y;
				game->enemies[index].health = 100;
				game->enemies[index].alive = 1;
				game->enemies[index].is_attacking = 0;
				game->enemies[index].attack_frame_index = 0;
				game->enemies[index].last_attack_time_ms = 0;
				game->enemies[index].attack_anim_start_ms = 0;
				game->enemies[index].is_dying = 0;
				game->enemies[index].death_start_ms = 0;
				
				// Load textures for this enemy
				load_enemy_textures(game, &game->enemies[index]);
				
				printf("Enemy initialized at position (%d, %d)\n", x, y);
				index++;
			}
			x++;
		}
		y++;
	}
	
	printf("Total enemies initialized: %d\n", game->enemy_count);
}

void	free_enemies(t_game *game)
{
	int	i;

	if (!game->enemies)
		return;
	
	i = 0;
	while (i < game->enemy_count)
	{
		if (game->enemies[i].idle_img)
			mlx_destroy_image(game->mlx, game->enemies[i].idle_img);
		if (game->enemies[i].attack_frames[0])
			mlx_destroy_image(game->mlx, game->enemies[i].attack_frames[0]);
		if (game->enemies[i].attack_frames[1])
			mlx_destroy_image(game->mlx, game->enemies[i].attack_frames[1]);
		i++;
	}
	
	free(game->enemies);
	game->enemies = NULL;
}
