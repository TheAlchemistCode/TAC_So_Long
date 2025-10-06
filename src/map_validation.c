/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alchemist <alchemist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 16:00:00 by alchemist           #+#    #+#             */
/*   Updated: 2025/10/04 16:00:00 by alchemist          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdio.h> // For error messages

// Forward declarations for static functions
static void	check_components(t_game *game);
static void	check_walls(t_game *game);
static void	check_shape(t_game *game);

void	validate_map(t_game *game)
{
	check_components(game);
	check_shape(game);
	check_walls(game);
	// We will add the path check function here later
	printf("Map validation successful (so far)!\n");
}static void	check_components(t_game *game)
{
    int	p_count;
    int	e_count;
    int	c_count;
    int	y;
    int	x;

    p_count = 0;
    e_count = 0;
    c_count = 0;
    y = -1;
    while (++y < game->map_height)
    {
        x = -1;
        while (++x < game->map_width)
        {
            if (game->map[y][x] == 'P')
            {
                p_count++;
                game->player_x = x;
                game->player_y = y;
            }
            else if (game->map[y][x] == 'E')
                e_count++;
            else if (game->map[y][x] == 'C')
                c_count++;
        }
    }
    if (p_count != 1 || e_count != 1 || c_count < 1)
    {
        // Proper error handling will be added later
        printf("Error: Map component count is wrong!\n");
        exit(1);
    }
    game->collectibles = c_count;
}

static void	check_shape(t_game *game)
{
    int	y;
    int	row_len;

    y = 0;
    while (y < game->map_height)
    {
        row_len = 0;
        while (game->map[y][row_len] && game->map[y][row_len] != '\n' && game->map[y][row_len] != '\r')
            row_len++;
        if (row_len != game->map_width)
        {
            printf("Error: Map is not rectangular! Row %d has length %d, expected %d\n", y, row_len, game->map_width);
            printf("Row content: '%s'\n", game->map[y]);
            exit(1);
        }
        y++;
    }
}

static void	check_walls(t_game *game)
{
    int	i;
    
    // Check top and bottom walls
    i = 0;
    while (i < game->map_width)
    {
        if (game->map[0][i] != '1' || game->map[game->map_height - 1][i] != '1')
        {
            printf("Error: Map is not enclosed by walls (top/bottom)!\n");
            exit(1);
        }
        i++;
    }
    
    // Check left and right walls
    i = 0;
    while (i < game->map_height)
    {
        if (game->map[i][0] != '1' || game->map[i][game->map_width - 1] != '1')
        {
            printf("Error: Map is not enclosed by walls (left/right)!\n");
            exit(1);
        }
        i++;
    }
}