// Behaviour test for CTCSmashableBase::OnInitialActivate (0x007EECF0): persist the thing when its def is smashable;
// the temporary def pointer adds and drops one reference.
#include <cstdio>

static int g_persistCalls = 0;
static bool g_persistArg = false;
static int g_destroyCalls = 0;

class CThing { public:
    void SetAsGameStatePersisted_004c9c10(bool persisted) { ++g_persistCalls; g_persistArg = persisted; }
};

class CSmashableDef { public:
    long m_refCount;
    bool Smashable;
    void Destroy() { ++g_destroyCalls; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};

struct CDefPointer {
    CSmashableDef* m_pDef;
    CDefPointer(CSmashableDef* def) : m_pDef(def) { }
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) ++m_pDef->m_refCount; }
    ~CDefPointer() { m_pDef->Release(); }
    CSmashableDef* operator->() const { return m_pDef; }
};

class CTCSmashableBase { public:
    CThing*     ParentThing;
    long        LastWorldFrameHit;
    bool        Smashed;
    CDefPointer Definition;
    CTCSmashableBase(CThing* thing, CSmashableDef* def) : ParentThing(thing), Definition(def) { }
    CDefPointer GetDef() const { return Definition; }
    void OnInitialActivate();
};

void CTCSmashableBase::OnInitialActivate()
{
    bool smashable = GetDef()->Smashable;
    if (smashable)
        ParentThing->SetAsGameStatePersisted_004c9c10(true);
}

int main()
{
    CThing thing;
    CSmashableDef def; def.m_refCount = 2; def.Smashable = true;
    {
        CTCSmashableBase s(&thing, &def);
        s.OnInitialActivate();
        if (g_persistCalls != 1 || !g_persistArg || def.m_refCount != 2 || g_destroyCalls != 0) { std::printf("007eecf0_TEST FAIL\n"); return 1; }
        def.Smashable = false;
        s.OnInitialActivate();
        if (g_persistCalls != 1 || def.m_refCount != 2) { std::printf("007eecf0_TEST FAIL\n"); return 1; }
        def.m_refCount = 1;
    }
    if (g_destroyCalls != 1) { std::printf("007eecf0_TEST FAIL\n"); return 1; }
    std::printf("007eecf0_TEST PASS\n");
    return 0;
}