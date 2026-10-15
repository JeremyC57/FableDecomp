// Behaviour test for CTCActionUseBed::~CTCActionUseBed @ 0x007EBDD0: the order is RemoveFromGlobalTCs,
// PThingUser, PBuilding, the action-use def release (Delete only when the count reaches zero), ~CTCBase.
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
    CDefPointeeBase() : m_refCount(1) {}
    virtual ~CDefPointeeBase() {}
    virtual void Delete() { Log('D'); }
};

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

struct CBaseIntelligentPointer {
    char m_tag;
    CBaseIntelligentPointer() : m_tag('?') {}
    virtual ~CBaseIntelligentPointer() { Log(m_tag); }
};

struct CTCActionUseBed : CTCActionUseBase {
    CBaseIntelligentPointer PBuilding;
    CBaseIntelligentPointer PThingUser;
    virtual ~CTCActionUseBed() { RemoveFromGlobalTCs(); }
};

int main()
{
    bool ok = true;
    CDefPointeeBase def;
    def.m_refCount = 2;
    {
        CTCActionUseBed bed;
        bed.PActionUseDef.m_pDef = &def;
        bed.PBuilding.m_tag = 'b';
        bed.PThingUser.m_tag = 'u';
    }
    if (std::strcmp(g_log, "RubB") != 0 || def.m_refCount != 1) ok = false;   // shared def: no Delete

    g_n = 0; g_log[0] = 0;
    {
        CTCActionUseBed* bed = new CTCActionUseBed;
        bed->PActionUseDef.m_pDef = &def;
        bed->PBuilding.m_tag = 'b';
        bed->PThingUser.m_tag = 'u';
        CTCBase_Calls* base = bed;
        delete base;                                 // virtual: reaches the bed destructor
    }
    if (std::strcmp(g_log, "RubDB") != 0 || def.m_refCount != 0) ok = false;  // last reference: Delete

    g_n = 0; g_log[0] = 0;
    {
        CTCActionUseBed bed;                         // no def at all
        bed.PBuilding.m_tag = 'b';
        bed.PThingUser.m_tag = 'u';
    }
    if (std::strcmp(g_log, "RubB") != 0) ok = false;

    if (!ok) { std::printf("007ebdd0_TEST FAIL\n"); return 1; }
    std::printf("007ebdd0_TEST PASS\n");
    return 0;
}