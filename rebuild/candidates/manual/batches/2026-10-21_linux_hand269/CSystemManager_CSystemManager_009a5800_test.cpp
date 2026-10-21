// Behaviour test for 0x009A5800 CSystemManager::~CSystemManager: every counted pointer drops its reference
// (destroying the object on the last one) and is cleared; the strings and the base are destroyed.
#include <cstdio>

static int g_destroyed = 0, g_strings = 0, g_base = 0;

struct SCountBlock {
    long  m_references;
    void (*m_pfnDestroy)(void*);
    void* m_pObject;
};
static void DestroyObj(void* p) { ++g_destroyed; (void)p; }

template<class T> struct CCountedPointer {
    T*           m_pObject;
    SCountBlock* m_pCount;
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct CCharString { void* PStringData; ~CCharString() { ++g_strings; } };
struct CWideString { void* PStringData; ~CWideString() { ++g_strings; } };
struct CInitBaseClass { bool m_initialised; CInitBaseClass() : m_initialised(true) {} virtual ~CInitBaseClass() { ++g_base; } };
struct X;
struct CSystemManager : CInitBaseClass {
    CCountedPointer<X> LibDebugManager, PInputManager, PDisplayManager;
    X* DebugManager;
    CCountedPointer<X> PGraphicBankManager, PMeshBankManager, PFontManager, PExtraManager;
    CWideString WinAppName;
    CCountedPointer<X> ProfileManager;
    CWideString TempPath;
    CCharString ApplicationName;
    virtual ~CSystemManager() {}
};

static SCountBlock* Block(long refs)
{
    SCountBlock* b = new SCountBlock;
    b->m_references = refs; b->m_pfnDestroy = DestroyObj; b->m_pObject = 0;
    return b;
}

int main()
{
    SCountBlock* shared = Block(2);
    {
        CSystemManager* m = new CSystemManager;
        m->LibDebugManager.m_pCount = Block(1);
        m->PInputManager.m_pCount = 0;
        m->PDisplayManager.m_pCount = shared;
        m->PGraphicBankManager.m_pCount = Block(1);
        m->PMeshBankManager.m_pCount = 0;
        m->PFontManager.m_pCount = 0;
        m->PExtraManager.m_pCount = 0;
        m->ProfileManager.m_pCount = Block(1);
        CInitBaseClass* b = m;
        delete b;
    }
    if (g_destroyed != 3 || g_strings != 3 || g_base != 1 || shared->m_references != 1) {
        std::printf("009a5800_TEST FAIL\n");
        return 1;
    }
    delete shared;
    std::printf("009a5800_TEST PASS\n");
    return 0;
}
