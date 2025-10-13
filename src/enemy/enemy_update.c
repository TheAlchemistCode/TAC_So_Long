/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_update.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alchemist <alchemist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 00:00:00 by alchemist           #+#    #+#             */
/*   Updated: 2025/10/13 00:00:00 by alchemist          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/so_long.h"
#include <stdio.h>

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
	
	// Check if enemies should attack player
	check_enemy_attacks(game);
}
