compile:
	clang -Wall -Oz -x c -o ./build/hrsh ./src/*.c ./src/*.h
debug:
	clang -Wall -g -x c -o ./build/hrsh ./src/*.c ./src/*.h
