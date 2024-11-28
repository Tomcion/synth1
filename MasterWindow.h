#pragma once
#include <Windows.h>
#include <iostream>
#include "Envelope.h"
#include "Oscillator.h"
#include "LFO.h"
#include "Saturator.h"
#include "Filter.h"

class MasterWindow : public WindowSection {
private:
    OscillatorsWindow oscillatorsWindow;
    EnvelopesWindow envelopesWindow;
    LFOsWindow lfosWindow;

    Envelope* masterEnvelope;
    Saturator* saturator;
    Biquad* filter;
public:
    MasterWindow()
    {
        this->name = "Master Window";

        PhaseLFO* lfo1 = new PhaseLFO("LFO Phase", SINE, 2.0f);
        lfosWindow.AddLFO(lfo1);
 
        LFO* lfo2 = new LFO("LFO Volume", SINE, 0.5f, 1.0f);
        lfosWindow.AddLFO(lfo2);

        Oscillator* osc1 = new Oscillator(1, SINE, 0.5f, 2);
        oscillatorsWindow.AddOscillator(osc1);

        Oscillator* osc2 = new Oscillator(2, SQUARE, 0.1f, 3);
        oscillatorsWindow.AddOscillator(osc2);

        Oscillator* osc3 = new Oscillator(3, NOISE, 0.05f, 3);
        oscillatorsWindow.AddOscillator(osc3);

        masterEnvelope = new Envelope(0.0f, 0.5f, 0.3f, 3.0f);
        envelopesWindow.AddEnvelope(masterEnvelope);

        saturator = new Saturator(1.0f);

        filter = new Biquad();
        filter->setBiquad(bq_type_lowpass, 0.3, 0.707, 0);
 
        osc1->SetPhaseModulator(lfo1);
        osc2->SetPhaseModulator(lfo1);
    }

    virtual void RenderWindow()
    {
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        oscillatorsWindow.RenderWindow();
        envelopesWindow.RenderWindow();
        lfosWindow.RenderWindow();
        saturator->Render();
        filter->RenderFilter();
    }

    double MixSound(double time)
    {
        double output = oscillatorsWindow.MixOscillators(time);
        output *= masterEnvelope->CalcAutomation(time);
        output = filter->process(output);
        output = saturator->Saturate(output);
        return output;
    }
 
    void ProcessNotes(bool& notePressed, int& currentNote, double time)
    {
		notePressed = false;
		for (int k = 0; k < 16; k++)
		{
			if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k])) & 0x8000)
			{
				if (currentNote != k)
				{					
                    //std::cout << "key down " << k << std::endl;
                    oscillatorsWindow.SetNoteFrequency(k);
                    envelopesWindow.SetNoteOnTime(time);
					currentNote = k;
				}

				notePressed = true;
			}
		}

		if (!notePressed)
		{	
			if (currentNote != -1)
			{
                //std::cout << "note released\n";
                envelopesWindow.SetNoteOffTime(time);
				currentNote = -1;
			}

            //oscillatorsWindow.SetNoteFrequency(-1);
		}
    }
};

