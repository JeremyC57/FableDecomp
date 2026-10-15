// Behaviour test for CTCActionUseBuyHouse's scalar deleting destructor @ 0x0072F040: the order is
// RemoveFromGlobalTCs, the buy-house def release, the action-use def release (each Delete only when its
// count reaches zero), ~CTCBase, then the storage is freed.
#include <cstdio>
#include <cstring>

static char g_log[16];
static int g_n = 0;
static void Log(char c) { if (g_n < 15) g_log[g_n++] = c; g_log[g_n] = 0; }

struct CTCBase_Calls {
    bool IsInGlobalUpdateTCs;
    CTCBase_Calls() : IsInGlobalUpdateTCs(true) {}
    virtual ~CTCBase_Calls() { Log('B'); }
    void RemoveFromGlobalTCs() { IsInGlobalUpdateTCs = false; Log('R'); }
};

struct CDefPointeeBase {
    long m_refCount;
    char m_tag;
    CDefPointeeBase(char tag) : m_refCount(1), m_tag(tag) {}
    virtual ~CDefPointeeBase() {}
    virtual void Delete() { Log(m_tag); }
};
static int g_freed = 0;

template<class T>
struct CDefPointer {
    T* m_pDef;
    CDefPointer() : m_pDef(0) {}
    ~CDefPointer()
    {
        T* def = m_pDef;
        if (def != 0) {
            if (--def->m_refCount == 0)
                def->Delete();
            m_pDef = 0;
        }
    }
};

struct CTCActionUseBase : CTCBase_Calls {
    CDefPointer<CDefPointeeBase> PActionUseDef;
};

struct CTCActionUseBuyHouse : CTCActionUseBase {
    CDefPointer<CDefPointeeBase> PDef;
    bool WaitingForConfirmation;
    long HouseQuestion;
    virtual ~CTCActionUseBuyHouse() { RemoveFromGlobalTCs(); }
    static void operator delete(void* p) { ++g_freed; ::operator delete(p); }
};

int main()
{
    bool ok = true;
    CDefPointeeBase useDef('U'), houseDef('H');
    useDef.m_refCount = 1;
    houseDef.m_refCount = 2;
    CTCActionUseBuyHouse* house = new CTCActionUseBuyHouse;
    house->PActionUseDef.m_pDef = &useDef;
    house->PDef.m_pDef = &houseDef;
    CTCBase_Calls* base = house;
    delete base;
    if (std::strcmp(g_log, "RUB") != 0 || useDef.m_refCount != 0 || houseDef.m_refCount != 1 || g_freed != 1) ok = false;

    g_n = 0; g_log[0] = 0;
    house = new CTCActionUseBuyHouse;
    house->PActionUseDef.m_pDef = 0;
    house->PDef.m_pDef = &houseDef;
    delete house;
    if (std::strcmp(g_log, "RHB") != 0 || houseDef.m_refCount != 0 || g_freed != 2) ok = false;

    if (!ok) { std::printf("0072f040_TEST FAIL\n"); return 1; }
    std::printf("0072f040_TEST PASS\n");
    return 0;
}
