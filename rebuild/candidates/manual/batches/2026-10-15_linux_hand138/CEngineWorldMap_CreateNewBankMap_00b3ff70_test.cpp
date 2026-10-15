// Behaviour test for 0x00b3ff70 CEngineWorldMap::CreateNewBankMap
#include <cstdio>
#include <cstring>
static int g_deleted = 0;
struct CBankStateBlock { virtual ~CBankStateBlock() {} };
struct CStaticMapBankStateBlock : CBankStateBlock { long State; virtual ~CStaticMapBankStateBlock() { ++g_deleted; } };
static void DeleteBlock(void* b) { delete static_cast<CBankStateBlock*>(b); }
struct SCountBlock {
    long m_references; void (*m_pfnDestroy)(void*); void* m_pObject;
    SCountBlock(void* o, void (*d)(void*)) : m_references(1), m_pfnDestroy(d), m_pObject(o) {}
};
template <class T> struct CCountedPointer {
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* o, void (*d)(void*)) : m_pObject(o) { if (o) m_pCount = new SCountBlock(o, d); else m_pCount = 0; }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer() { if (m_pCount) { if (--m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } }
};
struct CStaticMapBankFile {
    const char* m_names[4]; int m_count; long m_refsSeen;
    void AddEntry(const char* symbol, unsigned long type, const char* filename, CCountedPointer<CBankStateBlock> state)
    {
        if (type == 0 && std::strcmp(symbol, filename) == 0 && state.m_pObject) m_names[m_count++] = symbol;
        m_refsSeen = state.m_pCount->m_references;
    }
    unsigned long FindEntry(const char* symbol) { for (int i = 0; i < m_count; ++i) if (std::strcmp(m_names[i], symbol) == 0) return i + 100; return 0; }
};
struct CEngineWorldMap {
    CStaticMapBankFile StaticMapFile;
    unsigned long CreateNewBankMap(const char* name)
    {
        CCountedPointer<CBankStateBlock> state(new CStaticMapBankStateBlock, DeleteBlock);
        StaticMapFile.AddEntry(name, 0, name, state);
        return StaticMapFile.FindEntry(name);
    }
};
int main()
{
    CEngineWorldMap w; w.StaticMapFile.m_count = 0;
    unsigned long a = w.CreateNewBankMap("MAP_A");
    unsigned long b = w.CreateNewBankMap("MAP_B");
    bool ok = a == 100 && b == 101 && w.StaticMapFile.m_refsSeen == 2 && g_deleted == 2;   // the local block dies with the function
    if (!ok) { std::printf("00b3ff70_TEST FAIL\n"); return 1; }
    std::printf("00b3ff70_TEST PASS\n");
    return 0;
}
