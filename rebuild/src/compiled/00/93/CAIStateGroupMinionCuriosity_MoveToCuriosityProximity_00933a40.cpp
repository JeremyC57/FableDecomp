// CAIStateGroup_MinionCuriosity::MoveToCuriosityProximity(float) -- walk towards CuriosityPos until within the
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// given radius, repeating while the move action reports "in progress" (4); true when it reports "arrived" (2).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00933A40, length 86 bytes (+ int3 padding), `ret 4`.
// Callee: CAIStateGroupBase::AIAction_MoveToPosProximity @ 0x0097E890 (12 arguments: repeat mode 2, the
// C3DVector CuriosityPos (+0x14) BY VALUE, the radius argument, &reached, movement type 3, true, false, true, no
// leader, true, 0.0f) -- the same signature the landed AvoidRegionExit::Update uses.
// Shape: `bool reached = false;` (the explicit `mov byte ptr [esp+0Bh],0` in the high byte of the `push ecx`
// slot), a do/while loop re-issuing the action while the result is 4 (the loop head is 16-byte aligned with
// the `mov edi,edi` pad), then `return result == 2;` as a full-register bool (`xor edx,edx ; sete dl ; mov
// al,dl`). The by-value C3DVector is built in its argument slot (`sub esp,0Ch ; mov eax,esp`) because it has
// a user copy constructor. Base layout local; no engine header for this class.
class CThing;

#include "engine/C3DVector.h"

// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h (was a local CAIStateGroupBase model
// with long / bool* / CThing* parameters): the callee 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; header _MEMBERS + _AIACTIONS + _MOVETOPOSPROXIMITY) with the Ego_r enums of engine/fable_ai_action_enums.h.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"

struct CAIStateGroup_MinionCuriosity : CAIStateGroupBase {
    C3DVector CuriosityPos;                 // +0x14
    bool MoveToCuriosityProximity(float radius);
};

bool CAIStateGroup_MinionCuriosity::MoveToCuriosityProximity(float radius)
{
    bool reached = false;
    long result;
    do {
        result = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, CuriosityPos, radius, reached, AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f);
    } while (result == 4);
    return result == 2;
}