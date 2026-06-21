#include <math.h>

// Parâmetros do PWM
#define FCLK 15e6f
#define FSW 20000.0f
#define VDC 400.0f

/*
// Entradas do controle
#define Iref Input(0)
#define Iinv Input(1)
#define Vt Input(2)
*/

// Ganhos
#define kp 41.88f
#define kr 58472.85f
#define k1 0.000025f
#define k2 1.999f
#define kdc (float)(1.0/VDC)

// PWM variables
static int N;
static int count;
static int dir = 1;
static float carrier;
static int ctrl_count;  // counts FCLK ticks per control period
static int M;           // FCLK ticks per control period = FCLK/FSW

// Controller variables
static float e;
static float iref;
static float iinv;
static float vt;

// PR states
static float x1 = 0.0;
static float x2 = 0.0;
static float xe1 = 0.0; // e[k-1]
static float xe2 = 0.0; // e[k-2]
static float xv1 = 0.0; // vr[k-1]
static float xv2 = 0.0; // vr[k-2]
// control output
static float vref;

// PWM outputs
static int S1, S2, S3, S4;

void fullbridge_pr_controller (float* input_from_plecs, float* output_from_plecs);
