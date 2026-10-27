// NCameraModes::CSnapToPoint::GetLookDirection() const -- the LookDirection vector by value
// Leaf: ?GetLookDirection@CSnapToPoint@NCameraModes@@UBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E3B0, 33 bytes, thiscall, `ret 4` (the hidden result pointer).
// Identity (proven): slot 29 of vtable 0x0124DE5C (RTTI .?AVCSnapToPoint@NCameraModes@@; the only vtable holding it); Ego_r.pdb
//   ?GetLookDirection@CSnapToPoint@NCameraModes@@UBE?AVC3DVector@@XZ is the class's only C3DVector-returning virtual and LookDirection (Ego_r.pdb +0xE5) is the member
//   read at retail +0xC1.
// Shape (family M138, representative 0x0056E3B0): `return LookDirection;` -- C3DVector's copy (three dword moves) into the result.
// Header debt: the class is modelled by the member's retail offset (its other members as opaque bytes; the class is byte-packed: the member sits at an odd offset).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
#pragma pack(push, 1)
namespace NCameraModes {
class CSnapToPoint {
public:
    virtual ~CSnapToPoint();
    virtual C3DVector GetLookDirection() const;
    unsigned char _pad004[0xC1 - 4];
    C3DVector LookDirection;                                        // +0xC1
};
C3DVector CSnapToPoint::GetLookDirection() const
{
    return LookDirection;
}
}
#pragma pack(pop)