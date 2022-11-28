#include "minitalk.h"

/*
  La fonction vérifie que le string message est fini.
  Dans le cas où la null terminator string est reçue, imprime la string
  enière, libère de la mémoire heap et informe le client que le message
  a bien été reçu (client va exit), avec send_bit() sans signal retour
  attendu de client.
*/

static void	server_is_message_finished(
	t_protocol *t_server, int *i, pid_t client_pid)
{
	if (t_server->bits == 8 && t_server->flag == 1)
	{
		t_server->message[*i] = t_server->data;
		(*i)++;
		if (t_server->data == '\0')
		{
			ft_putstr_fd("\e[92mreceived message = [", STDOUT_FILENO);
			ft_putstr_fd(t_server->message, STDOUT_FILENO);
			ft_putstr_fd("]\n\e[0m", STDOUT_FILENO);
			free(t_server->message);
			t_server->message = NULL;
			t_server->flag = 0;
			*i = 0;
			send_bit(client_pid, 1, 0);
		}
		t_server->bits = 0;
	}
}

/*
  La fonction vérifie si la longueur de bits de la string est finie
  Si oui, la longueur est print par stdout et utilisée pour allouer
  de la mémoire dans la heap de la taille exacte de la string + le /n
*/

static void	server_is_str_length_finished(t_protocol *t_server)
{
	if (t_server->bits == sizeof(int) * 8 && t_server->flag == 0)
	{
		t_server->flag = 1;
		ft_putstr_fd("\e[92mreceived length = [", STDOUT_FILENO);
		ft_putnbr_fd(t_server->data, STDOUT_FILENO);
		ft_putstr_fd("]\n\e[0m", STDOUT_FILENO);
		t_server->message = ft_calloc(t_server->data + 1, sizeof(char));
		if (t_server->message == NULL)
		{
			ft_putstr_fd("\e[31m## error - ft_calloc() ##\n\e[0m", STDOUT_FILENO);
			exit(EXIT_FAILURE);
		}
		t_server->message[t_server->data] = '\0';
		t_server->bits = 0;
	}
}

/*
  La fonction intercepte les signaux SIGUSR1/SIGUSR2 reçus du client qui
  représentent l'état de chaque bit (soit 0 soit 1)
  Les variables statiques sont initialisées à 0, et après chaque 8 bits reçus
  sont à nouveau reinitialisées à 0
  Pour chauqe bit reçu du client, le serveur envoie un signal ACK 
  (ensuite le client ne devrait envoyer que le bit suivant après ce signal ACK)
*/

static void	server_handler(int num, siginfo_t *info, void *context)
{
	static t_protocol	t_server;
	static int			i;

	usleep(WAIT_US);
	(void)context;
	(void)info;
	if (t_server.bits == 0)
		t_server.data = 0;
	if (num == SIGUSR2 && t_server.flag == 0)
		t_server.data |= 1 << (((sizeof(int) * 8) - 1) - t_server.bits);
	else if (num == SIGUSR2 && t_server.flag == 1)
		t_server.data |= 1 << (((sizeof(char) * 8) - 1) - t_server.bits);
	t_server.bits++;
	server_is_str_length_finished(&t_server);
	server_is_message_finished(&t_server, &i, info->si_pid);
	send_bit(info->si_pid, 0, 0);
}

/*
  Ce prpgramme (serveur) imprime avec stdout le PID process et reste
  attentif pour les messages entrants
  
  Chaque client doit utiliser la séquence suivante :

               (in)               (char)              (char)
       longueur du message -> regular chars -> null string terminator
  
  Le message de string est print par stdout seulement quand la string entière 
  est reçue.
  Pour chaque signal reçu de client, ce programme (serveur) envoie
  un signal correspondant en retour.
  
  En utilisant la structure sigaction pour définir quel signal intercepter
  par le handler
  La boucle infinie ne fera que maintenir le process en cours d'excécution

  La fonction pause() permet de laisser en pause tant qu'un signal n'est pas
  reçu
*/

int	main(void)
{
	struct sigaction	s_server;

	sigemptyset(&s_server.sa_mask);
	s_server.sa_sigaction = server_handler;
	s_server.sa_flags = SA_SIGINFO | SA_RESTART;
	configure_sigaction_signals(&s_server);
	ft_putstr_fd("\e[92mserver [PID = ", STDOUT_FILENO);
	ft_putnbr_fd(getpid(), STDOUT_FILENO);
	ft_putstr_fd("]\n\e[0m", STDOUT_FILENO);
	while (1)
	{
		pause();
	}
	return (EXIT_SUCCESS);
}
