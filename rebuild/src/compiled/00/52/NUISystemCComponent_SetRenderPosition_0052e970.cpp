// NUISystem::CComponent::SetRenderPosition(C2DVector& v) -- set the component's RenderPosition
// Leaf: ?SetRenderPosition@CComponent@NUISystem@@UAEXAAVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E970, 18 bytes, thiscall, `ret 4`.
// Identity (proven): slot 28 of vtable 0x0124608C (RTTI .?AVCComponent@NUISystem@@; the same slot in the CSprite, CYesNo, COk,
//   CControllerDisconnect, CChangingStateComponent and CObserver-derived vtables); slot 25 (0x0052E940) returns its address: GetRenderPosition (const C2DVector&); Ego_r.pdb
//   ?SetRenderPosition@CComponent@NUISystem@@UAEXAAVC2DVector@@@Z. Retail layout: Ego_r.pdb's members 8 bytes lower (the 12-byte STLport
//   set in the CObserver base and the States map), so Position +0x34 (GetPosition, slot 16) and RenderPosition +0x54.
// Shape (family M120, representative 0x0052E8D0): `RenderPosition = v;` -- C2DVector's implicit assignment, two dword moves.
// Header debt: CComponent is modelled locally (its bases as one opaque block; the members before RenderPosition as opaque bytes).
class C2DVector { public: float X, Y; };
namespace NUISystem {
class CComponent {
public:
    virtual ~CComponent();
    virtual void SetRenderPosition(C2DVector& v);
    unsigned char _pad04[0x54 - 4];                    // bases (CObserver, ...), Name, States, Time, Position ...
    C2DVector RenderPosition;                                        // +0x54
};
void CComponent::SetRenderPosition(C2DVector& v)
{
    RenderPosition = v;
}
}