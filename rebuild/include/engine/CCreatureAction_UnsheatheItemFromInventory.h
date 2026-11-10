#ifndef FABLE_ENGINE_CCREATUREACTION_UNSHEATHEITEMFROMINVENTORY_H
#define FABLE_ENGINE_CCREATUREACTION_UNSHEATHEITEMFROMINVENTORY_H
// CCreatureAction_UnsheatheItemFromInventory (new shared header, chunk 1004, row 0x005CA68E CTCInventoryWeapons::SetupActiveWeaponsOnCarrier):
// RTTI .?AVCCreatureAction_UnsheatheItemFromInventory@@ : CCreatureActionBase@0 (ghidra_out/rtti_bases.tsv); needs the retail
// CCreatureActionBase model (FABLE_RETAIL_CCREATUREACTIONBASE, sizeof 0xA8). Ego_r.pdb members (struct_layouts_egor.tsv, 0xC4 there):
// PUnsheatheItem (CIntelligentPointer<CThing>) and PFollowUpAction (CCountedPointer<CCreatureActionBase>), at retail +0xA8 / +0xB0,
// sizeof 0xB8 (the row's local spans [ebp-74h] .. [ebp+44h]). Ego_r.pdb / FableWin.pdb methods:
//   ??0CCreatureAction_UnsheatheItemFromInventory@@QAE@AAVCThingCreatureBase@@AAVCThing@@PAVCCreatureActionBase@@J@Z   retail 0x006A0150
//   ??0CCreatureAction_UnsheatheItemFromInventory@@QAE@AAVCThingCreatureBase@@ABVCCharString@@AAVCThing@@JJPAVCCreatureActionBase@@@Z
//   ??1CCreatureAction_UnsheatheItemFromInventory@@UAE@XZ (retail 0x005CC091, emitted beside its caller), Initialise / PerformAction
//   (retail 0x0069F9E0, vtable 0x0125CCBC slot 3) / FinishAction (UAEXXZ), the private Clone (EBEPAVCCreatureActionBase@@XZ) and the
//   private static GetAnimationToUse (CI?AVCCharString@@ABVCThing@@0@Z). Declarations only (the row calls the constructor, PerformAction
//   on the local and the destructor directly).
#ifndef FABLE_RETAIL_CCREATUREACTIONBASE
#error engine/CCreatureAction_UnsheatheItemFromInventory.h needs the retail CCreatureActionBase: define FABLE_RETAIL_CCREATUREACTIONBASE
#else
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
class CThing; class CThingCreatureBase; class CCharString;
class CCreatureAction_UnsheatheItemFromInventory : public CCreatureActionBase {
public:
    CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase& creature, CThing& item, CCreatureActionBase* followUp, long priority);   // 0x006A0150
    CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase& creature, const CCharString& animation, CThing& item, long, long,
                                               CCreatureActionBase* followUp);
    virtual ~CCreatureAction_UnsheatheItemFromInventory();                    // 0x005CC091
    virtual void Initialise();
    virtual void PerformAction();                                             // 0x0069F9E0
    virtual void FinishAction();
private:
    virtual CCreatureActionBase* Clone() const;
    static CCharString __fastcall GetAnimationToUse(const CThing& creature, const CThing& item);
#ifdef FABLE_RETAIL_CCREATUREACTION_UNSHEATHE_PRIVATE
// Ego_r22fe0: original access of both contained pointers is private.
private:
#else
public:
#endif
    CIntelligentPointer<CThing> PUnsheatheItem;                               // +0xA8
    CCountedPointer<CCreatureActionBase> PFollowUpAction;                     // +0xB0
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_UnsheatheItemFromInventory) == 0xb8);
#endif
#endif // FABLE_ENGINE_CCREATUREACTION_UNSHEATHEITEMFROMINVENTORY_H
