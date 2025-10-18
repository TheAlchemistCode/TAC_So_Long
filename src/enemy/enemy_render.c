/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_render.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clyon <clyon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 00:00:00 by clyon             #+#    #+#             */
/*   Updated: 2025/10/18 16:34:19 by clyon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/so_long.h"

void	render_enemies(t_game *game)
{
	int		i;
	void	*img_to_draw;
	int		pixel_x;
	int		pixel_y;

	if (!game->enemies)
		return;
	
	i = 0;
	while (i < game->enemy_count)
	{
		if (game->enemies[i].alive)
		{
			pixel_x = game->enemies[i].x * TILE_SIZE;
			pixel_y = game->enemies[i].y * TILE_SIZE;
			
			// Choose which sprite to render
			if (game->enemies[i].is_dying)
			{
				// Render death sprite
				img_to_draw = game->enemies[i].dead_img;
			}
			else if (game->enemies[i].is_attacking)
			{
				// Render attack animation frame
				img_to_draw = game->enemies[i].attack_frames[game->enemies[i].attack_frame_index];
			}
			else
			{
				// Render idle sprite
				img_to_draw = game->enemies[i].idle_img;
			}
			
			if (img_to_draw)
			{
				mlx_put_image_to_window(game->mlx, game->win, 
					img_to_draw, pixel_x, pixel_y);
			}
		}
		i++;
	}
}
