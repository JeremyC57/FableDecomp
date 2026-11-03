// Behaviour test for 0x00B24010 CEngine::OverrideRain: the forwarder must call CEngineWeatherRenderer::OverrideRain on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineWeatherRenderer { public:
    void OverrideRain(float amount)
    {
        ++gCalls; gThis = this;
        gVal[0] = (double)amount;
    }
};
static CEngineWeatherRenderer gComponent;
CEngineWeatherRenderer* g_pEngineWeatherRenderer = &gComponent;
class CEngine { public:
    virtual void OverrideRain(float amount);
};
void CEngine::OverrideRain(float amount)
{
    g_pEngineWeatherRenderer->OverrideRain(amount);
}
int main()
{
    CEngine engine;
    engine.OverrideRain(2.5f);
    if (gCalls != 1 || gThis != &gComponent || (gVal[0] < 2.49 || gVal[0] > 2.51)) {
        std::printf("00b24010_TEST FAIL\n");
        return 1;
    }
    std::printf("00b24010_TEST PASS\n");
    return 0;
}