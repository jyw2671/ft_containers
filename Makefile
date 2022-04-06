CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

INC_DIR = ./includes
STD_NAMESPACE = std
FT_NAMESPACE = ft
RM = rm -f

all :

clean :

fclean : clean

re : fclean all

.PHONY : all clean fclean re
