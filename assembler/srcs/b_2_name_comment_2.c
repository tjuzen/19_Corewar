/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_2_name_comment_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabbenbr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 15:37:47 by fabbenbr          #+#    #+#             */
/*   Updated: 2020/01/24 15:37:49 by fabbenbr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"

void		check_for_label(t_all *all, char *line, int fd)
{
	int	k;

	k = 0;
	while (line[k] != '#' && line[k] != '%' && line[k] != ','\
	&& line[k] != '\0')
	{
		if (line[k] == ':')
		{
			if (!(all->l = add_label(all->l, line, 1, fd)))
				error_message("Malloc error\n", all, line);
			return ;
		}
		k++;
	}
	if (line[k] == '\0')
		while (line[--k] != 'r')
			if (k == 0)
				break ;
	if (line[k] == '%' || line[k] == ',' || line[k] == 'r')
	{
		if (!(all->l = add_label(all->l, line, 2, fd)))
			error_message("Malloc error\n", all, line);
	}
	if (k == 0)
		error_message("Error in parameter\n", all, line);
}
