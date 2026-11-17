// CAIStateGroup_MinionReactToNoise::Update() -- wait (yielding) until the creature has finished its current action,
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// turn to look at the noise, then walk to within 6 units of it; on arrival clear the investigate/found flags.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091CCC0, length 183 bytes.
// Callees: CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0, CAIStateGroupBase::IsStopping @ 0x008FD010, the
// brain's virtual slot 2 (+0x08, the entity-script yield), AIAction_LookAtPos @ 0x0097D580 (repeat 1, by-value
// C3DVector, flags 0x23, speed 0.05), AIAction_MoveToPosProximity @ 0x0097E890 (repeat 2, 6.0, &reached, movement 3,
// true, false, true, 0, true, 0.0) repeated while it returns 4, the finish call @ 0x008FCF40.
// Shape: the rotated `while (IsPerformingAnyAction()) { if (IsStopping()) break-to-finish; yield; }` loop, the
// landed MoveToCuriosityProximity do/while, by-value C3DVector copied memberwise into the argument block.
// Engine header names (NoisePosition +0x14, InvestigatingNoise +0x20, FoundNoiseMaker +0x21); the header flattens
// the base, so the class is modelled locally.
// Name fix (agentNF5 2026-11-10): CAIStateGroupBase::AIAction_LookAtPos 0x0097D580 is protected, EAIActionReturn(EAIActionRepeat, C3DVector, long, float) (?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z, Ego_r.pdb / FableWin.pdb).
class CThing;
#include "engine/C3DVector.h"

class CThingCreatureBase { public:
public:
    bool IsPerformingAnyAction() const;   // 0x006622A0
};

class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual bool SuspendSuspendableProcess();                       // slot 2 (+0x08) 0x00A44840 ?SuspendSuspendableProcess@CSuspendableProcessMicrothread@@UAE_NXZ (Ego_r.pdb + FableWin.pdb, inherited; was the stand-in Yield; name-fix 2026-11-12)
    unsigned char _pad[0x14];                   // +0x04..+0x17
    CThingCreatureBase* PCreature;              // +0x18
};

// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h (was a local CAIStateGroupBase model
// with long / bool* / CThing* parameters): the callee 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; header _MEMBERS + _AIACTIONS + _MOVETOPOSPROXIMITY) with the Ego_r enums of engine/fable_ai_action_enums.h;
// IsStopping 0x008FD010 and AIAction_LookAtPos 0x0097D580 (_MEALACTIONS) are the header's protected members. Update overrides the
// base's virtual slot 12, so it mangles ?Update@CAIStateGroup_MinionReactToNoise@@UAEXXZ (both PDBs; was QAE).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"

struct CAIStateGroup_MinionReactToNoise : CAIStateGroupBase {
    C3DVector NoisePosition;                    // +0x14
    bool InvestigatingNoise;                    // +0x20
    bool FoundNoiseMaker;                       // +0x21
    long FrameOfLastInteraction;                // +0x24
    void Update();
};

void CAIStateGroup_MinionReactToNoise::Update()
{
    while (PBrain->PCreature->IsPerformingAnyAction()) {
        if (IsStopping()) {
            Terminate();
            return;
        }
        PBrain->SuspendSuspendableProcess();
    }
    if (AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, NoisePosition, 0x23, 0.05f) == AI_ACTION_FINISHED) {
        bool reached;
        long result;
        do {
            result = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, NoisePosition, 6.0f, reached, AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f);
        } while (result == 4);
        if (result == 2) {
            InvestigatingNoise = false;
            FoundNoiseMaker = false;
        }
    }
    Terminate();
}