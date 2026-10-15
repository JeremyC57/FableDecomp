// Behaviour test for 00781da0 CTCBoastingPosition::CTCBoastingPosition(CThing*): base gets the owner, both pointers start empty, the three flags start false.
#include <cstdio>

struct CThing { int id; };

struct CBaseIntelligentPointer
{
    virtual void v00() {}
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer
{
    virtual void v00() {}
    CIntelligentPointer() {}
};

struct C3DVector { float X, Y, Z; };

struct CTCBase
{
    virtual void v00() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing* owner) : ParentThing(owner), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
    virtual ~CTCBase() {}
};

struct CTCBoastingPosition : CTCBase
{
    CIntelligentPointer<CThing> PBoastingPodium;
    C3DVector     OrbPosition;
    bool          PositionActivated;
    long          BoastIndex;
    long          Cost;
    bool          BoastOn;
    bool          HeroInsideRadius;
    CIntelligentPointer<CThing> PParticleEffect;

    virtual void v00() {}
    CTCBoastingPosition(CThing* owner)
        : CTCBase(owner)
        , PositionActivated(false)
        , BoastOn(false)
        , HeroInsideRadius(false)
    {
    }
};

int main()
{
    CThing owner = { 7 };
    CTCBoastingPosition pos(&owner);
    if (pos.ParentThing != &owner || pos.PositionActivated || pos.BoastOn || pos.HeroInsideRadius ||
        pos.PBoastingPodium.m_pItem != 0 || pos.PParticleEffect.m_pItem != 0)
    {
        std::printf("00781da0_TEST FAIL\n");
        return 1;
    }
    std::printf("00781da0_TEST PASS\n");
    return 0;
}