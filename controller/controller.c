#include "controller.h"
#include <math.h>

void fullbridge_pr_controller (float* input_from_plecs)
{
    N     = (int)(FCLK / (2.0 * FSW));  // = 375, carrier half-period
    M     = (int)(FCLK / FSW);          // = 750, control period

    count      = 0;
    dir        = 1;
    ctrl_count = 0;

    xe1 = 0.0f;
    xe2 = 0.0f;
    xv1 = 0.0f;
    xv2 = 0.0f;
    carrier = 0.0f;

    // PWM CARRIER GENERATION (runs every FCLK tick)
    count += dir;

    if (count >= N)
    {
        count = N;
        dir = -1;
    }

    else if (count <= 0)
    {
        count = 0;
        dir = 1;
    }

    carrier = 2.0 * ((float)count / (float)N) - 1.0;

    //__________________________________________________________________
    // CONTROL LAW (runs only once every M ticks = once per switching cycle)
    ctrl_count++;
    if (ctrl_count >= M)
    {
        ctrl_count = 0;

        //______________________________________________________________
        // ERROR
        //e = iref - iinv;
        e = input_from_plecs[0] - input_from_plecs[1];

        //______________________________________________________________
        // PROPORTIONAL PART
        float vp = kp * e;

        //______________________________________________________________
        // PR (resonant discrete implementation)
        // vr[k] = k1*kr*(e[k] - e[k-2]) + k2*vr[k-1] - vr[k-2]

        float vr = k1 * kr * (e - xe2) + k2 * xv1 - xv2;

        // update error states
        xe2 = xe1;
        xe1 = e;

        // update output states
        xv2 = xv1;
        xv1 = vr;

        //______________________________________________________________
        // FEEDFORWARD GRID VOLTAGE
        // float vff = vt;
        float vff = input_from_plecs[2];

        //______________________________________________________________
        // TOTAL CONTROLLER OUTPUT
        vref = (vp + vr + vff) * kdc;

        // normalize to modulation index [-1,1]
        if (vref >  1.0) vref =  1.0;
        if (vref < -1.0) vref = -1.0;
    }

    //__________________________________________________________________
    // PWM LOGIC (FULL BRIDGE)

    // leg A
    /*
    S1 = (vref >= carrier);
    S2 = !S1;

    // leg B (inverted reference)
    S3 = (-vref >= carrier);
    S4 = !S3;
*/
    //__________________________________________________________________
    // OUTPUTS
    /*
    output[0] = S1;
    output[1] = S2;
    output[2] = S3;
    output[3] = S4;
    //Output(4) = vref;
    //Output(5) = carrier;
    */
}
