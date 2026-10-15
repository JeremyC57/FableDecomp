#include <cstdio>
// Behaviour model of the deleting destructor 006e7bd0: the counted PImp is released (destroy + free the block at zero, both
// fields cleared), the root destructor runs, and the object is freed only when bit 0 of the flag is set.
static int g_destroyed = 0, g_rootDtors = 0, g_freed = 0;
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
static void DestroyImp(void*) { ++g_destroyed; }
struct CCountedPointer {
    void* m_pObject; SCountBlock* m_pCount;
    ~CCountedPointer()
    {
        if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; ++g_freed; } }
        m_pObject = 0; m_pCount = 0;
    }
};
struct CRoot { virtual ~CRoot() { ++g_rootDtors; } };
struct CScriptGameResourceObjectBase : CRoot { virtual ~CScriptGameResourceObjectBase() {} };
struct CScriptGameResourceObjectMovieBase : CScriptGameResourceObjectBase { CCountedPointer PImp; };
int main()
{
    int imp = 0;
    SCountBlock* shared = new SCountBlock; shared->m_references = 2; shared->m_pfnDestroy = DestroyImp; shared->m_pObject = &imp;
    CScriptGameResourceObjectMovieBase* a = new CScriptGameResourceObjectMovieBase; a->PImp.m_pObject = &imp; a->PImp.m_pCount = shared;
    delete a;                                  // count 2 -> 1: nothing destroyed
    if (g_destroyed != 0 || g_freed != 0 || g_rootDtors != 1 || shared->m_references != 1) { std::printf("006e7bd0_TEST FAIL\n"); return 1; }
    CScriptGameResourceObjectMovieBase* b = new CScriptGameResourceObjectMovieBase; b->PImp.m_pObject = &imp; b->PImp.m_pCount = shared;
    CScriptGameResourceObjectBase* base = b; delete base;   // through the virtual destructor: count 1 -> 0
    if (g_destroyed != 1 || g_freed != 1 || g_rootDtors != 2) { std::printf("006e7bd0_TEST FAIL\n"); return 1; }
    CScriptGameResourceObjectMovieBase c; c.PImp.m_pObject = 0; c.PImp.m_pCount = 0;   // empty PImp
    std::printf("006e7bd0_TEST PASS\n");
    return 0;
}
