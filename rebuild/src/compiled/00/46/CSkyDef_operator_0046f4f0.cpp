// CSkyDef::operator=(const CSkyDef&) -- memberwise copy: CParentDefClassBase part, the thirteen sun/moon/star scalars, then the lens-flare element vector; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046F4F0, length 117 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every scalar goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<CLensFlareElementDef>::operator= @ 0x0046F565.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of CSkyDef's
// vtable 0x0123353C is the 5-byte `jmp 0x0046F4F0` right before this body. The PDB CSkyDef members sit 4
// bytes lower in retail (12-byte retail SubDefInfoMap): SunRadius +0x3c .. TwinkleMax +0x70,
// FlareElements +0x74 (retail size 0x84).
// Shape: compiler-style memberwise assignment written out -- base part out of line, then each member in
// declaration order; the vector goes through its out-of-line operator=.
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes, 16-byte map); the retail base is
// 0x39 bytes, so it is modelled locally as a packed polymorphic base.
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CLensFlareElementDef;
struct CLensFlareElementDefVector {         // retail std::vector<CLensFlareElementDef> (12 bytes)
    CLensFlareElementDef* m_first;
    CLensFlareElementDef* m_last;
    CLensFlareElementDef* m_end;
    CLensFlareElementDefVector& operator=(const CLensFlareElementDefVector& other);   // 0x0046F565
};

struct CSkyDef : CParentDefClassBase {      // vtable 0x0123353C
    long  SunRadius;                        // +0x3c
    long  SunTexture;                       // +0x40
    long  MoonRadius;                       // +0x44
    long  MoonTexture;                      // +0x48
    long  SunFlareRadius;                   // +0x4c
    long  SunFlareTexture;                  // +0x50
    long  StarTexture;                      // +0x54
    long  StarChartTextureSize;             // +0x58
    float StarSize;                         // +0x5c
    long  StarChartFilter;                  // +0x60
    long  TwinkleInterval;                  // +0x64
    float TwinkleSpeed;                     // +0x68
    float TwinkleMin;                       // +0x6c
    float TwinkleMax;                       // +0x70
    CLensFlareElementDefVector FlareElements;   // +0x74
    CSkyDef& operator=(const CSkyDef& other);
};

CSkyDef& CSkyDef::operator=(const CSkyDef& other)
{
    CParentDefClassBase::operator=(other);
    SunRadius = other.SunRadius;
    SunTexture = other.SunTexture;
    MoonRadius = other.MoonRadius;
    MoonTexture = other.MoonTexture;
    SunFlareRadius = other.SunFlareRadius;
    SunFlareTexture = other.SunFlareTexture;
    StarTexture = other.StarTexture;
    StarChartTextureSize = other.StarChartTextureSize;
    StarSize = other.StarSize;
    StarChartFilter = other.StarChartFilter;
    TwinkleInterval = other.TwinkleInterval;
    TwinkleSpeed = other.TwinkleSpeed;
    TwinkleMin = other.TwinkleMin;
    TwinkleMax = other.TwinkleMax;
    FlareElements = other.FlareElements;
    return *this;
}