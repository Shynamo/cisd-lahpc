# VARIABLE
SRC_DIR=src
BLD_DIR=build
TST_DIR=test

SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
TST_FILES=$(wildcard $(TST_DIR)/*.c)
SRC_OBJ=$(SRC_FILES:%.c=%.o)
TST_OBJ=$(TST_FILES:%.c=%.o)

CFLAGS= -Wall -Wextra -Wno-unused-parameter -I ./perf/ -I ./$(SRC_DIR)/
CLIBS=

# COMPILATION

main: $(SRC_OBJ)
	mkdir -p $(BLD_DIR)
	gcc $(CLIBS) $^ -o $(BLD_DIR)/main

libs: $(SRC_OBJ)
	mkdir -p $(BLD_DIR)
	gcc -shared $(CLIBS) -o $(BLD_DIR)/libmyblas.so  $(SRC_DIR)/my_lapack.o -fPIC
	gcc -shared $(CLIBS) -o $(BLD_DIR)/libmylapack.so  $(SRC_DIR)/my_lapack.o -fPIC
	echo "Il faut écrire la commande pour les lib en .a"

test: $(SRC_OBJ) $(TST_OBJ)
	mkdir -p $(BLD_DIR)
	echo "TODO: retirer 'main.o' des fichiers SRC_OBJ de cette commande"
	gcc $(CLIBS) $(SRC_OBJ) $(TST_DIR)/test_perf.o -o $(BLD_DIR)/test_perf
	gcc $(CLIBS) $(SRC_OBJ) $(TST_DIR)/test_valid.o -o $(BLD_DIR)/test_valid

# RUN BINAIRIES
runtest: test
	$(BLD_DIR)/test_valid
	$(BLD_DIR)/test_perf

run: main
	$(BLD_DIR)/main


# USEFULL TOOLS
clean:
	rm -f main
	rm -f *.o $(SRC_DIR)/*.o
	rm -f *.so *.a
	rm -rf $(BLD_DIR)

%.o: %.c
	gcc -c $(CFLAGS) $< -o $@