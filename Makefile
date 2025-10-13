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

SRC_FILES := main.c map_parse.c map_validation.c graphics.c \
			 combat/attack_system.c \
			 enemy/enemy_init.c enemy/enemy_update.c enemy/enemy_render.c \
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
		cd $(MLX_DIR) && chmod +x configure && ./configure && make -f makefile.gen; \
	fi

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

