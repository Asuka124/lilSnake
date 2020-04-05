
#include "collectc/array.h"

#include <stdbool.h>

#define map_width 20
#define map_heigth 20

#define food_tag "FF"
#define snake_tag "SS" 

#define row_start 2
#define col_start 1

typedef enum
{
    Empty,
    Snake,
    Food,
}CellRole;

typedef enum 
{
    Left,
    Right,
    Up,
    Down,
}WalkDirect;

typedef struct 
{
    int row;
    int col;
}PosA;

typedef struct 
{
    PosA pos;
    CellRole role;
}FoodElement;

typedef FoodElement SnakeElement;
typedef FoodElement CellElement;

void init_game();

void snake_walk();

bool is_dead();

bool is_win();

int get_cur_score();

void set_highest_score(int score);

int get_higest_score();

Array* get_snake();

FoodElement* get_current_food();

void set_walk_direction(WalkDirect direct);

WalkDirect get_walk_direction();

