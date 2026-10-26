// Behaviour test for CTCAICreatureWillPowerIndicator::CTCAICreatureWillPowerIndicator @ 0x007E0E30: the base
// keeps the owner, PoweredUp/Timer/ResetCount start cleared, ShowEffect starts true, PEmitter is empty.
#include <cstdio>
#include <new>

class CThing { public: int unused; };

struct CTCBase_Calls {
    virtual ~CTCBase_Calls() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
};

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() {}
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CTCAICreatureWillPowerIndicator : public CTCBase_Calls { public:
    bool                PoweredUp;
    long                Timer;
    long                ResetCount;
    bool                ShowEffect;
    CIntelligentPointer PEmitter;
    CTCAICreatureWillPowerIndicator(CThing* owner)
        : CTCBase_Calls(owner), PoweredUp(false), Timer(0), ResetCount(0), ShowEffect(true), PEmitter() {}
};

int main()
{
    CThing owner;
    unsigned char storage[sizeof(CTCAICreatureWillPowerIndicator)];
    for (unsigned i = 0; i < sizeof(storage); ++i) storage[i] = 0xCD;
    CTCAICreatureWillPowerIndicator* p = new (storage) CTCAICreatureWillPowerIndicator(&owner);
    bool ok = p->ParentThing == &owner && !p->PoweredUp && p->Timer == 0 && p->ResetCount == 0
           && p->ShowEffect && p->PEmitter.m_pItem == 0;
    p->~CTCAICreatureWillPowerIndicator();
    if (!ok) { std::printf("007e0e30_TEST FAIL\n"); return 1; }
    std::printf("007e0e30_TEST PASS\n");
    return 0;
}