#pragma once

class ParameterAutomator {
protected:
public: 
    ParameterAutomator()
    { 
    }

    virtual float CalcAutomation(double time) = 0;
};

