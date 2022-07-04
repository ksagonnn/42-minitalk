SERVER = server

CLIENT = client

LIBFT = ./libft/libft.a

HEADER = includes/minitalk.h

CFLAGS = -Wall -Werror -Wextra

all: $(LIBFT) $(CLIENT) $(SERVER) $(HEADER)

$(SERVER): src/server.c $(HEADER)
	@gcc src/server.c $(LIBFT) -0 $@
	@echo $@ ready

$(CLIENT): src/client.c $(HEADER)
	@gcc $(CFLAGS) src/client.c $(LIBFT) -o $@
	@echo $@ ready

$(LIBFT):
	@make -C ./libft

clean:
	@make clean -C ./libft

fclean: clean
	rm -rf $(SERVER) $(CLIENT)
	make fclean -C ./libft

re: fclean all

.PHONY: all re clean fclean
