// Behaviour test for 0x00B23DB0 CEngine::CloseStaticMapFile: the forwarder must call CEngineWorldMap::CloseStaticMapFile on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineWorldMap { public:
    void CloseStaticMapFile()
    {
        ++gCalls; gThis = this;
    }
};
static CEngineWorldMap gComponent;
CEngineWorldMap* g_pEngineWorldMap = &gComponent;
class CEngine { public:
    virtual void CloseStaticMapFile();
};
void CEngine::CloseStaticMapFile()
{
    g_pEngineWorldMap->CloseStaticMapFile();
}
int main()
{
    CEngine engine;
    engine.CloseStaticMapFile();
    if (gCalls != 1 || gThis != &gComponent) {
        std::printf("00b23db0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23db0_TEST PASS\n");
    return 0;
}