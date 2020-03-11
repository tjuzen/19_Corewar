/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_2_opcode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabbenbr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 15:26:18 by fabbenbr          #+#    #+#             */
/*   Updated: 2020/01/21 15:26:19 by fabbenbr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/asm.h"

int			get_opcode(char *line)
{
	int i;
	int x;

	x = -1;
	i = 0;
	while (ft_iswhitespace(line[i]))
		i++;
	while (++x < 16)
		if (ft_strncmp(&line[i], g_optab[x].command,\
		ft_strlen(g_optab[x].command)) == 0\
		&& ft_iswhitespace(line[i + ft_strlen(g_optab[x].command)]))
		{
			i += ft_strlen(g_optab[x].command);
			if (line[i])
				while (line[i])
				{
					if (line[i] == '%' || line[i] == 'r' || line[i] == ','\
					|| line[i - 1] == ':')
						return (x + 1);
					i++;
				}
		}
	return (-1);
}
