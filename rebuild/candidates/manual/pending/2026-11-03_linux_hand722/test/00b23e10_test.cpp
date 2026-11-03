// Behaviour test for 0x00B23E10 CEngine::AddMap: the forwarder must call CEngineWorldMap::AddMap on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineWorldMap { public:
    void AddMap(long mapIndex, long mapUID)
    {
        ++gCalls; gThis = this;
        gVal[0] = (double)mapIndex;
        gVal[1] = (double)mapUID;
    }
};
static CEngineWorldMap gComponent;
CEngineWorldMap* g_pEngineWorldMap = &gComponent;
class CEngine { public:
    virtual void AddMap(long mapIndex, long mapUID);
};
void CEngine::AddMap(long mapIndex, long mapUID)
{
    g_pEngineWorldMap->AddMap(mapIndex, mapUID);
}
int main()
{
    CEngine engine;
    engine.AddMap(-7, -8);
    if (gCalls != 1 || gThis != &gComponent || gVal[0] != -7.0 || gVal[1] != -8.0) {
        std::printf("00b23e10_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23e10_TEST PASS\n");
    return 0;
}
