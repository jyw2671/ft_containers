CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

INC_DIR = ./includes
TESTER_DIR = ./mytester
TESTER_LOG_DIR = ./mytester/log

RM = rm -f

STD = std
FT = ft
CONT = vector_test
TIME = time

ifeq ($(TESTED_NAMESPACE),)
TESTED_NAMESPACE = ft
endif

all :
	@make start
	@make test

start :
	@$(TESTER_DIR)/mytest.sh

test :
	@make mytest CONT=vector_test
	@make mytest CONT=stack_test
	@make mytest CONT=map_test
	# @make mytest CONT=set_test

mytest :
	@mkdir -p $(TESTER_LOG_DIR)
	@$(CC) $(CFLAGS) $(TESTER_DIR)/$(CONT).cpp -o $(CONT) -I$(INC_DIR) -DTESTED_NAMESPACE=$(FT)
	@./$(CONT) > $(TESTER_LOG_DIR)/$(FT)_$(CONT)
	@$(CC) $(CFLAGS) $(TESTER_DIR)/$(CONT).cpp -o $(CONT) -I$(INC_DIR) -DTESTED_NAMESPACE=$(STD)
	@./$(CONT) > $(TESTER_LOG_DIR)/$(STD)_$(CONT)
	@diff $(TESTER_LOG_DIR)/$(STD)_$(CONT) $(TESTER_LOG_DIR)/$(FT)_$(CONT)
	@rm $(CONT)
	@$(TESTER_DIR)/compare.sh $(CONT)

print :
	@$(CC) $(CFLAGS) $(TESTER_DIR)/print_RBtree.cpp -o map -I$(INC_DIR)
	@./map
	@rm map

time :
	@make time_unit CONT=vector_test
	@make time_unit CONT=stack_test
	@make time_unit CONT=map_test
	# @make time_unit CONT=set_test

time_unit :
	@$(CC) $(CFLAGS) $(TESTER_DIR)/$(CONT).cpp -o $(CONT) -I$(INC_DIR) -DTESTED_NAMESPACE=$(FT)
	@printf "\n=====\t$(CONT)\t====="
	@printf "\ntime 'FT'"
	@$(TIME) ./$(CONT) > $(TESTER_LOG_DIR)/$(FT)_$(CONT)
	@$(CC) $(CFLAGS) $(TESTER_DIR)/$(CONT).cpp -o $(CONT) -I$(INC_DIR) -DTESTED_NAMESPACE=$(STD)
	@printf "time 'STD'"
	@$(TIME) ./$(CONT) > $(TESTER_LOG_DIR)/$(STD)_$(CONT)
	@rm $(CONT)

clean :
	@$(RM) -r $(TESTER_LOG_DIR)

fclean : clean

re : fclean all

.PHONY: all clean fclean re start test mytest time time_unit
