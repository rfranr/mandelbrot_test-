#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 600    
#define WINDOW_HEIGHT 400

#define CONSTANTR -0.00027
#define CONSTANTI -0.0025

#define NUMOFINTERATIONS 7

#define bool int

//#define DEBUG

static double _ZOOM = 0.7;

static double _CENTER_CAMERA_X = 0.0;
static double _CENTER_CAMERA_Y = 0.0;


struct complex {
	double r;
	double i;
};

struct complex multiply2complex ( struct complex *a, struct complex *b ){
	struct complex number;
	double r;
	double i;
	// (a + bi) · (c + di) = (ac − bd) + (ad + bc)i
	r = a->r * b->r;
	r += (a->i * b->i ) * -1; 

	i = a->r * b->i;
	i += a->i * b->r;
	
	number.r  = r;
	number.i = i;
    
	return number;
	
}

// scape function
bool isNanComplex(struct complex *a) {
	//return SDL_FALSE;
    return ( a->r != a->r ) || ( a->i != a->i );
}

struct complex madelbro_it_funcion ( struct complex *in , struct complex *z0) {
	struct complex c1 = multiply2complex ( in , in );
	c1.r += z0->r;
	c1.i += z0->i;
    return c1;
}


int scapeColorFunction ( struct complex c ) {	
	int iterations = 0;	
    struct complex z0 = c;
   
    //static double offz0 = 0;
    //offz0 += 0.00000001;
    //z0.r = CONSTANTR + offz0 + c.r;
    //z0.i = CONSTANTI + offz0 + c.i;

    
    for ( int i=0; i<NUMOFINTERATIONS; i++ ) {
		//printf ( "%d %f @@@ %f\n\n", isNanComplex(&c), c.r, c.i);
		c = madelbro_it_funcion(&c,&z0);
		iterations++;
		if ( isNanComplex(&c) ){
			return iterations;
		};
	}

	//return (c.r*c.r) + (c.i*c.i);
	//return    ((int)((c.r*c.r) + (c.i*c.i))) % 255;
	
	//return sqrt( (c.r*c.r) + (c.i*c.i) ); // modulo de complex
	
	return iterations;
}

int scapeColorFunction_c ( struct complex c, double offz0 ) {	
	int iterations = 0;	
    struct complex z0 = c;
    
    //static double offz0 = 0;
    //offz0 += 0.00000001;

    z0.r += offz0;
    z0.i += 0;
    
	for ( int i=0; i<NUMOFINTERATIONS; i++ ) {
		//printf ( "%d %f @@@ %f\n\n", isNanComplex(&c), c.r, c.i);
		c = madelbro_it_funcion(&c,&z0);
	}

	//return (c.r*c.r) + (c.i*c.i);
	//return    ((int)((c.r*c.r) + (c.i*c.i))) % 255;
	
	//return sqrt( (c.r*c.r) + (c.i*c.i) ); // modulo de complex
	//return  (c.r*c.r) + (c.i*c.i) ; // moire
	
    // angle, iterations 50
    
    
    /*double modulus = sqrt( (c.r*c.r) + (c.i*c.i) ); 
    double radians; 
	if ( c.r > 0 ) 
        radians = atan(c.i/c.r) + (3.14/2); 
	else 
        radians = atan(c.i/c.r) + 3.14; 
    return radians * ( modulus * 100  );
    */
    
    return   (c.i*c.i);
}



void draw_t ( SDL_Renderer * renderer, struct complex offset ) {
	//printf("\e[1;1H\e[2J");
	time_t t;
	struct complex c;
	
	srand((unsigned) time(&t));
	
	c.r = -1 + offset.r;
	c.i = -1 + offset.i;
	
	// draw set
    static double offz0 = 0;
    
    offz0 = 0;
    
	for ( int i=0; i<WINDOW_WIDTH; i++ ) {
		for ( int j=0; j<WINDOW_HEIGHT; j++ ) {
			//int it = scapeColorFunction(c);
            int it = scapeColorFunction_c(c, offz0);
			int color = it;
            color = (color % 125);
           
			//fprintf ( file, "%c", color );
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            SDL_RenderDrawPoint(renderer, i, j);
            
			c.i = c.i + 0.005 * _ZOOM;
		}
		c.i = -1 + offset.i;
		//fprintf (file, "\n" );
		c.r = c.r + 0.005 * _ZOOM;
	}
}

#ifdef DEBUG
int main(int argc, char ** argv) {
    struct complex c,z0;
    z0.r = 0.27;
    z0.i = -0.05;
    
    
    c = madelbro_it_funcion ( c, z0);
    c = madelbro_it_funcion ( c, z0);
    c = madelbro_it_funcion ( c, z0);
    c = madelbro_it_funcion ( c, z0);
 
    printf( "%f %f", c.r, c.i);
    
}
#endif

#ifndef DEBUG
int main(int argc, char ** argv) {
  /* Declare window and renderer */
  SDL_Window * window;
  SDL_Renderer * renderer;

  /* Initialize SDL */
  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow("Basic_Window",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            WINDOW_WIDTH, WINDOW_HEIGHT,
			    0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  /* Main game loop */
  struct complex offset;
  offset.r = -151;
  offset.i = -151;

  offset.r = _CENTER_CAMERA_X;
  offset.i = _CENTER_CAMERA_Y;

  
    double r=0;

  
  SDL_Event e;
      int quit=0;
      while(!quit){
          //Here the test
          while (!SDL_PollEvent(&e)){
             if (e.type==SDL_QUIT)
                quit=1;
             else if ( e.type == SDL_KEYDOWN ){
               printf( "kley");
            }
            else if ( e.type == SDL_MOUSEMOTION ){
               //ZOOM = e.motion.y*0.2;
                _CENTER_CAMERA_X = -2 +  e.motion.x / 200.0 ;
                _CENTER_CAMERA_Y = -1 + e.motion.y / 200.0;
            }
            else if ( e.type == SDL_MOUSEWHEEL ){
                if(e.wheel.y > 0) // scroll up
                {
                    _ZOOM += -0.002;
                }
                else if(e.wheel.y < 0) // scroll down
                {
                    _ZOOM += 0.002;
                }
                
            }
            
            //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);   
            //SDL_RenderClear(renderer);
            
          
            //offset.r -= 0.001; // MAGIC NUMBER 0.001;
            offset.r -= 0.00000001; // MAGIC NUMBER 0.001;
            //offset.r -= 0.0001; // SUPER MAGICK
            
            //offset.r -= 0.001; // SUPER MAGICK

            
            offset.i -= 0.00001; 
            offset.r -= 0.00001; 
            
            offset.r = _CENTER_CAMERA_X;
            offset.i = _CENTER_CAMERA_Y;
            
            draw_t ( renderer, offset );
            
            SDL_RenderPresent(renderer);  
          }
      }
  
  /*
  while (!SDL_QuitRequested()) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);   
    SDL_RenderClear(renderer);
    --
    
    offset.r -= 0.01;
    offset.i -= 0.01;
    draw_t ( renderer, offset );
    
    SDL_RenderPresent(renderer);
  }
  */

  /* Deinitialize everything */
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
#endif

