# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llebugle <lucas.lebugle@student.s19.be>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/25 18:26:27 by llebugle          #+#    #+#              #
#    Updated: 2025/08/26 19:31:03 by jfranco          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minis2

FLAGS = -Wall -Wextra -Werror -std=c++17 -g3 

SRCS_DIR = srcs
OBJS_DIR = objs

SRCS = 	srcs/cpp/main.cpp \
		srcs/cpp/ClassMenagerMiniS2.cpp \
		srcs/Logger/srcs/Logger.cpp \
		srcs/camera/src/Camera.cpp

INC = -I ./srcs/include/ -I ./srcs/camera/include/ -I ./srcs/Logger/include/ -I ./srcs/include/external/

OBJS = $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
DEPS = $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.d)

all: $(NAME) 

$(NAME): $(OBJS)
	c++ $(FLAGS) $(OBJS) -llgpio -o  $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	c++ $(FLAGS) $(INC) -MMD -MP -c $< -o  $@

# Include dependency files if they exist
-include $(DEPS)

run: all
	./$(NAME)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf build
	rm -f $(NAME)

re: fclean all
	
.PHONY: all clean fclean re

valgrind: all
	valgrind ./$(NAME)
