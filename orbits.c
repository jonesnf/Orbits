#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
//#include "SpiceUsr.h"
#include "planets.h"

#define SUN 0
#define MER 1
#define VEN 2
#define EAR 3
#define MAR 4
#define JUP 5
#define SAT 6
#define URA 7
#define NEP 8
#define PLU 9  // I still consider you a planet little buddy! 

#define WORDZ    41
#define TARGET   "sun"
#define OBSERVER "earth"
#define FRAME    "J2000"
#define ABCORR   "LT+S"
#define AU       150000000
#define SCALE    4
void draw(SpiceInt* x, SpiceInt* y) {
  int y_ax, x_ax;
  for (  y_ax= 0;  y_ax< 24; y_ax++ ) {
    for ( x_ax = 0; x_ax < 50; x_ax++ ) {
      if ( x_ax == 25 && y_ax == 12 ) {
        printf("Su");
      } else if ( x_ax == (*x * (-1) +25) && y_ax == (*y+12) ) {
        printf(" E");
      } else {
        printf(" "); 
      } 
    } printf("\n");
  }
}


// TODO: make this into general get_position function
//       which will be used to gather position data of all planets
void earth_pos ( SpiceDouble* et, SpiceDouble epos[] ) {
  SpiceDouble ltsec;
  spkpos_c( TARGET, *et, FRAME, ABCORR, OBSERVER, epos, &ltsec); 
}

SpiceDouble math ( SpiceDouble p[], SpiceInt* xpos, SpiceInt* ypos ) {
  SpiceDouble result = sqrt((pow(p[0],2) + pow(p[1],2) +  pow(p[2],2)));
  *xpos = p[0] / AU * SCALE;
  *ypos = p[1] / AU * SCALE;
  return result;
}

void get_dists (){} 

void fill_plnts (struct Planets plnts[]) {
   for ( int i = 0; i < 10; i++ ) {
       strcpy(plnts[i].name, plnt_names[i]);
   } 
}

// Get time (UTC)
void get_time ( char* mytime ) {
  time_t timet;
  struct tm * s_t;
  time(&timet);
  s_t = gmtime(&timet);
  // Convert current time data to a string 'mytime' 
  sprintf(mytime, "%d %d %d %d:%d:%d", s_t->tm_year+1900, s_t->tm_mon+1,\
                  s_t->tm_mday, s_t->tm_hour, s_t->tm_min, s_t->tm_sec );
} 

int main ( int argc, char** argv ){
  SpiceDouble et, dist; // ephemiris-based time
  SpiceDouble pos[3];
  // Need these for the time being
  SpiceInt xpos, ypos;
  char frmt_time[30];
  struct Planets p[9];
  fill_plnts(p);

  get_time(frmt_time);
  printf("Time (UTC): %s\n", frmt_time);
  printf("Loading Kernels...\n");
  // Load the necessary kernels
  furnsh_c( "metakernel.tm" );
  str2et_c(frmt_time, &et);
  earth_pos(&et, p[3].pos); 
  // get integer x & y pos 
  dist = math(p[3].pos, &xpos, &ypos); 
  printf("SUN -> EAR (AU) (X, Y) Coord: (%f, %f)\n", pos[0] / AU, pos[1] / AU);
  // Testing out integer forms if each space char is .3 AU
  printf("SUN -> EAR (AU) (X, Y) Coord: (%d, %d)\n", \
        (SpiceInt)(pos[0] / AU * SCALE), (SpiceInt)(pos[1] / AU * SCALE));
  draw(&xpos, &ypos);
  
  return 0;
}
