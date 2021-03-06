/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuzen <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 14:08:06 by tjuzen            #+#    #+#             */
/*   Updated: 2020/02/18 14:08:07 by tjuzen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/corewar.h"

short curs_color(int fg)
{
    switch (7 & fg)
    {
    case 0:
        return (COLOR_BLACK);
    case 1:
        return (COLOR_BLUE);
    case 2:
        return (COLOR_GREEN);
    case 3:
        return (COLOR_CYAN);
    case 4:
        return (COLOR_RED);
    case 5:
        return (COLOR_MAGENTA);
    case 6:
        return (COLOR_YELLOW);
    case 7:
        return (COLOR_WHITE);
    }
    return (COLOR_BLACK);
}

int colornum(int fg, int bg)
{
    int B, bbb, ffff;

    B = 1 << 7;
    bbb = (7 & bg) << 4;
    ffff = 7 & fg;

    return (B | bbb | ffff);
}

void init_colorpairs(void)
{
    int fg, bg;
    int colorpair;

    for (bg = 0; bg <= 7; bg++)
    {
        for (fg = 0; fg <= 7; fg++)
        {
            colorpair = colornum(fg, bg);
            init_pair(colorpair, curs_color(fg), curs_color(bg));
        }
    }
}

void setcolor(int fg, int bg, t_main *struk)
{
    wattron(struk->map->winfos, COLOR_PAIR(colornum(fg, bg)));
    wattron(struk->map->warena, COLOR_PAIR(colornum(fg, bg)));
}

void unsetcolor(int fg, int bg, t_main *struk)
{
    wattroff(struk->map->winfos, COLOR_PAIR(colornum(fg, bg)));
    wattroff(struk->map->warena, COLOR_PAIR(colornum(fg, bg)));
}

/*
** Cree mes deux windows
*/

void init_ncurses(t_main *struk)
{
    initscr();
    cbreak();
    noecho();
    clear();
    start_color();
    init_colorpairs();
    signal(SIGWINCH, NULL);
    struk->map->maxcols = 1;
    struk->map->maxlines = LINES - 1;
    struk->map->warena = newwin(68, 200, 0, 0);
    struk->map->winfos = newwin(68, 55, 0, 200);
    box(struk->map->warena, '*', '*');
    box(struk->map->winfos, '*', '*');
}

/*
** Kill les deux windows et exit ncurses
*/

void end_ncurses(t_main *struk)
{
    delwin(struk->map->warena);
    delwin(struk->map->winfos);
    endwin();
}

/*
** Nous affiche un COREWAR clignotant et brillant de milles feu
*/

void print_header(t_main *struk)
{
    setcolor(12, 0, struk); // ca serait cool si ca pouvais marcher
    wattron(struk->map->winfos, A_BLINK);
    mvwprintw(struk->map->winfos, 2, 3, " ####   ####  #####  ###### #    #   ##   #####  ");
    mvwprintw(struk->map->winfos, 3, 3, "#    # #    # #    # #      #    #  #  #  #    # ");
    mvwprintw(struk->map->winfos, 4, 3, "#      #    # #    # #####  #    # #    # #    # ");
    mvwprintw(struk->map->winfos, 5, 3, "#      #    # #####  #      # ## # ###### #####  ");
    mvwprintw(struk->map->winfos, 6, 3, "#    # #    # #   #  #      ##  ## #    # #   #  ");
    mvwprintw(struk->map->winfos, 7, 3, " ####   ####  #    # ###### #    # #    # #    # ");
    wattroff(struk->map->winfos, A_BLINK);
    refresh();
    wrefresh(struk->map->winfos);
    unsetcolor(12, 0, struk); // mais ca marche pas
}

/*
** Window de droite, affiche les infos sur la partie en cours
*/

void print_infos(t_main *struk)
{
    wattron(struk->map->winfos, A_BOLD);
    mvwprintw(struk->map->winfos, 8, 1, "_____________________________________________________");
    mvwprintw(struk->map->winfos, 10, 3, "Cycle         : %i", 0);
    mvwprintw(struk->map->winfos, 12, 3, "Processes     : %i", 0);
    mvwprintw(struk->map->winfos, 14, 3, "Cycles to die : %i", CYCLE_TO_DIE);
    mvwprintw(struk->map->winfos, 16, 3, "Cycle delta   : %i", CYCLE_DELTA);
    mvwprintw(struk->map->winfos, 18, 3, "Nbr live      : %i", NBR_LIVE);
    mvwprintw(struk->map->winfos, 20, 3, "Max checks    : %i", MAX_CHECKS);
    mvwprintw(struk->map->winfos, 21, 1, "_____________________________________________________");
    wattroff(struk->map->winfos, A_BOLD);
    refresh();
    wrefresh(struk->map->winfos);
    mvaddstr(struk->map->maxlines, 0, "Press any key to quit");
    getch();
}

/*
** Affiche l'etat de la memoire dans la window principale en bouclant dessus et en l'affichant en hexadecimal
*/

void write_colors(int fg, int bg, t_main *struk, int i, int j, int z)
{
    setcolor(bg, fg, struk);
    mvwprintw(struk->map->warena, i + 2, j + 1, "%02x", struk->map->arena[z++]);
    refresh();
    wrefresh(struk->map->warena);
    unsetcolor(fg, bg, struk);
}

void print_arena(t_main *struk)
{
    int i;
    int j;
    int z;
    int x;

    x = 0;
    i = 0;
    j = 3;
    z = 0;

    while (i < 64)
    {
        j = 3;
        x = 0;
        while (x < 64)
        {
            if (struk->map->code_property[z] == 1)
            {
                if (struk->map->cursor_map[z] != 1)
                    write_colors(0, 12, struk, i, j, z++);
                else
                    write_colors(12, 0, struk, i, j, z++);
            }
            else if (struk->map->code_property[z] == 2)
            {
                if (struk->map->cursor_map[z] != 1)
                    write_colors(0, 10, struk, i, j, z++);
                else
                    write_colors(10, 0, struk, i, j, z++);
            }
            else if (struk->map->code_property[z] == 3)
            {
                if (struk->map->cursor_map[z] != 1)
                    write_colors(0, 8, struk, i, j, z++);
                else
                    write_colors(8, 0, struk, i, j, z++);
            }
            else if (struk->map->code_property[z] == 4)
            {
                if (struk->map->cursor_map[z] != 1)
                    write_colors(0, 6, struk, i, j, z++);
                else
                    write_colors(6, 0, struk, i, j, z++);
            }
            else
                mvwprintw(struk->map->warena, i + 2, j + 1, "%02x", struk->map->arena[z++]);
            j += 3;
            x++;
        }
        i++;
    }
    refresh();
    wrefresh(struk->map->warena);
}

/*
** Affiche le visu, utilisant ncurses, si l'option est activee
*/

void start_ncurses(t_main *struk)
{
    if (struk->visu != 1)
        return;
    init_ncurses(struk);
    print_arena(struk);
    print_header(struk);
    print_infos(struk);
    end_ncurses(struk);
}