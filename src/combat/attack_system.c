/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attack_system.c                                    :+:      :+:    :+:   */
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

int	is_adjacent(int x1, int y1, int x2, int y2)
{
	int	dx;
	int	dy;

	dx = abs(x1 - x2);
	dy = abs(y1 - y2);
	return ((dx + dy) == 1);
}

void	player_attack(t_game *game)
{
	unsigned long	current_time;
	int				i;
	int				dx;

	current_time = get_time_ms();
	
	// Prevent attack if already in animation
	if (game->player_is_attacking)
		return;
	
	// Check attack cooldown
	if (current_time - game->player_last_attack_ms < PLAYER_ATTACK_COOLDOWN_MS)
		return;
	
	// Check for adjacent enemies
	i = 0;
	while (i < game->enemy_count)
	{
		if (game->enemies[i].alive && 
			is_adjacent(game->player_x, game->player_y, 
						game->enemies[i].x, game->enemies[i].y))
		{
			// Determine attack direction based on enemy position
			dx = game->enemies[i].x - game->player_x;
			
			// Set attack animation state
			game->player_is_attacking = 1;
			game->player_attack_start_ms = current_time;
			
			if (dx > 0)
			{
				// Enemy is to the right, attack right
				game->player_attack_direction = ATTACK_DIR_RIGHT;
				printf("⚔️  Player attacks RIGHT!\n");
			}
			else if (dx < 0)
			{
				// Enemy is to the left, attack left
				game->player_attack_direction = ATTACK_DIR_LEFT;
				printf("⚔️  Player attacks LEFT!\n");
			}
			else
			{
				// Enemy is above/below, use previous direction or default to right
				game->player_attack_direction = ATTACK_DIR_RIGHT;
				printf("⚔️  Player attacks (vertical)!\n");
			}
			
			game->enemies[i].health -= PLAYER_ATTACK_DAMAGE;
			printf("💥 Enemy health: %d\n", game->enemies[i].health);
			
			if (game->enemies[i].health <= 0)
			{
				// Start death animation instead of immediate death
				game->enemies[i].is_dying = 1;
				game->enemies[i].death_start_ms = current_time;
				// Clear the 'B' from the map immediately for movement
				if (game->map[game->enemies[i].y][game->enemies[i].x] == 'B')
					game->map[game->enemies[i].y][game->enemies[i].x] = '0';
				printf("💀 Enemy defeated! Playing death animation...\n");
			}
			
			game->player_last_attack_ms = current_time;
			game->player_last_action_ms = current_time;
			game->player_idle_frame = 0;
			game->player_idle_frame_start_ms = current_time;
			return;
		}
		i++;
	}
	
	printf("No enemy in range to attack!\n");
}

void	check_enemy_attacks(t_game *game)
{
	unsigned long	current_time;
	int				i;

	current_time = get_time_ms();
	i = 0;
	
	while (i < game->enemy_count)
	{
		if (game->enemies[i].alive && 
			is_adjacent(game->player_x, game->player_y, 
						game->enemies[i].x, game->enemies[i].y))
		{
			// Check if enemy can attack (cooldown expired)
			if (current_time - game->enemies[i].last_attack_time_ms >= ATTACK_COOLDOWN_MS)
			{
				game->player_health -= ENEMY_ATTACK_DAMAGE;
				game->enemies[i].last_attack_time_ms = current_time;
				game->enemies[i].is_attacking = 1;
				game->enemies[i].attack_anim_start_ms = current_time;
				game->enemies[i].attack_frame_index = 0;
				
				printf("Enemy attacks! Player health: %d/%d\n", 
					game->player_health, PLAYER_MAX_HEALTH);
				
				if (game->player_health <= 0)
				{
					printf("Game Over! You were defeated!\n");
					close_game(game);
				}
			}
		}
		i++;
	}
}

// Helper function to check if there's a living enemy at specific coordinates
int	has_living_enemy_at(t_game *game, int x, int y)
{
	int	i;

	i = 0;
	while (i < game->enemy_count)
	{
		if (game->enemies[i].alive && !game->enemies[i].is_dying &&
			game->enemies[i].x == x && game->enemies[i].y == y)
			return (1);
		i++;
	}
	return (0);
}
