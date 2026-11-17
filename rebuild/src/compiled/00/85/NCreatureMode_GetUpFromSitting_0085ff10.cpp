// NCreatureMode::GetUpFromSitting(CThingCreatureBase& creature, long sit_mode_type, const CCharString& anim, CAIBrain* brain) -- get the creature up: when its current mode is the sitting mode, end that mode with a CCreatureAction_GetUpFromSitting (true); otherwise, when the creature is free to perform a plain get-up action, hand a clone of it to the brain's output (or set it directly without a brain) and return false
// Leaf: ?GetUpFromSitting@NCreatureMode@@YI_NAAVCThingCreatureBase@@JABVCCharString@@PAVCAIBrain@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members Object->Data, Block->Info are the header CCountedPointer's (same offsets).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085FF10, 336 bytes incl. int3 padding, /Gr `__fastcall` (creature in ecx,
//   the mode type in edx, anim and brain on the stack), `ret 8`.
// Identity: PDB name set `?GetUpFromSitting@NCreatureMode@@YI_NAAVCThingCreatureBase@@JABVCCharString@@PAVCAIBrain@@@Z`;
//   callers 0x009388B9, 0x0094993C, 0x0096D095.
// Callees: CVectorMap::LowerBound const 0x004365B0 (PeekTC 0x31), CTCCreatureModeManager::GetCurrentMode 0x0073A6E0
//   (catalog "CPixelShader::CountReferences" propagated), the mode's virtual slot 1 GetType, CCreatureAction_GetUpFromSitting
//   ctor (creature, anim, long, bool) 0x00860150, CModeSitBase::RemoveModeWithAction 0x008D7230, ~CCombatActionBase
//   0x0062E900 (catalog "~CTCLightningOrb"), CCombatActionBase ctor (creature, long, const CSoundPair&) 0x00858030,
//   CCombatActionBase::SetCombatAnimation 0x008574D0 (catalog "CTCInGameMenu::SetPDef"), CThingCreatureBase::
//   IsFreeToPerformAction 0x00662210 / SetCurrentAction 0x006644F0, CBrainOutput ctor 0x00835620 / dtor 0x006D60C0, the
//   action's virtual slot 11 Clone, CCountedPointer<CCreatureActionBase>::Reset 0x00668D90, CAIBrain::SetOutput 0x0088B8F0
//   (catalog "CQuestManager::RemoveAvailableQuestCardType").
// Layout: CThing UsingComponents +0x20, Components +0x44; TC 0x31 CTCCreatureModeManager. CCreatureAction_GetUpFromSitting
//   (vtable 0x01276C2C) is 0x11C bytes: CCombatActionBase 0x114, the mode type +0x114, a bool +0x118. CBrainOutput 0x48
//   bytes: Types +0, PAction +0x20.
// Shape: the mode manager pointer comes from the inline CThing::PeekTC(0x31) out-parameter form (absent: the dead key slot is
//   read); the sitting path's action is a temporary passed to RemoveModeWithAction, built by an out-of-line call of the
//   4-argument ctor 0x00860150; the other path's action is a named local (it shares the temporary's frame slot) for which
//   VC7.1 expands the same ctor inline (0 / false; the base ctor call, the vtable store and SetCombatAnimation); the clone
//   goes through a `const CCreatureActionBase&` (retail keeps the slot-11 virtual call).
// NF84 (round 7, q11045 binder): port form on the shared headers (the NF65 0x0085F6F0 GetUpFromSleeping recipe):
//   NCreatureMode::CModeSitBase from engine/NCreatureMode_CModeSitFloor.h (RemoveModeWithAction opt-in), CCreatureAction_GetUpFromSitting
//   from its header with the 4-argument ctor body visible in this TU (retail's TU holds its COMDAT 0x00860150 next to this function).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CAIBRAIN_SETOUTPUT
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_SETCOMBATANIMATION
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_REMOVEMODEWITHACTION
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_GetUpFromSitting.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode_CModeSitFloor.h"
#include "engine/CCountedPointer.h"
#include "engine/CAIBrain.h"

class CBrainOutput {                                        // 0x48 bytes (no shared header; the landed 0x0085F6F0 / 0x0097D8B0 model)
public:
    unsigned long Types;                                    // +0x00 (8: perform action)
    long          LookFlags;                                // +0x04
    unsigned char NavigationInput[0x18];                    // +0x08
    CCountedPointer<CCreatureActionBase> PAction;           // +0x20
    unsigned char _pad28[0x48 - 0x28];
    CBrainOutput();                                         // 0x00835620
    ~CBrainOutput();                                        // 0x006D60C0
};

inline CCreatureAction_GetUpFromSitting::CCreatureAction_GetUpFromSitting(CThingCreatureBase& creature, const CCharString& animation,
                                                                          long sittingMode, bool removeOnPerformAction)
    : CCombatActionBase(creature, 0x32, CSoundPair())
    , SittingMode(sittingMode)
    , RemoveOnPerformAction(removeOnPerformAction)
{
    SetCombatAnimation(animation);
}

namespace NCreatureMode {

bool __fastcall GetUpFromSitting(CThingCreatureBase& creature, long sit_mode_type, const CCharString& anim, CAIBrain* brain)
{
    const CTCCreatureModeManager* modeManager;
    creature.PeekTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
    CTCCreatureModeManager::CModeBase& mode = modeManager->GetCurrentMode();
    if (mode.GetType() == sit_mode_type) {
        static_cast<CModeSitBase&>(mode).RemoveModeWithAction(CCreatureAction_GetUpFromSitting(creature, anim, sit_mode_type, true));
        return true;
    } else {
        CCreatureAction_GetUpFromSitting action(creature, anim, 0, false);
        if (creature.IsFreeToPerformAction(action)) {
            if (brain) {
                CBrainOutput output;
                output.Types |= 8;
                const CCreatureActionBase& base = action;
                output.PAction.Reset(base.Clone());
                brain->SetOutput(output);
            } else
                creature.SetCurrentAction(action);
        }
        return false;
    }
}
}
