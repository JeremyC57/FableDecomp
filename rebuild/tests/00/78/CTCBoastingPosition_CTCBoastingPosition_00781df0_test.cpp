// Behaviour test for 00781df0 CTCBoastingPosition::~CTCBoastingPosition: a live particle effect is killed (immediately), a dead or missing one is left alone.
#include <cstdio>

static int g_kills = 0;
static bool g_lastImmediate = false;

class CThing
{ public:
    unsigned char Dead : 1;
    void Kill(bool immediately) { ++g_kills; g_lastImmediate = immediately; }
};

struct CBaseIntelligentPointer
{
    virtual void v00() {}
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return m_pItem; }
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer
{ public:
    T* Get() const { return (T*)GetPItem(); }
};

class CTCBase
{ public:
    virtual void v00() {}
    CThing* ParentThing;
    CTCBase() : ParentThing(0) {}
    ~CTCBase() {}
};

class CTCBoastingPosition : public CTCBase
{ public:
    CIntelligentPointer<CThing> PBoastingPodium;
    CIntelligentPointer<CThing> PParticleEffect;
    ~CTCBoastingPosition()
    {
        if (PParticleEffect.Get() && !PParticleEffect.Get()->Dead)
            PParticleEffect.Get()->Kill(true);
    }
};

int main()
{
    CThing alive; alive.Dead = 0;
    CThing dead;  dead.Dead = 1;
    { CTCBoastingPosition p; p.PParticleEffect.m_pItem = &alive; }
    if (g_kills != 1 || !g_lastImmediate) { std::printf("00781df0_TEST FAIL\n"); return 1; }
    { CTCBoastingPosition p; p.PParticleEffect.m_pItem = &dead; }
    { CTCBoastingPosition p; }
    if (g_kills != 1) { std::printf("00781df0_TEST FAIL\n"); return 1; }
    std::printf("00781df0_TEST PASS\n");
    return 0;
}