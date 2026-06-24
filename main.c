//*********************************************
// Assignment 2 of Embedded Simulation Class.
// Autor: Anonmixs Amaro Afonso
//*********************************************

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "scicomm.h"
#include <math.h>
#include "controller/controller.h"

// Variables from CPU that CLA uses
#pragma DATA_SECTION(input,"CpuToCla1MsgRAM");
float input[3] = {0};

// Variables from CLA that CPU uses
#pragma DATA_SECTION(output, "Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(e, "Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(xe1, "Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(xe2, "Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(xv1, "Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(xv2, "Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(vref, "Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(ctrl_count, "Cla1ToCpuMsgRAM");
float output[4] = {0};
float e   = 0.0f;
float xe1 = 0.0f; // e[k-1]
float xe2 = 0.0f; // e[k-2]
float xv1 = 0.0f; // vr[k-1]
float xv2 = 0.0f; // vr[k-2]
float vref = 0.0f;
int ctrl_count;  // counts FCLK ticks per control period

// flag to avoid CLA enter before a input value arrive.
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
            if (SCI_getRxFIFOStatus(SCI0_BASE) >= 6)
            {
                // function that parser the data in 8 bits chunks and sent to PLECS via SCI interface
                protocolReceiveData(SCI0_BASE, &input, 6); 
                receive_value = true; 
                CLA_forceTasks(myCLA0_BASE,CLA_TASKFLAG_1);
            }
        }

}

// CLA interrupt to calculate the control law of the conversor.
__interrupt void cla1Isr1 ()
{
    if (receive_value)
    {
        // function that receive the data in 8 bits chunks via SCI from PLECS and uses it to the calculations
        protocolSendData(SCI0_BASE, &output, 8);
        receive_value = false;
    }

    Interrupt_clearACKGroup(INT_myCLA01_INTERRUPT_ACK_GROUP);
}
