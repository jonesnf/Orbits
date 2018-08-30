#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
//#include "SpiceUsr.h"
#include "planets.h"

#define WORDZ    41
#define TARGET   "sun"
#define OBSERVER "sun"
#define FRAME    "J2000"
#define ABCORR   "LT+S"
#define AU       150000000
#define SCALE    10 

// unfortunately, resorted to global matrix (for now)
char map[25][50];

void draw ( struct Planets plnts[] ) {
    for ( int row = 0; row < 25; row++) {
        for ( int col = 0; col < 50; col++) {
            map[row][col] = ' ';
        }
    }  
    map[plnts[1].ypos + 12][(plnts[1].xpos * -1) + 25] = 'M';
    map[plnts[2].ypos + 12][(plnts[2].xpos * -1) + 25] = 'V';
    map[plnts[3].ypos + 12][(plnts[3].xpos * -1) + 25] = 'E';
    map[12][25] = 'S';
    for ( int row = 0; row < 25; row++) {
        for ( int col = 0; col < 50; col++) {
            printf("%c", map[row][col]);
        }
        printf("\n");
    }  
}


// TODO: make this into general get_position function
//       which will be used to gather position data of all planets
void planet_pos ( const SpiceDouble* et, struct Planets* plnt ) {
  SpiceDouble ltsec;
  spkpos_c( TARGET, *et, FRAME, ABCORR, plnt->name, plnt->pos, &ltsec); 
  printf("%s: %f, %f\n", plnt->name, plnt->pos[0] / AU * SCALE, plnt->pos[1] / AU * SCALE);
}

void math ( SpiceDouble p[], SpiceInt* xpos, SpiceInt* ypos ) {
  *xpos = p[0] / AU * SCALE;
  *ypos = p[1] / AU * SCALE;
}


void fill_plnts (struct Planets plnts[], const SpiceDouble* et) {
   // Fill names of planets, positions, and convert to ints
   // Only have data for sun, mer, venus, earth
   for ( int i = SUN; i < MAR; i++ ) {
       strcpy(plnts[i].name, plnt_names[i]);
       planet_pos(et, &plnts[i]);
       math(plnts[i].pos, &plnts[i].xpos, &plnts[i].ypos); 
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
  char frmt_time[30];
  struct Planets p[9];
  // Get the current time in specific format (UTC)
  get_time(frmt_time);
  printf("Time (UTC): %s\n", frmt_time);
  printf("Loading Kernels...\n");
  // Load the necessary kernels
  furnsh_c( "metakernel.tm" );
  str2et_c(frmt_time, &et);
  fill_plnts(p, &et);
  // Testing out integer forms if each space char is 1 AU
  printf("SUN -> EAR (AU) (X, Y) Coord: (%d, %d)\n", p[3].xpos, p[3].ypos);
  draw(p);
  return 0;
}
