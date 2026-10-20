// behaviour test for 007b5ba0 CTCWaterWader::~CTCWaterWader: a live particle is killed and its pointer cleared, a dead or
// missing one is left alone; the base destructor runs
#include <cstdio>
static int g_kills = 0, g_baseDtor = 0;
class CThing {
public:
    unsigned char m_flags91;
    bool killed;
    void Kill(bool immediately) { if (immediately) { killed = true; ++g_kills; } }
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* PItem;
    CBaseIntelligentPointer() : PItem(0) {}
    ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* item) { PItem = item; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCBase {
    virtual ~CTCBase() { ++g_baseDtor; }
    CThing* ParentThing;
};
static CThing* g_seen[2];
struct CTCWaterWader : CTCBase {
    CIntelligentPointerThing PParticle;
    CIntelligentPointerThing PWaterfallParticle;
    bool                     OldWade;
    CTCWaterWader(CThing* a, CThing* b) : PParticle(a), PWaterfallParticle(b) {}
    virtual ~CTCWaterWader();
};
CTCWaterWader::~CTCWaterWader()
{
    if (IS_THING_ALIVE(PParticle)) {
        PParticle.GetPItem()->Kill(true);
        CIntelligentPointerThing cleared(0);
        CThing* item = cleared.GetPItem();
        PParticle.SetPItem(item);
    }
    if (IS_THING_ALIVE(PWaterfallParticle)) {
        PWaterfallParticle.GetPItem()->Kill(true);
        CIntelligentPointerThing cleared(0);
        CThing* item = cleared.GetPItem();
        PWaterfallParticle.SetPItem(item);
    }
    g_seen[0] = PParticle.GetPItem(); g_seen[1] = PWaterfallParticle.GetPItem();   // test probe (not in the retail body)
}
int main()
{
    CThing live = { 0, false }, dead = { 1, false };
    { CTCWaterWader wader(&live, &dead); }
    bool ok = g_kills == 1 && live.killed && !dead.killed && g_seen[0] == 0 && g_seen[1] == &dead && g_baseDtor == 1;
    { CTCWaterWader wader(0, &live); }
    ok = ok && g_kills == 2 && g_seen[1] == 0 && g_baseDtor == 2;
    if (!ok) { std::printf("007b5ba0_TEST FAIL\n"); return 1; }
    std::printf("007b5ba0_TEST PASS\n");
    return 0;
}
