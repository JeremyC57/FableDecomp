// Behaviour test for 0x00B23E30 CEngine::SetMapAsLoaded: the forwarder must call CEngineWorldMap::OnGameMapLoaded on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineWorldMap { public:
    void OnGameMapLoaded(long mapIndex)
    {
        ++gCalls; gThis = this;
        gVal[0] = (double)mapIndex;
    }
};
static CEngineWorldMap gComponent;
CEngineWorldMap* g_pEngineWorldMap = &gComponent;
class CEngine { public:
    virtual void SetMapAsLoaded(long mapIndex);
};
void CEngine::SetMapAsLoaded(long mapIndex)
{
    g_pEngineWorldMap->OnGameMapLoaded(mapIndex);
}
int main()
{
    CEngine engine;
    engine.SetMapAsLoaded(-7);
    if (gCalls != 1 || gThis != &gComponent || gVal[0] != -7.0) {
        std::printf("00b23e30_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23e30_TEST PASS\n");
    return 0;
}