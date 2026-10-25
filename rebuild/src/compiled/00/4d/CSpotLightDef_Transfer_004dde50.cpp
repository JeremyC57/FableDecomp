// CSpotLightDef::Transfer(CPersistContext& context) -- persist the def's fields one by one (Colour, InnerRadius, OuterRadius, Width, Flicker, FlickerSpeed, Angle)
// Leaf: ?Transfer@CSpotLightDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DDE50, 90 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CSpotLightDef::Transfer(CPersistContext&) -- proven; evidence: the catalogue row is this virtual (manifest
//   ?Transfer@CSpotLightDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb name set); the seven transferred offsets and their callee
//   overloads match Ego_r.pdb CSpotLightDef's members and types exactly (retail offset = Ego_r offset). Family G061 (seven def Transfers of seven fields; the G057 recipe).
// Callees: CPersistContext::Transfer(unsigned long&) 0x00431020, Transfer(long&) 0x00431102, Transfer(float&) 0x00431061,
//   Transfer(bool&) 0x0043314A (each bound under that name by landed defs); member-template instances Transfer<CRGBColour> 0x004595A3.
// Shape: context in edi and this in esi for the whole body; the last field's address is `add esi, N` in place.
#pragma optimize("s", on)
class CRGBColour { public: unsigned char B, G, R, A; };          // 4 bytes
class CPersistContext {
public:
    void Transfer(unsigned long& value);                   // 0x00431020
    void Transfer(long& value);                            // 0x00431102
    void Transfer(float& value);                           // 0x00431061
    void Transfer(bool& value);                            // 0x0043314A
    template <class T> void Transfer(T& value);            // enum / class instances in this TU
};
class CSpotLightDef {                                              // retail fields from +0x28
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x24];                    // +0x04 def base body
    CRGBColour Colour;                       // +0x28
    float InnerRadius;                       // +0x2c
    float OuterRadius;                       // +0x30
    float Width;                             // +0x34
    float Flicker;                           // +0x38
    float FlickerSpeed;                      // +0x3c
    float Angle;                             // +0x40
};
void CSpotLightDef::Transfer(CPersistContext& context)
{
    context.Transfer(Colour);
    context.Transfer(InnerRadius);
    context.Transfer(OuterRadius);
    context.Transfer(Width);
    context.Transfer(Flicker);
    context.Transfer(FlickerSpeed);
    context.Transfer(Angle);
}