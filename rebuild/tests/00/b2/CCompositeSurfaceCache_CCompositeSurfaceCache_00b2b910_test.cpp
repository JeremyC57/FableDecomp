// Behaviour test for CCompositeSurfaceCache::~CCompositeSurfaceCache (0x00B2B910): Release the managed texture, destroy it,
// drop the shared graphic frame (uninitialise + free the frame data and its counted texture on the last reference only),
// free both morphs' blend storage, then the CResource base.
#include <cstdio>
#include <cstdlib>
static char g_log[64];
static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
static int g_textureDeletes = 0;
static void DeleteTexture(void*) { ++g_textureDeletes; }
class CCPPointerInfo { public: unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
struct CCountedTexture {
    void* Data; CCPPointerInfo* Info;
    ~CCountedTexture() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } Data = 0; Info = 0; }
};
class CGraphicFrameData { public:
    unsigned int ReferenceCount; CCountedTexture PTexture; bool IsInitialised;
    void Uninitialise() { Log('U'); }
    ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
};
class CGraphicFrame { public:
    CGraphicFrameData* FrameData;
    ~CGraphicFrame()
    {
        CGraphicFrameData* data = FrameData;
        if (data) {
            if (--data->ReferenceCount == 0) { Log('F'); delete data; }
            FrameData = 0;
        }
    }
};
class CManagedTexture { public: bool Release() { Log('R'); return true; } ~CManagedTexture() { Log('M'); } };
struct BlendVector { void* start; ~BlendVector() { if (start) { Log('V'); std::free(start); } } };
class CEngineTextureMorph { public: long OriginalTextureIndex, MorphID; BlendVector Blends; };
class CResource { public: ~CResource() { Log('Z'); } };
class CCompositeSurfaceCache : public CResource { public:
    CEngineTextureMorph CurrentMorph, CachedMorph;
    CGraphicFrame GraphicFrame;
    CManagedTexture ManagedTexture;
    ~CCompositeSurfaceCache() { ManagedTexture.Release(); }
};
static CGraphicFrameData* MakeFrame(unsigned int refs)
{
    CGraphicFrameData* d = new CGraphicFrameData;
    d->ReferenceCount = refs; d->IsInitialised = true;
    d->PTexture.Data = 0; d->PTexture.Info = new CCPPointerInfo; d->PTexture.Info->RefCount = 1; d->PTexture.Info->DeleteFunc = DeleteTexture; d->PTexture.Info->Data = 0;
    return d;
}
int main()
{
    {
        CCompositeSurfaceCache* c = new CCompositeSurfaceCache;
        c->CurrentMorph.Blends.start = std::malloc(16);
        c->CachedMorph.Blends.start = 0;
        c->GraphicFrame.FrameData = MakeFrame(1);
        delete c;
    }
    const char* want = "RMFUVZ";
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) { std::printf("00b2b910_TEST FAIL\n"); return 1; }
    if (g_textureDeletes != 1) { std::printf("00b2b910_TEST FAIL\n"); return 1; }
    g_n = 0; g_log[0] = 0;
    CGraphicFrameData* shared = MakeFrame(2);
    {
        CCompositeSurfaceCache* c = new CCompositeSurfaceCache;
        c->CurrentMorph.Blends.start = 0; c->CachedMorph.Blends.start = 0;
        c->GraphicFrame.FrameData = shared;
        delete c;
    }
    if (shared->ReferenceCount != 1 || g_textureDeletes != 1 || g_log[0] != 'R' || g_log[1] != 'M' || g_log[2] != 'Z' || g_log[3]) { std::printf("00b2b910_TEST FAIL\n"); return 1; }
    std::printf("00b2b910_TEST PASS\n");
    return 0;
}