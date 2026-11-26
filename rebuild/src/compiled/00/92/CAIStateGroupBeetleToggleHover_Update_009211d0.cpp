// CAIStateGroup_BeetleToggleHover::Update() -- land or take off, then terminate
// Leaf: ?Update@CAIStateGroup_BeetleToggleHover@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009211D0, length 84 bytes.
// Callees: CCreatureAction_BeetleLand::CCreatureAction_BeetleLand(CThingCreatureBase*) @ 0x00982C70
// (vtable 0x01288488) when IsLanding (+0x14, PDB name), else CCreatureAction_BeetleTakeOff
// @ 0x00982C10 (vtable 0x012884A0); CAIStateGroupBase::AIAction_PerformActionAndWait
// (CCreatureActionBase*, bool) @ 0x0097DB70, CCreatureActionBase::~CCreatureActionBase @ 0x0062E900,
// Terminate @ 0x008FCF40.
// Both actions are temporaries of the same size sharing one frame slot (0x114 bytes): `false` is
// pushed before the choice, each branch builds its temporary and the common tail (call, base
// destructor, Terminate) is merged by the compiler. The derived actions add no destructor. Each
// branch reads the creature into its own named local (that is what gives retail's eax/eax and
// ecx/eax load pairs; the bare expression allocates differently).
// NF134 (2026-11-20, q5021): moved onto the shared engine/CAIStateGroupBase.h (PERFORMACTION: ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z,
//   Ego_r.pdb 0x00848940 + FableWin.pdb 0x02892570) and engine/CCombatActionBase.h: the row's own local CCreatureAction_BeetleLand / _BeetleTakeOff
//   now derive from the shared CCombatActionBase (ghidra_out/rtti_bases.tsv: both : CCombatActionBase; no header exists for either), so the
//   temporaries' implicit destructors call ??1CCombatActionBase@@UAE@XZ 0x0062E900. The two constructors keep the spelling their landed self rows
//   0x00982C70 / 0x00982C10 define (pointer form; the PDB reference form is in NAME_FIXES). Was the namespaced N_0062e900 stand-in.
//   The state group itself is the shared engine/CAIStateGroup_BeetleToggleHover.h (IsLanding +0x14); own leaf now the PDB protected virtual
//   ?Update@CAIStateGroup_BeetleToggleHover@@MAEXXZ (Ego_r.pdb 0x009440A0 + FableWin.pdb 0x02C2C800; was UAE on the row-local struct).
// Naming (NF459, r454b unit with 0x00982C10): the CCreatureAction_BeetleTakeOff constructor takes `CThingCreatureBase&` (PDB spelling ??0CCreatureAction_BeetleTakeOff@@QAE@AAVCThingCreatureBase@@@Z); the construction passes `*p` for `p`, the same pointer, so the object is unchanged but for the callee name.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_UNDEADRETURNTOEARTH_CLASS
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CAIStateGroup_BeetleToggleHover.h"

class CCreatureAction_BeetleLand : public CCombatActionBase { public:
    CCreatureAction_BeetleLand(CThingCreatureBase& creature);      // 0x00982C70 (NF181 q11740: by reference, Ego_r.pdb 0x009A3280 + FableWin.pdb 0x02C809F0)
};

class CCreatureAction_BeetleTakeOff : public CCombatActionBase { public:
    CCreatureAction_BeetleTakeOff(CThingCreatureBase& creature);   // 0x00982C10
};

void CAIStateGroup_BeetleToggleHover::Update()
{
    if (IsLanding) {
        AIAction_PerformActionAndWait(CCreatureAction_BeetleLand(GetCreature()), false);
    } else {
        AIAction_PerformActionAndWait(CCreatureAction_BeetleTakeOff(GetCreature()), false);
    }
    Terminate();
}
