/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuzen <tjuzen@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 16:38:01 by tjuzen            #+#    #+#             */
/*   Updated: 2020/03/03 17:57:14 by bsuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef LEM_IN_H
# define LEM_IN_H

# include "../libft/includes/libft.h"
#include <ncurses.h>
# include "op.h"

typedef struct s_po t_process;

typedef struct s_lo
{
		char *name;
		int code_length;
		uint8_t *code;
		int ID;
		char *comment;
}				t_player;

typedef struct s_lut
{
	unsigned char arena[MEM_SIZE];
	WINDOW *warena;
    WINDOW *winfos;
	int maxlines;
	int maxcols;
}				t_arena;

typedef struct 		s_ro
{
		int			max_cycle;
		int			nbr_live;
		int			die;
}					t_tab;

typedef struct 		s_po
{
		int			id;
		int			carry;
		int 		position;
		int			last_live;
		int			wait_cycles;
		int			jump;
		int			registeries[REG_NUMBER];
		t_process	*next;
}					t_process;

typedef struct		s_la
{
		int			nbr_player;
		int			visu;
  		int     	order_player[MAX_PLAYERS][1];
  		long     	nbr_cycles;
		t_player	player[MAX_PLAYERS];
		t_arena		*map;
		int			nbr_live;
		int			die;
		int			last_player;
		int			max_cycle;
		t_process	*agent;

}					t_main;

void 				init_vm(int argc, char **argv);
void 				check_params(int argc, char **argv);
int 				check_if_file(char **argv, int i);
t_main 				*check_arguments(int argc, char **argv, t_main *struk);
t_main				*check_cycles(t_main *struk, char **argv, int i);
int 				check_previous_player_number(t_main *struk);
int 				open_files(int argc, char **argv, t_main *struk, int arg);
int check_if_zero(uint8_t *buf);
void problem_occured(char *problem);


/*
** LEAVE.C
*/

void 				leave(char *msg);

/*
** INIT_MEMORY.C
*/

t_main				init_memory(t_main struk, int num_player);

/*
** INIT_RULES.C
*/

void	init_all_rules(t_main *struk);
void	upload_rules_var(t_main *struk);
void	refresh_cursor(t_main *struk, t_process *tmp, int cycles);
void 	init_cursor(t_main *struk, int i);
int		circular_memory(int position, int movement, int size);

/*
** RULES_ROUND.C
*/

int				rules_round(t_main *struk);

/*
** CHECK_RULES.C
*/

int     check_operation(t_main *struk, t_process *tmp);
int     check_die_cycle(t_main *struk, int cycles);
int     check_kill(int cycles, t_process *tmp, t_main *struk);

/*
** VISU.C
*/

void start_ncurses(t_main *struk);

#endif
