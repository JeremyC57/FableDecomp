// Behaviour test for CTCWillLightning::~CTCWillLightning (0x007A07F0): the beam list is cleared first, each live hand
// effect (right then left) with a particle-emitter interface is sent to its timeline end, dead/absent effects are
// skipped, then the members are destroyed in reverse order (list storage and vector storage freed, def released) and
// the CTCBase destructor runs last.
#include <cstdio>

static int g_log[64];
static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
static void TestFree(void* p) { Log(p ? 60 : 61); }

class CTCBase;
struct CTCDParticleEmitter {
    int id;
    void EnterTimelineEnd() { Log(id); }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CTCDParticleEmitter* emitter;
    bool m_bDead : 1;
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> T* GetTC(long) { return (T*)emitter; }
};
struct CBaseIntelligentPointer {
    int id;
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    ~CBaseIntelligentPointer() { Log(id); }
};
struct CBeamNode { int dummy; };
struct CBeamList {
    CBeamNode* _M_node;
    void clear() { Log(50); }
    ~CBeamList() { clear(); if (_M_node) TestFree(_M_node); }
};
struct CLongVector {
    long* _M_start;
    ~CLongVector() { if (_M_start) TestFree(_M_start); }
};
struct CWillButtonHelperTimer {
    CBaseIntelligentPointer m_pThing;
    CLongVector m_values;
};
struct CDefPointee {
    long RefCount;
    virtual void Destroy() { Log(70); }
    virtual ~CDefPointee() {}
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CLightningSpellDefPointer {
    CDefPointee* Object;
    ~CLightningSpellDefPointer() { if (Object) { Object->Release(); Object = 0; } }
};
class CTCBase {
public:
    virtual ~CTCBase() { Log(99); }
};
struct CTCWillLightning : CTCBase {
    CBaseIntelligentPointer PItemRemoved;
    CLightningSpellDefPointer m_lspDef;
    CWillButtonHelperTimer m_tTimer;
    CBeamList m_lbBeams;
    CBaseIntelligentPointer m_cbpPlayer;
    CBaseIntelligentPointer m_tpLeftHandEffect;
    CBaseIntelligentPointer m_tpRightHandEffect;
    CBaseIntelligentPointer PTargetOverride;
    virtual ~CTCWillLightning();
};

CTCWillLightning::~CTCWillLightning()
{
    m_lbBeams.clear();
    if (m_tpRightHandEffect.GetPItem() && !m_tpRightHandEffect.GetPItem()->m_bDead) {
        CTCDParticleEmitter* emitter = 0;
        CThing* effect = m_tpRightHandEffect.GetPItem();
        if (effect->HasTC(0x4b))
            emitter = effect->GetTC<CTCDParticleEmitter>(0x4b);
        emitter->EnterTimelineEnd();
    }
    if (m_tpLeftHandEffect.GetPItem() && !m_tpLeftHandEffect.GetPItem()->m_bDead) {
        CTCDParticleEmitter* emitter = 0;
        CThing* effect = m_tpLeftHandEffect.GetPItem();
        if (effect->HasTC(0x4b))
            emitter = effect->GetTC<CTCDParticleEmitter>(0x4b);
        emitter->EnterTimelineEnd();
    }
}

static bool Run(bool rightAlive, bool leftAlive, const int* expect, int count)
{
    g_n = 0;
    CTCDParticleEmitter right = { 21 }, left = { 22 };
    CThing rt, leftThing;
    for (int i = 0; i < 9; ++i) { rt.m_presentBits[i] = 0; leftThing.m_presentBits[i] = 0; }
    rt.m_presentBits[2] = 1UL << 11; leftThing.m_presentBits[2] = 1UL << 11;   // interface 0x4b
    rt.emitter = &right; leftThing.emitter = &left;
    rt.m_bDead = !rightAlive; leftThing.m_bDead = !leftAlive;
    CDefPointee* def = new CDefPointee;
    def->RefCount = 1;
    static CBeamNode head;
    static long storage[2];
    CTCWillLightning* w = new CTCWillLightning;
    w->PItemRemoved.id = 1; w->PItemRemoved.m_pItem = 0;
    w->m_lspDef.Object = def;
    w->m_tTimer.m_pThing.id = 2; w->m_tTimer.m_values._M_start = storage;
    w->m_lbBeams._M_node = &head;
    w->m_cbpPlayer.id = 3; w->m_cbpPlayer.m_pItem = 0;
    w->m_tpLeftHandEffect.id = 4; w->m_tpLeftHandEffect.m_pItem = &leftThing;
    w->m_tpRightHandEffect.id = 5; w->m_tpRightHandEffect.m_pItem = &rt;
    w->PTargetOverride.id = 6; w->PTargetOverride.m_pItem = 0;
    CTCBase* base = w;
    delete base;
    delete def;
    if (g_n != count) return false;
    for (int i = 0; i < count; ++i)
        if (g_log[i] != expect[i]) return false;
    return true;
}

int main()
{
    static const int both[] = { 50, 21, 22, 6, 5, 4, 3, 50, 60, 60, 2, 70, 1, 99 };
    static const int leftOnly[] = { 50, 22, 6, 5, 4, 3, 50, 60, 60, 2, 70, 1, 99 };
    int ok = Run(true, true, both, 14) && Run(false, true, leftOnly, 13);
    if (!ok) { std::printf("007a07f0_TEST FAIL\n"); return 1; }
    std::printf("007a07f0_TEST PASS\n");
    return 0;
}
