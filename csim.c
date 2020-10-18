/*
Shayna Kaushal
shaynak
Project 3 - Cache
*/

#include "cachelab.h"
#include <math.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

typedef long unsigned int lint;

lint **arr_init (int s, int E){
    int rows, cols, i, j;
    rows = 1 << s;
    cols = (E * 3) + 1;

    lint **a = (lint**) malloc (sizeof (lint*) * rows);

    for (i = 0; i < rows; i++){
        a[i] = (lint*) malloc (sizeof (lint) * cols);
    }

    for (i = 0; i < rows; i++){
        for(j = 0; j < cols; j++){
            a[i][j] = 0;
        }
    }
    return a;
}

void arr_free(lint **a, int s){
    int i, rows;
    rows = 1 << s;
    for (i = 0; i < rows; i ++){
        free (a[i]);
    }
    free(a);
}

int main (int argc, char *argv[]){
    char *filename;
    int option;

    int s, E, b;

    while ((option = getopt (argc, argv, "hs:E:b:t:")) != -1){
        switch (option){
            case 'h':
                printf ("Usage: ./csim [-h] -s <s> -E <E> -b <b> -t <tracefile>\n");
                printf ("Options:\n-h \t\t Print this help message\n-s <num> \t ");
                printf ("No. of set index bits\n-E <num> \t No. of lines per set\n-b ");
                printf ("<num> \t No. of block offset bits\n-t <file> \t Trace file\n\n");
                break;
            case 's':
                s = atoi (optarg);
                break;
            case 'E':
                E = atoi (optarg);
                break;
            case 'b':
                b = atoi (optarg);
                break;
            case 't':
                filename = optarg;
                break;
            default:
                fprintf (stderr, "Usage: ./csim -s <s> -E <E> -b <b> -t <tracefile>\n");
                break;
        }
    }

    lint **cache = arr_init (s, E);
    FILE *fp = fopen (filename, "r");
    
    if (fp == NULL){
        printf ("Failed to read file %s\n", filename);
        exit(2);
    }

    int hits, misses, evics;
    hits = 0;
    misses = 0;
    evics = 0;

    char task;
    lint add;
    int bytes;

    int min, lru_pos = 0, ctr, hit_tag, i, j;
    ctr = 0;

    int cols = (E * 3) + 1;
    
    //going through file lines
    while (fscanf (fp, " %c %lx,%d", &task, &add, &bytes) > 0){
        lint tag = add >> (b + s);
        lint S = (add << (64 - b - s)) >> (64 - s);
        
        hit_tag = 0;
        
        int lru_tag;
        lru_tag = cache[S][0];
        
        if (task != 'I'){
            ctr++;

            if (task == 'M'){
                for (i = 0; i < 2; i++){
                    for(j = 1; j < cols; j += 3){
                        if ((cache[S][j] == 1) && (cache[S][j + 1] == tag)){
                            hits++;
                            hit_tag = 1;
                            cache[S][j+2] = ctr;
                            break;
                        }
                    }
                    if (hit_tag == 0){
                        misses++;
                        if (cache[S][(lru_tag * 3) + 1] != 1){
                            cache[S][(lru_tag * 3) + 1] = 1;
                            cache[S][(lru_tag * 3) + 2] = tag;
                            cache[S][(lru_tag * 3) + 3] = ctr;
                        }
                        else{
                            evics++;
                            cache[S][(lru_tag * 3) + 2] = tag;
                            cache[S][(lru_tag * 3) + 3] = ctr;
                        }
                    }
                }
            }

            else if (task != 'M'){
                for (i = 1; i < cols; i += 3){
                    if ((cache[S][i] == 1) && (cache[S][i +1 ] == tag)){
                        hits++;
                        hit_tag = 1;
                        cache[S][i + 2] = ctr;
                        break;
                    }
                }
                if (hit_tag == 0){
                    misses++;
                    if (cache[S][(lru_tag * 3) + 1] != 1){
                        cache[S][(lru_tag * 3) + 1] = 1;
                        cache[S][(lru_tag * 3) + 2] = tag;
                        cache[S][(lru_tag * 3) + 3] = ctr;
                    }
                    else{
                        evics++;
                        cache[S][(lru_tag * 3) + 2] = tag;
                        cache[S][(lru_tag * 3) + 3] = ctr;
                    }
                }
            }

            lru_pos = lru_tag;
            min = cache[S][(lru_tag * 3) + 3];

            for (i = 1; i < cols; i += 3){
                if (min > cache[S][i + 2]){
                    min = cache[S][i + 2];
                    lru_pos = ((i + 2) / 3) - 1;
                }
            }

            cache[S][0] = lru_pos;
        }
    }

    arr_free(cache, s);
    printSummary (hits, misses, evics);

    return 0;
}