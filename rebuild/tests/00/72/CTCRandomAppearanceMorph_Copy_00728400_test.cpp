#include <cstdio>
struct SCountBlock;
static int g_destroyed = 0;
static void Destroy(void* object) { ++g_destroyed; *(int*)object = -1; }
struct SCountBlock {
    long m_references;
    void (*m_pfnDestroy)(void*);
    void* m_pObject;
};
struct CCountedPointer {
    int* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        int* object = other.m_pObject;
        SCountBlock* count = other.m_pCount;
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
static SCountBlock* NewBlock(int* object) { SCountBlock* b = new SCountBlock; b->m_references = 1; b->m_pfnDestroy = Destroy; b->m_pObject = object; return b; }
static int g_updates = 0;
class CTCRandomAppearanceMorph { public:
    CCountedPointer PMeshMorphSet;
    unsigned long   Seed;
    void UpdateMorphSet() { ++g_updates; }
    void Copy(const CTCRandomAppearanceMorph& other)
    {
        PMeshMorphSet = other.PMeshMorphSet;
        Seed = other.Seed;
        UpdateMorphSet();
    }
};
int main()
{
    int objA = 1, objB = 2;
    CTCRandomAppearanceMorph a, b;
    a.PMeshMorphSet.m_pObject = &objA; a.PMeshMorphSet.m_pCount = NewBlock(&objA); a.Seed = 5;
    b.PMeshMorphSet.m_pObject = &objB; b.PMeshMorphSet.m_pCount = NewBlock(&objB); b.Seed = 77;
    SCountBlock* blockB = b.PMeshMorphSet.m_pCount;
    a.Copy(b);
    if (g_destroyed != 1 || objA != -1 || a.PMeshMorphSet.m_pObject != &objB || a.PMeshMorphSet.m_pCount != blockB ||
        blockB->m_references != 2 || a.Seed != 77 || g_updates != 1) { std::printf("00728400_TEST FAIL\n"); return 1; }
    a.Copy(b);                                          // same block: no release, no add-ref
    if (g_destroyed != 1 || blockB->m_references != 2 || g_updates != 2) { std::printf("00728400_TEST FAIL\n"); return 1; }
    std::printf("00728400_TEST PASS\n");
    return 0;
}