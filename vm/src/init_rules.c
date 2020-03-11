/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_rules.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuarez- <bsuarez-@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:13:38 by bsuarez-          #+#    #+#             */
/*   Updated: 2020/03/03 17:53:17 by bsuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/corewar.h"

void  init_all_rules(t_main *struk)
{
    int i;
    t_arena *init;

    if (!(init = ft_memalloc(sizeof(t_arena))))
        leave("Failed malloc of arena");
    struk->map = init;
    i = 0;
    struk->nbr_cycles = CYCLE_TO_DIE;
    struk->max_cycle = 0;
    struk->nbr_live = 21;
    struk->die = 0;
    struk->last_player = 0;
    while (i < 2)
    {
        init_cursor(struk, i);
        i++;
    }
    i = -1;
    while (++i <= MEM_SIZE)
        struk->map->arena[i] = '0';
    struk->map->arena[i] = '\0';
    // Initier les premier curseur en fonction de l'ordre établis dans le parsing
}

// On réinitialise nos variables entre deux CYCLE_TO_DIE

void  upload_rules_var(t_main *struk)
{
    struk->nbr_cycles -= CYCLE_DELTA; // Si on a décrémenter ça veut dire,
    struk->max_cycle = 0;
    struk->nbr_live = 25;
    struk->die = 1; // qu'on peut tuer les process qui n'on pas dis live.
}

void  refresh_cursor(t_main *struk, t_process *tmp, int cycles)
{
    // tmp->id = 0;
    // tmp->carry = 0;
    // tmp->position = 0;
    tmp->last_live = 24000;
    tmp->wait_cycles = 75;
    // tmp->jump = 0;
    // tmp->registeries[REG_NUMBER] = 0;
}

void  init_cursor(t_main *struk, int i)
{
    t_process   *cursor;

    if (!(cursor = ft_memalloc(sizeof(t_process))))
        leave("maloc cursor failed");
    printf("struk->nbr_player: %i\n", struk->nbr_player);
    cursor->id = i;
    cursor->carry = 0;
    cursor->position = 0;
    cursor->wait_cycles = 1;
    cursor->jump = 0;
    cursor->last_live = -1;
    cursor->registeries[REG_NUMBER] = 0;
    if (struk->agent == NULL)
        struk->agent = cursor;
    else
    {
        cursor->next = struk->agent;
        struk->agent = cursor;
    }
}


/*
**  Cette fonction transforme notre aire de jeux en mémoire circulaire.
*/
int     circular_memory(int position, int movement, int memory_size)
{
    int result;

    result = position + movement;
    if (result >= memory_size)
        return (result % memory_size);
    return (result);
}
