#pragma once

#include <cmath>
#include "WindowSection.h"

class Saturator {
private:
    float drive;
    bool enabled;
    float outputVolume;

public:
    Saturator(float drive)
        : drive(drive), enabled(false), outputVolume(1)
    {
    }

    float Saturate(float input)
    {
        if (enabled)
            return tanh(drive * input) * outputVolume;
        return input;
    }

    void Render()
    { 
        ImGui::Begin("Saturator");

        ImGui::Checkbox("Enabled", &(this->enabled));

        ImGuiKnobs::Knob("Drive", &(this->drive),
            0.0f, 10.0f, 0.05f, "%.2f", ImGuiKnobVariant_Wiper,
            0.0f, ImGuiKnobFlags_DragVertical);
        ImGui::SameLine();
        ImGuiKnobs::Knob("Output", &(this->outputVolume),
            0.0f, 1.0f, 0.01f, "%.2f", ImGuiKnobVariant_Wiper,
            0.0f, ImGuiKnobFlags_DragVertical);
        ImGui::SameLine();

        ImGui::End();
    }
};
