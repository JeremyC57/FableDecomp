// CTCLight::SetColour(const CRGBColour& colour) -- while overridden, change the shared light info's colour: a light that was black (0,0,0,255) becomes active, one set to black inactive, then refresh the owner's appearance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640BB0, length 136 bytes (`ret 4`).
// Callees: the CThing forwarder @ 0x004C7590 (request 4 to the graphic appearance; manifest "CRenderTarget::DetachTarget" is
// propagated), CThing::AddToRenderList @ 0x004C7AC0.
// Layout (retail = PDB CTCLight, see landed ~CTCLight 0x00640F80): ParentThing +0x04, PLightInfo +0x10 (CCountedPointer: Data,
// Info), Active +0x1C, Overridden +0x1D; CTCLight::CLightInfo (0x18): Colour +0, InnerRadius +4, OuterRadius +8, Flicker +0xC,
// LightingChannelsEffected +0x10, Inverted +0x14. CRGBColour is D3DCOLOR byte order (B,G,R,A), compared R,G,B,A (+2,+1,+0,+3).
// Shape: member `operator!=` / `operator==` of CRGBColour inlined against the colour argument and a black constant (the 0xFF held in dl
// for both black tests); the 4-byte colour assignment is one dword copy.
class CThing { public:
    void ForwardToGraphicAppearance(long request);  // 0x004C7590
    void AddToRenderList();                         // 0x004C7AC0
};
class CRGBColour { public:                                 // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    bool operator==(const CRGBColour& o) const { return R == o.R && G == o.G && B == o.B && A == o.A; }
    bool operator!=(const CRGBColour& o) const { return !(*this == o); }
};
struct CLightInfo {                                 // CTCLight::CLightInfo
    CRGBColour    Colour;                           // +0x00
    float         InnerRadius;                      // +0x04
    float         OuterRadius;                      // +0x08
    float         Flicker;                          // +0x0c
    unsigned long LightingChannelsEffected;         // +0x10
    bool          Inverted;                         // +0x14
};
class CCPPointerInfo;
template<class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};
class CLightDef;
class CTCLight { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _base08[0x10 - 0x08];             // CTCBase flags, CAGraphicAppearanceComponent vptr +0x0c
    CCountedPointer<CLightInfo> PLightInfo;         // +0x10
    const CLightDef* PLightDef;                     // +0x18
    bool  Active;                                   // +0x1c
    bool  Overridden;                               // +0x1d
    void SetColour(const CRGBColour& colour);
};

void CTCLight::SetColour(const CRGBColour& colour)
{
    if (Overridden) {
        if (PLightInfo->Colour != colour) {
            if (PLightInfo->Colour == CRGBColour(0, 0, 0, 255))
                Active = true;
            PLightInfo->Colour = colour;
            if (PLightInfo->Colour == CRGBColour(0, 0, 0, 255))
                Active = false;
            ParentThing->ForwardToGraphicAppearance(4);
            ParentThing->AddToRenderList();
        }
    }
}