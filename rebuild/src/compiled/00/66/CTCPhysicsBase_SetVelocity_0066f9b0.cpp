// CTCPhysicsBase::SetVelocity(const C3DVector&) -- store the velocity and flag the body as moving
// Leaf: ?SetVelocity@CTCPhysicsBase@@QAEXABVC3DVector@@@Z
// Retail: Fable.exe @ 0x0066F9B0, 38 bytes. Catalog label CTCPhysicsRigidBody::SetFacingAnglesFromRHSet is wrong: the
// body copies one 12-byte C3DVector (not a 24-byte CRightHandedSet) into +0x28 -- Ego_r.pdb CTCPhysicsBase Velocity --
// then tail-calls SetAsMoving(true) (0x006B0130, landed; the `true` overwrites the argument slot before the `jmp`).
// Ego_r.pdb names it ?SetVelocity@CTCPhysicsBase@@QAEXABVC3DVector@@@Z (0x01F56820, beside GetVelocity).
// Callers 0x007BBDC2.. are direct calls.
// Layout: Ego_r.pdb CTCPhysicsBase (Velocity +0x28, retail agrees: engine/CTCPhysicsBase.h). The header flattens the
// C3DVector into three floats, and the retail `lea eax,[ecx+28h]` + three stores is a whole-C3DVector assignment, so the
// class is modelled locally with the vector member.
class C3DVector {
public:
    float X, Y, Z;
};
class CTCPhysicsBase {
public:
    void SetAsMoving(bool moving);                       // 0x006B0130
    void SetVelocity(const C3DVector& velocity);
    unsigned char _pad00[0x28];                          // +0x00 vptr, CTCBase, Position, OldPosition, ...
    C3DVector     Velocity;                              // +0x28
};
void CTCPhysicsBase::SetVelocity(const C3DVector& velocity)
{
    Velocity = velocity;
    SetAsMoving(true);
}