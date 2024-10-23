#pragma once
#include "WaveGenerator.h"
#include "LFO.h"
 
class Oscillator : public WaveGenerator {
private:
    char id;
    std::string windowName;
    int MAX_OCTAVE = 5;

    float detune; 
    int octave; 
 
public:
    Oscillator(char id, WaveType type, float amplitude, int octave)
        : WaveGenerator(type, amplitude, 0.0f), octave(octave), detune(0.0f)
    {
        this->SetId(id);
    }

    void SetId(char num)
    {
        this->id = num;
        windowName = "Oscillator " + std::to_string(num);
    }

    const int GetNumber()
    {
        return this->id;
    }

    const void ApplyDetune(double* note_freq)
    {
        *note_freq = *note_freq * pow(2.0f, this->detune / 12.0f);
    }

    void SetOscFrequencyRad(double frequency)
    {
        ApplyDetune(&frequency); 
        this->freq = ToRad(frequency * pow(2, octave));
    } 

    void RenderOsc()
    {
        ImGui::Begin(windowName.c_str());
        const char* items[4] = { "Sine", "Triangle", "Square", "Sawtooth"};
        int selected_item = type;
        if (ImGui::Combo("Waveform", &selected_item, items, 4))
            type = (WaveType)selected_item;

        ImGuiKnobs::Knob("Level", &(this->amplitude),
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

class OscillatorsWindow : public WindowSection {
private:
    double lowest_octave = 55.0;
    std::vector<Oscillator> oscs;
    double note_freq;
 
public:
    virtual void RenderWindow()
    {
        for (int i = 0; i < oscs.size(); i++)
            oscs[i].RenderOsc();
    }

    OscillatorsWindow()
    { 
        this->name = "Oscillators";
        this->note_freq = 0.0f; 
    }

    void AddOscillator(Oscillator osc)
    {
        oscs.push_back(osc);
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
 
