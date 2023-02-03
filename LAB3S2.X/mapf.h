

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MAPF_H
#define	MAPF_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h>

long map(long x, long eMin, long eMax, long sMin, long sMax);

#endif
