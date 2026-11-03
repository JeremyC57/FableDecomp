// Behaviour test for 0x00B23E50 CEngine::SetMapVisibilityAllMaps: the forwarder must call CEngineWorldMap::SetMapVisibilityAllMaps on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineWorldMap { public:
    void SetMapVisibilityAllMaps(bool visible)
    {
        ++gCalls; gThis = this;
        gVal[0] = (double)visible;
    }
};
static CEngineWorldMap gComponent;
CEngineWorldMap* g_pEngineWorldMap = &gComponent;
class CEngine { public:
    virtual void SetMapVisibilityAllMaps(bool visible);
};
void CEngine::SetMapVisibilityAllMaps(bool visible)
{
    g_pEngineWorldMap->SetMapVisibilityAllMaps(visible);
}
int main()
{
    CEngine engine;
    engine.SetMapVisibilityAllMaps(true);
    if (gCalls != 1 || gThis != &gComponent || gVal[0] != 1.0) {
        std::printf("00b23e50_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23e50_TEST PASS\n");
    return 0;
}
