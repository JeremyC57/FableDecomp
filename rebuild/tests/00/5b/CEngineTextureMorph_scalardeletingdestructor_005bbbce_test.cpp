// Behaviour test for CEngineTextureMorph's scalar deleting destructor @ 0x005BBBCE: deleting the texture morph
// frees its Blends vector buffer (when present) and then the morph's own storage.
#include <cstdio>
#include <cstdlib>
#include <new>

static void* g_freed[4];
static int g_freeCount = 0;
static void FreeStub(void* p) { if (g_freeCount < 4) g_freed[g_freeCount] = p; ++g_freeCount; }
static int g_deleteCount = 0;

template<class T>
class BlendVector { public:
    T* m_first;
    T* m_last;
    T* m_end;
    ~BlendVector() { if (m_first) FreeStub(m_first); }
};

struct CEngineTextureMorph {
    long         OriginalTextureIndex;
    long         MorphID;
    BlendVector<long> Blends;
    static void operator delete(void* p) { ++g_deleteCount; std::free(p); }
};

int main()
{
    static long items[3];
    bool ok = true;
    CEngineTextureMorph* r = new CEngineTextureMorph;
    r->OriginalTextureIndex = 1; r->MorphID = 2;
    r->Blends.m_first = items; r->Blends.m_last = items + 3; r->Blends.m_end = items + 3;
    delete r;
    if (g_freeCount != 1 || g_freed[0] != items || g_deleteCount != 1) ok = false;

    CEngineTextureMorph* e = new CEngineTextureMorph;
    e->Blends.m_first = 0; e->Blends.m_last = 0; e->Blends.m_end = 0;
    delete e;
    if (g_freeCount != 1 || g_deleteCount != 2) ok = false;

    // Destroy in place (the flag-0 path used by _Destroy_range): frees the array, keeps the storage.
    CEngineTextureMorph local;
    local.Blends.m_first = items; local.Blends.m_last = items; local.Blends.m_end = items;
    local.~CEngineTextureMorph();
    if (g_freeCount != 2 || g_deleteCount != 2) ok = false;
    new (&local) CEngineTextureMorph;
    local.Blends.m_first = 0;

    if (!ok) { std::printf("005bbbce_TEST FAIL\n"); return 1; }
    std::printf("005bbbce_TEST PASS\n");
    return 0;
}