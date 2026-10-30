// CStatueMasterChest::Main() -- script body of the Statue Master chest: when the parent script's statue does not point at position 3 the chest thing is removed through the quest (unless the script was aborted)
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED4830, 54 bytes (+10 int3 padding); slot 1 of the no-RTTI quest-entity
// vtable 0x012F2FE4 (Main). Callees: CV_StatueMasterScript::GetStatuePointingPosition @ 0x00ED4420 (long), the abort
// test @ 0x00F35B30 (on `this`, bool; manifest "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated -- the
// landed CHH_CellarDoor / CDoorToMothersTortureChamber Main bodies use it the same way), CGameScriptInterface slot 108
// RemoveThing(const CScriptThing&, bool, bool) const (ghidra_out/cgsi_slots_aligned.tsv).
// Layout: Ego_r.pdb CStatueMasterChest (28 bytes): the quest interface at +4 and the chest CScriptThing at +8 (base),
// ParentClass +0x14 (CV_StatueMasterScript*), PMasterData +0x18.
// Shape: both arms run the abort test (the statue-at-3 arm only tests it, so it becomes the sibling-call `jmp`);
// `this` is moved into ecx before the branch for both calls.
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                             // 8 bytes at +8 of the script object
    unsigned char _body[8];
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CV_StatueMasterScript {
    long GetStatuePointingPosition();              // 0x00ED4420
};
struct CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

struct CStatueMasterChest : public CEntityScriptBase {
    void*                   __vftable;             // +0x00
    CGameScriptInterface*   PQuest;                // +0x04
    CScriptThing            Thing;                 // +0x08
    unsigned char           _pad10[4];             // +0x10
    CV_StatueMasterScript*  ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    void Main();
};

void CStatueMasterChest::Main()
{
    if (ParentClass->GetStatuePointingPosition() == 3) {
        if (IsActiveThreadTerminating())
            return;
    } else {
        if (IsActiveThreadTerminating())
            return;
        PQuest->RemoveThing(Thing, false, true);
    }
}