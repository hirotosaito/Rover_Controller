#include <Arduino.h>
#include "SharpIR.h"

SharpIR::SharpIR(int irPin)
{
    _irPin = irPin;
    pinMode(_irPin,INPUT);
}

void SharpIR::sort(int a[], int size)
{
    for(int i =0; i <size -1;i++)
    {
        bool flag = true;
        for(int j=0;j<(size-(i+1));j++)
        {
            if(a[j] > a[j+1])
            {
                int t = a[j];
                a[j] = a[j+1];
                a[j+1] = t;
                flag = false;
            }
        }
        if(flag) break;
    }
}

int SharpIR::ir_strength()
{
    int ir_val[N_SAMPLE];
    int distanceCM;
    float current;

    for(int i = 0 ; i < N_SAMPLE; i++)
    {
        ir_val[i] = analogRead(_irPin);
    }

    sort(ir_val,N_SAMPLE);

    distanceCM = ir_val[N_SAMPLE / 2];//60.374 * pow(map(ir_val[N_SAMPLE / 2], 0, 1023, 0, 5000)/1000.0, -1.16);

    return distanceCM;
}