#include "mlx.h"
#include "game.h"

int main(void) {
    void *mlx;
    void *win;

    mlx = mlx_init();
    win = mlx_new_window(mlx, 800, 600, "Game Title");

    // Load assets and initialize game state here

    mlx_loop(mlx);
    return 0;
}