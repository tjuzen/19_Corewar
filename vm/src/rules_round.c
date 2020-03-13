/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules_round.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuarez- <bsuarez-@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:13:26 by bsuarez-          #+#    #+#             */
/*   Updated: 2020/03/03 17:55:13 by bsuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/corewar.h"

void execute(t_main *struk)
{
    printf("         |   J'execute! _____\n");
}

void do_actions(t_main *struk, t_process *tmp, int cycles)
{
    // Récupérer l'actions a effectuer
    if (check_operation(struk, tmp))
        execute(struk);
    refresh_cursor(struk, tmp, cycles);
}

t_process *kill_cursor(t_main *struk, t_process *prev)
{
    t_process *tmp;

    // printf("CYCLES: %d\n", struk->nbr_cycles);
    if (prev == NULL)
    {
        printf("IF__ tmp_id: ____ %d  ", struk->agent->id);
        printf("JE TUE BRAAAAH BRHH! ____\n");
        tmp = struk->agent->next;
        // je link avec le début de la liste, c'est le premniere element qu'on kill
        free(struk->agent);
        struk->agent = tmp;
        return (struk->agent);
    }
    else
    {
        tmp = prev->next;
        printf("ELSE tmp_id: ____ %d  ", tmp->id);
        printf("JE TUE BRAAAAH BRHH! ____\n");
        // je relink et je delete le curseur
        prev->next = tmp->next;
        free(tmp);
        return (prev->next);
    }
    // printf("JE TUE BRAAAAH BRHH! ____\n");
    return (struk->agent);
    // Tuer les process qui ne sont plus en vie, et le relier a la liste.
}

// Ici on parcours tous nos process, on check voir si ils doivent mourrir,
// si oui on les tues, si non on check voir si ils peuvent jouer.
// Si ils peuvent ben ils jouent :D
int process_actions(t_main *struk, int cycles)
{
    t_process *tmp;
    t_process *prev;

    tmp = struk->agent;
    prev = NULL;
    // printf ("tmp[ID]: %i\n", tmp->id);

    while (tmp)
    {
        // if ((tmp->last_live - struk->nbr_cycles + CYCLE_DELTA) >= 0)
        if (struk->die == 1 && check_kill(cycles, tmp, struk) == 1) /*tmp->last_live <= (cycles - struk->nbr_cycles + CYCLE_DELTA))*/
        {
            printf("CYCLES: %d\n", struk->nbr_cycles + CYCLE_DELTA);
            tmp = kill_cursor(struk, prev);
        }
        else
        {
            tmp->wait_cycles--;
            if (tmp->wait_cycles == 0)
                do_actions(struk, tmp, cycles);
            prev = tmp;
            tmp = tmp->next;
        }
    }
    if (struk->die == 1)
        struk->die = 0;
    return (0);
}

// Ici on a le maitre du jeu, qui s'assure que les règles sont bien respecter
// et check tous les X tours que tous le monde a dis qu'ils étaient en vie.

int rules_round(t_main *struk)
{
    int cycles;
    int inter_cycles;

    cycles = 0;
    printf("nbr_cycles: %i\n", struk->nbr_cycles);
    init_all_rules(struk);
    load_champ_to_arena(struk); // en attente de la fonction de greg
    printf("Je suce ici 11 %i\n", struk->map->cursor_map[0]);
    printf("Je suce ici 22 %i\n", struk->map->cursor_map[1]);
    printf("Je suce ici 33 %i\n", struk->map->cursor_map[2]);
    while (struk->nbr_cycles > 0 && struk->agent)
    {
        // printf ("cycles %% nbr_cycles: %i\n", (cycles % struk->nbr_cycles));
        if (struk->nbr_cycles - inter_cycles == 0)
            inter_cycles = check_die_cycle(struk, cycles); // La fonction qui décrémente le cycle_to_die
        if (process_actions(struk, cycles) == -1)
            return (-1);
        inter_cycles++;
        cycles++;
    }
    printf("\nLAST_PLAYER: %d\n", struk->last_player);
    return (1);
}