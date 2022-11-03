#include "LIMACH.h"

int calculateDither(int dither){
    if(dither<=ditherRange)
    {
        dither++;
    }
    return dither;
}