// Behaviour test for CEngineLandscapeMemoryManager::AllocateTexture @ 0x00BDA470.
#include <cstdio>
#include <cstdlib>

enum ESurfacePool { SURFACE_POOL_DEFAULT = 0, SURFACE_POOL_MANAGED = 1 };
struct C2DExtentsI { long w, h; };
struct CPixelFormat { long f; };

static int g_flushes = 0, g_creates = 0, g_locks = 0, g_unlocks = 0, g_fail = 0;
static unsigned char g_block[64];

class CTexture;
extern unsigned long g_texSize;
struct CMemoryAllocator {
    void* InternalAllocate(long size, unsigned long, bool) { if (g_fail) return 0; return (unsigned long)size == g_texSize ? g_block : 0; }
};
static CMemoryAllocator g_alloc;
static CMemoryAllocator* g_pGlobalAllocator = 0;
static void GFCreateGlobalAllocator() { ++g_creates; g_pGlobalAllocator = &g_alloc; }
static void GFEnterAllocatorLock() { ++g_locks; }
static void GFLeaveAllocatorLock() { ++g_unlocks; }

static const C2DExtentsI* s_size; static long s_levels; static const CPixelFormat* s_fmt;
static unsigned long s_usage; static int s_pool; static bool s_dyn, s_rt;

class CTexture {
public:
    void* PD3DTexture;
    unsigned int ByteLengthAndSource;
    CTexture() : PD3DTexture(0), ByteLengthAndSource(0) { }
    static void* operator new(size_t size) throw()
    {
        if (!g_pGlobalAllocator) GFCreateGlobalAllocator();
        GFEnterAllocatorLock();
        void* block = g_pGlobalAllocator->InternalAllocate((long)size, 0, false);
        GFLeaveAllocatorLock();
        return block;
    }
    static void operator delete(void*) { }
    bool Initialise(const C2DExtentsI& size, long levels, const CPixelFormat& format, unsigned long usage,
                    ESurfacePool pool, bool dynamic, bool renderTarget)
    { s_size = &size; s_levels = levels; s_fmt = &format; s_usage = usage; s_pool = pool; s_dyn = dynamic; s_rt = renderTarget; return true; }
};

struct CEngineLandscapeMemoryManager {
    void FlushCycle() { ++g_flushes; }
    CTexture* AllocateTexture(unsigned long, const C2DExtentsI& size, long levels, const CPixelFormat& format,
                              unsigned long usage, bool dynamic)
    {
        FlushCycle();
        CTexture* texture = new CTexture;
        if (!texture) return 0;
        texture->Initialise(size, levels, format, usage, (ESurfacePool)(~usage & 1), dynamic, false);
        return texture;
    }
};

unsigned long g_texSize = sizeof(CTexture);
int main()
{
    CEngineLandscapeMemoryManager m; C2DExtentsI e = { 4, 4 }; CPixelFormat f = { 7 };
    for (int i = 0; i < 64; ++i) g_block[i] = 0xAB;
    CTexture* t = m.AllocateTexture(1, e, 3, f, 0, true);
    int ok = t == (CTexture*)g_block && t->PD3DTexture == 0 && t->ByteLengthAndSource == 0 && g_creates == 1 && g_flushes == 1
          && g_locks == 1 && g_unlocks == 1 && s_size == &e && s_levels == 3 && s_fmt == &f && s_pool == SURFACE_POOL_MANAGED
          && s_dyn && !s_rt && s_usage == 0;
    t = m.AllocateTexture(1, e, 1, f, 1, false);
    ok = ok && t && s_pool == SURFACE_POOL_DEFAULT && !s_dyn && g_creates == 1 && g_flushes == 2;
    g_fail = 1;
    s_levels = 99;
    t = m.AllocateTexture(1, e, 1, f, 1, false);
    ok = ok && t == 0 && s_levels == 99 && g_flushes == 3 && g_unlocks == 3;
    if (!ok) { std::printf("00bda470_TEST FAIL\n"); return 1; }
    std::printf("00bda470_TEST PASS\n");
    return 0;
}
