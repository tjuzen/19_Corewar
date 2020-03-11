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
    switch (7 & fg) {           /* RGB */
    case 0:                     /* 000 */
        return (COLOR_BLACK);
    case 1:                     /* 001 */
        return (COLOR_BLUE);
    case 2:                     /* 010 */
        return (COLOR_GREEN);
    case 3:                     /* 011 */
        return (COLOR_CYAN);
    case 4:                     /* 100 */
        return (COLOR_RED);
    case 5:                     /* 101 */
        return (COLOR_MAGENTA);
    case 6:                     /* 110 */
        return (COLOR_YELLOW);
    case 7:                     /* 111 */
        return (COLOR_WHITE);
    }
}

int colornum(int fg, int bg)
{
    int B, bbb, ffff;

    B = 1 << 7;
    bbb = (7 & bg) << 4;
    ffff = 7 & fg;

    return (B | bbb | ffff);
}

int is_bold(int fg)
{
    /* return the intensity bit */

    int i;

    i = 1 << 3;
    return (i & fg);
}

void init_colorpairs(void)
{
    int fg, bg;
    int colorpair;

    for (bg = 0; bg <= 7; bg++) {
        for (fg = 0; fg <= 7; fg++) {
            colorpair = colornum(fg, bg);
            init_pair(colorpair, curs_color(fg), curs_color(bg));
        }
    }
}


void setcolor(int fg, int bg, t_main *struk)
{
    /* set the color pair (colornum) and bold/bright (A_BOLD) */

    wattron(struk->map->winfos, COLOR_PAIR(colornum(fg, bg)));
    if (is_bold(fg)) {
        attron(A_BOLD);
    }
}

void unsetcolor(int fg, int bg, t_main *struk)
{
    /* unset the color pair (colornum) and
       bold/bright (A_BOLD) */

    wattroff(struk->map->winfos, COLOR_PAIR(colornum(fg, bg)));
    if (is_bold(fg)) {
        attroff(A_BOLD);
    }
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
    signal (SIGWINCH, NULL);
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
    int fg;
    int bg;

    fg = 12;
    bg = 0;
    setcolor(fg, bg, struk);                               // ca serait cool si ca pouvais marcher
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
    unsetcolor(fg, bg, struk);                            // mais ca marche pas

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

char	*ft_itoa_theo(long long n, int base)
{
	char		*s;
	long long	nb;
	int			len;

	len = 1;
	n < 0 ? ++len : 0;
	nb = n < 0 ? -n : n;
	while (nb >= base)
	{
		nb /= base;
		++len;
	}
	if (!(s = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	s[len] = '\0';
	n < 0 ? *s = '-' : 0;
	n < 0 ? n = -n : 0;
	while (n >= base)
	{
		s[--len] = n % base < 10 ? (n % base) + 48 : (n % base) + 55;
		n /= base;
	}
	s[--len] = n % base < 10 ? (n % base) + 48 : (n % base) + 55;
	return (s);
}

/*
** Prends une valeur et la convertis en hexa sous la forme char[2]
*/

char *hex(char c)
{
    char *ret;

    if (!(ret = ft_itoa_theo(c - 48, 16)))
        leave("Failed to transform in hexadecimal");
    return (ret);
}

/*
** Convertis chaque octet d'une des lignes de la memoire en hexa, séparés par des espaces
*/

char *arena_for_visu(t_main *struk, int j)
{
    char *ret;
    char *str;
    char *conv;
    int i;

    i = 0;
    if (!(str = ft_strsub((char *)struk->map->arena, j, 64)))
        leave("Failed to show arena");
    if (!(ret = ft_memalloc(sizeof(char) * 193)))
         leave("Failed to show arena");
    j = 0;
    while (i + 2 < 193 && str[j])
    {
        conv = hex(str[j]);
        if (conv[0] && conv[1])
        {
            ret[i] = conv[0];
            ret[i + 1] = conv[1];
        }
        else if (conv[0])
        {
            ret[i] = '0';
            ret[i + 1] = conv[0];
        }
        ret[i + 2] = ' ';
        i += 3;
        j++;
    }
    return (ret);
}

/*
** Affiche l'etat de la memoire dans la window principale
*/

void print_arena(t_main *struk)
{
    int     i;
    int     j;
    char    *substr;

    i = 0;
    j = 0;
    substr = NULL;
    struk->map->arena[0] = '1';
    struk->map->arena[4095] = '9';
    while (i < 64)
    {
        if (!(substr = arena_for_visu(struk, j)))
            leave("Failed to show arena");
        mvwprintw(struk->map->warena,  i + 2, 4, substr);
        i++;
        j += 64;
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
        return ;
    init_ncurses(struk);
    print_arena(struk);
    print_header(struk);
    print_infos(struk);
    end_ncurses(struk);
}