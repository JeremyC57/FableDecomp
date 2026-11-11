#include <cstdio>

#ifdef _MSC_VER
typedef unsigned __int64 U64;
#else
typedef unsigned long long U64;
#endif

struct SCountBlock {
    long m_references;
    void (*m_pfnDestroy)(void*);
    void* m_pObject;
};

static int g_destroyed = 0;
static void DestroyObject(void* p) { ++g_destroyed; *(int*)p = -1; }

struct CCountedPointer {
    void*        m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount;
        void* object = other.m_pObject;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};

static int g_objects[8];
static SCountBlock* NewBlock(int i)
{
    SCountBlock* b = new SCountBlock;
    b->m_references = 1; b->m_pfnDestroy = DestroyObject; b->m_pObject = &g_objects[i]; g_objects[i] = i;
    return b;
}
static void Set(CCountedPointer& p, SCountBlock* b) { p.m_pCount = b; p.m_pObject = b ? b->m_pObject : 0; }

struct SElem { long first; CCountedPointer second; };
struct STag { STag() { } };

static SElem* CopyBackward(SElem* first, SElem* last, SElem* result, STag, long*)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static void Key(SElem& e, long k) { e.first = k; }
static bool HasKey(const SElem& e, long k) { return e.first == k; }

int main()
{
    STag tag;
    SElem src[3], dst[3];
    SCountBlock* sb0 = NewBlock(0); SCountBlock* sb1 = NewBlock(1);
    SCountBlock* db0 = NewBlock(2); SCountBlock* db2 = NewBlock(3);
    // src: block0, block1, null ; dst: own block2, shares block1, own block3
    Set(src[0].second, sb0); Set(src[1].second, sb1); Set(src[2].second, 0);
    Set(dst[0].second, db0); Set(dst[1].second, sb1); ++sb1->m_references; Set(dst[2].second, db2);
    for (int i = 0; i < 3; ++i) { Key(src[i], 10 + i); Key(dst[i], 90 + i); }
    SElem* r = CopyBackward(src, src + 3, dst + 3, tag, 0);
    bool ok = r == dst;
    int o = 0;
    for (int i = 0; i < 3; ++i) if (!HasKey(dst[i], 10 + i)) ok = false;
    if (dst[0].second.m_pCount != sb0 || dst[0].second.m_pObject != &g_objects[0] || sb0->m_references != 2) ok = false;
    if (dst[1].second.m_pCount != sb1 || sb1->m_references != 2) ok = false;
    if (dst[2].second.m_pCount != 0 || dst[2].second.m_pObject != 0) ok = false;
    if (g_destroyed != 2 || g_objects[2] != -1 || g_objects[3] != -1 || g_objects[0] != 0) ok = false;
    if (CopyBackward(src, src, dst + 1, tag, 0) != dst + 1) ok = false;
    (void)o;
    if (!ok) { std::printf("006b9100_TEST FAIL\n"); return 1; }
    std::printf("006b9100_TEST PASS\n");
    return 0;
}