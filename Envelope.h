#pragma once
#include "WindowSection.h"
#include "ParameterAutomator.h"
#include <vector>
#include <iostream>

class Envelope : public ParameterAutomator {
private:
    double noteOnTime;
    double noteOffTime;
    float attack, decay, sustain, release;

    float ProcessNoteOn(double time); 

    float ProcessNoteOff(double time);

public:
    Envelope(float attack, float decay, float sustain, float release);
    
    void RenderEnvelope(); 

    float CalcAutomation(double time) override;
    
    void SetNoteOffTime(double offTime);
    
    void SetNoteOnTime(double onTime); 

};


class EnvelopesWindow : public RegularWindow {
private:
    std::vector<Envelope*> envs;

    virtual void RenderElements()
    {
        ImGui::Text("lalalla env");
    }

public:
    EnvelopesWindow();

    ~EnvelopesWindow();

public:
    virtual void RenderWindow();

    void AddEnvelope(Envelope* osc); 

    void SetNoteOffTime(double offTime);

    void SetNoteOnTime(double onTime);

};



