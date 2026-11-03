// Behaviour test for 0x00B23CE0 CEngine::AddCustomMorphTexture: the forwarder must call CEngineSurfaceCompositionManager::AddCustomMorphTexture on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CTexture { public: int v; };
class CCharString { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineSurfaceCompositionManager { public:
    unsigned long AddCustomMorphTexture(CTexture& texture, const CCharString& name)
    {
        ++gCalls; gThis = this;
        gPtr[0] = &texture;
        gPtr[1] = &name;
        return 0x1234u;
    }
};
static CEngineSurfaceCompositionManager gComponent;
CEngineSurfaceCompositionManager* g_pSurfaceComposition_01436eac = &gComponent;
class CEngine { public:
    virtual unsigned long AddCustomMorphTexture(CTexture& texture, const CCharString& name) const;
};
unsigned long CEngine::AddCustomMorphTexture(CTexture& texture, const CCharString& name) const
{
    return g_pSurfaceComposition_01436eac->AddCustomMorphTexture(texture, name);
}
int main()
{
    CTexture o0;
    CCharString o1;
    CEngine engine;
    unsigned long r = engine.AddCustomMorphTexture(o0, o1);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0 || gPtr[1] != &o1 || r != 0x1234u) {
        std::printf("00b23ce0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23ce0_TEST PASS\n");
    return 0;
}
