/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_update.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clyon <clyon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 00:00:00 by clyon             #+#    #+#             */
/*   Updated: 2025/10/18 16:34:17 by clyon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/so_long.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void	update_enemies(t_game *game)
{
	unsigned long	current_time;
	int				i;

	if (!game->enemies)
		return;
	
	current_time = get_time_ms();
	i = 0;
	
	while (i < game->enemy_count)
	{
		if (game->enemies[i].alive)
		{
			// Check if death animation should end
			if (game->enemies[i].is_dying)
			{
				if (current_time - game->enemies[i].death_start_ms >= DEATH_ANIM_DURATION_MS)
				{
					game->enemies[i].alive = 0;
					printf("Enemy death animation complete - enemy removed\n");
				}
			}
			else
			{
				// Update animation state only for living enemies
				update_enemy_animation(&game->enemies[i], current_time);
			}
		}
		i++;
	}
	
	// Update enemy movement (1 tile every 2 seconds)
	update_enemy_movement(game);
	
	// Check if enemies should attack player
	check_enemy_attacks(game);
}

void	update_enemy_movement(t_game *game)
{
	unsigned long	current_time;
	int				i;
	int				distance;
	int				new_x;
	int				new_y;

	if (!game->enemies)
		return;
	
	current_time = get_time_ms();
	i = 0;
	
	while (i < game->enemy_count)
	{
		if (game->enemies[i].alive && !game->enemies[i].is_dying && 
			!game->enemies[i].is_attacking)
		{
			// Check if 2 seconds have passed since last move
			if (current_time - game->enemies[i].last_move_time >= ENEMY_MOVE_DELAY_MS)
			{
				// Move exactly 1 tile per movement
				distance = 1;
				
				// Calculate new position based on pattern
				new_x = game->enemies[i].x;
				new_y = game->enemies[i].y;
				calculate_next_position(game, &game->enemies[i], distance, &new_x, &new_y);
				
				// Validate and apply movement
				if (is_valid_enemy_position(game, new_x, new_y))
				{
					game->enemies[i].x = new_x;
					game->enemies[i].y = new_y;
					printf("Enemy %d moved to (%d, %d) - distance: %d\n", 
						i, new_x, new_y, distance);
				}
				
				game->enemies[i].last_move_time = current_time;
			}
		}
		i++;
	}
}

int	is_valid_enemy_position(t_game *game, int x, int y)
{
	int	i;
	
	// Check map boundaries
	if (x < 0 || x >= game->map_width || y < 0 || y >= game->map_height)
		return (0);
	
	// Check if position is a wall
	if (game->map[y][x] == '1')
		return (0);
	
	// Check if position is exit (enemies shouldn't block exit)
	if (game->map[y][x] == 'E')
		return (0);
	
	// Check collision with other enemies
	i = 0;
	while (i < game->enemy_count)
	{
		if (game->enemies[i].alive && !game->enemies[i].is_dying)
		{
			if (game->enemies[i].x == x && game->enemies[i].y == y)
				return (0);
		}
		i++;
	}
	
	return (1);
}

void	calculate_next_position(t_game *game, t_enemy *enemy, 
								int distance, int *new_x, int *new_y)
{
	int	step;
	int	dx;
	int	dy;
	
	*new_x = enemy->x;
	*new_y = enemy->y;
	
	if (enemy->pattern == PATROL_HORIZONTAL)
	{
		// Horizontal patrol (left-right)
		if (enemy->direction == 1 || enemy->direction == 0)  // Moving right
		{
			step = 0;
			while (step < distance)
			{
				if (*new_x + 1 >= enemy->start_x + enemy->patrol_length || 
					!is_valid_enemy_position(game, *new_x + 1, *new_y))
				{
					enemy->direction = 3;  // Switch to left
					break;
				}
				(*new_x)++;
				step++;
			}
		}
		else  // Moving left
		{
			step = 0;
			while (step < distance)
			{
				if (*new_x - 1 <= enemy->start_x - enemy->patrol_length || 
					!is_valid_enemy_position(game, *new_x - 1, *new_y))
				{
					enemy->direction = 1;  // Switch to right
					break;
				}
				(*new_x)--;
				step++;
			}
		}
	}
	else if (enemy->pattern == PATROL_VERTICAL)
	{
		// Vertical patrol (up-down)
		if (enemy->direction == 2 || enemy->direction == 1)  // Moving down
		{
			step = 0;
			while (step < distance)
			{
				if (*new_y + 1 >= enemy->start_y + enemy->patrol_length || 
					!is_valid_enemy_position(game, *new_x, *new_y + 1))
				{
					enemy->direction = 0;  // Switch to up
					break;
				}
				(*new_y)++;
				step++;
			}
		}
		else  // Moving up
		{
			step = 0;
			while (step < distance)
			{
				if (*new_y - 1 <= enemy->start_y - enemy->patrol_length || 
					!is_valid_enemy_position(game, *new_x, *new_y - 1))
				{
					enemy->direction = 2;  // Switch to down
					break;
				}
				(*new_y)--;
				step++;
			}
		}
	}
	else  // PATROL_RANDOM
	{
		// Random movement - try a random direction
		dx = 0;
		dy = 0;
		
		if (enemy->direction == 0)
			dy = -1;  // Up
		else if (enemy->direction == 1)
			dx = 1;   // Right
		else if (enemy->direction == 2)
			dy = 1;   // Down
		else
			dx = -1;  // Left
		
		step = 0;
		while (step < distance)
		{
			if (!is_valid_enemy_position(game, *new_x + dx, *new_y + dy))
			{
				// Hit obstacle, try new random direction
				enemy->direction = rand() % 4;
				break;
			}
			*new_x += dx;
			*new_y += dy;
			step++;
		}
	}
}
