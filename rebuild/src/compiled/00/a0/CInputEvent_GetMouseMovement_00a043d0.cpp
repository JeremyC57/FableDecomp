// CInputEvent::GetMouseMovement() const -- the mouse movement carried by a mouse-move event
// Leaf: ?GetMouseMovement@CInputEvent@@QBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A043D0, 24 bytes, thiscall, `ret 4` (hidden C3DVector result).
// Identity (proven): Ego_r.pdb ?GetMouseMovement@CInputEvent@@QBE?AVC3DVector@@XZ; its callers are
//   CGamePlayerInterface::GetMouseMovement 0x004458F0, CInputTypeMouseMovementEvent::GetControlDirection 0x0066F8B0 and
//   CInputProcessControlFreeCamera::ProcessInput 0x0068CB90; it sits with the CInputEvent accessors (GetJoystickPosition
//   0x00A043A0). The copied 12 bytes are the event data union's second arm (UInputEventData +12). The manifest's
//   "CTCPhysicsRigidBody::GetFacingVector" is propagated across byte-identical bodies.
// Shape (family M199, representative 0x0069E1F0): `return <the movement arm>.MouseMovement;` (a union cannot hold the
//   copy-constructible C3DVector in VC7.1, so the arm is read through its own record).
class C3DVector {                                          // Ego_r.pdb, 12 bytes
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // the engine's member-wise copy (the implicit one allocates differently)
};
struct CMouseMovementArm { unsigned long Head[3]; C3DVector MouseMovement; };
union UInputEventData {                                  // Ego_r.pdb, 32 bytes (the arms' names are not in the PDB)
    unsigned long Words[8];
};
class CInputEvent {
public:
    C3DVector GetMouseMovement() const;
    UInputEventData Data;                                 // +0x00
};
C3DVector CInputEvent::GetMouseMovement() const
{
    return ((const CMouseMovementArm&)Data).MouseMovement;
}