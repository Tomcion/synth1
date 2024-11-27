#include "Oscillator.h"

void  Oscillator::AddAutomatorDetune(ParameterAutomator* aut)
{
    (this->detune).SetAutomator(aut);
}

Oscillator::Oscillator(char id, WaveType type, float amplitude, int octave)
    : WaveGenerator(type, amplitude, 0.0f), octave(octave),
      detune("Detune", 0.0f, -1.0f, 1.0f)
{
    mod_fields.AddField(&detune);
    this->SetId(id);
}

void Oscillator::SetId(char num)
{
    this->id = num;
    windowName = "Oscillator " + std::to_string(num);
}

const int Oscillator::GetNumber()
{
    return this->id;
}

void Oscillator::SetOscFrequencyRad(double frequency)
{
    this->raw_freq_rad = ToRad(frequency * pow(2, octave));
}

void Oscillator::UpdateFields(double time)
{
    mod_fields.UpdateFields(time);
}

void Oscillator::UpdateParameters(double time)
{
    float detuneValue = (this->detune).GetValue();
    this->freq_rad = this->raw_freq_rad * pow(2.0f, detuneValue / 12.0f);
}

void Oscillator::RenderOsc()
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
    detune.RenderField();
    ImGui::End();;
} 

void OscillatorsWindow::RenderWindow()
{
    for (int i = 0; i < oscs.size(); i++)
        oscs[i]->RenderOsc();
}

OscillatorsWindow::OscillatorsWindow()
{ 
    this->name = "Oscillators";
    this->note_freq = 0.0f; 
}

OscillatorsWindow::~OscillatorsWindow()
{ 
    for (int i = 0; i < oscs.size(); i++)
        delete[] oscs[i];
} 

void OscillatorsWindow::AddOscillator(Oscillator* osc)
{
    oscs.push_back(osc);
}

void OscillatorsWindow::SetNoteFrequency(int note)
{
    if (note == -1)
        note_freq = 0.0;
    else
    {
        note_freq = lowest_octave * pow(halfToneRatio, note);
    }

    for (int i = 0; i < oscs.size(); i++)
    {
        oscs[i]->SetOscFrequencyRad(note_freq);
    }
}

double OscillatorsWindow::MixOscillators(double time)
{
    double output = 0.0;
    for (int i = 0; i < oscs.size(); i++)
    {
        oscs[i]->UpdateFields(time);
        output += (this->oscs[i])->ProduceWave(time);
    }
    return output;
}
