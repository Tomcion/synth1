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

