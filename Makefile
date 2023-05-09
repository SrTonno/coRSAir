include colors.mk
###############################################################################
#                                              FILES                          #
###############################################################################
CFILE	=	corsair.c
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
	@echo "${God}${Green}Created '${NAME}'.${NoColor}"


git: fclean
	@echo "\t>>Push To Git<<"
	@git add . ;
	@read -p "Name the commit (One Word): " commit ;\
	git commit -m $$commit ;\
	git push origin master ;


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

help:
	@echo "${UGreen}Options of MakeFile:${NoColor}"
	@echo "Used: make [options]"
	@echo "\t${BICyan}Default:${NoColor} Created '${NAME}'"
	@echo "\t${IRed}clean:${NoColor} Delete '.o'."
	@echo "\t${BIRed}fclean:${NoColor} Delete'.o'"
	@echo "\t${BICyan}re:${NoColor} Delete '.o', '${NAME}' and creates '${NAME}'"
	@echo "\t${BIPurple}git:${NoColor} Push to git."
	@echo "MakeFile by ${UBlue}tvillare${NoColor}."

.PHONY = all clean fclean re help git normi