SRCS = main.cpp

OBJS = ${SRCS:.cpp=.o}

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -f

NAME = webserver

${NAME}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}

all: ${NAME}

clean:	
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}
	${RM} client server

server:
	${CXX} poll_server.cpp -o server

client:
	${CXX} client.cpp -o client

re: fclean all

.PHONY: fclean all clean re