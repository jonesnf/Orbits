#ifndef __PLANETS__H__
#define __PLANETS__H__

#include "SpiceUsr.h"    
    

// definition of planets 
#define SUN 0
#define MER 1
#define VEN 2
#define EAR 3
#define MAR 4
#define JUP 5
#define SAT 6
#define URA 7
#define NEP 8
#define PLU 9  // I'll still plot you little buddy even though you're not a planet :)  

char* plnt_names[10] = {"SUN", "MERCURY", "VENUS", "EARTH", "MARS", "JUPITER",\
                        "SATURN", "URANUS", "NEPTUNE", "pluto"};

struct Planets {
    char        name[10];
    SpiceDouble pos[3];
    SpiceInt    xpos;
    SpiceInt    ypos;
}; 


#endif
