// NUISystem::CComponent::GetParentColour() const -- the component's ParentColour by value
// Leaf: ?GetParentColour@CComponent@NUISystem@@UBE?AVCRGBColour@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F580, 54 bytes, thiscall, `ret 4` (the hidden result pointer).
// Identity (proven): slot 21 of vtable 0x0124608C (RTTI .?AVCComponent@NUISystem@@; inherited by CSprite, CYesNo, COk,
//   CControllerDisconnect, CChangingStateComponent); the member read is +0x90 = Ego_r.pdb CComponent::ParentColour (Ego_r offset
//   - 8: the 12-byte STLport containers before it, family M120); Ego_r.pdb ?GetParentColour@CComponent@NUISystem@@UBE?AVCRGBColour@@XZ.
// Shape (family M121, representative 0x0052F500): `return ParentColour;` -- CRGBColour's user copy constructor `Set(other.R,
//   other.G, other.B, other.A)` (the landed CRGBColour model), four byte moves.
// Header debt: CComponent is modelled locally.
class CRGBColour {                                          // D3DCOLOR byte order (the landed model)
public:
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
namespace NUISystem {
class CComponent {
public:
    virtual ~CComponent();
    virtual CRGBColour GetParentColour() const;
    unsigned char _pad04[0x90 - 4];
    CRGBColour ParentColour;                                       // +0x90
};
CRGBColour CComponent::GetParentColour() const
{
    return ParentColour;
}
}