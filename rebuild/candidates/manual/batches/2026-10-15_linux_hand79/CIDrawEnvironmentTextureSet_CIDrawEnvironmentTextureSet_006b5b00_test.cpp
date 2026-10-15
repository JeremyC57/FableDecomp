// Behaviour test for CIDrawEnvironmentTextureSet::~CIDrawEnvironmentTextureSet @ 0x006b5b00: the second array's buffer is freed before the first's,
// null buffers are skipped.
#include <cstdio>

static void* g_freed[4];
static int g_freeCount = 0;
static void FreeStub(void* p) { if (g_freeCount < 4) g_freed[g_freeCount] = p; ++g_freeCount; }

template<class T>
struct CArray {
    T* m_first;
    T* m_last;
    T* m_end;
    ~CArray() { if (m_first) FreeStub(m_first); }
};

class CIDrawEnvironmentTextureSet {
public:
    ~CIDrawEnvironmentTextureSet() {}
    CArray<long> Textures;
    CArray<float> Blends;
};

int main()
{
    static long a[4];
    static float b[4];
    bool ok = true;
    {
        CIDrawEnvironmentTextureSet r;
        r.Textures.m_first = a; r.Textures.m_last = a + 4; r.Textures.m_end = a + 4;
        r.Blends.m_first = b; r.Blends.m_last = b + 4; r.Blends.m_end = b + 4;
    }
    if (g_freeCount != 2 || g_freed[0] != (void*)b || g_freed[1] != (void*)a) ok = false;
    {
        CIDrawEnvironmentTextureSet r;
        r.Textures.m_first = 0; r.Textures.m_last = 0; r.Textures.m_end = 0;
        r.Blends.m_first = b; r.Blends.m_last = b; r.Blends.m_end = b + 4;
    }
    if (g_freeCount != 3 || g_freed[2] != (void*)b) ok = false;
    if (!ok) { std::printf("006b5b00_TEST FAIL\n"); return 1; }
    std::printf("006b5b00_TEST PASS\n");
    return 0;
}
