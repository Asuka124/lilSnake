#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <stdio.h>

#include "game.h"


#define delay_base 20

int main() 
{
    FILE* fp = fopen("./result.txt", "r");
    char chscore[4];
    memset(chscore,0,sizeof(chscore));
    fread(chscore, sizeof(char), sizeof(chscore)/sizeof(char), fp);
    set_highest_score(atoi(chscore));
    fclose(fp);
    fp = NULL;

    int delayx = delay_base;

    init_game();

    initscr();
    curs_set(0);
    keypad(stdscr, TRUE);
    
   
    refresh();
    flash();
    

    int chin = 0;
    int countx = 0;
    while(chin !='q' && !is_dead() && !is_win())
    {
        int next_delay = delayx;

        while(next_delay>0)
        {
            clock_t t1, t2;

            t1 = clock();

            halfdelay(next_delay);


            if(is_dead() || is_win())
            {
                break;
            }

            chin = getch();
        
            t2 = clock();

            next_delay = (t2 - t1)*100/CLOCKS_PER_SEC;
        }

        switch (chin)
        {
        case KEY_UP:
        {
            if(get_walk_direction() == Left || get_walk_direction() == Right)
                set_walk_direction(Up);
        }break;
        case KEY_DOWN:
        {
            if(get_walk_direction() == Left || get_walk_direction() == Right)
                set_walk_direction(Down);
        }break;
        case KEY_LEFT:
        {
            if(get_walk_direction() == Up || get_walk_direction() == Down)
                set_walk_direction(Left);
        }break;
        case KEY_RIGHT:
        {
            if(get_walk_direction() == Up || get_walk_direction() == Down)
                set_walk_direction(Right);
        }break;
        default:
            break;
        }
        if(get_cur_score()!=0)
            delayx = delay_base / get_cur_score();
        if (delayx <= 2)
        {
            delayx = 2;
        }

        snake_walk();
        
        clear();
        //update game score
        mvprintw(0, 0, "cur score:%d", get_cur_score());
        mvprintw(0, 20, "highest score:%d", get_higest_score());

        //draw wall
        {
            int temp = 0;
            for(temp=1; temp< map_heigth + 3; ++temp) //vertical
            {
                mvprintw(temp, 0, "*");
                mvprintw(temp, 2*(map_width+1), "*");
            }
            for(temp=1; temp< (map_width + 1)*2; ++temp)  //horizental
            {
                mvprintw(1, temp, "*");
                mvprintw(map_heigth+2, temp, "*");
            }
        }
        //draw food
        FoodElement* food = get_current_food();
        mvprintw(food->pos.row+row_start, (food->pos.col+col_start)*2, food_tag);

        //draw snake
        {
            Array* snake = get_snake();
            ArrayIter ai;
            array_iter_init(&ai, snake);
            void *next;
            while (array_iter_next(&ai, &next) != CC_ITER_END) 
            {
                SnakeElement* ccc = (SnakeElement*)next;
                mvprintw(ccc->pos.row+row_start, 2*(ccc->pos.col+col_start), 
                    snake_tag);
            }
        }

        refresh();
    }
    clear();
    if(chin == 'q')
    {
        mvprintw(10, 5, "you quit game");
    }
    else if(is_dead())
    {
        mvprintw(10, 5, "you lose game");
    }
    else if(is_win())
    {
        mvprintw(10, 5, "you win!");
    }
    //mvprintw(0,0, "game over");

    fp = fopen("./result.txt", "w+");
    memset(chscore, 0, sizeof(chscore));
    sprintf(chscore, "%d", get_higest_score());
    fprintf(fp, "%d", get_higest_score());
    //fwrite(chscore, sizeof(char), sizeof(chscore)/sizeof(char), fp);
    fclose(fp);
    fp = NULL;

    refresh();
    sleep(2);
    endwin();
    exit(EXIT_SUCCESS);
}