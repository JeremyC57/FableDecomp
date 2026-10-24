// NCameraModes::CCameraPointModeBase::GetLookDirection() const -- the default look direction of a camera-point mode: the unit X axis
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E2B0, 21 bytes, `ret 4` (C3DVector returned through the hidden pointer).
// Leaf: ?GetLookDirection@CCameraPointModeBase
// Name: vtable 0x0124DDDC slot 29; FableWin.pdb ?GetLookDirection@CCameraPointModeBase@NCameraModes@@UBE?AVC3DVector@@XZ
//       (CSnapToPoint overrides the same slot 29 with 0x0056E3B0, returning its LookDirection member).
// Shape: a C3DVector with a user constructor is built straight into the caller's return slot (1.0f stored as an immediate,
//        the two zeros through ecx); eax already holds the hidden pointer.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
namespace NCameraModes {
struct CBase {
    virtual void v00();
    unsigned char _body[0xa0];                     // +0x04..+0xa3 (retail)
};
struct CCameraPointModeBase : CBase {
    virtual C3DVector GetLookDirection() const;
};
C3DVector CCameraPointModeBase::GetLookDirection() const
{
    return C3DVector(1.0f, 0.0f, 0.0f);
}
}