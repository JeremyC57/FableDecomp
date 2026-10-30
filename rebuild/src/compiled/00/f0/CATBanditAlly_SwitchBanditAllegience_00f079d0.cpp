// CAT_BanditAlly::SwitchBanditAllegience() -- script action: the bandit ally turns on the hero: clear its
// "has information" flag, unset it and the hero as allies of each other, set them as enemies of each other (both
// directions), remove the stored quest-info element, stop it following and disallow following the hero.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F079D0, length 162 bytes (+ int3 padding).
// Callees: CGameScriptInterface virtual slots (names from ghidra_out/cgsi_slots_aligned.tsv): 361
// ClearThingHasInformation, 70 GetHero (four times), 600 EntityUnsetThingAsAllyOfThing (twice), 597
// EntitySetThingAsEnemyOfThing (twice), 338 RemoveQuestInfoElement, 230 EntityStopFollowing, 235
// EntitySetAsAllowedToFollowHero, through the interface pointer at +4.
// Shape: as CAT_ConvoySpy::Init -- the CScriptThing at +8 is formed once in edi and passed by reference; for the
// nested calls VC fetches the outer vtable (ebx) before evaluating the inner GetHeroThing and re-reads the
// interface member afterwards; the (thing, hero) / (hero, thing) argument swaps follow right-to-left evaluation.
// Layout: +4 interface, +8 CScriptThing (12 bytes), +0x24 the quest-info element id. Local model.
class CScriptThing { public:
    void* __vftable;
    void* PImp_Data;
    void* PImp_Info;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CAT_BanditAlly {
    virtual void v00();
    CGameScriptInterface* m_pInterface;                 // +0x04
    CScriptThing          m_thing;                      // +0x08
    unsigned char         _pad_0x14[0x10];
    long                  m_questInfoElement;           // +0x24
    void SwitchBanditAllegience();
};

void CAT_BanditAlly::SwitchBanditAllegience()
{
    m_pInterface->ClearThingHasInformation(m_thing);
    m_pInterface->EntityUnsetThingAsAllyOfThing(m_thing, m_pInterface->GetHero());
    m_pInterface->EntityUnsetThingAsAllyOfThing(m_pInterface->GetHero(), m_thing);
    m_pInterface->EntitySetThingAsEnemyOfThing(m_thing, m_pInterface->GetHero());
    m_pInterface->EntitySetThingAsEnemyOfThing(m_pInterface->GetHero(), m_thing);
    m_pInterface->RemoveQuestInfoElement(m_questInfoElement);
    m_pInterface->EntityStopFollowing(m_thing);
    m_pInterface->EntitySetAsAllowedToFollowHero(m_thing, false);
}