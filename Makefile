compile:
	mkdir -p ./build
	clang -Wall -Wextra -Oz -x c -o ./build/hrsh ./src/*.c ./src/*.h
debug:
	mkdir -p ./build
	clang -Wall -Wextra  -g -x c -o ./build/hrsh ./src/*.c ./src/*.h
