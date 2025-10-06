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

#include "so_long.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
	{
		// Later, we'll use a more robust error handling function.
		write(2, "Error: Invalid number of arguments\n", 35);
		return (1);
	}
	parse_map(argv[1], &game);
	validate_map(&game);
	// We will add game initialization and loop here.
	// For now, let's just print the map to test if parsing works.
	for (int i = 0; i < game.map_height; i++)
		printf("%s", game.map[i]);
	// Free allocated memory
	for (int i = 0; i < game.map_height; i++)
		free(game.map[i]);
	free(game.map);
	return (0);
}
