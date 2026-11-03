// Behaviour test for 0x00B23E00 CEngine::SetWorld: the forwarder must call CEngineWorldMap::SetWorld on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CIDrawWorld { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineWorldMap { public:
    void SetWorld(const CIDrawWorld* world)
    {
        ++gCalls; gThis = this;
        gPtr[0] = world;
    }
};
static CEngineWorldMap gComponent;
CEngineWorldMap* g_pEngineWorldMap = &gComponent;
class CEngine { public:
    virtual void SetWorld(const CIDrawWorld* world);
};
void CEngine::SetWorld(const CIDrawWorld* world)
{
    g_pEngineWorldMap->SetWorld(world);
}
int main()
{
    CIDrawWorld o0;
    CEngine engine;
    engine.SetWorld(&o0);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0) {
        std::printf("00b23e00_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23e00_TEST PASS\n");
    return 0;
}
