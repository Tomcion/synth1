#pragma once
#include <Windows.h>
#include <iostream>
#include "Envelope.h"
#include "Oscillator.h"
#include "LFO.h"

class MasterWindow : public WindowSection {
private:
    OscillatorsWindow oscillatorsWindow;
    EnvelopesWindow envelopesWindow;
    LFOsWindow lfosWindow;
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
 
        osc1->SetPhaseModulator(lfo1);
        osc2->SetPhaseModulator(lfo1);
    }

    virtual void RenderWindow()
    {
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        oscillatorsWindow.RenderWindow();
        envelopesWindow.RenderWindow();
        lfosWindow.RenderWindow();
    }

    double MixSound(double time)
    {
        double output = oscillatorsWindow.MixOscillators(time);
        return output;
    }
 
    void ProcessNotes(bool& notePressed, int currentNote)
    {
		notePressed = false;
		for (int k = 0; k < 16; k++)
		{
			if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k])) & 0x8000)
			{
				if (currentNote != k)
				{					
                    std::cout << "key down " << k << std::endl;
                    oscillatorsWindow.SetNoteFrequency(k);
					currentNote = k;
				}

				notePressed = true;
			}
		}

		if (!notePressed)
		{	
			if (currentNote != -1)
			{
                // release
                std::cout << "note released\n";
				currentNote = -1;
			}

            oscillatorsWindow.SetNoteFrequency(-1);
		}
    }
};

