// CAIStateGroup_MinionReactToAlert::CAIStateGroup_MinionReactToAlert() -- base CAIStateGroupBase() then this class's vtable
// Leaf: ??0CAIStateGroup_MinionReactToAlert@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FD50D, oracle length 18 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroupBase::CAIStateGroupBase ; mov dword ptr [esi],offset CAIStateGroup_MinionReactToAlert::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base constructor @ 0x008FD255
// (??0CAIStateGroupBase@@QAE@XZ) runs, then the compiler stores this class's vtable (0x01289070) and returns `this`.
// NF474 (r467f rework of NF467 b3; r459a vftable port): no engine header exists for this class, so it is modelled here (r462a) on the
//   shared engine/CAIStateGroupBase.h retail view, with its Ego_r.pdb members and size 0x1C (struct_layouts_egor.tsv; retail Alloc
//   0x008FF190 pushes the same size before operator new) and the overrides of retail's table in their PDB spelling; so the emitted
//   ??_7CAIStateGroup_MinionReactToAlert@@6B@ has retail's 14 slots at retail's targets (was a one-slot 0x14-byte local model, 4 bytes).
// TU mode: size-optimised, as retail's TU was (r467c): the ??_GCAIStateGroup_MinionReactToAlert@@UAEPAXI@Z the full table brings is then 28 bytes,
//   equal to retail 0x008FF19C (under /O2 it is 30 bytes and differs), and the implicit ??1CAIStateGroupBase 19 bytes = 0x008FD288.
//   The ctor's own bytes are the same in both modes.
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"

class CAIStateGroup_MinionReactToAlert : public CAIStateGroupBase {     // vftable 0x01289070
public:
    CAIStateGroup_MinionReactToAlert();
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name); // slot 1 0x0090F0D0 ?Initialise@CAIStateGroup_MinionReactToAlert@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z (Ego_r.pdb + FableWin.pdb)
    virtual bool IsRunnable();                                             // slot 11 0x0090F1B0 ?IsRunnable@CAIStateGroup_MinionReactToAlert@@UAE_NXZ (Ego_r.pdb + FableWin.pdb)
    virtual void Update();                                                 // slot 12 0x0090F1C0 ?Update@CAIStateGroup_MinionReactToAlert@@UAEXXZ (Ego_r.pdb + FableWin.pdb)
    unsigned long LastAlertReactedTo;                    // +0x14
    long          Timer;                                 // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_MinionReactToAlert) == 0x1C);

CAIStateGroup_MinionReactToAlert::CAIStateGroup_MinionReactToAlert()
    : CAIStateGroupBase()
{
}
