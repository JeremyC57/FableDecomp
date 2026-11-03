// Behaviour test for 0x00B23E60 CEngine::SetMapAsUnloaded: the forwarder must call CEngineWorldMap::OnGameMapUnloaded on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineWorldMap { public:
    void OnGameMapUnloaded(long mapIndex)
    {
        ++gCalls; gThis = this;
        gVal[0] = (double)mapIndex;
    }
};
static CEngineWorldMap gComponent;
CEngineWorldMap* g_pEngineWorldMap = &gComponent;
class CEngine { public:
    virtual void SetMapAsUnloaded(long mapIndex);
};
void CEngine::SetMapAsUnloaded(long mapIndex)
{
    g_pEngineWorldMap->OnGameMapUnloaded(mapIndex);
}
int main()
{
    CEngine engine;
    engine.SetMapAsUnloaded(-7);
    if (gCalls != 1 || gThis != &gComponent || gVal[0] != -7.0) {
        std::printf("00b23e60_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23e60_TEST PASS\n");
    return 0;
}
