##
## EPITECH PROJECT, 2023
## MAKEFILE
## File description:
## Makefile
##

NAME		=	unit_tests

CFLAGS  	=	-I$(CORE_DIR)	\
				-I$(ITRF_DIR)	\
				-I$(DYLOAD_DIR)	\
				-I$(CONFIG_DIR)	\

CPPFLAGS	=	-W -Wall -Wextra -Werror

LDFLAGS		=	--coverage -lcriterion

COVFLAGS	=	-lcriterion -lgcov

SRC_DIR		=	../src/

CORE_DIR	=	$(SRC_DIR)Core/
CONFIG_DIR	=	$(SRC_DIR)Config/
ITRF_DIR	=	$(SRC_DIR)Interfaces/
DYLOAD_DIR	=	$(SRC_DIR)DynamicLoader/

SRC_TESTS	=	$(SRC_DIR)CheckErrors.cpp		\

UT_FILES	=	Tests_CheckErrors.cpp		\

SRC_OBJS	=	$(SRC_TESTS:.cpp=.o)

UT_OBJS		=	$(UT_FILES:.cpp=.o)

all: $(NAME)
	@echo "Build ok!"

$(NAME) :
	g++ -c $(SRC_TESTS) $(CPPFLAGS) $(CFLAGS) $(LFLAGS) $(LDFLAGS)
	g++ -c $(UT_FILES) $(CPPFLAGS) $(CFLAGS) $(LFLAGS)
	g++ -o $(NAME) *.o $(CPPFLAGS) $(CFLAGS) $(LFLAGS) $(COVFLAGS)
	./$(NAME)


clean:
	@$(RM) *.html
	@$(RM) *.css
	@$(RM) $(SRC_DIR)*.gcno
	@$(RM) $(SRC_DIR)*.gcda
	@$(RM) $(TEST_DIR)*.gcda
	@$(RM) $(TEST_DIR)*.gcno
	@$(RM) $(TEST_DIR)*.o
	@$(RM) $(TESTS_OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
