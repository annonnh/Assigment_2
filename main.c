
//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "scicomm.h"
#include <math.h>
#include "controller/controller.h"

#define B0 0.001033
#define B1 0.000767
#define B2 -0.000266
#define A1 -1.521886
#define A2 0.521886

#define REF 20
#define UL 1
#define LL 0

#pragma DATA_SECTION(fVal,"CpuToCla1MsgRAM");
#pragma DATA_SECTION(vo,"CpuToCla1MsgRAM");
float vo = 0;
float fVal = 0;
float copy_vo[3] = {0};

#pragma DATA_SECTION(fResult, "Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(x, "Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(y, "Cla1ToCpuMsgRAM");

float x[3]={0};
float y[3]={0};
float fResult;

// PR Controller Inputs
static float input[3];

// PR Controller Outputs
static float output[4];

// PR Controller
//static float output[4];

bool receive_value = false;


void main(void)
{
        Device_init();

        Interrupt_initModule();

        Interrupt_initVectorTable();

        Board_init();
        EINT;
        ERTM;

        for(;;)
        {
            
            if (SCI_getRxFIFOStatus(SCI0_BASE)>3)
            {
                protocolReceiveData(SCI0_BASE,&input, 3*sizeof(float));
                fullbridge_pr_controller(input, output);
                protocolSendData(SCI0_BASE, &output, 4*sizeof(float));
                //receive_value = true;
                //CLA_forceTasks(myCLA0_BASE,CLA_TASKFLAG_1);
            }
        }

}


__interrupt void cla1Isr1 ()
{
    if (receive_value)
    {
        //protocolSendData(SCI0_BASE, &y[0] ,sizeof(float));
        receive_value = false;
    }

    Interrupt_clearACKGroup(INT_myCLA01_INTERRUPT_ACK_GROUP);
}

