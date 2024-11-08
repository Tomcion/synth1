#pragma once
#include "WaveGenerator.h"
#include "ParameterAutomator.h"
#include <iostream>

class LFO : public WaveGenerator, public ParameterAutomator {
private:
    char id;
    std::string windowName;

    float freq_hz;

public:
    LFO(char id, WaveType type, float amplitude, float freq_hz)
        : ParameterAutomator(), WaveGenerator(type, amplitude, 0.0f), freq_hz(freq_hz)
    {
        this->freq = ToRad(freq_hz);
        this->amplitude *= 0.001f;
        this->SetId(id);
    }
 
    void SetId(char num)
    {
        this->id = num;
        windowName = "LFO " + std::to_string(num);
    }

    const int GetNumber()
    {
        return this->id;
    }

    virtual float CalcAutomation(double time)
    {
        return ProduceWave(time); 
    }
 
    void RenderLFO()
    {
        ImGui::Begin(windowName.c_str());
        const char* items[4] = { "Sine", "Triangle", "Square", "Sawtooth"};
        int selected_item = type;
        if (ImGui::Combo("Waveform", &selected_item, items, 4))
            type = (WaveType)selected_item;

        ImGuiKnobs::Knob("Amount", &(this->amplitude),
            0.0f, 1.0f, 0.01f, "%.2f", ImGuiKnobVariant_Wiper,
            0.0f, ImGuiKnobFlags_DragVertical);
        ImGui::SameLine(); 

        ImGuiKnobs::Knob("Freq.", &(this->freq_hz),
            0.1, 20, 0.1f, "%.2f", ImGuiKnobVariant_Wiper,
            0.0f, ImGuiKnobFlags_DragVertical);

        ImGui::End();
    } 
};
 
class LFOsWindow : public WindowSection {
private:
    std::vector<LFO*> lfos;

public: 
    virtual void RenderWindow()
    {
        for (int i = 0; i < lfos.size(); i++)
            lfos[i]->RenderLFO();
    }

    LFOsWindow()
    { 
        this->name = "LFOs"; 
    } 

    ~LFOsWindow()
    { 
        for (int i = 0; i < lfos.size(); i++)
            delete[] lfos[i];
    } 

    void AddLFO(LFO* lfo)
    {
        lfos.push_back(lfo);
    }
};
