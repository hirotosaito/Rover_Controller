#ifndef SharpIR_h
#define SharpIR_h

#define N_SAMPLE 25

#ifndef ARDUINO
    #include "Arduino.h"
#elif defined(SPARK)
    #include "Particle.h"
#endif


class SharpIR
{
    public:
        SharpIR(int irpin);
        int ir_strength();

    private:

        void sort(int a[], int size);
        
        int _irPin;
};

#endif