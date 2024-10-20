#pragma once
#include "WindowSection.h"

class LFOsWindow : public RegularWindow {
private:
    virtual void RenderElements()
    {
        ImGui::Text("lalalla lfo");
    }
public:
    LFOsWindow()
    {
        this->name = "LFOs";
    }

};
