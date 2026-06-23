#include <math.h>

#define VDC 400.0f

// Gains
#define kp  41.88f
#define kr  58472.85f
#define k1  0.000025f
#define k2  1.999f
#define kdc (float)(1.0/VDC)
#define FCLK 15e6
#define FSW 20000.0

void fullbridge_pr_controller(float* input_from_plecs, float* output_from_plecs);
