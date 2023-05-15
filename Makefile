include colors.mk
###############################################################################
#                                              FILES                          #
###############################################################################
CFILE	=	corsair.c create_private.c decrypt_file.c
OBJS	=	${CFILE:.c=.o}

###############################################################################
#                                              SETTINGS                       #
###############################################################################
NAME = corsair
HDRS = . #corsair.h


CC = clang
#CFLAGS = -Wall -Werror -Wextra
SANITIZE = -fsanitize=address -g

AR = ar
ARFLAGS = -rcs
RM = rm -f

CFLAGS += -I /goinfre/tvillare/.brew/opt/openssl@1.1/include 
LDLIBS = -L /goinfre/tvillare/.brew/opt/openssl@1.1/lib -lssl -lcrypto

###############################################################################
#                                              OPTIONS                        #
###############################################################################
all: ${NAME}

${NAME}: ${OBJS}
	@${CC} ${CFLAGS} ${LDLIBS} ${OBJS} -o $@
	@echo "${Good}${Green}Created '${NAME}'.${NoColor}"


.c.o:
		@${CC} ${CFLAGS} -c $< -o ${<:.c=.o} -I ${HDRS}

###############################################################################
clean:
		@${RM} ${OBJS} ${LIBFT}
		@echo "${Bad}${Red}Delete .o.${NoColor}"

fclean: clean
		@${RM} ${NLIBRARY} ${NAME}
		@echo "${Bad}${Red}Delete '${NLIBRARY}' and '${NAME}'.${NoColor}"

re: fclean all

###############################################################################

git: fclean
	@echo "\t>>Push To Git<<"
	@git add . ;
	@read -p "Name the commit (One Word): " commit ;\
	git commit -m $$commit ;\
	git push origin master ;

setup:
	@rm -rf $$HOME/.brew && git clone https://github.com/Homebrew/brew $$HOME/goinfre/.brew
	#echo 'export PATH=$$HOME/goinfre/.brew/bin:$$PATH' >> $$HOME/.zshrc && source $$HOME/.zshrc
	@brew update
	@brew install openssl@1.1
	@printf "${Good}${BICyan}Install brew and library in MAC.${Good} "


help:
	@echo "${UGreen}Options of MakeFile:${NoColor}"
	@echo "Used: make [options]"
	@echo "\t${BICyan}Default:${NoColor} Created '${NAME}'"
	@echo "\t${IRed}clean:${NoColor} Delete '.o'."
	@echo "\t${BIRed}fclean:${NoColor} Delete'.o'"
	@echo "\t${BICyan}re:${NoColor} Delete '.o', '${NAME}' and creates '${NAME}'"
	@echo "\t${BIPurple}git:${NoColor} Push to git."
	@echo "MakeFile by ${UBlue}tvillare${NoColor}."

.PHONY = all clean fclean re help git normi setup
