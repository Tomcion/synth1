#pragma once
#include "WindowSection.h"

class EnvelopesWindow : public RegularWindow {
private:
    virtual void RenderElements()
    {
        ImGui::Text("lalalla env");
    }
public:
    EnvelopesWindow()
    {
        this->name = "Envelopes";
    }

};
