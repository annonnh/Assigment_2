#include "controller.h"

// PR states (persist between calls)
static float e   = 0.0f;
static float xe1 = 0.0f; // e[k-1]
static float xe2 = 0.0f; // e[k-2]
static float xv1 = 0.0f; // vr[k-1]
static float xv2 = 0.0f; // vr[k-2]
static float vref = 0.0f;
static int ctrl_count;  // counts FCLK ticks per control period
static int M;           // FCLK ticks per control period = FCLK/FSW
static int N;           // FCLK ticks per control period = FCLK/FSW

N     = (int)(FCLK / (2.0 * FSW));  // = 375, carrier half-period
M     = (int)(FCLK / FSW);          // = 750, control period



void fullbridge_pr_controller(float* input_from_plecs, float* output_from_plecs)
{
    float iref = input_from_plecs[0];
    float iinv = input_from_plecs[1];
    float vt   = input_from_plecs[2];
    

    ctrl_count = 0;
    // ERROR
    e = iref - iinv;

    // PROPORTIONAL PART
    float vp = kp * e;

    // PR (resonant discrete implementation)
    // vr[k] = k1*kr*(e[k] - e[k-2]) + k2*vr[k-1] - vr[k-2]
    float vr = k1 * kr * (e - xe2) + k2 * xv1 - xv2;

    // update error states
    xe2 = xe1;
    xe1 = e;

    // update output states
    xv2 = xv1;
    xv1 = vr;

    // FEEDFORWARD GRID VOLTAGE
    float vff = vt;

    // TOTAL CONTROLLER OUTPUT
    vref = (vp + vr + vff) * kdc;
    if (vref >  1.0f) vref =  1.0f;
    if (vref < -1.0f) vref = -1.0f;
    

    // Send back diagnostic values instead of gate states
    output_from_plecs[0] = vref;
    output_from_plecs[1] = vref;
    output_from_plecs[2] = vref;
    output_from_plecs[3] = e;
}
