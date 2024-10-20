#pragma once
#include "WindowSection.h"
#include <vector>
#include <atomic>

class Oscillator {
protected:
    enum OscillatorType {
        SINE,
        TRIANGLE,
        SQUARE,
        SAWTOOTH,
        NOISE
    };
    double freq;
    double level;
    double detune;
    int octave; 
    OscillatorType type;

private: 
    double SineWave(double time)
    {
        double output = sin(freq * time);
        return output * level;
    }

    double TriangleWave(double time)
    {
        double output = sin(freq * time);
        return output * level;
    }

    double SquareWave(double time)
    {
        double output = sin(freq * time);
        return output * level;
    }

    double SawtoothWave(double time)
    {
        double output = sin(freq * time);
        return output * level;
    } 

    double WhiteNoise(double time)
    {
        double output = sin(freq * time);
        return output * level;
    } 

 
public:
    Oscillator()
    {
        this->freq = 0.0f;
        this->level = 0.5f;
        this->octave = 3;
        this->detune = 0.0f;
        this->type = SINE;
    }

    void SetOscFrequencyRad(double frequency)
    {
        this->freq = frequency * pow(2, octave) * 2.0 * 3.14159;
    }
 
    double ProduceWave(double time)
    {
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

class OscillatorsWindow : public RegularWindow {
private:
    std::vector<Oscillator> oscs;
    double note_freq;
    double lowest_octave;
    double halfToneRatio;
 
    virtual void RenderElements()
    {
        ImGui::Text("lalalla osc");
    }
public:
    OscillatorsWindow()
    {
        lowest_octave = 55.0;
        halfToneRatio = pow(2.0, 1.0 / 12.0);

        this->name = "Oscillators";
        this->note_freq = 0.0f;

        Oscillator osc1;
        this->oscs.push_back(osc1);
    }

    void SetNoteFrequency(int note)
    {
        if (note == -1)
            note_freq = 0.0;
        else
        {
            note_freq = lowest_octave * pow(halfToneRatio, note);
        }

        for (int i = 0; i < oscs.size(); i++)
        {
            oscs[i].SetOscFrequencyRad(note_freq);
        }
    }

    double MixOscillators(double time)
    {
        double output = 0.0;
        for (int i = 0; i < oscs.size(); i++)
        {
            output += oscs[i].ProduceWave(time);
        }
    }
};
 
