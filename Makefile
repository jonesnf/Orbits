orbits: 
	gcc -O2 orbits.c -I./cspice/cspice/include ./cspice/cspice/lib/cspice.a -lm -o orbits
clean: 
	rm orbits
