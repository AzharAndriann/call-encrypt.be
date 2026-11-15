main: src/main.c library/mongoose.c
	gcc src/main.c library/mongoose.c -o main -Ilibrary -lpthread