NAME := so_long
CC   := cc
CFLAGS := -Wall -Wextra -Werror -Iinclude -Iminilibx-linux

# Libft configuration
LIBFT_DIR := ./libft
LIBFT_LIB := $(LIBFT_DIR)/libft.a
LIBFT_LNK := -L$(LIBFT_DIR) -lft

MLX_DIR := ./minilibx-linux
MLX_LIB := $(MLX_DIR)/libmlx.a
MLX_LNK := -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz

SRC_FILES := main.c map_parse.c map_validation.c \
			 validation/map_components.c validation/map_structure.c \
			 validation/map_pathfinding.c validation/map_path_utils.c \
			 validation/map_flood_fill.c \
			 graphics/init_graphics.c graphics/textures.c graphics/render.c \
			 graphics/input_handler.c graphics/movement.c graphics/cleanup.c \
			 graphics/player_sprite.c graphics/level_management.c \
			 graphics/level_setup.c \
			 combat/attack_system.c combat/attack_helpers.c \
			 enemy/enemy_init.c enemy/enemy_update.c enemy/enemy_render.c \
			 enemy/enemy_movement_patrol.c enemy/enemy_movement_direction.c \
			 enemy/enemy_validation.c enemy/enemy_cleanup.c \
			 animation/animation_system.c \
			 utils/timer.c
SRC := $(addprefix src/, $(SRC_FILES))
OBJ := $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(MLX_LIB) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJ) $(MLX_LNK) $(LIBFT_LNK) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MLX_LIB):
	@if [ ! -f $(MLX_LIB) ]; then \
		echo "Building MiniLibX..."; \
		cd $(MLX_DIR) && chmod +x configure && ./configure && make -f Makefile.gen; \
	fi

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	rm -f src/graphics.o
	rm -f src/enemy/enemy_movement_helpers.o
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

