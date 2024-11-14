#pragma once
#include "WaveGenerator.h"
#include "ParameterAutomator.h"

class LFO : public WaveGenerator, public ParameterAutomator {
protected:
    std::string windowName; 
    float freq_hz; 

public:
    LFO(std::string name, WaveType type, float amplitude, float freq_hz);

    float CalcAutomation(double time); 
 
    virtual void RenderLFO();
};


class PhaseLFO : public LFO {
public:
    PhaseLFO(std::string name, float amplitude, float freq_hz);

    void CalcPMAmplitude(double carrier_freq); 
    void RenderLFO() override;
};

 
class LFOsWindow : public WindowSection {
private:
    std::vector<LFO*> lfos;

public: 
    virtual void RenderWindow(); 

    LFOsWindow(); 

    ~LFOsWindow(); 

    void AddLFO(LFO* lfo); 
};
