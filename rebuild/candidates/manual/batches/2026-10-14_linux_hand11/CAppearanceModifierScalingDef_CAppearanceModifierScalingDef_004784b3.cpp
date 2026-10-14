#pragma optimize("s", on)
// CAppearanceModifierScalingDef::CAppearanceModifierScalingDef(const CAppearanceModifierScalingDef&) -- copy constructor: base CParentDefClassBase(other), own vtable, three dword members +0x3c..+0x44 copied memberwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004784B3, length 45 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (one register for every copy pair).
// Callees: CParentDefClassBase::CParentDefClassBase(const CParentDefClassBase&) @ 0x0046EE98.
// The manifest label (CAbilityTrackLevelDef) is propagated: the vtable stored, 0x01236434, is
// CAppearanceModifierScalingDef in rtti-vtables.tsv. Memberwise one-register copies = a user
// copy constructor. Retail CParentDefClassBase is 0x3c bytes (local model, see 0046f1b4).
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _body[0x38];                                   // +0x04..+0x3c
    CParentDefClassBase(const CParentDefClassBase& other);       // 0x0046EE98
};

struct CAppearanceModifierScalingDef : CParentDefClassBase {     // vtable 0x01236434
    long m_scaleX;                                               // +0x3c
    long m_scaleY;                                               // +0x40
    long m_scaleZ;                                               // +0x44
    virtual void v00();
    CAppearanceModifierScalingDef(const CAppearanceModifierScalingDef& other);
};

CAppearanceModifierScalingDef::CAppearanceModifierScalingDef(const CAppearanceModifierScalingDef& other)
    : CParentDefClassBase(other)
    , m_scaleX(other.m_scaleX)
    , m_scaleY(other.m_scaleY)
    , m_scaleZ(other.m_scaleZ)
{
}
