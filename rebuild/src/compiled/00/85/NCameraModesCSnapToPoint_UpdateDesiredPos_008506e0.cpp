// NCameraModes::CSnapToPoint::UpdateDesiredPos(C3DVector& desired) -- a snap-to-point camera wants to sit exactly on its camera point's position
// Retail: Fable.exe (TLC, VC7.1) @ 0x008506E0, 29 bytes, `ret 4`.
// Leaf: ?UpdateDesiredPos@CSnapToPoint
// Name: vtable 0x0124DE5C slot 27; FableWin.pdb ?UpdateDesiredPos@CSnapToPoint@NCameraModes@@UAEXAAVC3DVector@@@Z. The slot map
//       (24 OnInitLeaf, 25 OnDieLeaf, 27 UpdateDesiredPos, 28 IsTrackObservedThing, 29 GetLookDirection) follows from the
//       neighbours 0x0056E390 (empty), 0x0056E3A0 (`mov al,[ecx+0xc0]`) and 0x0056E3B0 (copies +0xc1 to the hidden result).
// Layout: CCameraPointModeBase::Pos at +0xac (landed CSnapToPoint ctor 0x0056E330: PCameraPoint +0xa4, Pos +0xac, FOV +0xb8).
// Shape: the default C3DVector assignment through the reference: `add ecx,0ACh` then three dword moves.
class C3DVector { public:
    float X, Y, Z;
};
namespace NCameraModes {
class CBase { public:
    virtual void v00();
    unsigned char _body[0xa0];                     // +0x04..+0xa3 (retail)
};
struct CCameraPointModeBase : CBase {
    void*     PCameraPoint[2];                     // +0xa4 CIntelligentPointer<const CThing>
    C3DVector Pos;                                 // +0xac
    float     FOV;                                 // +0xb8
    bool      CutInto;                             // +0xbc
    bool      CutOutOf;                            // +0xbd
};
struct CSnapToPoint : CCameraPointModeBase {
    virtual void UpdateDesiredPos(C3DVector& desired);
};
void CSnapToPoint::UpdateDesiredPos(C3DVector& desired)
{
    desired = Pos;
}
}