// CAIStateGroup_ServeFood::WalkTo(C3DVector, C3DVector, C3DVector) -- walk to the position (repeating while the action returns 4); on arrival turn to look at position + look offset; report whether the look completed.
// Leaf: WalkTo
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00922FC0, length 175 bytes (+ int3 padding), `ret 24h` (three by-value
// C3DVectors; the middle one is unused here).
// Callees: CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 (repeat 2, pos, &reached, movement 3, true, true, 0,
// true, 0.0), CAIStateGroupBase::AIAction_LookAtPos @ 0x0097D580 (repeat 2, by-value C3DVector, flags 0x11, speed
// 0.0027777f).
// Shape: the landed do/while action loops; the look-at point is three named float sums (x, y, z in source order) fed to
// the value constructor, so VC7.1 keeps X/Y in the 0xC frame and homes Z in the dead pos.X argument slot as retail (an
// operator+ through the value constructor evaluates z first; a field-by-field sum needs a 0x10 frame);
// `return result == 2;` (`sete cl`). No engine header for this class.
// Name fix (agentNF5 2026-11-10): CAIStateGroupBase::AIAction_LookAtPos 0x0097D580 is protected, EAIActionReturn(EAIActionRepeat, C3DVector, long, float) (?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z, Ego_r.pdb / FableWin.pdb).
// Name fix (agentNF5 2026-11-10): CAIStateGroupBase::AIAction_MoveToPos 0x0097EC00 is protected, EAIActionReturn(EAIActionRepeat, C3DVector, bool&, EMovementType, bool, bool, const CThing*, bool, float) (Ego_r.pdb / FableWin.pdb).
enum EMovementType;                           // MS forward enum
enum EAIActionRepeat;                         // MS forward enum
enum EAIActionReturn;                         // MS forward enum
class CThing;
#include "engine/C3DVector.h"

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x04..+0x13
    protected:
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    public:
    protected:
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& reached, EMovementType movementType, bool a, bool b, const CThing* leader, bool c, float d);   // 0x0097EC00
    public:
};

struct CAIStateGroup_ServeFood : CAIStateGroupBase {
    bool WalkTo(C3DVector pos, C3DVector facing, C3DVector lookOffset);
};

bool CAIStateGroup_ServeFood::WalkTo(C3DVector pos, C3DVector facing, C3DVector lookOffset)
{
    bool reached;
    long result;
    do {
        result = AIAction_MoveToPos((EAIActionRepeat)(2), pos, reached, (EMovementType)(3), true, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2)
        return false;
    float x = pos.X + lookOffset.X;
    float y = pos.Y + lookOffset.Y;
    float z = pos.Z + lookOffset.Z;
    C3DVector lookAt(x, y, z);
    do {
        result = AIAction_LookAtPos((EAIActionRepeat)(2), lookAt, 0x11, 0.0027777f);
    } while (result == 4);
    return result == 2;
}