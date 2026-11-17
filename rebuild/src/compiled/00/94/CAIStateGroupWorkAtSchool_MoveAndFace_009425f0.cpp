// CAIStateGroup_WorkAtSchool::MoveAndFace(C3DVector pos, float radius, C3DVector facePos) -- walk (run when 10 or
// Leaf: ?MoveAndFace@CAIStateGroup_WorkAtSchool@@QAE_NVC3DVector@@M0@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLength respelled GetMagnitude (the header / FableWin.pdb method, same computation).
// more units away) to within `radius` of pos, repeating while the move is in progress, then turn to look at facePos
// the same way; true when both actions finish (2).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009425F0, length 218 bytes (+ int3 padding), `ret 1Ch`.
// Callees: CThing::GetPos @ 0x004C73D0, CAIStateGroupBase::AIAction_MoveToPosProximity @ 0x0097E890 (2, pos by
// value, radius, &reached, movement type, true, false, true, no leader, true, 0.0f), AIAction_LookAtPos @ 0x0097D580
// (2, facePos by value, 0x11, 0.0027777f). Float 10.0f @ 0x01292A1C.
// Shape: the distance is the inline C3DVector difference (components built z-first, constructor arguments
// evaluated right to left) and sqrt length; movement type 3 (walk) or 5 (run); the radius lives in edi so the
// uninitialised `reached` flag takes its argument slot; the two do/while loops re-issue the action while it returns
// 4; the C3DVector arguments have a user copy constructor (built in the argument slot). Local model. Fix (retry 10):
// GetLength sums into a named float before the sqrt (the single expression schedules fsqrt above the leftover pops),
// `return result == 2 ? true : false;` gives the bare `sete al`, and AIAction_MoveToPosProximity has a same-TU
// stand-in that writes only its `reached` out flag.
// Name fix (agentNF5 2026-11-10): CAIStateGroupBase::AIAction_LookAtPos 0x0097D580 is protected, EAIActionReturn(EAIActionRepeat, C3DVector, long, float) (?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z, Ego_r.pdb / FableWin.pdb).
enum EAIActionRepeat;                         // MS forward enum
enum EAIActionReturn;                         // MS forward enum
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class CThing;

#include "engine/C3DVector.h"

#include "engine/CThing.h"
struct CThingPos : public CThing {
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingPos* PCreature;                       // +0x18
};

// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h (was a local CAIStateGroupBase model
// with long / bool* / CThing* parameters): the callee 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; _MEMBERS + _AIACTIONS + _MOVETOPOSPROXIMITY + _MEALACTIONS for AIAction_LookAtPos 0x0097D580) with the
// Ego_r enums of engine/fable_ai_action_enums.h. The same-TU stand-in body of the move (which places the uninitialised `reached` in
// the dead radius argument slot) now defines the header member with its PDB signature.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"
long MoveSink_s7(const void* group, float x, float radius, long movementType);   // stand-in: the movement action's work
#pragma auto_inline(off)
// Same-TU stand-in for AIAction_MoveToPosProximity (0x0097E890): reads the position, writes the out flag, lets no
// argument escape.
__declspec(noinline) EAIActionReturn CAIStateGroupBase::AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float radius, bool& reached, EMovementType movementType, bool a, bool b, bool c, const CThing* leader, bool d, float e)
{
    long status = MoveSink_s7(this, pos.X, radius, movementType);
    reached = status == 2;
    return (EAIActionReturn)status;
}
#pragma auto_inline(on)

struct CAIStateGroup_WorkAtSchool : CAIStateGroupBase {
    bool MoveAndFace(C3DVector pos, float radius, C3DVector facePos);
};

bool CAIStateGroup_WorkAtSchool::MoveAndFace(C3DVector pos, float radius, C3DVector facePos)
{
    EMovementType movementType = (pos - PBrain->PCreature->GetPos()).GetMagnitude() < 10.0f ? AI_MOVEMENT_WALKING : AI_MOVEMENT_RUNNING;
    bool reached;
    long result;
    do {
        result = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, pos, radius, reached, movementType, true, false, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2)
        return false;
    do {
        result = AIAction_LookAtPos(ACTION_REPEAT_CALLBACK, facePos, 0x11, 0.0027777f);
    } while (result == 4);
    return result == 2 ? true : false;
}