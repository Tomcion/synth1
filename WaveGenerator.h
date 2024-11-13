#pragma once
#include "WindowSection.h"
#include "olcNoiseMaker.h"
#include "ModulableField.h"
#include <vector>
#include <string>
#include <iostream>

const double halfToneRatio = pow(2.0, 1.0 / 12.0);

enum WaveType {
    SINE = 0,
    TRIANGLE,
    SQUARE,
    SAWTOOTH,
    NOISE
};

class PhaseLFO;

class WaveGenerator {
protected:
    WaveType type;
    PhaseLFO* PhaseModulator;

    float amplitude;
    ModulableField phase;

    double freq_rad;
 
    const double ToRad(double n);
    
    const double SineWave(double time); 

    const double TriangleWave(double time); 

    const double SquareWave(double time); 

    const double SawtoothWave(double time);

    const double WhiteNoise(double time); 

    void UpdatePhase(double time);

    virtual void UpdateParameters(double time);

public: 
    WaveGenerator(WaveType type, float amplitude, float freq_rad);

    void SetPhaseModulator(PhaseLFO* lfo);
    
    const double ProduceWave(double time); 

};

