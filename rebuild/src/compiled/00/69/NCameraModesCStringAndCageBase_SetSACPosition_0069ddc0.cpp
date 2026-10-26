// NCameraModes::CStringAndCageBase::SetSACPosition(const C3DVector& position) -- move the string-and-cage camera's go-to position straight to `position`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069DDC0, 27 bytes, `ret 4`.
// Leaf: ?SetSACPosition@CStringAndCageBase
// Name: vtable 0x0125C37C slot 30; FableWin.pdb lists ?SetSACPosition@ for CStringAndCageBase, CStringAndCage and CSACCombat, the
//       only one-argument virtual the three share (the CStringAndCage/CSACCombat overrides 0x00871E40/0x00871E60 instead add the
//       position as a go-to attractor of weight 1).
// Layout: NCameraModes::CBase::GoTo, a CPhysicsState whose Position is its first member, at +0x54 (landed CBase dtor 0x0056E0E0).
// Shape: the C3DVector copy loads from the argument before `add ecx,54h` (a C3DVector member of a member).
class C3DVector { public:
    float X, Y, Z;
};
namespace NCameraModes {
class CPhysicsState { public:
    C3DVector Position;                            // +0x00
    C3DVector Velocity;                            // +0x0c
};
class CBase { public:
    virtual void v00();
    unsigned char _pad04[0x54 - 0x04];
    CPhysicsState GoTo;                            // +0x54
    unsigned char _pad6c[0xa4 - 0x6c];
};
class CStringAndCageBase : public CBase { public:
    virtual void SetSACPosition(const C3DVector& position);
};
void CStringAndCageBase::SetSACPosition(const C3DVector& position)
{
    GoTo.Position = position;
}
}