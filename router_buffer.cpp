/*
* CPSC 441 Assignment 3
* Athena McNeil-Roberts
* ID# 30042085
*/
// First-In-First-Out packet simulation


#include <stdio.h>
#include <queue> 

#define MAX_BUFFER 100
#define SPEED 5000000.0

struct pkt{
    float arr_time;
    int size;
    float in_q_time;
};

int main(int argc, char *argv[]){

    float total_delay, avg_delay;
    float time_stamp;
    int pkt_size;
    std::queue<pkt> incoming;
    std::queue<pkt> buffer;

    float curr_time = 0.0;
    int pkts_in = 0, pkts_out = 0, pkts_lost = 0;
    int bytes_in = 0, bytes_out = 0, bytes_lost = 0;
    int total_bytes = 0, total_pkts, pkts_remaining;
    int pkts_in_queue = 0, bytes_in_queue = 0;


    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("file to read not provided. Exiting.. ");
        return 1;
    }

    while(fscanf(file, "%f %d\n", &time_stamp, &pkt_size) != EOF){
        pkt arrive;
        arrive.size = pkt_size;
        arrive.arr_time = time_stamp;
        incoming.push(arrive);
        total_bytes += arrive.size;
    }

    total_pkts = incoming.size();
    pkts_remaining = incoming.size();
    pkt current;

    while(1){

        if(pkts_remaining == 0){
            break;
        }

        while(incoming.size() > 0 && incoming.front().arr_time <= curr_time){
            if(buffer.size() < MAX_BUFFER){ // buffer has room
                incoming.front().in_q_time = curr_time; // record the time the packet entered the buffer
                buffer.push(incoming.front()); // add packet to buffer
                pkts_in++; 
                bytes_in += incoming.front().size;
                pkts_in_queue++;
                bytes_in_queue += incoming.front().size;
            }else{ // buffer is full
                pkts_lost++;
                bytes_lost += incoming.front().size; 
                pkts_remaining--;
            }
            incoming.pop(); // remove packet from incoming 
        }

        if(buffer.size() > 0){ // if buffer isnt empty process the first element
            current = buffer.front();
            buffer.pop();
            total_delay += curr_time - current.in_q_time;
            float t = (current.size / SPEED) * 8;
            curr_time+= t;
            pkts_out++;
            bytes_out+= current.size;
            pkts_in_queue--;
            bytes_in_queue -= current.size;
            pkts_remaining--; // reduce number of pkts processed
        }else{
            curr_time = incoming.front().arr_time; // if buffer empty, jump time to the next arrival time
        }
    }

    printf("\n---Results---\n");
    printf("Buffer size: %d pkts\n", MAX_BUFFER); // buffer size
    printf("Link Speed: %.0f bps\n", SPEED); // WLAN link speed
    printf("End time: %f\n", curr_time); // simulation end time
    printf("Incoming traffic: %d pkts    %d bytes\n", total_pkts, total_bytes);  //  number of incoming packets & bytes
    printf("Outgoing traffic: %d pkts   %d bytes\n", pkts_out, bytes_out); // the number of delivered packets & bytes
    printf("Buffered traffic: %d pkts   %d bytes\n", pkts_in_queue, bytes_in_queue); // the number of delivered packets & bytes
    printf("Discarded traffic: %d pkts   %d bytes\n", pkts_lost, bytes_lost); // the number of discarded/lost packets & bytes
    float lost_pkt_perc = ((float)pkts_lost / (float)total_pkts) * 100.0; //  packet loss percentage
    float lost_byte_perc = ((float)bytes_lost / (float)total_bytes) * 100.0; // byte loss percentage
    printf("Lost traffic: %.2f%% pkts   %.2f%% bytes\n", lost_pkt_perc , lost_byte_perc);
    avg_delay = total_delay / (float)pkts_out; // average queueing delay for (delivered) packets
    printf("Average Queueing delay: %f sec\n", avg_delay);
    printf("\n");
    printf("Summary: %d %.0f %d %d %d %d %d %d %f %f %f\n", MAX_BUFFER, SPEED, total_pkts, total_bytes, pkts_out, bytes_out, pkts_lost, bytes_lost, lost_pkt_perc , lost_byte_perc, avg_delay);
}
