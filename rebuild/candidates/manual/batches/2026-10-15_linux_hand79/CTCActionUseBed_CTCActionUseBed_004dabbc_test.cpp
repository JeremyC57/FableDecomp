// Behaviour test for CTCActionUseBed::CTCActionUseBed @ 0x004DABBC: no fade / atomic / confirmation state,
// no user (-1), zero cost, usable by the hero, not owned, and both thing pointers empty.
#include <cstdio>
#include <new>

struct CThing { int unused; };

struct CTCActionUseBase {
    virtual ~CTCActionUseBase() {}
    CThing* ParentThing;
    CTCActionUseBase(CThing* owner) : ParentThing(owner) {}
};

struct CIntelligentPointer {
    virtual ~CIntelligentPointer() {}
    CThing* m_pItem;
    CIntelligentPointer(CThing* thing) : m_pItem(thing) {}
};

struct CTCActionUseBed : CTCActionUseBase {
    bool                WaitingForFadeOut;
    bool                WaitingForFadeIn;
    bool                InAtomicAction;
    bool                WaitingForConfirmation;
    long                PlayerNumberOfThingUsingBed;
    long                Cost;
    bool                UseableByHero;
    bool                OwnedByHero;
    bool                ForceTrespassOnceFadedOut;
    CIntelligentPointer PBuilding;
    CIntelligentPointer PThingUser;
    CTCActionUseBed(CThing* owner)
        : CTCActionUseBase(owner), WaitingForFadeOut(false), WaitingForFadeIn(false), InAtomicAction(false),
          WaitingForConfirmation(false), PlayerNumberOfThingUsingBed(-1), Cost(0), UseableByHero(true),
          OwnedByHero(false), ForceTrespassOnceFadedOut(false), PBuilding(0), PThingUser(0) {}
};

int main()
{
    CThing owner;
    unsigned char storage[sizeof(CTCActionUseBed)];
    for (unsigned i = 0; i < sizeof(storage); ++i) storage[i] = 0xCD;
    CTCActionUseBed* p = new (storage) CTCActionUseBed(&owner);
    bool ok = p->ParentThing == &owner && !p->WaitingForFadeOut && !p->WaitingForFadeIn && !p->InAtomicAction
           && !p->WaitingForConfirmation && p->PlayerNumberOfThingUsingBed == -1 && p->Cost == 0
           && p->UseableByHero && !p->OwnedByHero && !p->ForceTrespassOnceFadedOut
           && p->PBuilding.m_pItem == 0 && p->PThingUser.m_pItem == 0;
    p->~CTCActionUseBed();
    if (!ok) { std::printf("004dabbc_TEST FAIL\n"); return 1; }
    std::printf("004dabbc_TEST PASS\n");
    return 0;
}
