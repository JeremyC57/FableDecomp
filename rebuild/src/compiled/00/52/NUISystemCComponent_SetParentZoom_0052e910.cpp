// NUISystem::CComponent::SetParentZoom(C2DVector& v) -- set the component's ParentZoom
// Leaf: ?SetParentZoom@CComponent@NUISystem@@UAEXAAVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E910, 18 bytes, thiscall, `ret 4`.
// Identity (proven): slot 20 of vtable 0x0124608C (RTTI .?AVCComponent@NUISystem@@; the same slot in the CSprite, CYesNo, COk,
//   CControllerDisconnect, CChangingStateComponent and CObserver-derived vtables); slot 19 (0x0052E8F0) returns the same member by value: GetParentZoom; Ego_r.pdb
//   ?SetParentZoom@CComponent@NUISystem@@UAEXAAVC2DVector@@@Z. Retail layout: Ego_r.pdb's members 8 bytes lower (the 12-byte STLport
//   set in the CObserver base and the States map), so Position +0x34 (GetPosition, slot 16) and ParentZoom +0x74.
// Shape (family M120, representative 0x0052E8D0): `ParentZoom = v;` -- C2DVector's implicit assignment, two dword moves.
// Header debt: CComponent is modelled locally (its bases as one opaque block; the members before ParentZoom as opaque bytes).
class C2DVector { public: float X, Y; };
namespace NUISystem {
class CComponent {
public:
    virtual ~CComponent();
    virtual void SetParentZoom(C2DVector& v);
    unsigned char _pad04[0x74 - 4];                    // bases (CObserver, ...), Name, States, Time, Position ...
    C2DVector ParentZoom;                                        // +0x74
};
void CComponent::SetParentZoom(C2DVector& v)
{
    ParentZoom = v;
}
}