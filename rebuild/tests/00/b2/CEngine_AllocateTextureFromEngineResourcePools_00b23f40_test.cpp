// Behaviour test for 0x00B23F40 CEngine::AllocateTextureFromEngineResourcePools: the forwarder must call CEngineResourceManager::AllocateExternalTextureFromResourcePools on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CTexture { public: int v; };
class C2DExtentsI { public: int v; };
class CPixelFormat { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineResourceManager { public:
    CTexture* AllocateExternalTextureFromResourcePools(unsigned long usage, const C2DExtentsI& extents, long mipLevels, const CPixelFormat& format)
    {
        ++gCalls; gThis = this;
        gVal[0] = (double)usage;
        gPtr[1] = &extents;
        gVal[2] = (double)mipLevels;
        gPtr[3] = &format;
        static CTexture made; return &made;
    }
};
static CEngineResourceManager gComponent;
CEngineResourceManager* g_pResourceManager = &gComponent;
class CEngine { public:
    virtual CTexture* AllocateTextureFromEngineResourcePools(unsigned long usage, const C2DExtentsI& extents, long mipLevels, const CPixelFormat& format);
};
CTexture* CEngine::AllocateTextureFromEngineResourcePools(unsigned long usage, const C2DExtentsI& extents, long mipLevels, const CPixelFormat& format)
{
    return g_pResourceManager->AllocateExternalTextureFromResourcePools(usage, extents, mipLevels, format);
}
int main()
{
    C2DExtentsI o1;
    CPixelFormat o3;
    CEngine engine;
    CTexture* r = engine.AllocateTextureFromEngineResourcePools(40u, o1, -9, o3);
    if (gCalls != 1 || gThis != &gComponent || gVal[0] != 40.0 || gPtr[1] != &o1 || gVal[2] != -9.0 || gPtr[3] != &o3 || r == 0) {
        std::printf("00b23f40_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23f40_TEST PASS\n");
    return 0;
}