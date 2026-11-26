// CAIStateGroup_MinionReactToAlert::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&) -- base Initialise, then a two-second timer
// Leaf: ?Initialise@CAIStateGroup_MinionReactToAlert@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F0D0, length 51 bytes (+ int3 padding). Vtable slot 1
// of CAIStateGroup_MinionReactToAlert (0x01289070), its only retail reference.
// Callees: CAIStateGroupBase::Initialise @ 0x008FCEC0 (all five arguments forwarded; its landed self
//   ?Initialise@CAIStateGroupBase@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z). Global: the frame-rate
//   long @ 0x01375550 (?ConstantFPS@CMainGameComponent@@1JA).
// Shape: base Initialise, then `Timer = CMainGameComponent::GetConstantFPS() * 2` (two seconds in frames: `lea eax,[edx+edx]`).
// Identity (NF475, r467b unit with ctor 0x008FD50D): RTTI .?AVCAIStateGroup_MinionReactToAlert@@ vtable 0x01289070 slot 1 + Ego_r.pdb / FableWin.pdb
//   (EScriptAIPriority parameter, mangled W4; was `long`, ...JJABV...). Base and EScriptAIPriority now from the shared engine/CAIStateGroupBase.h
//   (the HEAD row's local CAIStateGroupBase model and `typedef long EScriptAIPriority` are gone); CMainGameComponent from engine/CMainGameComponent.h.
// Model: no engine header exists for this class, so it is the ctor row 0x008FD50D's local class (r462a), copied unchanged: Ego_r.pdb members,
//   size 0x1C (struct_layouts_egor.tsv; retail Alloc 0x008FF190 pushes the same size), the overrides of retail's table in their PDB spelling.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"
#include "engine/CMainGameComponent.h"

class CAIStateGroup_MinionReactToAlert : public CAIStateGroupBase {     // vftable 0x01289070
public:
    CAIStateGroup_MinionReactToAlert();
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name); // slot 1 0x0090F0D0
    virtual bool IsRunnable();                                             // slot 11 0x0090F1B0 ?IsRunnable@CAIStateGroup_MinionReactToAlert@@UAE_NXZ (Ego_r.pdb + FableWin.pdb)
    virtual void Update();                                                 // slot 12 0x0090F1C0 ?Update@CAIStateGroup_MinionReactToAlert@@UAEXXZ (Ego_r.pdb + FableWin.pdb)
    unsigned long LastAlertReactedTo;                    // +0x14
    long          Timer;                                 // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_MinionReactToAlert) == 0x1C);

void CAIStateGroup_MinionReactToAlert::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
    Timer = CMainGameComponent::GetConstantFPS() * 2;
}
