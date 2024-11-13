#pragma once
#include "WindowSection.h"
#include "olcNoiseMaker.h"
#include "PhaseLFO.h"
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

class WaveGenerator {
protected:
    WaveType type;
    PhaseLFO* PhaseModulator;

    float amplitude;
    ModulableField phase;

    double freq_rad;
 
    const double ToRad(double n)
    {
        return n * 2.0 * PI;
    }

    const double SineWave(double time)
    {
        //double output = sin(freq * time);
        double message_freq = ToRad(2.0);
        double freq_dev = freq_rad * (halfToneRatio - 1);
        double output = sin(freq_rad * time + phase.GetValue());
        return output * (double)amplitude;
    }

    const double TriangleWave(double time)
    {
        double output = (2.0f / PI) * asin(sin(freq_rad * time));
        return output * (double)amplitude;
    }

    const double SquareWave(double time)
    {
        double message_freq = ToRad(2.0);
        double freq_dev = freq_rad * (halfToneRatio - 1);
        double output = sin(freq_rad * time + freq_dev / message_freq * sin(message_freq * time)) > 0 ? 1.0f : -1.0f;
        //double output = (sin(freq * time) > 0 ? 1.0f : -1.0f);
        return output * (double)amplitude;
    }

    const double SawtoothWave(double time)
    {
        double output = (2.0f / PI) * (freq_rad * PI * fmod(time, 1.0 / freq_rad) - (PI / 2.0));
        return output * (double)amplitude;
    } 

    const double WhiteNoise(double time)
    {
        double output = sin(freq_rad * time);
        return output * (double)amplitude;
    } 

    void UpdatePhase(double time)
    { 
        if (this->PhaseModulator == nullptr)
            return;
        float amp = (this->PhaseModulator)->CalcPMAmplitude(this->freq_rad);
        (this->phase).CalcModuletedValue(time);
    }

    virtual void UpdateParameters(double time)
    { 
        return;
    }

public: 
    WaveGenerator(WaveType type = SINE, float amplitude = 0.3f, float freq_rad = 0.0f)
        : type(type), amplitude(amplitude), freq_rad(freq_rad),
        PhaseModulator(nullptr),
        phase("Phase", 0.0f, 0.0f, ToRad(180.0f))
    {
    }

    void SetPhaseModulator(PhaseLFO* lfo)
    {
        this->PhaseModulator = lfo;
        (this->phase).SetAutomator(lfo);
    }

    const double ProduceWave(double time)
    {
        UpdateParameters(time);
        UpdatePhase(time);

        switch (this->type)
        {
        case SINE:
            return SineWave(time);
        case TRIANGLE:
            return TriangleWave(time);
        case SQUARE:
            return SquareWave(time);
        case SAWTOOTH:
            return SawtoothWave(time);
        case NOISE:
            return WhiteNoise(time);
        } 
    }
};

