NAME = hashmap

SRCS =	$(wildcard *.c) \


CC = gcc

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) -g $(SRCS) -o $(NAME)

clean: 
	rm -f $(NAME)

re: clean all

.PHONY: all clean re
