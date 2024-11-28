#include "Envelope.h"
#include <iostream>

float Envelope::ProcessNoteOn(double time)
{ 
    double attackEnd = noteOnTime + attack;
    double decayEnd = attackEnd + decay;

    if (time < attackEnd) 
        return (time - noteOnTime) / attack;
    if (time >= attackEnd && time <= decayEnd)
        return 1 - ((time - attackEnd) * (1 - sustain)) / decay; 
    return sustain;
}

float Envelope::ProcessNoteOff(double time)
{
    if (noteOnTime > noteOffTime)
        return 1;
    if (time > noteOffTime + release)
        return 0;
    if (noteOffTime > noteOnTime)
        return 1 - (time - noteOffTime) / release;
    else
        return 0;
}

Envelope::Envelope(float attack, float decay, float sustain, float release)
    : attack(attack), decay(decay), sustain(sustain), release(release)
{ 
    noteOnTime = 0.0f;
    noteOffTime = 0.0f;
    cur_value = 0.0f;
}

float Envelope::CalcAutomation(double time)
{ 
    return ProcessNoteOn(time) * ProcessNoteOff(time);
}

void Envelope::SetNoteOffTime(double offTime)
{
    this->noteOffTime = offTime;
}

void Envelope::SetNoteOnTime(double onTime)
{
    this->noteOnTime = onTime;
} 

void Envelope::RenderEnvelope()
{
    ImGui::Begin("Envelope");

    ImGuiKnobs::Knob("Attack", &(this->attack),
        0.0f, 5.0f, 0.05f, "%.2f", ImGuiKnobVariant_Wiper,
        0.0f, ImGuiKnobFlags_DragVertical);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Decay", &(this->decay),
        0.0f, 5.0f, 0.05f, "%.2f", ImGuiKnobVariant_Wiper,
        0.0f, ImGuiKnobFlags_DragVertical);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Sustain", &(this->sustain),
        0.0f, 1.0f, 0.01f, "%.2f", ImGuiKnobVariant_Wiper,
        0.0f, ImGuiKnobFlags_DragVertical);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Release", &(this->release),
        0.0f, 5.0f, 0.05f, "%.2f", ImGuiKnobVariant_Wiper,
        0.0f, ImGuiKnobFlags_DragVertical);
    ImGui::SameLine(); 

    ImGui::End();
} 

void EnvelopesWindow::RenderWindow()
{
    for (int i = 0; i < envs.size(); i++)
        envs[i]->RenderEnvelope();
}

EnvelopesWindow::EnvelopesWindow()
{ 
    this->name = "Envelopes"; 
} 

EnvelopesWindow::~EnvelopesWindow()
{ 
    for (int i = 0; i < envs.size(); i++)
        delete[] envs[i];
} 

void EnvelopesWindow::AddEnvelope(Envelope* lfo)
{
    envs.push_back(lfo);
}

void EnvelopesWindow::SetNoteOffTime(double offTime)
{
    for (int i = 0; i < envs.size(); i++)
        envs[i]->SetNoteOffTime(offTime);
}

void EnvelopesWindow::SetNoteOnTime(double onTime)
{
    for (int i = 0; i < envs.size(); i++)
        envs[i]->SetNoteOnTime(onTime); 
}

