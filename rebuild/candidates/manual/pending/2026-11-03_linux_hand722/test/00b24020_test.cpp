// Behaviour test for 0x00B24020 CEngine::OverrideSnow: the forwarder must call CEngineWeatherRenderer::OverrideSnow on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineWeatherRenderer { public:
    void OverrideSnow(float amount)
    {
        ++gCalls; gThis = this;
        gVal[0] = (double)amount;
    }
};
static CEngineWeatherRenderer gComponent;
CEngineWeatherRenderer* g_pEngineWeatherRenderer = &gComponent;
class CEngine { public:
    virtual void OverrideSnow(float amount);
};
void CEngine::OverrideSnow(float amount)
{
    g_pEngineWeatherRenderer->OverrideSnow(amount);
}
int main()
{
    CEngine engine;
    engine.OverrideSnow(2.5f);
    if (gCalls != 1 || gThis != &gComponent || (gVal[0] < 2.49 || gVal[0] > 2.51)) {
        std::printf("00b24020_TEST FAIL\n");
        return 1;
    }
    std::printf("00b24020_TEST PASS\n");
    return 0;
}
