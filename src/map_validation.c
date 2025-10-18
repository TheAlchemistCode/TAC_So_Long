/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clyon <clyon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 16:00:00 by clyon             #+#    #+#             */
/*   Updated: 2025/10/04 16:00:00 by clyon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdio.h> // For error messages
#include "../libft/libft.h" // For ft_strdup

// Forward declarations for static functions
static void	check_components(t_game *game);
static void	check_walls(t_game *game);
static void	check_shape(t_game *game);

void	validate_map(t_game *game)
{
	check_components(game);
	check_shape(game);
	check_walls(game);
	validate_path(game);
	printf("Map validation successful!\n");
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
    
    // Check top and bottom walls (allow 'S' stat tiles)
    i = 0;
    while (i < game->map_width)
    {
        if (game->map[0][i] != '1' && game->map[0][i] != 'S')
        {
            printf("Error: Map is not enclosed by walls (top row)!\n");
            exit(1);
        }
        if (game->map[game->map_height - 1][i] != '1')
        {
            printf("Error: Map is not enclosed by walls (bottom row)!\n");
            exit(1);
        }
        i++;
    }
    
    // Check left and right walls
    i = 0;
    while (i < game->map_height)
    {
        if (game->map[i][0] != '1' && game->map[i][0] != 'S')
        {
            printf("Error: Map is not enclosed by walls (left side)!\n");
            exit(1);
        }
        if (game->map[i][game->map_width - 1] != '1')
        {
            printf("Error: Map is not enclosed by walls (right side)!\n");
            exit(1);
        }
        i++;
    }
}

static void	flood_fill(char **map_copy, int x, int y, int width, int height)
{
	// Check bounds
	if (x < 0 || x >= width || y < 0 || y >= height)
		return;
		
	// Check if already visited or is a wall or stat tile
	if (map_copy[y][x] == '1' || map_copy[y][x] == 'S' || map_copy[y][x] == 'V')
		return;
		
	// Mark as visited
	map_copy[y][x] = 'V';
	
	// Recursively visit all 4 directions
	flood_fill(map_copy, x + 1, y, width, height);
	flood_fill(map_copy, x - 1, y, width, height);
	flood_fill(map_copy, x, y + 1, width, height);
	flood_fill(map_copy, x, y - 1, width, height);
}

void	validate_path(t_game *game)
{
	char	**map_copy;
	int		i;
	int		j;
	int		collectibles_reachable = 0;
	int		exit_reachable = 0;
	
	// Create a copy of the map for flood fill
	map_copy = malloc(sizeof(char *) * game->map_height);
	if (!map_copy)
	{
		printf("Error: Memory allocation failed\n");
		exit(1);
	}
	
	for (i = 0; i < game->map_height; i++)
	{
		map_copy[i] = ft_strdup(game->map[i]);
		if (!map_copy[i])
		{
			printf("Error: Memory allocation failed\n");
			exit(1);
		}
	}
	
	// Start flood fill from player position
	flood_fill(map_copy, game->player_x, game->player_y, game->map_width, game->map_height);
	
	// Check if all collectibles and exit are reachable
	for (i = 0; i < game->map_height; i++)
	{
		for (j = 0; j < game->map_width; j++)
		{
			if (game->map[i][j] == 'C' && map_copy[i][j] == 'V')
				collectibles_reachable++;
			else if (game->map[i][j] == 'E' && map_copy[i][j] == 'V')
				exit_reachable = 1;
		}
	}
	
	// Free the map copy
	for (i = 0; i < game->map_height; i++)
		free(map_copy[i]);
	free(map_copy);
	
	// Check results
	if (collectibles_reachable != game->collectibles)
	{
		printf("Error: Not all collectibles are reachable from player start position\n");
		exit(1);
	}
	if (!exit_reachable)
	{
		printf("Error: Exit is not reachable from player start position\n");
		exit(1);
	}
	
	printf("Path validation: All %d collectibles and exit are reachable!\n", game->collectibles);
}