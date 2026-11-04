// CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction() -- the plain unsheathe FinishAction, then while the creature is in bullet time: give it a CTCHaste (adding one when missing) and set its action speed multiplier under this action's key to the bullet-time multiplier
// Grade: DIFFER(240v240) -- frame slot: retail's 0x14 frame keeps the map value_type temporary apart from the shared interface-key / CCharString slot; VC7.1 shares it (frame 0x10)
// Leaf: ?FinishAction@CCreatureAction_UnsheatheItemFromInventoryWithHasteAction@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A02B0, length 240 bytes (plain `ret`, virtual). Identity: Ego_r.pdb / FableWin.pdb
//   ?FinishAction@CCreatureAction_UnsheatheItemFromInventoryWithHasteAction@@UAEXXZ.
// Callees: CCreatureAction_UnsheatheItemFromInventory::FinishAction 0x0069FC40 (direct base call), CBaseIntelligentPointer::GetPItem
//   0x00A01B50 (PCreature->), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0 / non-const 0x0040F020 (inline
//   PeekTC<const CTCBulletTime> / GetTC<CTCHaste>), CThing::AddTC(const CCharString&, bool, const CTCParamsBase*) 0x004C9D60
//   ("CTCHaste", the implicit const char* conversion), CCharString 0x0099EBF0 / 0x0099EAE0, CTCBulletTime::GetTimeMultiplier() const
//   0x0079C740, the inline CTCHaste::SetActionSpeedMultiplier(void*, float) (map<void*, float>::insert -> _Rb_tree::insert_unique
//   0x00580D21, then CTCHaste::UpdateActionSpeedMultiplier 0x00800020).
// Layout: retail CCreatureActionBase 0xA8 (PCreature +0x08); CCreatureAction_UnsheatheItemFromInventory adds PUnsheatheItem +0xA8 and
//   PFollowUpAction +0xB0 (Ego_r.pdb); HasteKey +0xB8 (PDB +0xC4: the debug CCreatureActionBase is 0xC larger).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCHASTE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CTCBulletTime.h"
#include "engine/CTCHaste.h"
#define CCreatureAction_UnsheatheItemFromInventoryWithHasteAction DonorCCreatureAction_UnsheatheItemFromInventoryWithHasteAction
#include "engine/CCreatureAction_UnsheatheItemFromInventoryWithHasteAction.h"
#undef CCreatureAction_UnsheatheItemFromInventoryWithHasteAction

class CCreatureAction_UnsheatheItemFromInventory : public CCreatureActionBase {    // no shared header
public:
    virtual void FinishAction();                                                    // 0x0069FC40
protected:
    CIntelligentPointer<CThing>           PUnsheatheItem;                          // +0xA8
    CCountedPointer<CCreatureActionBase>  PFollowUpAction;                         // +0xB0
};

class CCreatureAction_UnsheatheItemFromInventoryWithHasteAction : public CCreatureAction_UnsheatheItemFromInventory {
public:
    virtual void FinishAction();
protected:
    void* HasteKey;                                                                 // +0xB8
};

void CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction()
{
    CCreatureAction_UnsheatheItemFromInventory::FinishAction();
    const CTCBulletTime* bulletTime;
    if (PCreature->PeekTC(&bulletTime)) {
        CTCHaste* haste;
        if (!PCreature->GetTC(&haste))
            haste = (CTCHaste*)PCreature->AddTC("CTCHaste", true, 0);
        haste->SetActionSpeedMultiplier(HasteKey, bulletTime->GetTimeMultiplier());
    }
}
