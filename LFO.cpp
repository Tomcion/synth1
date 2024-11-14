#include "LFO.h"

LFO::LFO(std::string name, WaveType type, float amplitude, float freq_hz)
    : ParameterAutomator(), WaveGenerator(type, amplitude, 0.0f),
    freq_hz(freq_hz), windowName(name)
{
    this->freq_rad = ToRad(freq_hz);
    this->amount = 0.0f;
} 

float LFO::CalcAutomation(double time)
{
    this->freq_rad = ToRad(this->freq_hz);
    return amount * ProduceWave(time); 
}

void LFO::RenderLFO()
{
    ImGui::Begin(windowName.c_str());
    const char* items[4] = { "Sine", "Triangle", "Square", "Sawtooth"};
    int selected_item = type;
    if (ImGui::Combo("Waveform", &selected_item, items, 4))
        type = (WaveType)selected_item;

    ImGuiKnobs::Knob("Amount", &(this->amount),
        0.0f, 1.0f, 0.01f, "%.2f", ImGuiKnobVariant_Wiper,
        0.0f, ImGuiKnobFlags_DragVertical);
    ImGui::SameLine(); 

    ImGuiKnobs::Knob("Freq.", &(this->freq_hz),
        0.1, 20, 0.1f, "%.2f", ImGuiKnobVariant_Wiper,
        0.0f, ImGuiKnobFlags_DragVertical);

    ImGui::End();
} 

PhaseLFO::PhaseLFO(std::string name, float amplitude, float freq_hz)
    : LFO(name, SINE, 1.0f, freq_hz)
{ 
}

void  PhaseLFO::CalcPMAmplitude(double carrier_freq)
{ 
    double freq_dev = carrier_freq * (halfToneRatio - 1);
    this->amplitude = (float)(freq_dev / this->freq_rad);
}

void PhaseLFO::RenderLFO()
{
    ImGui::Begin(windowName.c_str());

    ImGuiKnobs::Knob("Amount", &(this->amount),
        0.0f, 1.0f, 0.01f, "%.2f", ImGuiKnobVariant_Wiper,
        0.0f, ImGuiKnobFlags_DragVertical);
    ImGui::SameLine(); 

    ImGuiKnobs::Knob("Freq.", &(this->freq_hz),
        0.1, 20, 0.1f, "%.2f", ImGuiKnobVariant_Wiper,
        0.0f, ImGuiKnobFlags_DragVertical);

    ImGui::End();
} 
 
void LFOsWindow::RenderWindow()
{
    for (int i = 0; i < lfos.size(); i++)
        lfos[i]->RenderLFO();
}

LFOsWindow::LFOsWindow()
{ 
    this->name = "LFOs"; 
} 

LFOsWindow::~LFOsWindow()
{ 
    for (int i = 0; i < lfos.size(); i++)
        delete[] lfos[i];
} 

void LFOsWindow::AddLFO(LFO* lfo)
{
    lfos.push_back(lfo);
}

