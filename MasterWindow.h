#pragma once
#include <Windows.h>
#include <iostream>
#include "LFO.h"
#include "Envelope.h"
#include "Oscillator.h"

class MasterWindow : public WindowSection {
private:
    OscillatorsWindow oscillatorsWindow;
    EnvelopesWindow envelopesWindow;
    LFOsWindow lfosWindow;
public:
    MasterWindow()
    {
        this->name = "Master Window";

        Oscillator* osc1 = new Oscillator(1, TRIANGLE, 0.5f, 2);
        oscillatorsWindow.AddOscillator(osc1);

        Oscillator* osc2 = new Oscillator(2, SQUARE, 0.1f, 3);
        oscillatorsWindow.AddOscillator(osc2);
 
        LFO* lfo1 = new LFO(1, SINE, 0.5f, 1.0f, osc1->GetModPointer()); 
        lfosWindow.AddLFO(lfo1);
        //lfo1->SetTarget(osc1->GetModPointer());

        LFO* lfo2 = new LFO(2, SINE, 0.5f, 1.0f, osc2->GetModPointer());
        lfosWindow.AddLFO(lfo2);
        //lfo2->SetTarget(osc2->GetModPointer());
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
        lfosWindow.UpdateLFOs(time);
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

