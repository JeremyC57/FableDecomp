// CAIStateGroupBase::AIAction_LookAtThing(EAIActionRepeat, CIntelligentPointer<const CThing>, long, float) -- AI action: turn the
// creature to look at a live thing (its graphic appearance's focal point, else its position), posting a look-at-thing brain
// output and suspending the brain microthread each pass until the creature has the point inside its view angle.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097D6F0, length 444 bytes (`ret 14h`). Ego_r.pdb / FableWin.pdb
//          ?AIAction_LookAtThing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@JM@Z.
// Leaf: ?AIAction_LookAtThing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@JM@Z
// Callees: CAIStateGroupBase::IsStopping @ 0x008FD010, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10,
//          NGraphicAppearance::CTCGraphicAppearance::GetFocalPos @ 0x004C1060, CThing::GetPos @ 0x004C73D0,
//          CThingCreatureBase::IsPosInViewAngle @ 0x006650E0 (both PDBs ?IsPosInViewAngle@CThingCreatureBase@@QBE_NABVC3DVector@@M_N1@Z;
//          the manifest label "CanSeeTarget" is not a PDB name of this class), CBrainOutput::CBrainOutput @ 0x00835620 (out of line
//          here), CBrainOutput::SetAsLookAtThing @ 0x0093E760 (PDB: const CThing&), CAIBrain::SetOutput @ 0x0088B8F0,
//          CSuspendableProcessMicrothread::SuspendSuspendableProcess (PBrain vtable slot 2, +8), operator delete @ 0x00BFE9BC
//          (inline CCountedPointer release in the implicit ~CBrainOutput).
// Shape: an early IsStopping() -> AI_ACTION_TERMINATE_PROCESS (1); then a do/while pass: IS_THING_ALIVE(thing) else AI_ACTION_FAILED (3);
//          `pos` is assigned in an if/else (focal point through a hidden-result temp, else the GetPos reference: one 12-byte copy per
//          arm, which a ternary initialiser would tail-merge); the thing is compared with the brain's creature as
//          `thing.Get() == &GetCreature()` (creature read first into esi, `cmp eax,esi`) and GetCreature() is read again for the
//          view-angle test, whose two look flags are `(lookFlags & N) > 0` bools; a hit is AI_ACTION_FINISHED (2), a miss posts the
//          output and is AI_ACTION_IN_PROGRESS (4). ACTION_REPEAT_UNTIL_FINISHED (1) loops while in progress.
// Layout: every class comes from the shared engine headers except CBrainOutput / CNavigationInput, which have none yet (Ego_r.pdb
//          layout as in the landed 0x0097D580: Types +0, LookFlags +4, NavigationInput +8, PAction +0x20, MoveToPos +0x28,
//          LookAtPos +0x34, LookAtThing +0x40, size 0x48; its destructor is the implicit one).
// Header opt-in this row adds: FABLE_RETAIL_CTHINGCREATUREBASE_ISPOSINVIEWANGLE (engine/CThingCreatureBase.h, declaration only).
#include "fable_compiler.h"
#include <stddef.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ISPOSINVIEWANGLE
#define FABLE_RETAIL_CTHINGAICREATURE
#include "engine/CThingAICreature.h"
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/ThingAlive.h"
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#include "engine/CAIStateGroupBase.h"
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CAIBRAIN_SETOUTPUT
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#include "engine/CAIBrain.h"

#include "engine/EAIAction.h"
#include "engine/EMovementType.h"

class CCreatureActionBase;
class CNavigationInput { public:                       // PDB: Destination +0, Proximity +0xc, MovementType +0x10, Speed +0x14
    C3DVector Destination;
    float Proximity;
    EMovementType MovementType;
    float Speed;
};
class CBrainOutput { public:                           // no shared header yet; Ego_r.pdb layout, size 0x48
    CBrainOutput();                                                      // 0x00835620
    void SetAsLookAtThing(const CThing& thing, long lookFlags);          // 0x0093E760
    unsigned long                       Types;            // +0x00
    long                                LookFlags;        // +0x04
    CNavigationInput                    NavigationInput;  // +0x08
    CCountedPointer<CCreatureActionBase> PAction;         // +0x20
    C3DVector                           MoveToPos;        // +0x28
    C3DVector                           LookAtPos;        // +0x34
    CIntelligentPointer<const CThing>   LookAtThing;      // +0x40
};

EAIActionReturn CAIStateGroupBase::AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, long lookFlags, float viewAngle)
{
    if (IsStopping())
        return AI_ACTION_TERMINATE_PROCESS;
    EAIActionReturn result;
    do {
        if (IS_THING_ALIVE(thing)) {
            C3DVector pos;
            if (thing->GetTCGraphicAppearance())
                pos = thing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
            else
                pos = thing->GetPos();
            if (thing.Get() == &GetCreature() || GetCreature().IsPosInViewAngle(pos, viewAngle, (lookFlags & 2) > 0, (lookFlags & 4) > 0))
                result = AI_ACTION_FINISHED;
            else {
                CBrainOutput output;
                output.SetAsLookAtThing(*thing.Get(), lookFlags);
                PBrain->SetOutput(output);
                result = AI_ACTION_IN_PROGRESS;
            }
        } else
            result = AI_ACTION_FAILED;
        PBrain->SuspendSuspendableProcess();
        if (IsStopping())
            return AI_ACTION_TERMINATE_PROCESS;
    } while (repeat == ACTION_REPEAT_UNTIL_FINISHED && result == AI_ACTION_IN_PROGRESS);
    return result;
}