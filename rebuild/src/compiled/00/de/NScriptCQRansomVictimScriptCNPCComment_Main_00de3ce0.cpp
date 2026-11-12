// NScript::CQ_RansomVictimScript::CNPCComment::Main() -- Ransom Victim quest NPC comments: alive filter on Me, find the victim's father's guard and the victim by script name, then each frame let each of them make their NPC comment once (when alive and within the quest's NPC comment distance of Me); once both have spoken, remove Me
// Inline-budget: the mixed inline/out-of-line ~CScriptThing of the generated entity scripts (retail expands it at the loop's top return, 0x00DE3ECC, and calls 0x004AA840 at the in-loop returns, 0x00DE3F6B) is reached with scoped `#pragma inline_depth(0)` regions around those returns and in ~CIsThingAlive, as the landed CCampHostageDoor::Main 0x00D08B40.
// Leaf: ?Main@CNPCComment@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE3CE0, 676 bytes (+ int3 padding), thiscall; entity-script vtable slot 1 (Main).
// Identity: Ego_r.pdb ?Main@CNPCComment@CQ_RansomVictimScript@NScript@@EAEXXZ; slot 1 of the CNPCComment vtable (its landed
//   GetParentScript 0x00DE3CA0 / Init 0x00DE3CB0 / scalar deleting dtor 0x00DE3CC0 neighbours).
// Callees: CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840 (also expanded at the loop exit), ~CBaseClass 0x0099A2E0,
//   operator delete 0x00BFE9BC, CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CCharString
//   0x0099EBF0 / ~ 0x0099EAE0, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CScriptThing::GetDataString slot 3,
//   CScriptThing::IsAlive (expanded: the impl's slot 75), CQ_RansomVictimScript::MakeNPCComment 0x00DE3F90; Game slots 7
//   NewScriptFrame, 72 GetThingWithScriptName, 108 RemoveThing. Literals "VictimsFatherGuard" 0x012DD060, "Victim" 0x012CA958.
// Layout: engine/CQ_RansomVictimScript.h (CNPCComment: ParentClass +0x14, PMasterData +0x18, VictimsFatherGuardSaidIt +0x1c,
//   VictimSaidIt +0x1d); pGlobalScriptDef 0x0143E90C NPC_COMMENT_DISTANCE +0x54 (engine/CScriptDef.h).
// Shape: the mixed inline/out-of-line ~CScriptThing of the generated entity scripts (landed CCampHostageDoor::Main 0x00D08B40):
//   the dtor is FABLE_FORCEINLINE (expanded at the loop's top return), the filter class's member dtor stays a call, and the
//   in-loop returns sit in inline_depth(0) regions so they call 0x004AA840. The loop is `for (;;) { if (IsActiveThreadTerminating())
//   return; ... NewScriptFrame(); }` (the CCampHostageDoor form): the pre-loop early return then targets the final epilogue after
//   the out-of-line dtors and the scratch rotation of the inline dtors / RemoveThing matches retail; a `while (!...)` loop sent the
//   early return to the first epilogue and shifted the rotation by one (eax/ecx/edx picks at 0x00DE3EEF, 0x00DE3F29, 0x00DE3F5C).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CQ_RANSOMVICTIMSCRIPT_NPCCOMMENT
#include "fable_compiler.h"
#include "engine/CQ_RansomVictimScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
FABLE_FORCEINLINE CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Get() && PImp->IsAlive(); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
FABLE_FORCEINLINE NScript::CIsThingAlive::~CIsThingAlive() {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
}
#ifdef _MSC_VER
#pragma inline_depth(255)
#endif

void NScript::CQ_RansomVictimScript::CNPCComment::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    long distance = pGlobalScriptDef->NPC_COMMENT_DISTANCE;
    CScriptThing guard = Game.GetThingWithScriptName("VictimsFatherGuard");
    CScriptThing victim = Game.GetThingWithScriptName("Victim");
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (!VictimsFatherGuardSaidIt && guard.IsAlive() && IsDistanceBetweenThingsUnder(Me, guard, (float)distance)) {
            if (IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                return;
#ifdef _MSC_VER
#pragma inline_depth(255)
#endif
            }
            if (ParentClass.MakeNPCComment(Me.GetDataString(), guard, NPC_COMMENT_GUARD)) {
                if (IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    return;
#ifdef _MSC_VER
#pragma inline_depth(255)
#endif
                }
                VictimsFatherGuardSaidIt = true;
            }
        }
        if (!VictimSaidIt && victim.IsAlive() && IsDistanceBetweenThingsUnder(Me, victim, (float)distance)) {
            if (IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                return;
#ifdef _MSC_VER
#pragma inline_depth(255)
#endif
            }
            if (ParentClass.MakeNPCComment(Me.GetDataString(), victim, NPC_COMMENT_GUARD)) {
                if (IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    return;
#ifdef _MSC_VER
#pragma inline_depth(255)
#endif
                }
                VictimSaidIt = true;
            }
        }
        if (VictimsFatherGuardSaidIt && VictimSaidIt) {
            if (IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                return;
#ifdef _MSC_VER
#pragma inline_depth(255)
#endif
            }
            Game.RemoveThing(Me, false, true);
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            return;
#ifdef _MSC_VER
#pragma inline_depth(255)
#endif
        }
        Game.NewScriptFrame();
    }
}