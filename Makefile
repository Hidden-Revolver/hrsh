COMPILE_DIRECTORY=./bin

compile:
	mkdir -p $(COMPILE_DIRECTORY)
	clang -std=c99 -fsanitize=memory -Wall -Wextra -Werror -Oz -x c -o $(COMPILE_DIRECTORY)/hrsh ./src/*.c ./src/*.h # -Oz es porque buscamos el tamaño más pequeño para la shell
run: compile
	$(COMPILE_DIRECTORY)/hrsh
debug:
	mkdir -p $(COMPILE_DIRECTORY)
	clang -std=c99 -Wall -Wextra -g -x c -o $(COMPILE_DIRECTORY)/hrsh ./src/*.c ./src/*.h
