/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clyon <clyon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 00:00:00 by clyon             #+#    #+#             */
/*   Updated: 2025/10/18 16:34:19 by clyon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/so_long.h"
#include <sys/time.h>
#include <stdio.h>

unsigned long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static void	print_time_message(t_game *game, int time_left)
{
	if (time_left != game->last_printed_time)
	{
		if (time_left > 5)
			printf("⏱️ Time remaining: %d seconds\n", time_left);
		else if (time_left > 0)
			printf("⚠️ HURRY! %d seconds left!\n", time_left);
		else
			printf("⏰ TIME'S UP!\n");
		game->last_printed_time = time_left;
	}
}

int	update_map_timer(t_game *game)
{
	unsigned long	elapsed_ms;
	unsigned long	now;
	int				time_left;
	int				prev_time;

	if (game->game_ended)
		return (1);
	now = get_time_ms();
	if (game->map_start_time_ms == 0 || game->map_start_time_ms > now)
	{
		game->map_start_time_ms = now;
		game->time_remaining = MAP_TIME_LIMIT;
		game->last_printed_time = MAP_TIME_LIMIT;
		return (1);
	}
	elapsed_ms = now - game->map_start_time_ms;
	prev_time = game->time_remaining;
	time_left = MAP_TIME_LIMIT - (elapsed_ms / 1000);
	if (time_left < 0)
		time_left = 0;
	if (time_left > prev_time + 2)
	{
		game->last_printed_time = time_left;
		game->time_remaining = time_left;
		return (1);
	}
	game->time_remaining = time_left;
	print_time_message(game, time_left);
	return (time_left > 0);
}
