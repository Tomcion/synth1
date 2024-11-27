#pragma once
#include <string>
#include <vector>

#include "ParameterAutomator.h"
#include "imgui.h"
#include "imgui-knobs.h"

#include <iostream>

class ModulableField {
private:
    std::string name;
    ParameterAutomator* automator;
    bool is_automated;
    float raw_value;
    float res_value;
    float min_value;
    float max_value;
public:
    ModulableField(std::string name, float value, float min_value, float max_value)
        : name(name), raw_value(value), res_value(value),
          min_value(min_value), max_value(max_value)
    {
        automator = nullptr;
        is_automated = false;
    }

    void SetValue(float new_value)
    {
        this->raw_value = new_value;
    }

    void SetAutomator(ParameterAutomator* aut)
    {
        this->automator = aut;
    }

    float GetValue()
    {
        return this->res_value;
    } 

    void RenderField()
    { 
        ImGuiKnobs::Knob("Detune", &(this->raw_value),
            min_value, max_value, 0.01f, "%.2f", ImGuiKnobVariant_Wiper,
            0.0f, ImGuiKnobFlags_DragVertical);
    }

    void CalcModuletedValue(double time)
    {
        if (this->automator == nullptr)
            this->res_value = this->raw_value;
        else
            this->res_value = this->raw_value + (this->automator)->CalcAutomation(time);
    }
};


class ModFieldList {
private:
    std::vector<ModulableField*> mod_list;
public:
    void AddField(ModulableField* field)
    {
        mod_list.push_back(field);
    }

    void UpdateFields(double time)
    {
        for (int i = 0; i < mod_list.size(); i++)
            mod_list[i]->CalcModuletedValue(time);
    }
};
