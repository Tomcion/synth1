#pragma once
#include "WindowSection.h"
#include <vector>
#include <string>
#include <atomic>

enum OscillatorType {
    SINE = 0,
    TRIANGLE,
    SQUARE,
    SAWTOOTH,
    NOISE
};

class Oscillator {
protected:
    char oscNumber;
    std::string windowName;
    double freq;
    int MAX_OCTAVE = 5;
    double halfToneRatio = pow(2.0, 1.0 / 12.0);

    float level;
    float detune;
    int octave;
    OscillatorType type;

private: 
    double PI = 3.14159;

    double ToRad(double n)
    {
        return n * 2.0 * PI;
    }

    double SineWave(double time)
    {
        double output = sin(freq * time);
        //double output = sin(freq * time + 0.01 * freq * sin(ToRad(2.0) * time));
        return output * (double)level;
    }

    double TriangleWave(double time)
    {
        double output = (2.0f / PI) * asin(sin(freq * time));
        return output * (double)level;
    }

    double SquareWave(double time)
    {
        double output = (sin(freq * time) > 0 ? 1.0f : -1.0f);
        return output * (double)level;
    }

    double SawtoothWave(double time)
    {
        double output = (2.0f / PI) * (freq * PI * fmod(time, 1.0 / freq) - (PI / 2.0));
        return output * (double)level;
    } 

    double WhiteNoise(double time)
    {
        double output = sin(freq * time);
        return output * (double)level;
    } 

 
public:
    Oscillator(char pOscNumber, OscillatorType pType, float pLevel, int pOctave, float pDetune)
    {
        this->freq = 0.0f;
        this->level = pLevel;
        this->octave = pOctave;
        this->detune = pDetune;
        this->type = pType;
        this->SetNumber(pOscNumber);
    }

    void SetNumber(char num)
    {
        this->oscNumber = num;
        windowName = "Oscillator " + std::to_string(num);
    }

    int GetNumber()
    {
        return this->oscNumber;
    }

    void ApplyDetune(double time)
    { 
        freq += pow(2, halfToneRatio * this->detune);
        //freq += 0.01 * freq * sin(ToRad(2.0) * time);
    }

    void SetOscFrequencyRad(double frequency)
    {
        this->freq = ToRad(frequency * pow(2, octave));
    }
 
    double ProduceWave(double time)
    {
        ApplyDetune(time);
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

    void RenderOsc()
    {
        ImGui::Begin(windowName.c_str());
        const char* items[4] = { "Sine", "Triangle", "Square", "Sawtooth"};
        int selected_item = type;
        if (ImGui::Combo("Waveform", &selected_item, items, 4))
            type = (OscillatorType)selected_item;

        ImGuiKnobs::Knob("Level", &(this->level),
            0.0f, 1.0f, 0.01f, "%.2f", ImGuiKnobVariant_Wiper,
            0.0f, ImGuiKnobFlags_DragVertical);
        ImGui::SameLine();
        ImGuiKnobs::KnobInt("Octave", &(this->octave),
            0, MAX_OCTAVE, 0.1f, "%i", ImGuiKnobVariant_Stepped,
            0, ImGuiKnobFlags_DragVertical, MAX_OCTAVE + 1); 
        ImGui::SameLine();
        ImGuiKnobs::Knob("Detune", &(this->detune),
            -1.0f, 1.0, 0.01f, "%.2f", ImGuiKnobVariant_Wiper,
            0.0f, ImGuiKnobFlags_DragVertical);
        ImGui::End();;
    }
};

class OscillatorsWindow : public RegularWindow {
private:
    double lowest_octave = 55.0;
    double halfToneRatio = pow(2.0, 1.0 / 12.0);
    std::vector<Oscillator> oscs;
    double note_freq;
 
    virtual void RenderElements()
    {
        for (int i = 0; i < oscs.size(); i++)
            oscs[i].RenderOsc();
    }
public:
    OscillatorsWindow()
    { 
        this->name = "Oscillators";
        this->note_freq = 0.0f;

        Oscillator osc1(1, TRIANGLE, 0.5f, 2, 0.0f);
        this->oscs.push_back(osc1);

        Oscillator osc2(2, SQUARE, 0.1f, 3, 0.0f);
        this->oscs.push_back(osc2);
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
            output += this->oscs[i].ProduceWave(time);
        }
        return output;
    } 
};
 
