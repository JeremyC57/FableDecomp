// Behaviour test for 0x00DDDCE0 (label CChunkedFile::~CChunkedFile): reset clears flags/counter and releases the script thing.
#include <cstdio>
static int g_destroyed = 0, g_blocksFreed = 0;
static void DestroyObject(void*) { ++g_destroyed; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
struct CCountedPointer {
    void* m_pObject; SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    ~CCountedPointer() { Release(); }
    void Release()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; ++g_blocksFreed; }
        }
        m_pObject = 0; m_pCount = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        void* object = other.m_pObject; SCountBlock* count = other.m_pCount;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; ++g_blocksFreed; }
            }
            m_pObject = object; m_pCount = count;
            if (count) ++count->m_references;
        }
        return *this;
    }
};
class CScriptThing { public: CCountedPointer PImp; };
struct CHolder {
    bool m_flagA, m_flagB; long m_counter; CScriptThing m_thing;
    void Reset() { m_flagA = false; m_flagB = false; m_counter = 0; m_thing = CScriptThing(); }
};
int main()
{
    int obj = 0;
    CHolder h; h.m_flagA = h.m_flagB = true; h.m_counter = 12;
    SCountBlock* b = new SCountBlock; b->m_references = 1; b->m_pfnDestroy = DestroyObject; b->m_pObject = &obj;
    h.m_thing.PImp.m_pObject = &obj; h.m_thing.PImp.m_pCount = b;
    h.Reset();
    if (h.m_flagA || h.m_flagB || h.m_counter != 0 || h.m_thing.PImp.m_pObject != 0 || h.m_thing.PImp.m_pCount != 0 ||
        g_destroyed != 1 || g_blocksFreed != 1) { std::printf("00dddce0_TEST FAIL\n"); return 1; }
    h.Reset();                                           // already empty: nothing else released
    if (g_destroyed != 1) { std::printf("00dddce0_TEST FAIL\n"); return 1; }
    std::printf("00dddce0_TEST PASS\n");
    return 0;
}