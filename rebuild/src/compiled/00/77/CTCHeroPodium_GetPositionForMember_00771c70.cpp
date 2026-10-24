// CTCHeroPodium::GetPositionForMember(long member) const -- the crowd slot for a member index: an angle of (member / 8 / 2 - 1/4) turns around the podium on the ellipse spanned by its E_X / E_Y axes, scaled by 4 and offset from the podium thing's position
// Leaf: ?GetPositionForMember@CTCHeroPodium
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771C70, 187 bytes (+5 int3), `ret 8` (hidden C3DVector result + member).
// Name: FableWin.pdb `?GetPositionForMember@CTCHeroPodium@@ABE?AVC3DVector@@J@Z` (private const); it sits between
//   CTCHeroPodium::OnInitialActivate 0x00771BD0 and GetCrowdPos 0x00771D50 (both CTCHeroPodium) and has no direct call
//   site left (its users inline it). Label correction: catalog "CVertexBufferWin32_ComputeTransformedPosition" ->
//   CTCHeroPodium::GetPositionForMember.
// Callees: CThing::GetPosition() const 0x004C73D0.
// Constants: 0.125f 0x0123095C, 0.5f 0x0122F59C, 0.25f 0x0123078C, the double 6.2831854820251465 (2 * (float)pi)
//   0x0124F2B8, 4.0f 0x0126A890.
// Layout (Ego_r.pdb CTCHeroPodium with the 12-byte retail vector): NumberOfCrowdOccupants +0x0c, CrowdOccupation +0x10,
//   E_X +0x1c, E_Y +0x28.
// Shape: `member / 8.0f` is its own statement (a `fraction` local): inside one expression VC7.1 folds `/ 8 * 0.5` into a
//   single `* 0.0625` (retail keeps `fmul 0.125 ; fmul 0.5`). The angle stays on the x87 stack for the inline fsin /
//   fcos; the sum is written `E_Y * cos + E_X * sin` (the right operand -- the sine term -- is evaluated first, as
//   retail does); the products, the sum and the scaling are inline C3DVector operators, the parent's position added last.
#include <math.h>
#pragma intrinsic(sin, cos)

const float PI = 3.14159265f;

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};

class CThing {
public:
    const C3DVector& GetPosition() const;               // 0x004C73D0
};

class CTCHeroPodium {
public:
    void* __vftable;                                    // +0x00 (CTCBase)
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    long NumberOfCrowdOccupants;                        // +0x0c
    long* CrowdOccupation[3];                           // +0x10 (vector<long>)
    C3DVector E_X;                                      // +0x1c
    C3DVector E_Y;                                      // +0x28
    CThing* GetParent() const { return ParentThing; }
private:
    C3DVector GetPositionForMember(long member) const;
};

C3DVector CTCHeroPodium::GetPositionForMember(long member) const
{
    float fraction = (float)member / 8.0f;
    float angle = (fraction * 0.5f - 0.25f) * (2.0 * PI);
    C3DVector offset = (E_Y * (float)cos(angle) + E_X * (float)sin(angle)) * 4.0f;
    return offset + GetParent()->GetPosition();
}