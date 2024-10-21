#pragma once 
#include "imgui.h"
#include "imgui-knobs.h"
#include <string>

class WindowSection {
protected:
    const char* name;
public: 
    const char* getName()
    {
        return this->name;
    }

    virtual void RenderWindow() = 0;
};

class RegularWindow : public WindowSection {
private:
    virtual void RenderElements() = 0;

public:
    virtual void RenderWindow()
    {
        ImGui::Begin(this->name);
        RenderElements();
        ImGui::End();
    }
};

