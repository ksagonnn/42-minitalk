// header 42

#ifndef MINITALK_H
# define MINITALK_H

# include <signal.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "../libft/libft.h"

typedef struct s_signal
{
	unsigned int	bit;
	char		symbol;
	int		pid;
}			t_signal
t_signal	g_bites;

#endif
