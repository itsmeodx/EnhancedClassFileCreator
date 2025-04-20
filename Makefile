SHELL = /bin/bash

NAME = hpp

SRCS = main.cpp

INCLUDES = main.hpp

FLAGS = -Wall -Wextra -Werror -O2

all : $(NAME)

dir :
	@echo -n "Enter the directory where you want to install the program (default is ~/.local/bin): "
	@read DIR && [ -z $$DIR ] && echo ~/.local/bin > /tmp/.dir || echo $$DIR > /tmp/.dir
	@mkdir -p $$(cat /tmp/.dir)
# @echo "Directory set to $$(cat /tmp/.dir)"

$(NAME) : dir $(SRCS) $(INCLUDES) path
	@g++ $(FLAGS) $(SRCS) -o $$(cat /tmp/.dir)/$@ && echo "Installed in $$(cat /tmp/.dir)" \
	|| echo "Error while compiling"
	@echo "you may need to reopen the shell to use the program"

path :
	@for profile in ~/.bashrc ~/.zshrc ~/.profile; do \
		if [ -f "$$profile" ]; then \
			grep -q "export PATH=.*$$(cat /tmp/.dir)" "$$profile" || \
			(echo "$$(cat /tmp/.dir)" | grep -q "^$$HOME/" && { \
				dir_without_home="$$(echo "$$(cat /tmp/.dir)" | sed "s|^$$HOME/||")"; \
				grep -q "export PATH=.*\\\$$HOME/$$dir_without_home" "$$profile" || \
				grep -q "export PATH=.*~/$$dir_without_home" "$$profile"; \
			}) || \
			{ echo "Updating $$profile"; echo "export PATH=\$$PATH:$$(cat /tmp/.dir)" >> "$$profile"; }; \
		fi; \
	done
# @echo "PATH has been updated in your shell configuration files"

update :
	@git pull 2> /dev/null | grep -qxF "Already up to date." && echo "Already up to date" || (make && echo "update done")

.PHONY : all update path dir
