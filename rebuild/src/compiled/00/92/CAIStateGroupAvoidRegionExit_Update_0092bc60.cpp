// CAIStateGroup_AvoidRegionExit::Update() -- walk back to the stored position, then terminate
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092BC60, length 69 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::AIAction_MoveToPosProximity @ 0x0097E890 (12 arguments: repeat
// mode 1, the C3DVector Position (+0x14, engine header) by value, radius 2.0f, &reached,
// movement type 5, true, false, true, no leader, true, 0.0f), CAIStateGroupBase::Terminate
// @ 0x008FCF40 (sets Terminated at +0x10).
// The by-value C3DVector is built in its argument slot (`sub esp,0Ch ; mov eax,esp ; mov [eax]..`)
// because it has a user copy constructor; the `reached` bool is an uninitialised local that VC
// homes in the high byte of the `push ecx` frame slot (`lea eax,[esp+23h]`).
// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h (was a local CAIStateGroupBase model
// with long / bool* / CThing* parameters): the callee 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; header _MEMBERS + _AIACTIONS + _MOVETOPOSPROXIMITY) with the Ego_r enums of engine/fable_ai_action_enums.h.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CAIStateGroupBase.h"

struct CAIStateGroup_AvoidRegionExit : CAIStateGroupBase {
    C3DVector Position;                     // +0x14
    virtual void Update();
};

void CAIStateGroup_AvoidRegionExit::Update()
{
    bool reached;
    AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED, Position, 2.0f, reached, AI_MOVEMENT_RUNNING, true, false, true, 0, true, 0.0f);
    Terminate();
}