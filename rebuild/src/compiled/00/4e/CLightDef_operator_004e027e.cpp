// CLightDef::operator=(const CLightDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CLightDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E027E, 111 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CLightDef::Copy 0x004E0279; the 15 member operations read from
//   these bytes (byte/dword copies) are Ego_r.pdb CLightDef's members in order (names and types are Ego_r.pdb's).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10.
// Shape (family M035, representative 0x00453FC9): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CLightDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
class CRGBColour { public: union { unsigned long Value; unsigned char Channel[4]; }; };   // Ego_r.pdb: one 32-bit union (B, G, R, A)
enum EPointLightChannelEffect { EPOINTLIGHTCHANNELEFFECT_NONE = 0 };
class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
    unsigned char _base[0x25];
};
class CLightDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    CRGBColour                Colour;                            // +0x28
    long                      HaloGraphic;                       // +0x2c
    float                     InnerRadius;                       // +0x30
    float                     OuterRadius;                       // +0x34
    float                     Flicker;                           // +0x38
    float                     FlickerSpeed;                      // +0x3c
    bool                      StartActive;                       // +0x40
    bool                      SunlightAttenuator;                // +0x41
    bool                      DaylightAttenuator;                // +0x42
    float                     DaylightAttenuatorFadeInStart;     // +0x44
    float                     DaylightAttenuatorFadeInEnd;       // +0x48
    float                     DaylightAttenuatorFadeOutStart;    // +0x4c
    float                     DaylightAttenuatorFadeOutEnd;      // +0x50
    EPointLightChannelEffect  EffectChannel;                     // +0x54
    bool                      Inverted;                          // +0x58
};
void CLightDef::Copy(const CDefClassBase* other)
{
    *this = *(const CLightDef*)other;
}