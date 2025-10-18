# Enemy Movement System - Developer Quick Reference

## Quick Configuration

### Timing Settings
```c
#define ENEMY_MOVE_DELAY_MS 2000   // Movement interval (2 seconds)
#define ENEMY_MIN_TILES 2          // Minimum distance
#define ENEMY_MAX_TILES 3          // Maximum distance
```

### Enemy Structure Fields
```c
typedef struct s_enemy {
    // Position
    int x, y;                      // Current position
    int start_x, start_y;          // Spawn point
    
    // Movement
    unsigned long last_move_time;  // Last movement timestamp
    int direction;                 // 0=up, 1=right, 2=down, 3=left
    int patrol_length;             // Patrol radius (default: 5)
    t_movement_pattern pattern;    // HORIZONTAL, VERTICAL, or RANDOM
}
```

## API Reference

### Core Functions

#### `void update_enemy_movement(t_game *game)`
**Description**: Main update loop for enemy movement  
**Call Frequency**: Every game loop iteration  
**Performance**: O(n) where n = enemy count  

**Usage**:
```c
void update_enemies(t_game *game) {
    update_enemy_movement(game);  // Add this line
}
```

#### `int is_valid_enemy_position(t_game *game, int x, int y)`
**Description**: Validates if enemy can move to position  
**Returns**: 1 if valid, 0 if blocked  
**Checks**: Boundaries, walls, enemies, exit  

**Usage**:
```c
if (is_valid_enemy_position(game, new_x, new_y)) {
    enemy->x = new_x;
    enemy->y = new_y;
}
```

#### `void calculate_next_position(t_game *game, t_enemy *enemy, int distance, int *new_x, int *new_y)`
**Description**: Calculates next position based on pattern  
**Parameters**:
- `game`: Game state
- `enemy`: Enemy to move
- `distance`: Tiles to move (2-3)
- `new_x`, `new_y`: Output parameters  

**Usage**:
```c
int new_x, new_y;
calculate_next_position(game, enemy, 3, &new_x, &new_y);
```

## Movement Patterns

### PATROL_HORIZONTAL
```c
enemy->pattern = PATROL_HORIZONTAL;
enemy->direction = 1;  // Start moving right
enemy->patrol_length = 5;  // 5 tiles each direction
```
**Behavior**: Moves left-right, reverses at boundaries

### PATROL_VERTICAL
```c
enemy->pattern = PATROL_VERTICAL;
enemy->direction = 2;  // Start moving down
enemy->patrol_length = 5;  // 5 tiles each direction
```
**Behavior**: Moves up-down, reverses at boundaries

### PATROL_RANDOM
```c
enemy->pattern = PATROL_RANDOM;
enemy->direction = rand() % 4;  // Random start
```
**Behavior**: Random direction, changes on obstacles

## Initialization Template

```c
void init_custom_enemy(t_enemy *enemy, int x, int y) {
    // Position
    enemy->x = x;
    enemy->y = y;
    enemy->start_x = x;
    enemy->start_y = y;
    
    // Movement
    enemy->last_move_time = get_time_ms();
    enemy->direction = rand() % 4;
    enemy->patrol_length = 5;
    enemy->pattern = PATROL_HORIZONTAL;
    
    // Status
    enemy->alive = 1;
    enemy->health = 100;
}
```

## Common Customizations

### Change Movement Speed
```c
// In so_long.h
#define ENEMY_MOVE_DELAY_MS 1000  // Faster (1 second)
#define ENEMY_MOVE_DELAY_MS 3000  // Slower (3 seconds)
```

### Change Movement Distance
```c
// In so_long.h
#define ENEMY_MIN_TILES 1  // Move 1-2 tiles
#define ENEMY_MAX_TILES 2

#define ENEMY_MIN_TILES 3  // Move 3-5 tiles
#define ENEMY_MAX_TILES 5
```

### Change Patrol Range
```c
// In enemy_init.c
enemy->patrol_length = 3;   // Shorter patrol
enemy->patrol_length = 10;  // Longer patrol
```

### Add New Pattern
```c
// 1. Add to enum in so_long.h
typedef enum e_movement_pattern {
    PATROL_HORIZONTAL,
    PATROL_VERTICAL,
    PATROL_RANDOM,
    PATROL_DIAGONAL  // NEW
} t_movement_pattern;

// 2. Implement in calculate_next_position()
else if (enemy->pattern == PATROL_DIAGONAL) {
    // Diagonal movement logic
    *new_x = enemy->x + distance;
    *new_y = enemy->y + distance;
}
```

## Debugging Tips

### Enable Movement Logging
```c
// In enemy_update.c
printf("Enemy %d: pos(%d,%d) dir=%d pattern=%d\n",
    i, enemy->x, enemy->y, enemy->direction, enemy->pattern);
```

### Check Timer Accuracy
```c
unsigned long elapsed = current_time - enemy->last_move_time;
printf("Enemy %d: elapsed=%lu should_move=%d\n",
    i, elapsed, elapsed >= ENEMY_MOVE_DELAY_MS);
```

### Visualize Patrol Range
```c
printf("Enemy patrol: x=[%d,%d] y=[%d,%d]\n",
    enemy->start_x - enemy->patrol_length,
    enemy->start_x + enemy->patrol_length,
    enemy->start_y - enemy->patrol_length,
    enemy->start_y + enemy->patrol_length);
```

## Performance Optimization

### Spatial Partitioning (Future)
```c
// Instead of checking all enemies for collision
// Use grid-based lookup
t_enemy* enemy_grid[MAP_HEIGHT][MAP_WIDTH];

int is_valid_position(game, x, y) {
    return enemy_grid[y][x] == NULL;
}
```

### Movement Caching (Future)
```c
// Pre-calculate patrol paths
typedef struct s_patrol_cache {
    int positions[10][2];  // Pre-calculated positions
    int position_count;
} t_patrol_cache;
```

## Troubleshooting

### Enemies Not Moving
```c
// Check 1: Timer initialized?
if (enemy->last_move_time == 0) {
    enemy->last_move_time = get_time_ms();
}

// Check 2: Function called?
update_enemy_movement(game);  // Add to update_enemies()

// Check 3: Conditions blocking?
if (enemy->is_dying || enemy->is_attacking) {
    return;  // These block movement
}
```

### Enemies Moving Too Fast/Slow
```c
// Verify timer precision
printf("Current: %lu, Last: %lu, Diff: %lu\n",
    current_time, enemy->last_move_time,
    current_time - enemy->last_move_time);

// Should be ~2000ms
```

### Enemies Stuck
```c
// Check collision validation
if (!is_valid_enemy_position(game, new_x, new_y)) {
    printf("Enemy stuck: can't move to (%d,%d)\n", new_x, new_y);
    enemy->direction = (enemy->direction + 2) % 4;  // Reverse
}
```

## Testing Checklist

```c
// Quick test suite
void test_enemy_movement() {
    // Test 1: Timer
    assert(current_time - last_move >= ENEMY_MOVE_DELAY_MS);
    
    // Test 2: Distance
    int dist = abs(new_x - old_x) + abs(new_y - old_y);
    assert(dist >= 2 && dist <= 3);
    
    // Test 3: Collision
    assert(game->map[new_y][new_x] != '1');
    
    // Test 4: Boundaries
    assert(new_x >= 0 && new_x < game->map_width);
    assert(new_y >= 0 && new_y < game->map_height);
}
```

## Integration Points

### With Combat System
```c
void update_enemies(t_game *game) {
    update_enemy_animation(game);  // Existing
    update_enemy_movement(game);   // NEW
    check_enemy_attacks(game);     // Existing
}
```

### With Rendering System
```c
void render_enemies(t_game *game) {
    for (int i = 0; i < game->enemy_count; i++) {
        if (game->enemies[i].alive) {
            // Render at current position
            render_enemy_sprite(&game->enemies[i]);
        }
    }
}
```

### With Game Loop
```c
int game_loop(t_game *game) {
    update_player_input(game);
    update_enemies(game);        // Includes movement
    update_animations(game);
    check_collisions(game);
    render_frame(game);
    return (0);
}
```

## Constants Reference

| Constant | Value | Description |
|----------|-------|-------------|
| `ENEMY_MOVE_DELAY_MS` | 2000 | Time between moves (ms) |
| `ENEMY_MIN_TILES` | 2 | Minimum tiles per move |
| `ENEMY_MAX_TILES` | 3 | Maximum tiles per move |
| `patrol_length` | 5 | Default patrol radius |

## Direction Reference

| Value | Direction | Delta X | Delta Y |
|-------|-----------|---------|---------|
| 0 | Up | 0 | -1 |
| 1 | Right | +1 | 0 |
| 2 | Down | 0 | +1 |
| 3 | Left | -1 | 0 |

---

*Quick Reference Version 1.0*  
*Last Updated: October 18, 2025*
