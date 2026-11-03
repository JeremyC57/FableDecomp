// Behaviour test for 0x00B23DC0 CEngine::SetStaticMapFileForUse: the forwarder must call CEngineWorldMap::SetStaticMapFileForUse on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CWideString { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineWorldMap { public:
    void SetStaticMapFileForUse(const CWideString& fileName)
    {
        ++gCalls; gThis = this;
        gPtr[0] = &fileName;
    }
};
static CEngineWorldMap gComponent;
CEngineWorldMap* g_pEngineWorldMap = &gComponent;
class CEngine { public:
    virtual void SetStaticMapFileForUse(const CWideString& fileName);
};
void CEngine::SetStaticMapFileForUse(const CWideString& fileName)
{
    g_pEngineWorldMap->SetStaticMapFileForUse(fileName);
}
int main()
{
    CWideString o0;
    CEngine engine;
    engine.SetStaticMapFileForUse(o0);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0) {
        std::printf("00b23dc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23dc0_TEST PASS\n");
    return 0;
}
