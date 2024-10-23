#pragma once
#include "WaveGenerator.h"

class ParameterAutomator {
protected:
    float* target;
public:

    void SetTarget(float *pTarget)
    {
        this->target = pTarget;
    }

    virtual void ApplyAutomation(double time) = 0;
};

class LFO : public WaveGenerator, public ParameterAutomator {
private:
    char id;
    std::string windowName;
    float* target;

    float temp_freq;

public:
    LFO(char id, WaveType type, float amplitude)
        : WaveGenerator(type, amplitude, 0.0f)
    {
        this->freq = 1;
        this->target = nullptr;
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

    virtual void ApplyAutomation(double time)
    {
        if (this->target == nullptr)
            return;
        *(this->target) += ProduceWave(time); 
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

        this->temp_freq = (float)this->freq;
        if (ImGuiKnobs::Knob("Freq.", &(this->temp_freq),
            0.1, 20, 0.1f, "%.2f", ImGuiKnobVariant_Wiper,
            0.0f, ImGuiKnobFlags_DragVertical))
        {
            this->freq = (double)(this->temp_freq);
        }

        ImGui::End();
    } 
};
 
class LFOsWindow : public WindowSection {
private:
    std::vector<LFO> lfos;

public: 
    virtual void RenderWindow()
    {
        for (int i = 0; i < lfos.size(); i++)
            lfos[i].RenderLFO();
    }

    LFOsWindow()
    { 
        this->name = "LFOs"; 
    } 

    void AddLFO(LFO lfo)
    {
        lfos.push_back(lfo);
    }

    void UpdateLFOs(double time)
    {
        for (int i = 0; i < lfos.size(); i++)
            lfos[i].ApplyAutomation(time);
    }
};
