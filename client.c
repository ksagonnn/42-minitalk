#include "minitalk.h"

/*
  La fonction gère les signaux SIGUSR1/SIGUSR2 reçus par le serveur
  
  Le serveur n'enverra un signal qu'à la réception d'un signal
  du serveur, expliqué ci-dessous :
  - Client envoie un bit au serveur et attend une réponse en
  utilisant pause(), ensuite
  - Le serveur envoie un signal pour  confirmer que chaque bit a été 
  reçu (ACK) ou alors
  - Le serveur  envoie un signal pour confirmer que le NULL terminator
  a été reçu (end of string message), et ensuite la fonction exits

*/

static void	client_handler(int sig)
{
	if (sig == SIGUSR1)
		ft_putstr_fd("\e[33m > ACK signal received from server\n\e[0m",
			STDOUT_FILENO);

//STDOUT_FILENO est un integer file descriptor (integer 1) 

	else if (sig == SIGUSR2)
	{
		ft_putstr_fd("\e[92m > end of message signal received from server\n\e[0m",
			STDOUT_FILENO);
		exit(EXIT_SUCCESS);

// EXIT_SUCCESS : send 0 back, (success, "contraire" de STDOUT_FILENO)

	} 
}

/*
    La fonction renvoie la longueur de la string au serveur, 
    et ensuite la string elle-même, en incluant le null terminator
  
    Note: dans le cas particulier d'une string nulle, 
    zero sera renvoyé comme longueur et ensuite le null terminator
*/

static void	client_send_message(int server_pid, char *str)
{
	int	i;

	i = 0;
	{
		ft_putstr_fd("\e[92msending length = [", STDOUT_FILENO);
		// send the lenght of the string to server
		ft_putnbr_fd(ft_strlen(str), STDOUT_FILENO);
		ft_putstr_fd("]\n\e[0m", STDOUT_FILENO);
		// send the null terminator
		send_int(server_pid, ft_strlen(str));
		ft_putstr_fd("\e[92msending message\n\e[0m", STDOUT_FILENO);
		while (str[i] != '\0')
			send_char(server_pid, str[i++]);
		ft_putstr_fd("\e[92msending null string terminator\n\e[0m", STDOUT_FILENO);
		send_char(server_pid, '\0');
	}
}

/*
  Ce programme (client) envoie un message sous forme de string à un autre process (server)
  
  Les arguments passés en ligne de commande sont vérifiés :
  - La string et le serveur PID doivent être passés, rien de + ou de -
  - Le serveur PID doit être valide, kill() et ft_atoi sont utilisés pour
  vérifier ça (ça doit être des chiffres + les bons)

  La structure sigaction est utilisée pour la bonne communication entre
  l'utilisateur et le serveur. Pour chaque signal envoyé par le client,
  un signal de réponse doit être attendu par le serveur.
*/

int	main(int argc, char **argv)
{	
	struct sigaction	s_client;

	if (argc != 3)
	{
		ft_putstr_fd("\e[31m## error - incorrect syntax ##\n\e[0m", STDOUT_FILENO);
		ft_putstr_fd(
			"\e[92m./client <the server PID> <the string to send>\n\e[0m",
			STDOUT_FILENO);
		return (EXIT_FAILURE);
	}
	else if (kill(ft_atoi(argv[1]), 0) < 0)
	{
		ft_putstr_fd("\e[31m## error - PID is invalid ##\n\e[0m", STDOUT_FILENO);
		return (EXIT_FAILURE);
	}
	sigemptyset(&s_client.sa_mask);
	s_client.sa_flags = SA_RESTART;
	s_client.sa_handler = client_handler;
	configure_sigaction_signals(&s_client);
	ft_putstr_fd("\e[92mclient [PID = ", STDOUT_FILENO);
	ft_putnbr_fd(getpid(), STDOUT_FILENO);
	ft_putstr_fd("]\n\e[0m", STDOUT_FILENO);
	client_send_message(ft_atoi(argv[1]), argv[2]);
	return (EXIT_SUCCESS);
}
