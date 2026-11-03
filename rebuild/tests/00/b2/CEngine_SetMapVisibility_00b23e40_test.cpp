// Behaviour test for 0x00B23E40 CEngine::SetMapVisibility: the forwarder must call CEngineWorldMap::SetMapVisibility on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineWorldMap { public:
    void SetMapVisibility(long mapIndex, bool visible)
    {
        ++gCalls; gThis = this;
        gVal[0] = (double)mapIndex;
        gVal[1] = (double)visible;
    }
};
static CEngineWorldMap gComponent;
CEngineWorldMap* g_pEngineWorldMap = &gComponent;
class CEngine { public:
    virtual void SetMapVisibility(long mapIndex, bool visible);
};
void CEngine::SetMapVisibility(long mapIndex, bool visible)
{
    g_pEngineWorldMap->SetMapVisibility(mapIndex, visible);
}
int main()
{
    CEngine engine;
    engine.SetMapVisibility(-7, true);
    if (gCalls != 1 || gThis != &gComponent || gVal[0] != -7.0 || gVal[1] != 1.0) {
        std::printf("00b23e40_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23e40_TEST PASS\n");
    return 0;
}