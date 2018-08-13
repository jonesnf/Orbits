#include <stdio.h>
#include <math.h>
#include <time.h>
#include "SpiceUsr.h"

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
#define TARGET   "earth"
#define OBSERVER "sun"
#define FRAME    "J2000"
#define ABCORR   "LT+S"
#define AU       150000000


// TODO: make this into general get_position function
//       which will be used to gather position data of all planets
void earth_pos ( SpiceDouble* et, SpiceDouble epos[] ) {
  SpiceDouble ltsec;
  spkpos_c( TARGET, *et, FRAME, ABCORR, OBSERVER, epos, &ltsec); 
}

SpiceDouble math ( SpiceDouble p[] ) {
  SpiceDouble result = sqrt((pow(p[0],2) + pow(p[1],2) +  pow(p[2],2)));
  return result;
}

// Get time (UTC)
void get_time ( char* mytime ) {
  time_t timet;
  struct tm * s_t;
  time(&timet);
  s_t = gmtime(&timet);
  // Convert current time data to a string 
  sprintf(mytime, "%d %d %d %d:%d:%d", s_t->tm_year+1900, s_t->tm_mon+1,\
                  s_t->tm_mday, s_t->tm_hour, s_t->tm_min, s_t->tm_sec );
} 

int main ( int argc, char** argv ){
  SpiceChar ttt[WORDZ]; 
  SpiceDouble et, dist; // ephemiris-based time
  SpiceDouble pos[3];
  char frmt_time[30];
  get_time(frmt_time);
  printf("Time (UTC): %s\n", frmt_time);
  printf("Loading Kernels...\n");
  furnsh_c( "metakernel.tm" );
  str2et_c(frmt_time, &et);
  //spkpos_c( TARGET, et, FRAME, ABCORR, OBSERVER, pos, &lt); 
  /*printf("et: %f\n", et);
  printf("J2000 x-pos (km): %f\n", pos[0]);
  printf("J2000 y-pos (km): %f\n", pos[1]);
  printf("J2000 z-pos (km): %f\n", pos[2]);
  */
  earth_pos(&et, pos); 
  dist = math(pos);
  printf("SUN -> EAR (AU): %f\n", dist / AU); 
  printf("SUN -> EAR AU (X, Y) Coord: (%f, %f)\n", pos[0] / AU, pos[1] / AU);
  return 0;
}
