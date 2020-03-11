/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuzen <tjuzen@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 14:08:10 by tjuzen            #+#    #+#             */
/*   Updated: 2020/02/24 14:23:26 by bsuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/corewar.h"

// Dans le pdf ou ous dit que"Next 2048 bytes represent champions comment.
// It is same as champion name excepting max size COMMENT_LENGTH.
// On read le commentaire et on le stocke dans la structure du joueur actuel
// Check si il y a bien NULL Bytes apres le commentaire comme dit dans le pdf
// Le reste du fichier .cor est le code du joueur, on le lit grace a la taille que l'on
// a récupérer précedement.

t_main *some_other_parsing(int fd, t_main *struk, int num_play)
{
	uint8_t buf[4];
	char buffer[COMMENT_LENGTH];
	uint8_t buff[struk->player[num_play].code_length];
	size_t size;
	int i;

	i = 0;
	size = read(fd, &buffer,COMMENT_LENGTH);
	while(i < 4)
		size = read(fd, &buf[i++], 1);
	i = 0;
	if(check_if_zero(buf))
		problem_occured("No NULL-bytes after comment");
	size = read(fd, buff,struk->player[num_play].code_length);
	struk->player[num_play].code = buff;
	i = 0;
 	struk->player[num_play].comment = buffer;
	return (struk);
}

// Focntion en cas d'erreur

void problem_occured(char *problem)
{
	printf("%s\n",problem);
	exit(0);
}

// verifier si ce sont bien tous des NULL BYTES

int check_if_zero(uint8_t *buf)
{
	int i;

	i = 0;
	while(i < 4)
	{
		if(buf[i] != 0)
			return(1);
		i++;
	}
	return (0);
}

// Dans le pdf "Next 128 bytes are champion's name. 128
// is the value of constant PROG_NAME_LENGTH from op.h file."
// On lit du coup les 128 bytes avec read en char.
// Mets dans la structure dans l'actuel joueur (voir num_play actuel)
// Ensuite on nous dit que "Next four bytes are NULL-bytes. Their goal is to be present at this
// place and to be NULL. If they are not, file is invalid."
// On check ça, si faux alors on appelle problem occured
// Ensuite on nous dit "Next four bytes represent the size of
// champion's executable code (only executable part of champion)."
// On fait pareil que pour le COREWAR_EXEC_MAGIC et on chope la taille du Champion et on la stocke

t_main		*some_parsing_checks(int fd, t_main *struk, int num_play)
{
	char buffer[PROG_NAME_LENGTH];
	size_t	size;
	uint8_t buf[4];
	uint32_t magic;
	int i;

	i = 0;
	size = read(fd, &buffer,PROG_NAME_LENGTH);
	struk->player[num_play].name = buffer;
	while(i < 4)
		size = read(fd, &buf[i++], 1);
	if(check_if_zero(buf))
		problem_occured("No NULL-bytes after file name");
	i = 0;
	while(i < 4)
		size = read(fd, &buf[i++], 1);
	magic = buf[0] << 24| (buf[1] << 16) | (buf[2] << 8) | (buf[3] << 0);
	if(magic > CHAMP_MAX_SIZE/* || magic < 0*/)
		problem_occured("Champ size too big");
	struk->player[num_play].code_length = magic;
	return (struk);
}

/*
** Recupere les executable code des champions et les stockes dans  l'arene (la memoire)
*/

void load_champ_to_arena(t_main *struk)
{
	printf("\n\nICI HEHEHEHEHEHEHEEEHHEEHEHEHEEEEHEHEHHEEHEHEH %s\n\n", struk->player[0].code);
}

/*
** S'occupe de tout le parsing 
*/

int go_parse(t_main *struk, int argc, char **argv)
{
	int fd;
	int num_play;
	int i;

	num_play = 0;
	i = 0;
	struk->order_player[0][0] = 0;
	struk->order_player[1][0] = 0;
	struk->order_player[2][0] = 0;
	struk->order_player[3][0] = 0;
	if(argc > 2)
	{
		struk = check_arguments(argc, argv, struk); // simplement checker ce que l'on recçoit en argv
		while(i < argc)
		{
			if(check_if_file(argv, i)) // si c'est un fichier ".cor"
			{
				fd = open_files(argc, argv, struk, i); // ouvre le fichier
				struk = some_parsing_checks(fd, struk,num_play); // voir fonction
				struk = some_other_parsing(fd, struk, num_play); // voir fonction
				struk->player[num_play].ID = struk->order_player[num_play][0]; // Donne l'ID a la structure du joueur (qu'on avait avant mis dans une array 2D)
				num_play++;
			}
			i++;
		}
		load_champ_to_arena(struk); // pour ajouter le code d'execution du champion dans notre memoire
		return(0);
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_main struk;

	go_parse(&struk, argc, argv);
	rules_round(&struk);
	start_ncurses(&struk);
	return (0);
}
