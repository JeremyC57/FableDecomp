// Behaviour test for 0x00B23F50 CEngine::FreeTextureFromEngineResourcePools: the forwarder must call CEngineResourceManager::FreeExternalTextureFromResourcePools on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CTexture { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineResourceManager { public:
    void FreeExternalTextureFromResourcePools(CTexture* texture)
    {
        ++gCalls; gThis = this;
        gPtr[0] = texture;
    }
};
static CEngineResourceManager gComponent;
CEngineResourceManager* g_pResourceManager = &gComponent;
class CEngine { public:
    virtual void FreeTextureFromEngineResourcePools(CTexture* texture);
};
void CEngine::FreeTextureFromEngineResourcePools(CTexture* texture)
{
    g_pResourceManager->FreeExternalTextureFromResourcePools(texture);
}
int main()
{
    CTexture o0;
    CEngine engine;
    engine.FreeTextureFromEngineResourcePools(&o0);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0) {
        std::printf("00b23f50_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23f50_TEST PASS\n");
    return 0;
}
