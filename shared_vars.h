/*
 * shared_vars.h
 *
 *  Created on: 4 de jul de 2025
 *      Author: Prof. Guilherme Marcio Soares
 */

#ifndef SHARED_VARS_H_
#define SHARED_VARS_H_

extern float input[3];
extern float output[4];

// PR states (persist between calls)
extern float e;
extern float xe1; // e[k-1]
extern float xe2; // e[k-2]
extern float xv1; // vr[k-1]
extern float xv2; // vr[k-2]
extern float vref;
extern int ctrl_count;  // counts FCLK ticks per control period

#endif /* SHARED_VARS_H_ */
