// Behaviour test for CDualArrayRecord::~CDualArrayRecord @ 0x007f3de0: the second array's buffer is freed before the first's,
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

class CDualArrayRecord {
public:
    ~CDualArrayRecord() {}
    CArray<long> m_first;
    CArray<long> m_second;
};

int main()
{
    static long a[4];
    static long b[4];
    bool ok = true;
    {
        CDualArrayRecord r;
        r.m_first.m_first = a; r.m_first.m_last = a + 4; r.m_first.m_end = a + 4;
        r.m_second.m_first = b; r.m_second.m_last = b + 4; r.m_second.m_end = b + 4;
    }
    if (g_freeCount != 2 || g_freed[0] != (void*)b || g_freed[1] != (void*)a) ok = false;
    {
        CDualArrayRecord r;
        r.m_first.m_first = 0; r.m_first.m_last = 0; r.m_first.m_end = 0;
        r.m_second.m_first = b; r.m_second.m_last = b; r.m_second.m_end = b + 4;
    }
    if (g_freeCount != 3 || g_freed[2] != (void*)b) ok = false;
    if (!ok) { std::printf("007f3de0_TEST FAIL\n"); return 1; }
    std::printf("007f3de0_TEST PASS\n");
    return 0;
}