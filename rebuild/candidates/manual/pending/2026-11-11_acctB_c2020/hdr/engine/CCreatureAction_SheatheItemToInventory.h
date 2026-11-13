#ifndef FABLE_ENGINE_CCREATUREACTION_SHEATHEITEMTOINVENTORY_H
#define FABLE_ENGINE_CCREATUREACTION_SHEATHEITEMTOINVENTORY_H
// CCreatureAction_SheatheItemToInventory (new shared header, w22 agent2012, row 0x006DE280 CTCTrophy::TerminateMinigame): the action that
// puts an item a creature holds back into its inventory. RTTI .?AVCCreatureAction_SheatheItemToInventory@@, vtable 0x0125C83C (slot 3
// PerformAction 0x0069F4F0, slot 4 FinishAction 0x0069F670, slot 12 Clone 0x0069F480; rebuild/manifest/rtti-vtables.tsv); derived from the
// retail CCreatureActionBase (FABLE_RETAIL_CCREATUREACTIONBASE, sizeof 0xA8). Ego_r.pdb member (struct_layouts_egor.tsv, sizeof 188 there):
// PFollowUpAction (CCountedPointer<CCreatureActionBase>) at Ego_r +0xB4 = retail +0xA8 (the base is 12 bytes smaller in retail, as for
// CCreatureAction_UnsheatheItemFromInventory); retail sizeof 0xB0 (TerminateMinigame's temporary spans [esp+24h]..[esp+0D4h], the end of
// its 0xC4-byte frame). Ego_r.pdb / FableWin.pdb methods:
//   ??0CCreatureAction_SheatheItemToInventory@@QAE@AAVCThingCreatureBase@@PAVCCreatureActionBase@@J@Z   retail 0x0069FFD0 (`push 64h ;
//     push 0 ; push creature ; lea ecx,<tmp> ; call`, 0x006DE2F9)
//   ??0CCreatureAction_SheatheItemToInventory@@QAE@AAVCThingCreatureBase@@ABVCCharString@@PAVCCreatureActionBase@@J@Z
//   ??1CCreatureAction_SheatheItemToInventory@@UAE@XZ (retail 0x0048C950, the copy TerminateMinigame calls for the temporary),
//   Initialise / PerformAction / FinishAction (UAEXXZ), the private Clone (EBEPAVCCreatureActionBase@@XZ) and the private static
//   GetAnimationToUse (CI?AVCCharString@@ABVCThing@@@Z). Declarations only.
#ifndef FABLE_RETAIL_CCREATUREACTIONBASE
#error engine/CCreatureAction_SheatheItemToInventory.h needs the retail CCreatureActionBase: define FABLE_RETAIL_CCREATUREACTIONBASE
#else
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCountedPointer.h"
class CThing; class CThingCreatureBase; class CCharString;
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase {
public:
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase& creature, CCreatureActionBase* followUp, long priority);   // 0x0069FFD0
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase& creature, const CCharString& animation, CCreatureActionBase* followUp,
                                           long priority);
    virtual ~CCreatureAction_SheatheItemToInventory();                        // 0x0048C950
    virtual void Initialise();
    virtual void PerformAction();                                             // 0x0069F4F0
    virtual void FinishAction();                                              // 0x0069F670
private:
    virtual CCreatureActionBase* Clone() const;                               // 0x0069F480
    static CCharString __fastcall GetAnimationToUse(const CThing& creature);
    CCountedPointer<CCreatureActionBase> PFollowUpAction;                     // +0xA8
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_SheatheItemToInventory) == 0xB0);
#endif
#endif // FABLE_ENGINE_CCREATUREACTION_SHEATHEITEMTOINVENTORY_H
