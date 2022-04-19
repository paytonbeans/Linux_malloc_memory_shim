all: shim tester tracer

shim:
	gcc -O2 -Wall -ldl -fPIC -o memory_shim.so -shared memory_shim.c

tester:
	gcc -ggdb -Wall -ldl -fPIC -o leakcount leakcount.c

tracer:
	gcc -ggdb -Wall -ldl -fPIC -o sctracer sctracer.c

clean:
	rm -f *.so leackcount sctracer
