#include "BufferedSerial.h"
#include "ThisThread.h"
#include "mbed.h"


BufferedSerial pc (USBTX,USBRX);

char men[40];

#define T_SAMPLE   625  // micros
#define N_SAMPLE   16


static float signal[N_SAMPLE]; 
static float aux=0.0;
int i=0;
int dec,ent;
bool flag=true;

AnalogIn  Sample(A0);


int main()
{
    while (true) 
    {
        if (flag)
        {
        signal[i]= Sample * 3.3;
        i++;
        if (i==N_SAMPLE) {i=0; flag=false;}
        wait_us(T_SAMPLE);
        
        for (i=0; i<N_SAMPLE; i++)
        {
            aux += (aux*aux);
            aux /= N_SAMPLE; 
            aux = sqrt(aux);
        } 
        }

        if (!flag)
        {
        ThisThread::sleep_for(500ms);
        ent = int(aux);
        dec = int(float(aux-ent)*10000);
        sprintf(men, "El vmin: %1u,%04u  volts \n\r", ent, dec);
        pc.write(men,26);
        flag=true;
        }            

    }
}

