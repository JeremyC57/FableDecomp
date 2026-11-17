// NCreatureMode::GetUpFromSleeping(CThingCreatureBase& creature, CThing& bed, const CCharString& animation, CAIBrain* brain) -- get a creature out of bed: while it is in the sleep-in-bed mode, leave that mode with a get-out-of-bed action (true); otherwise, when the creature is free to act, post the action through its brain (or set it directly without one) and return false
// Leaf: ?GetUpFromSleeping@NCreatureMode@@YI_NAAVCThingCreatureBase@@AAVCThing@@ABVCCharString@@PAVCAIBrain@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085F6F0, 320 bytes (+ int3 padding), /Gr __fastcall (creature in ecx, bed in edx;
//   animation and brain on the stack), `ret 8`.
// Identity: Ego_r.pdb ?GetUpFromSleeping@NCreatureMode@@YI_NAAVCThingCreatureBase@@AAVCThing@@ABVCCharString@@PAVCAIBrain@@@Z.
// Callees: CVectorMap::LowerBound const 0x004365B0 (inline GetTC 0x31 = CTCCreatureModeManager), CTCCreatureModeManager::
//   GetCurrentMode 0x0073A6E0, the mode's slot 1 (its type; 0x0D = CModeSleepBed), CCreatureAction_GetOutOfBed ctor
//   0x00860060 (landed), CModeSleepBed::RemoveModeWithAction 0x008D7250, ~CBaseIntelligentPointer 0x00A01C10 (PBed, +0xa8),
//   ~CCreatureActionBase 0x00693EF0, CThingCreatureBase::IsFreeToPerformAction 0x00662210 / SetCurrentAction 0x006644F0,
//   CBrainOutput ctor 0x00835620 / dtor 0x006D60C0, the action's slot 11 (+0x2c) Clone, CCountedPointer::Reset 0x00668D90,
//   CAIBrain::SetOutput 0x0088B8F0.
// Layout: CCreatureAction_GetOutOfBed = retail CCreatureActionBase (0xa8 bytes) + PBed CIntelligentPointer<CThing> +0xa8
//   (the landed 0x00860060 model); CBrainOutput 0x48 bytes, Flags +0, PAction +0x20 (the landed 0x0097D8B0 model).
// Shape: the mode manager is an unset local filled only when the creature has interface 0x31 (the absent path reads the
//   uninitialised word that shares the lookup key slot); the sleeping branch passes a temporary action (the ctor's
//   returned `this` pushed) and destroys it inline; the other branch's named action shares that frame slot, its Clone
//   goes through a CCreatureActionBase reference (a virtual call), and both branches' implicit action destructor is
//   the member then base destructor call pair. The named action sits in the else block: at function scope it gets
//   its own 0xb0-byte slot (frame 0x1ac v retail 0xfc).
// NF65 (round 7): ported onto the shared headers (engine/CThing.h PeekTC, CThingCreatureBase.h, CCreatureActionBase.h,
//   CCreatureAction_GetOutOfBed.h (FABLE_RETAIL_CCREATUREACTION_GETOUTOFBED: the by-reference PDB ctor), CTCCreatureModeManager.h (GetCurrentMode,
//   CModeBase), CAIBrain.h (SetOutput), CCountedPointer.h (Reset declared out of line), CIntelligentPointer.h, fable_string.h; the macro block of
//   the landed 0x0092DA20 CAIStateGroup_MinionSleep::Update). The ctor 0x00860060 now binds
//   ??0CCreatureAction_GetOutOfBed@@QAE@AAVCThingCreatureBase@@AAVCThing@@ABVCCharString@@@Z (was the pointer-parameter local spelling).
//   CModeSleepBed and CBrainOutput have no shared header and stay local (CBrainOutput as the landed 0x0085FF10 sibling models it).
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
#define FABLE_RETAIL_CCREATUREACTION_GETOUTOFBED
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CAIBRAIN_SETOUTPUT
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_GetOutOfBed.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CCountedPointer.h"
#include "engine/CAIBrain.h"

class CBrainOutput {                                        // 0x48 bytes (no shared header; the landed 0x0085FF10 / 0x0097D8B0 model)
public:
    unsigned long Types;                                    // +0x00 (8: perform action)
    long          LookFlags;                                // +0x04
    unsigned char NavigationInput[0x18];                    // +0x08
    CCountedPointer<CCreatureActionBase> PAction;           // +0x20
    unsigned char _pad28[0x48 - 0x28];
    CBrainOutput();                                         // 0x00835620
    ~CBrainOutput();                                        // 0x006D60C0
};

namespace NCreatureMode {
class CModeSleepBed : public CTCCreatureModeManager::CModeBase {   // no shared header
public:
    void RemoveModeWithAction(const CCreatureActionBase& action);   // 0x008D7250 (Ego_r.pdb ?RemoveModeWithAction@CModeSleepBed@NCreatureMode@@QAEXABVCCreatureActionBase@@@Z)
};

bool __fastcall GetUpFromSleeping(CThingCreatureBase& creature, CThing& bed, const CCharString& animation, CAIBrain* brain)
{
    const CTCCreatureModeManager* modeManager;
    creature.PeekTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
    CTCCreatureModeManager::CModeBase& mode = modeManager->GetCurrentMode();
    if (mode.GetType() == 0x0d) {
        static_cast<CModeSleepBed&>(mode).RemoveModeWithAction(CCreatureAction_GetOutOfBed(creature, bed, animation));
        return true;
    } else {
        CCreatureAction_GetOutOfBed action(creature, bed, animation);
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
