// Behaviour test for 0x00B1AE60 NParticleEngine::CPSCLight::EnterTimelineEnd() -- clears LightUseTimelineSecs only
#include <cstdio>
class CPSCLight {
public:
    bool LightUseFadeColour : 1;
    bool LightUseTimelineSecs : 1;
    bool StartedTimer : 1;
    virtual ~CPSCLight() { }
    virtual void EnterTimelineEnd() { LightUseTimelineSecs = false; }
};
int main()
{
    CPSCLight l;
    l.LightUseFadeColour = true; l.LightUseTimelineSecs = true; l.StartedTimer = true;
    l.EnterTimelineEnd();
    if (l.LightUseTimelineSecs || !l.LightUseFadeColour || !l.StartedTimer) { std::printf("00b1ae60_TEST FAIL\n"); return 1; }
    std::printf("00b1ae60_TEST PASS\n");
    return 0;
}