#include "minitalk.h"

/*
  La fonction définit les signaux a intercépter par le gestionnaire.
  Si cette action échoue, print to stdout un message d'erreur et 
  quitte le programme
 */

void	configure_sigaction_signals(struct sigaction *sa)
{
	if (sigaction(SIGUSR1, sa, NULL) < 0)
	{
		ft_putstr_fd("\e[31m## error - could not setup SIGUSR1 ##\n\e[0m",
			STDOUT_FILENO);
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGUSR2, sa, NULL) < 0)
	{
		ft_putstr_fd("\e[31m## error - could not setup SIGUSR2 ##\n\e[0m",
			STDOUT_FILENO);
		exit(EXIT_FAILURE);
	}
}

/*
  La fonction envoie un integer qui contient la longueur du message
  Pour chaque bit envoyé au client, attends en retour un signal avant
  de procéder en utilisant flag = 1 sur send_bit()
  Assuming 1 byte = 8 bits
 */

void	send_int(pid_t pid, int num)
{
	int		shift;
	char	bit;

	shift = (sizeof(int) * 8) - 1;
	while (shift >= 0)
	{
		bit = (num >> shift) & 1;
		send_bit(pid, bit, 1);
		shift--;
	}
}

/*
  La fonction envoie 1 char, qui est normalement un octet (8 bits)
  Pour chaque bit envoyé par client, attends en retour un signal avant
  de procéder en utilisant flag = 1 sur send_bit()
  Toujours 1 byte = 8 bits
*/

void	send_char(pid_t pid, char c)
{
	int		shift;
	char	bit;

	shift = (sizeof(char) * 8) - 1;
	while (shift >= 0)
	{
		bit = (c >> shift) & 1;
		send_bit(pid, bit, 1);
		shift--;
	}
}

/*
  La fonction envoie un bit (0 ou 1) au processeur PID
  Le retour de fonction arrive après que le signal ACK 
  soit reçu si le flag wait est set à non zero, autrement
  renvoie directement
 */

void	send_bit(pid_t pid, char bit, char flag_to_pause)
{
	if (bit == 0)
	{
		if (kill(pid, SIGUSR1) < 0)
		{
			ft_putstr_fd("\e[31m## error - sending SIGUSR1 ##\n\e[0m",
				STDOUT_FILENO);
			exit(EXIT_FAILURE);
		}
	}
	else if (bit == 1)
	{
		if (kill(pid, SIGUSR2) < 0)
		{
			ft_putstr_fd("\e[31m## error - sending SIGUSR2 ##\n\e[0m",
				STDOUT_FILENO);
			exit(EXIT_FAILURE);
		}
	}
	if (flag_to_pause != 0)
		pause();
}
