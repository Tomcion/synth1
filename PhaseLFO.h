#pragma once
#include "LFO.h" 


class PhaseLFO : public LFO {
public:
    PhaseLFO(std::string name, float amplitude, float freq_hz)
        : LFO(name, SINE, 1.0f, freq_hz)
    { 
    }

    float CalcPMAmplitude(double carrier_freq)
    { 
        double freq_dev = carrier_freq * (halfToneRatio - 1);
        return (float)(freq_dev / this->freq_rad);
    }
};
