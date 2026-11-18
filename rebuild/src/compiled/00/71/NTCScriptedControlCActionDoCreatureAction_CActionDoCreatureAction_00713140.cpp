// NTCScriptedControl::CActionDoCreatureAction::CActionDoCreatureAction(const CCreatureActionBase& action) -- an action that hands a clone of the creature action to its creature
// Leaf: ??0CActionDoCreatureAction@NTCScriptedControl@@QAE@ABVCCreatureActionBase@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00713140, 64 bytes (`ret 4`).
// Identity: Ego_r.pdb + FableWin.pdb ??0CActionDoCreatureAction@NTCScriptedControl@@QAE@ABVCCreatureActionBase@@@Z; the vtable it stores,
//   0x01265838, is .?AVCActionDoCreatureAction@NTCScriptedControl@@ (rtti-vtables.tsv).
// Callees: ??0CBaseClassNonCopyable@@IAE@XZ 0x0099A2F0 (the protected root ctor; engine/CBaseClassNonCopyable.h), the action's virtual
//   Clone (slot 11, +0x2C), ?Reset@?$CCountedPointer@VCCreatureActionBase@@@@QAEXPAVCCreatureActionBase@@@Z 0x00668D90 (out of line).
// Shape: CTCScriptedControl::CActionBase's constructor expanded (PTCScript / Activated / Terminated zeroed from one register, no
//   intermediate vtable store), the derived vtable, the counted pointer's two zero stores (its default ctor), then ActionID /
//   ActionSet assigned in the body (an initialiser list stores them before Action: DIFFER 64v64), then Action.Reset(action.Clone()).
// NF79 (round 7, q10957): re-authored on the shared headers (engine/CTCScriptedControl.h CActionBase + my overlay opt-in
//   CTCSCRIPTEDCONTROL_ACTIONBASE_INLINE_CTOR, the new engine/NTCScriptedControl_CActionDoCreatureAction.h, CCountedPointer.h,
//   CCreatureActionBase.h); was a free __fastcall stand-in ?CActionDoCreatureAction_CActionDoCreatureAction@@YIPAU..@Z over the flat generated
//   struct, calling the root ctor as ?Base_ctor@@YIXPAX@Z, the vtable as the cast global g_vtbl_1265838 and Reset as a stand-in Init.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE_INLINE_CTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/NTCScriptedControl_CActionDoCreatureAction.h"

namespace NTCScriptedControl {
CActionDoCreatureAction::CActionDoCreatureAction(const CCreatureActionBase& action)
{
    ActionID = 0;
    ActionSet = false;
    Action.Reset(action.Clone());
}
}
