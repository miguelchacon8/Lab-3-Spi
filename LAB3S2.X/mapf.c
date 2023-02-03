/*
 * File:   mapf.c
 * Author: Migue
 *
 * Created on 26 de enero de 2023, 11:18 AM
 */

#include "mapf.h"

long map(long x, long eMin, long eMax, long sMin, long sMax)
{
  return(x -eMin)*(sMax-sMin)/(eMax-eMin)+sMin;
}