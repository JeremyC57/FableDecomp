// CSpotLightDef::operator=(const CSpotLightDef& other) -- compiler-generated copy assignment: CSubDefClassBase::operator=, then Colour, InnerRadius, OuterRadius, Width, Flicker, FlickerSpeed, Angle memberwise
// Leaf: ??4CSpotLightDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E02F2, 63 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CSpotLightDef::operator=(const CSpotLightDef&) -- proven; evidence: its only caller is the landed CSpotLightDef::Copy 0x004E02ED
//   (a `jmp` to it: `*this = *static_cast<const CSpotLightDef*>(other)`); RTTI of vtable 0x0123A88C gives CSpotLightDef : CSubDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CSpotLightDef's seven members (retail offset = Ego_r offset); FableWin.pdb name set holds
//   ??4CSpotLightDef@@QAEAAV0@ABV0@@Z. Family G058 (seven def copy assignments; catalogue "CThingBuildingDef::operator=" is propagated).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10 (bound under that name), reached through the inline implicit CSubDefClassBase::operator=.
// Shape: implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax.
#pragma optimize("s", on)
class CRGBColour { public: unsigned char B, G, R, A; };          // 4 bytes, trivial copy
class CDefClassBase {                                      // retail 0x25 bytes (+ tail padding)
public:
    virtual ~CDefClassBase();
    CDefClassBase& operator=(const CDefClassBase& other);  // 0x00431F10
    unsigned char _body[0x25 - 4];
};
class CSubDefClassBase : public CDefClassBase {           // no members: its implicit operator= inlines to the base call
};
class CSpotLightDef : public CSubDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    CRGBColour Colour;                       // +0x28
    float InnerRadius;                       // +0x2c
    float OuterRadius;                       // +0x30
    float Width;                             // +0x34
    float Flicker;                           // +0x38
    float FlickerSpeed;                      // +0x3c
    float Angle;                             // +0x40
};
void CSpotLightDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CSpotLightDef*>(other);
}