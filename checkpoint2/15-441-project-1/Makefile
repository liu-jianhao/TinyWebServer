OBJ := src/liso/liso.c src/log/log.c src/parse/parse.c src/parse/lex.yy.c src/parse/y.tab.c \
		src/response/response.c src/client_pool/client_pool.c src/io/io.c src/utilities/utilities.c

all:
	gcc -g -Wall -o lisod $(OBJ)
clean:
	rm lisod
