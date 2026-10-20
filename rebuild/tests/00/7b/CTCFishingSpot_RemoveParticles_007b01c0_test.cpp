// Behaviour test for CTCFishingSpot::RemoveParticles (retail 0x007B01C0): model copy + stub callees.
#include <cstdio>

struct CBaseObject;
static int g_killCalls = 0;

class CThing {
public:
    unsigned char Flags91;
    bool killed;
    bool killedImmediately;
    void Kill(bool immediately) { killed = true; killedImmediately = immediately; ++g_killCalls; }
};

struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return (CThing*)m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->Flags91 & 1))
            return true;
    }
    return false;
}

struct CTCFishingSpot {
    CIntelligentPointer<CThing> PParticle1;
    CIntelligentPointer<CThing> PParticle2;
    CIntelligentPointer<CThing> PParticleFish;
    bool                        Enabled;
    CTCFishingSpot() : PParticle1(0), PParticle2(0), PParticleFish(0), Enabled(true) {}
    static void RemoveParticle(CIntelligentPointer<CThing>& particle)
    {
        if (IS_THING_ALIVE(particle)) {
            particle.GetPItem()->Kill(true);
            particle = CIntelligentPointer<CThing>(0);
        }
    }
    void RemoveParticles();
};

void CTCFishingSpot::RemoveParticles()
{
    RemoveParticle(PParticle1);
    RemoveParticle(PParticle2);
    RemoveParticle(PParticleFish);
    Enabled = false;
}

int main()
{
    int fail = 0;
    CThing a = { 0, false, false }, dead = { 1, false, false }, fish = { 0, false, false };
    CTCFishingSpot spot;
    spot.PParticle1.SetPItem((CBaseObject*)&a);
    spot.PParticle2.SetPItem((CBaseObject*)&dead);
    spot.PParticleFish.SetPItem((CBaseObject*)&fish);
    spot.RemoveParticles();
    if (!a.killed || !a.killedImmediately || spot.PParticle1.GetPItem() != 0) fail = 1;
    if (dead.killed || spot.PParticle2.GetPItem() != &dead) fail = 2;           // dead thing: left alone
    if (!fish.killed || spot.PParticleFish.GetPItem() != 0) fail = 3;
    if (spot.Enabled || g_killCalls != 2) fail = 4;
    // empty pointers: nothing to kill, still disabled
    spot.Enabled = true;
    spot.RemoveParticles();
    if (spot.Enabled || g_killCalls != 2) fail = 5;
    if (fail) { std::printf("007b01c0_TEST FAIL\n"); return 1; }
    std::printf("007b01c0_TEST PASS\n");
    return 0;
}