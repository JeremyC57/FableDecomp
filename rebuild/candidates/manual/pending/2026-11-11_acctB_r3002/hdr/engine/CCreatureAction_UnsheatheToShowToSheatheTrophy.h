#ifndef FABLE_ENGINE_CCREATUREACTION_UNSHEATHETOSHOWTOSHEATHETROPHY_H
#define FABLE_ENGINE_CCREATUREACTION_UNSHEATHETOSHOWTOSHEATHETROPHY_H
// New shared header (w22 agent2007, row 0x006DEEE0 CTCTrophy::GlobalUpdate): the hero action that unsheathes the trophy, plays the show
// animation and sheathes it again. RTTI .?AVCCreatureAction_UnsheatheToShowToSheatheTrophy@@ : CCreatureAction_UnsheatheItemFromInventory
// (ghidra_out/rtti_bases.tsv); vtable 0x0125F8D4. Members: Ego_r.pdb ShowAnimName (CCharString) and Trophy (CIntelligentPointer<CThing>)
// at retail +0xB8 / +0xBC (the landed constructor 0x006DF800 and copy constructor 0x006DFC50), sizeof 0xC4 (GlobalUpdate's local
// [esp+20h] .. [esp+0E4h], the end of its 0xD4-byte frame). Methods: the Ego_r.pdb / FableWin.pdb set
//   ??0CCreatureAction_UnsheatheToShowToSheatheTrophy@@QAE@AAVCThingCreatureBase@@AAVCThing@@ABVCCharString@@@Z   0x006DF800
//   ??0CCreatureAction_UnsheatheToShowToSheatheTrophy@@QAE@ABV0@@Z                                               0x006DFC50
//   ??1CCreatureAction_UnsheatheToShowToSheatheTrophy@@UAE@XZ   0x006DF860 (out of line: GlobalUpdate calls it)
//   ?Clone@...@@UBEPAVCCreatureActionBase@@XZ (public in this class), ?FinishAction@...@@UAEXXZ. Declarations only.
// Needs FABLE_RETAIL_CCREATUREACTIONBASE (+ the CCreatureAction_UnsheatheItemFromInventory header).
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventory.h"
#include "engine/CIntelligentPointer.h"
class CThing; class CThingCreatureBase;
class CCreatureAction_UnsheatheToShowToSheatheTrophy : public CCreatureAction_UnsheatheItemFromInventory {
public:
    CCreatureAction_UnsheatheToShowToSheatheTrophy(CThingCreatureBase& creature, CThing& trophy, const CCharString& animation);   // 0x006DF800
    CCreatureAction_UnsheatheToShowToSheatheTrophy(const CCreatureAction_UnsheatheToShowToSheatheTrophy&);                        // 0x006DFC50
    virtual ~CCreatureAction_UnsheatheToShowToSheatheTrophy();                                                                    // 0x006DF860
    virtual CCreatureActionBase* Clone() const;
    virtual void FinishAction();
    CCharString                 ShowAnimName;                                // +0xb8
    CIntelligentPointer<CThing> Trophy;                                      // +0xbc
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_UnsheatheToShowToSheatheTrophy) == 0xc4);
#endif
