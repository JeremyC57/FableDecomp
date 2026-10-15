// CEnvironmentThemeDaySetDef::operator=(const CEnvironmentThemeDaySetDef&) -- memberwise assignment: base CParentDefClassBase::operator=, the TimeTheme array by its operator=, then the sun/moon angles, colour column, editor colour and fish weight; returns *this
// Leaf: ??4CEnvironmentThemeDaySetDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00472E83, length 87 bytes.
// Pragma: #pragma optimize("s", on) (one register, eax, for every copy pair).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0,
//          CArray<CEnvironmentThemeDef>::operator= @ 0x00472EDA.
// Identity: the manifest label "CParentDefClassBase::operator=" is propagated. The body is reached
// through a `jmp` thunk at 0x00472E7E in slot 19 of vtable 0x0123360C = CEnvironmentThemeDaySetDef
// (rtti-vtables.tsv). The members are the PDB ones 4 bytes lower (retail CParentDefClassBase 0x3c,
// PDB 0x40) with the 12-byte retail CArray (PDB 16): TimeTheme +0x3c, SunTilt +0x48 .. MoonHeight
// +0x5c, ColourLookupColumn +0x60, EditorColour +0x64, FishWeightMult +0x68. The generated header
// engine/CEnvironmentThemeDaySetDef.h carries the PDB offsets (+0x40..), so the class is modelled
// locally.
// Shape: compiler-generated memberwise assignment; the floats and the 4-byte CRGBColour record are
// copied with integer load/store pairs (plain assignment never touches the FPU).
#pragma optimize("s", on)   // the def module is size-optimised: every copy pair goes through eax

struct CParentDefClassBase {
    virtual void v00();
    unsigned char _body[0x38];                                            // +0x04..+0x3c
    CParentDefClassBase& operator=(const CParentDefClassBase& other);     // 0x00431ED0
};

struct CEnvironmentThemeDefArray {                                        // retail CArray<CEnvironmentThemeDef> (12 bytes)
    void* m_pData;
    long  m_size;
    long  m_capacity;
    CEnvironmentThemeDefArray& operator=(const CEnvironmentThemeDefArray& other);   // 0x00472EDA
};

struct CRGBColour {
    unsigned char B, G, R, A;
};

struct CEnvironmentThemeDaySetDef : CParentDefClassBase {                 // vtable 0x0123360C
    CEnvironmentThemeDefArray TimeTheme;                                  // +0x3c
    float                     SunTilt;                                    // +0x48
    float                     SunRotate;                                  // +0x4c
    float                     SunHeight;                                  // +0x50
    float                     MoonTilt;                                   // +0x54
    float                     MoonRotate;                                 // +0x58
    float                     MoonHeight;                                 // +0x5c
    long                      ColourLookupColumn;                         // +0x60
    CRGBColour                EditorColour;                               // +0x64
    float                     FishWeightMult;                             // +0x68
    virtual void v00();
    CEnvironmentThemeDaySetDef& operator=(const CEnvironmentThemeDaySetDef& other);
};

CEnvironmentThemeDaySetDef& CEnvironmentThemeDaySetDef::operator=(const CEnvironmentThemeDaySetDef& other)
{
    CParentDefClassBase::operator=(other);
    TimeTheme = other.TimeTheme;
    SunTilt = other.SunTilt;
    SunRotate = other.SunRotate;
    SunHeight = other.SunHeight;
    MoonTilt = other.MoonTilt;
    MoonRotate = other.MoonRotate;
    MoonHeight = other.MoonHeight;
    ColourLookupColumn = other.ColourLookupColumn;
    EditorColour = other.EditorColour;
    FishWeightMult = other.FishWeightMult;
    return *this;
}