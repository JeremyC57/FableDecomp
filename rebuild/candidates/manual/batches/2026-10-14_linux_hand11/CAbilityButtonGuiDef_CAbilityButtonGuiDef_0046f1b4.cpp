#pragma optimize("s", on)
// CAbilityButtonGuiDef::CAbilityButtonGuiDef(const CAbilityButtonGuiDef&) -- copy constructor: base CParentDefClassBase(other), own vtable, eight dword members +0x3c..+0x58 copied memberwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046F1B4, length 75 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (one register for every copy pair).
// Callees: CParentDefClassBase::CParentDefClassBase(const CParentDefClassBase&) @ 0x0046EE98.
// Vtable 0x0123610C = CAbilityButtonGuiDef (rtti-vtables.tsv).
// Shape: the base copy constructor runs out of line, this class's vtable is stored, then the
// eight 4-byte members are copied one register at a time (one load/store pair each) -- the
// memberwise shape of a user-written copy constructor (an implicit one would block-copy).
// The retail CParentDefClassBase is 0x3c bytes (the PDB header says 0x3d because its Ego
// CVectorMap is 4 bytes wider than the retail one), so the base is modelled locally.
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _body[0x38];                                   // +0x04..+0x3c
    CParentDefClassBase(const CParentDefClassBase& other);       // 0x0046EE98
};

struct CAbilityButtonGuiDef : CParentDefClassBase {              // vtable 0x0123610C
    long m_buttonX;                                              // +0x3c
    long m_buttonY;                                              // +0x40
    long m_buttonWidth;                                          // +0x44
    long m_buttonHeight;                                         // +0x48
    long m_iconX;                                                // +0x4c
    long m_iconY;                                                // +0x50
    long m_iconWidth;                                            // +0x54
    long m_iconHeight;                                           // +0x58
    virtual void v00();
    CAbilityButtonGuiDef(const CAbilityButtonGuiDef& other);
};

CAbilityButtonGuiDef::CAbilityButtonGuiDef(const CAbilityButtonGuiDef& other)
    : CParentDefClassBase(other)
    , m_buttonX(other.m_buttonX)
    , m_buttonY(other.m_buttonY)
    , m_buttonWidth(other.m_buttonWidth)
    , m_buttonHeight(other.m_buttonHeight)
    , m_iconX(other.m_iconX)
    , m_iconY(other.m_iconY)
    , m_iconWidth(other.m_iconWidth)
    , m_iconHeight(other.m_iconHeight)
{
}
