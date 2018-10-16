#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "planets.h"
//                                                  _      _
//Probably can put these in planets.h but oh well    \(00)/
#define WORDZ    41
#define TARGET   "sun"
#define OBSERVER "sun"
#define FRAME    "J2000"
#define ABCORR   "LT+S"
#define AU       150000000
#define SCALE    6 
#define YAXIS    50 
#define XAXIS    90 

// unfortunately, resorted to global matrix (for now)
char map[YAXIS][XAXIS];

void draw ( struct Planets plnts[] ) {
    // First, draw map / coordinate plane / graph
    for ( int row = 0; row < YAXIS; row++) {
        for ( int col = 0; col < XAXIS; col++) {
            map[row][col] = ' ';
        }
    }  
    int cntrX = XAXIS / 2; int cntrY = YAXIS / 2;
    for ( int i = SUN; i < PLU + 1; i++ ) {
        map[plnts[i].ypos + cntrY][(plnts[i].xpos * -1) + cntrX] = plnts[i].sym;
    }
    // Plot newly added planets
    for ( int row = 0; row < YAXIS; row++) {
        for ( int col = 0; col < XAXIS; col++) {
            printf("%c", map[row][col]);
        }
        printf("\n");
    }  
}

void planet_pos ( const SpiceDouble* et, struct Planets* plnt ) {
  SpiceDouble ltsec;
  spkpos_c( TARGET, *et, FRAME, ABCORR, plnt->name, plnt->pos, &ltsec); 
  // For DEBUG only
  /*printf("%s: %f, %f\n", plnt->name,\
          plnt->pos[0] / AU * SCALE, plnt->pos[1] / AU * SCALE);*/
}

/* Scale linearly for Sun - Earth; Mars - pluto scale logrithmically, but remember
 *   their if their X (or Y) coordinate was negative.  This let's us take the log 
 *   w/o getting a decimal.
 * */
void math ( SpiceDouble p[], SpiceInt* xpos, SpiceInt* ypos, const int *plnt ) {
  if ( *plnt < 4 ) {
      *xpos = p[0] / AU * SCALE;
      *ypos = p[1] / AU * SCALE;
  } else {
      if ( p[0] > 0 ) 
        *xpos = (log10(fabs(p[0])) / AU + 1) * SCALE;
      else
        *xpos = (log10(fabs(p[0]) / AU) + 1) * SCALE * -1;

      if ( p[1] > 0 ) 
        *ypos = (log10(fabs(p[1]) / AU) + 1) * SCALE;
      else
        *ypos = (log10(fabs(p[1]) / AU) + 1) * SCALE * -1;
  }
}

void fill_plnts (struct Planets plnts[], const SpiceDouble* et) {
   // Fill syms/names of planets, positions, and convert to ints
   for ( int i = SUN; i < PLU + 1; i++ ) {
       plnts[i].sym = plnt_sym[i];
       strcpy(plnts[i].name, plnt_names[i]);
       planet_pos(et, &plnts[i]);
       math(plnts[i].pos, &plnts[i].xpos, &plnts[i].ypos, &i); 
   } 
}

// Get time (UTC) and convert it into a string so we can send to SPICE
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
  SpiceDouble et, dist; 
  char frmt_time[30];
  struct Planets p[10];
  get_time(frmt_time);
  printf("Time (UTC): %s\n", frmt_time);
  printf("Loading Kernels...\n");
  // Load the necessary kernels in order to grab planet data
  furnsh_c( "metakernel.tm" );
  str2et_c(frmt_time, &et);
  fill_plnts(p, &et);
  // printf("SUN -> EAR (AU) (X, Y) Coord: (%d, %d)\n", p[3].xpos, p[3].ypos);
  draw(p);
  return 0;
}
