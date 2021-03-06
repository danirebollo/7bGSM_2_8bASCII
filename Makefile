.PHONY: all clean

all: output

output:
	g++ src/main.cpp -o gsm7b_codec.out

clean:
	rm *.o output