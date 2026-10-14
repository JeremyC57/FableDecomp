// CAIStateGroup_HoverMovementBase::AIAction_MoveToPosAndFacePos(EAIActionRepeat, C3DVector*, C3DVector*, bool*, EMovementType)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091D7F0, length 36 bytes.
// Callee: CAIStateGroup_HoverMovementBase::AIAction_MoveToPosProximityAndFacePos(EAIActionRepeat, C3DVector*, float,
// C3DVector*, bool*, EMovementType) @ 0x0091D510 (manifest prototype).
//
// Shape: a forwarder that inserts a 0.0f proximity (`push 0`) as the third argument and passes `this` and the
// five stack arguments through; the callee pops six arguments, so it cannot be tail-jumped (`call ; ret 14h`),
// and the result rides through in eax. The stack arguments are read into eax/edx alternately as they are pushed.
struct C3DVector;
typedef long EAIActionRepeat;
typedef long EMovementType;
typedef long EAIActionReturn;

struct CAIStateGroup_HoverMovementBase {
    virtual void v00();
    EAIActionReturn AIAction_MoveToPosProximityAndFacePos(EAIActionRepeat repeat, C3DVector* pPos, float proximity,
                                                          C3DVector* pFacePos, bool* pArrived, EMovementType movement);   // 0x0091D510
    EAIActionReturn AIAction_MoveToPosAndFacePos(EAIActionRepeat repeat, C3DVector* pPos, C3DVector* pFacePos,
                                                 bool* pArrived, EMovementType movement);
};

EAIActionReturn CAIStateGroup_HoverMovementBase::AIAction_MoveToPosAndFacePos(EAIActionRepeat repeat, C3DVector* pPos, C3DVector* pFacePos,
                                                                               bool* pArrived, EMovementType movement)
{
    return AIAction_MoveToPosProximityAndFacePos(repeat, pPos, 0.0f, pFacePos, pArrived, movement);
}