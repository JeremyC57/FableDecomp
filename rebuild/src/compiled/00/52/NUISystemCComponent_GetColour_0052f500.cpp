// NUISystem::CComponent::GetColour() const -- the component's Colour by value
// Leaf: ?GetColour@CComponent@NUISystem@@UBE?AVCRGBColour@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F500, 54 bytes, thiscall, `ret 4` (the hidden result pointer).
// Identity (proven): slot 14 of vtable 0x0124608C (RTTI .?AVCComponent@NUISystem@@; inherited by CSprite, CYesNo, COk,
//   CControllerDisconnect, CChangingStateComponent); the member read is +0x84 = Ego_r.pdb CComponent::Colour (Ego_r offset
//   - 8: the 12-byte STLport containers before it, family M120); Ego_r.pdb ?GetColour@CComponent@NUISystem@@UBE?AVCRGBColour@@XZ.
// Shape (family M121, representative 0x0052F500): `return Colour;` -- CRGBColour's user copy constructor `Set(other.R,
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
    virtual CRGBColour GetColour() const;
    unsigned char _pad04[0x84 - 4];
    CRGBColour Colour;                                       // +0x84
};
CRGBColour CComponent::GetColour() const
{
    return Colour;
}
}