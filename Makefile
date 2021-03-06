.PHONY: all clean

#FLAGS=-DDEBUG

all: output

output:
	g++ $(FLAGS) src/main.cpp -o gsm7b_codec.out

clean:
	rm *.o output