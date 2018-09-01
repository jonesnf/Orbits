#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "planets.h"

#define WORDZ    41
#define TARGET   "sun"
#define OBSERVER "sun"
#define FRAME    "J2000"
#define ABCORR   "LT+S"
#define AU       150000000
#define SCALE    6 
#define YAXIS    50 
#define XAXIS    70 
#define FIT      4 
#define L2FIT    6 
#define L3FIT    11 

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
    // Assign plnt symbol on specific coordinates
    for ( int i = SUN; i < PLU+1; i++) {
        if ( i < 5 ) {
          map[plnts[i].ypos + cntrY][(plnts[i].xpos * -1) + cntrX] = plnts[i].sym;
        } else if ( i < 6 ) {
        
          map[plnts[i].ypos/FIT + cntrY][(plnts[i].xpos * -1)/FIT + cntrX] = plnts[i].sym;
        } else if ( i < 9 ) {
          map[plnts[i].ypos/L2FIT + cntrY][(plnts[i].xpos * -1)/L2FIT + cntrX]\
              = plnts[i].sym;
        } else {
          map[plnts[i].ypos/L3FIT + cntrY][(plnts[i].xpos * -1)\
              /L3FIT + cntrX] = plnts[i].sym;
        }
    }  
    // Plot
    for ( int row = 0; row < YAXIS; row++) {
        for ( int col = 0; col < XAXIS; col++) {
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

// Essentially converting coords in AU to coords in char spaces
void math ( SpiceDouble p[], SpiceInt* xpos, SpiceInt* ypos ) {
  *xpos = p[0] / AU * SCALE;
  *ypos = p[1] / AU * SCALE;
}


void fill_plnts (struct Planets plnts[], const SpiceDouble* et) {
   // Fill syms/names of planets, positions, and convert to ints
   // Only have data for sun, mer, venus, earth
   for ( int i = SUN; i < PLU + 1; i++ ) {
       plnts[i].sym = plnt_sym[i];
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
  // ephemiris-based time
  SpiceDouble et, dist; 
  char frmt_time[30];
  struct Planets p[10];
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
