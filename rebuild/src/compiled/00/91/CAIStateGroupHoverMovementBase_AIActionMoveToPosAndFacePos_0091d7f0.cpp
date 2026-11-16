// CAIStateGroup_HoverMovementBase::AIAction_MoveToPosAndFacePos(EAIActionRepeat, C3DVector*, C3DVector*, bool*, EMovementType)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091D7F0, length 36 bytes.
// Callee: CAIStateGroup_HoverMovementBase::AIAction_MoveToPosProximityAndFacePos(EAIActionRepeat, C3DVector*, float,
// C3DVector*, bool*, EMovementType) @ 0x0091D510 (manifest prototype).
//
// Shape: a forwarder that inserts a 0.0f proximity (`push 0`) as the third argument and passes `this` and the
// five stack arguments through; the callee pops six arguments, so it cannot be tail-jumped (`call ; ret 14h`),
// and the result rides through in eax. The stack arguments are read into eax/edx alternately as they are pushed.
// Name fix (agentNF36 2026-11-16): the callee 0x0091D510 is spelled as both PDB name sets do,
// ?AIAction_MoveToPosProximityAndFacePos@CAIStateGroup_HoverMovementBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@ABVC3DVector@@M1AA_NW4EMovementType@@@Z
// (protected; const C3DVector& positions, bool& arrived, real enums); this row's own parameters keep their landed long / pointer spelling.
class C3DVector;
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"

struct CAIStateGroup_HoverMovementBase {
    virtual void v00();
protected:
    EAIActionReturn AIAction_MoveToPosProximityAndFacePos(EAIActionRepeat repeat, const C3DVector& pos, float proximity,
                                                          const C3DVector& facePos, bool& arrived, EMovementType movement);   // 0x0091D510
public:
    long AIAction_MoveToPosAndFacePos(long repeat, C3DVector* pPos, C3DVector* pFacePos, bool* pArrived, long movement);
};

long CAIStateGroup_HoverMovementBase::AIAction_MoveToPosAndFacePos(long repeat, C3DVector* pPos, C3DVector* pFacePos,
                                                                    bool* pArrived, long movement)
{
    return AIAction_MoveToPosProximityAndFacePos((EAIActionRepeat)repeat, *pPos, 0.0f, *pFacePos, *pArrived, (EMovementType)movement);
}
