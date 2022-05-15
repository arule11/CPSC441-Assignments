/*
* CPSC 441 Assignment 4
* Athena McNeil-Roberts
*/
// Bozon channel access protocol

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define S 100.0 // sleep duration
#define Y 10.0 // yodel duration 
#define M 5 // number of bozons

#define BONUS 0

// simulation parameters
#define END_TIME 1000000.0 

// bozen state values
#define SLEEPING 0
#define YODELLING 1

/* Parameters for random number generation. */
#define MAX_INT 2147483647       /* Maximum positive integer 2^31 - 1 */

struct bozon{
    int state; // yodelling or sleeping
    double action_end_time; 
    double action_time; // duration
    bool perf_yodel;
};

/* Code to generate exponentially-distributed random numbers provided by
    Carey Williamson from course website */

/* Generate a random floating point number uniformly distributed in [0,1] */
double Uniform01(){
    double randnum;
    /* get a random positive integer from random() */
    randnum = (double) 1.0 * random();
    /* divide by max int to get something in the range 0.0 to 1.0  */
    randnum = randnum / (1.0 * MAX_INT);
    return( randnum );
}

/* Generate a random floating point number from an exponential distribution with mean mu. */
double Exponential(double mu){
    double randnum, ans;
    randnum = Uniform01();
    ans = -(mu) * log(randnum);
    return( ans );
}

int main(){

    double silent = 0, melodious = 0, screechy = 0, perfect = 0;
    int attempted_yodels = 0, successful_yodels = 0, active_yodellers = 0;
    
    struct bozon colony[M];
    double curr_time = 0.0;
    double next_action_time = 0.0;

    for(int i = 0; i < M; i++){
        colony[i].action_end_time = Exponential(S) + curr_time; // give each bozon an initial sleep
        colony[i].state = SLEEPING; // put each bozon to sleep
    }
    
    while(curr_time != END_TIME){ // simulation hasnt ended

        next_action_time = END_TIME; // set to end of simulation to find the smallest next time
        for(int i = 0; i < M; i++){

            if(colony[i].action_end_time < next_action_time){ // find the smallest next action time
               next_action_time = colony[i].action_end_time;
            }

            if(colony[i].action_end_time <= curr_time){ // the current bozons action has ended - switch state
                if(colony[i].state == YODELLING){ // put to sleep
                    
                    if(colony[i].perf_yodel){ // check if yodel was perfect
                        successful_yodels++;
                        perfect += colony[i].action_time;
                    }
                    colony[i].state = SLEEPING; 
                    colony[i].action_end_time = Exponential(S) + curr_time; // sleep for random time
                    active_yodellers--;

                }else if(colony[i].state == SLEEPING){ // wake up
                    colony[i].state = YODELLING;
                    colony[i].perf_yodel = true;
                    #if BONUS
                        colony[i].action_time = 10.0; // yodel for 10.0 time units
                    #else
                        colony[i].action_time = Exponential(Y); // yodel for random time
                    #endif
                    colony[i].action_end_time = colony[i].action_time + curr_time; 
                    attempted_yodels++;
                    active_yodellers++;
                }  
            } 
        }
        
        if(active_yodellers > 1){ // more than one bozen is yodelling ie screechy
            screechy += (next_action_time - curr_time);
            for(int i = 0; i < M; i++){ // all bozens currently yodelling arent perfect
                if(colony[i].state == YODELLING){
                    if(colony[i].perf_yodel){ // check if part of the yodel was perfect
                        perfect += curr_time - (colony[i].action_end_time - colony[i].action_time);
                    }
                    colony[i].perf_yodel = false;
                }
            }
        }else if(active_yodellers == 1){ // only one bozen yodelling ie melodious
            melodious += (next_action_time - curr_time);
        }else{ // no bozens yodelling / all sleeping ie silent
            silent += (next_action_time - curr_time);
        }
        curr_time = next_action_time; // set time to next bozon closest action
    }

    printf("\nM = %d, S = %.2f, Y = %.2f\n", M, S, Y);
    printf("Total time observing channel:  %.2f\n", END_TIME);
    printf("Idle time on the channel:  %.3f     %.3f%%\n", silent, (silent/END_TIME)*100.0);
    printf("Melodious time on channel: %.3f     %.3f%%\n", melodious, (melodious/END_TIME)*100.0);
    printf("Screech time on channel: %.3f       %.3f%%\n", screechy, (screechy/END_TIME)*100.0);

    printf("Attempted yodels: %d\n", attempted_yodels);
    printf("Perfect yodels: %d\n", successful_yodels);
    float perfDivAttemp = (float)successful_yodels/(float)attempted_yodels;
    printf("Perfect yodels/attempted yodels: %.3f   (%.3f%%)\n", perfDivAttemp, perfDivAttemp*100);
    printf("Perfect yodel time on the channel: %.3f     %.3f%%\n", perfect, (perfect/END_TIME)*100.0);
}
