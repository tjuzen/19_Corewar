/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_rules.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuarez- <bsuarez-@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:13:31 by bsuarez-          #+#    #+#             */
/*   Updated: 2020/03/03 17:55:34 by bsuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/corewar.h"

int     check_kill(int cycles, t_process *tmp, t_main *struk)
{
    if (tmp->last_live == -1)
        return (1);
    if (((tmp->last_live - cycles) * -1) <= struk->nbr_cycles + CYCLE_DELTA)
    {
        printf ("XXXX____: %d | %d\n", ((tmp->last_live - cycles) * -1), struk->nbr_cycles + CYCLE_DELTA);
        return (0);
    }
        printf ("FINISH____: %d | %d\n", ((tmp->last_live - cycles) * -1), struk->nbr_cycles + CYCLE_DELTA);

    return (1);
}

int     check_operation(t_main *struk, t_process *tmp)
{
    // check le opcode si valide
    // if (struk->map->arena[tmp->position] == 1) //est valide
    // {
    //     printf ("valide |");
    //     return (1);
    // }
    // else
        tmp->position = circular_memory(tmp->position, 1, MEM_SIZE);
    if (tmp->position % 100 == 0)
        printf ("   ID: %d [non valide] pos: %d\n", tmp->id, tmp->position);
    return (0);

}

// La focntion qui décrémente de CYCLE_DELTA, si les conditions sont bonnes.
int     check_die_cycle(t_main *struk, int cycles)
{
    printf("          nbr_live   [%i] ", struk->nbr_live);
    printf("          max_cycles [%i] ", struk->max_cycle);
    printf("je check les process |%i| ", cycles);
    printf ("____DIE: %i", struk->die);
    printf ("______cycle: %i", cycles);
    printf ("___DELTA: %i", CYCLE_DELTA);
    printf ("___CALCUL: % 5i", (struk->agent->last_live - cycles));
    printf ("___live: %i", struk->agent->last_live);
    printf("        CYCLETODIE   [%i]", struk->nbr_cycles);
    if (struk->nbr_live >= NBR_LIVE) // Si on a le bon nombre de live
    {
        printf (" NBR_LIVE ");
        upload_rules_var(struk);
    }
    else
    {
        struk->max_cycle++;
        if (struk->max_cycle == MAX_CHECKS) // Si on stagne..
        {
            printf (" MAX_CYCL ");
            upload_rules_var(struk);
        }
    }
    printf("  CYCLETODIE   [%i]\n", struk->nbr_cycles);
    return (0);
}
