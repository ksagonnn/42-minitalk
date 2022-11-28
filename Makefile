#	GENERAL CONFIG & FLAGS
# programme compilateur C
CC := cc

# Extra flags pour le compilateur C
CFLAGS := -Wall -Wextra -Werror

RM := /bin/rm -f

NAME_SERVER := server
NAME_CLIENT := client

SRC_SERVER := server.c minitalk.c
SRC_CLIENT := client.c minitalk.c

SRC_SERVER_BONUS := server_bonus.c minitalk_bonus.c
SRC_CLIENT_BONUS := client_bonus.c minitalk_bonus.c

INCLUDES := -I. -Ilibft

LIBFT := ./libft/libft.a
LIBFT_PATH := ./libft

#	RULES

all: $(NAME_SERVER) $(NAME_CLIENT)

$(NAME_SERVER): $(LIBFT)
	$(CC) $(CFLAGS) $(SRC_SERVER) $(LIBFT) -o $(NAME_SERVER)

$(NAME_CLIENT): $(LIBFT)
	$(CC) $(CFLAGS) $(SRC_CLIENT) $(LIBFT) -o $(NAME_CLIENT)

# Make libft
$(LIBFT): $(shell make -C $(LIBFT_PATH) -q libft.a)
	make -C $(LIBFT_PATH)

clean:

# Clean fichiers objets (*.o) et binaires
fclean: clean
	$(RM) $(NAME_SERVER) $(NAME_CLIENT)

# Clean fichiers objets (*.o) et binaires; 
# crée à nouveau le fichier binaire et génère la librairie et l'indexe
re: fclean all

# .PHONY pour pas que ça relink
.PHONY: all clean fclean re bonus rebonus
