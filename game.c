
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define top_pane_heigth 2
#define left_pane_width 2
#define wall_thick 1


#define window_width    (left_pane_width + wall_thick + map_width)
#define window_height   (top_pane_heigth + wall_thick + map_heigth)

static char ch_snake = 'H';
static char ch_food  = 'A';
static char ch_wall = 'o';

static char ch_map[window_height][window_width];
static int g_history_highest_score = 0;
static int g_cur_score = 0;
static WalkDirect g_cur_walk_direct = Right;
static bool g_dead = false;
static bool g_win = false;


static Array* g_snake = NULL; //first is head, last is tail
static Array* g_emptycell = NULL; //all empty cell
FoodElement* g_cur_food = NULL;
static Array* g_ate_not_digestion_foods = NULL;


void init_game()
{
    int row = 0;
    int col = 0;
    int inttemp = 0;
    void* premove;

    g_cur_score = 0;

    array_new(&g_snake);
    array_new(&g_emptycell);
    array_new(&g_ate_not_digestion_foods);
    for (row = 0; row < map_heigth; ++row)
    {
        for (col = 0; col < map_width; ++col)
        {
            CellElement* p = (CellElement*)calloc(1, sizeof(CellElement));
            PosA pos;
            pos.row = row;
            pos.col = col;
            p->pos = pos;
            p->role = Empty;
            array_add(g_emptycell, p);
        }
    }
    //set rand seed
    srand(time(NULL));

    //generate snake pos
    inttemp = (int)rand();
    inttemp = inttemp % (array_size(g_emptycell));


    array_remove_at(g_emptycell, inttemp, &premove);
    SnakeElement* pa = (SnakeElement*)(premove);
    pa->role = Snake;
    array_add(g_snake, premove);

    //generate food pos
    inttemp = (int)rand();
    inttemp = inttemp % (array_size(g_emptycell));
    array_remove_at(g_emptycell, inttemp, (void**)&g_cur_food);
    g_cur_food->role = Food;

    
}

void snake_walk()
{
    //snake walk forward a step, snake tail become head.
    SnakeElement* snake_tail = NULL;
    SnakeElement* snake_header = NULL;
    bool eat_cur_food = false;
    PosA old_header;
    PosA old_tail;
    PosA new_header;
    int inttemp = 0;
    old_header.row = -1;
    old_header.col = -1;
    old_tail.row = -1;
    old_tail.col = -1;
    array_remove_last(g_snake, (void**)&snake_tail);
    if (!snake_tail)
    {
        g_dead = true;
        return;
    }
    old_tail = snake_tail->pos;
    if (array_size(g_snake) > 0)
    {
        array_get_at(g_snake, 0, (void**)&snake_header);
        old_header = snake_header->pos;
    }
    else
    {
        old_header = snake_tail->pos;
    }
    new_header = old_header;
    switch (g_cur_walk_direct)
    {
    case Left:
    {
        new_header.col -= 1;
    }break;
    case Right:
    {
        new_header.col += 1;
    }break;
    case Up:
    {
        new_header.row -= 1;
    }break;
    case Down:
    {
        new_header.row += 1;
    }break;
    default:
        break;
    }
    if (new_header.col < 0 || new_header.col >= map_width
        || new_header.row < 0 || new_header.row >= map_heigth)
    {
        g_dead = true;
        return;
    }
    if (g_cur_food)
    {
        if (g_cur_food->pos.row == new_header.row && g_cur_food->pos.col == new_header.col)
        {
            array_add(g_ate_not_digestion_foods, g_cur_food);

            if (array_size(g_emptycell) == 0)
            {
                g_win = true;
                g_cur_score = map_heigth * map_width;
                g_history_highest_score = g_cur_score;
            }

            //generate food pos
            inttemp = (int)rand();
            inttemp = inttemp % (array_size(g_emptycell));
            array_remove_at(g_emptycell, inttemp, (void**)&g_cur_food);
            g_cur_food->role = Food;
            g_cur_score += 1;
            if (g_cur_score > g_history_highest_score)
            {
                g_history_highest_score = g_cur_score;
            }
        }
    }

    //prev snake's tail become new snake's header
    snake_tail->pos = new_header;
    array_add_at(g_snake, snake_tail, 0);
    if (array_size(g_ate_not_digestion_foods) > 0)
    {
        FoodElement* first_ate = NULL;
        array_remove_at(g_ate_not_digestion_foods, 0, (void**)&first_ate);
        if (first_ate->pos.col == old_tail.col && first_ate->pos.row == old_tail.row)
        {
            first_ate->role = Snake;
            array_add(g_snake, first_ate);
        }
        else //reset first_ate to array again
        {
            array_add_at(g_ate_not_digestion_foods, first_ate, 0);
        }
    }
}

bool is_dead()
{
    return g_dead;
}

bool is_win()
{
    return g_win;
}

int get_cur_score()
{
    return g_cur_score;
}

void set_highest_score(int score)
{
    g_history_highest_score = score;
}

int get_higest_score()
{
    return g_history_highest_score;
}

Array* get_snake()
{
    return g_snake;
}

FoodElement* get_current_food()
{
    return g_cur_food;
}

void set_walk_direction(WalkDirect direct)
{
    g_cur_walk_direct = direct;
}

WalkDirect get_walk_direction()
{
    return g_cur_walk_direct;
}

