// Behaviour test for 0x007A4350 CTCWillEnflame::SetPTarget: a temporary intelligent pointer is pointed at the
// target, read back into PTarget, destroyed before IsZTargetting is stored.
#include <cstdio>

struct CThing { long m_id; };
struct CBaseObject;

static int g_ctor, g_dtor;
static bool g_flagAtDtor;
static bool* g_pFlag;

struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctor; }
    ~CBaseIntelligentPointer() { ++g_dtor; if (g_pFlag) g_flagAtDtor = *g_pFlag; }
    CBaseObject* GetPItem() { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer() : CBaseIntelligentPointer() { } };

struct CTCWillEnflame {
    bool                    IsZTargetting;
    CBaseIntelligentPointer PTarget;
    void SetPTarget(CThing* target, bool isZTargetting);
};

void CTCWillEnflame::SetPTarget(CThing* target, bool isZTargetting)
{
    {
        CIntelligentPointer pointer;
        pointer.SetPItem((CBaseObject*)target);
        PTarget.SetPItem(pointer.GetPItem());
    }
    IsZTargetting = isZTargetting;
}

int main()
{
    CThing thing;
    CTCWillEnflame* c = new CTCWillEnflame;
    c->IsZTargetting = false;
    g_pFlag = &c->IsZTargetting;
    g_ctor = g_dtor = 0;
    c->SetPTarget(&thing, true);
    if (c->PTarget.GetPItem() != (CBaseObject*)&thing || !c->IsZTargetting || g_ctor != 1 || g_dtor != 1 || g_flagAtDtor) { std::printf("007a4350_TEST FAIL\n"); return 1; }
    g_ctor = g_dtor = 0;
    c->SetPTarget(0, false);
    if (c->PTarget.GetPItem() != 0 || c->IsZTargetting || g_ctor != 1 || g_dtor != 1 || !g_flagAtDtor) { std::printf("007a4350_TEST FAIL\n"); return 1; }
    g_pFlag = 0;
    delete c;
    std::printf("007a4350_TEST PASS\n");
    return 0;
}
