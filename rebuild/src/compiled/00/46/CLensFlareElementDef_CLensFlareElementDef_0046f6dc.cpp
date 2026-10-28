// CLensFlareElementDef::CLensFlareElementDef(const CLensFlareElementDef&) -- def copy constructor: CParentDefClassBase copy, vtable, then radius, colour, position and texture copied
// Leaf: ??0CLensFlareElementDef@@QAE@ABV0@@Z
// Retail: Fable.exe @ 0x0046F6DC, 71 bytes (/Os def module: unaligned entry), thiscall `ret 4`, returns this.
// Identity: the stored vtable 0x01236174 is RTTI .?AVCLensFlareElementDef@@; PDB name set
// `??0CLensFlareElementDef@@QAE@ABV0@@Z`; callers are the def copy / clone paths 0x0046F66B, 0x0046F7B2 and the
// vector<CLensFlareElementDef> copy helpers 0x00476B97..0x00476DB1.
// Callee: CParentDefClassBase::CParentDefClassBase(const CParentDefClassBase&) 0x0046EE98 (bound `??0CParentDefClassBase@@QAE@ABV0@@Z`).
// Layout: Ego_r.pdb CLensFlareElementDef - 4 (the base's container): Radius +0x3C, Colour (CRGBColour) +0x40,
// Position +0x44, Texture +0x48.
// Shape: a memberwise copy; CRGBColour's Set and copy constructor must be __forceinline (the /Os TU leaves a plain inline
// copy constructor out of line); CRGBColour's copy constructor is the channel-wise Set(R, G, B, A) as in the landed
// CEnginePrimitiveParticleDecal ctor 0x00B1FE80 (four byte loads into al/bl/cl/dl, then the four stores).
#pragma optimize("s", on)
class CParentDefClassBase { public:                      // retail 0x3C bytes
    CParentDefClassBase(const CParentDefClassBase& other);   // 0x0046EE98
    virtual ~CParentDefClassBase();
    unsigned char _pad04[0x38];
};
class CRGBColour { public:                               // the union's channel view: D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
class CLensFlareElementDef : public CParentDefClassBase { public:
    CLensFlareElementDef(const CLensFlareElementDef& other);
    float      Radius;                                   // +0x3C
    CRGBColour Colour;                                   // +0x40
    float      Position;                                 // +0x44
    long       Texture;                                  // +0x48
};
CLensFlareElementDef::CLensFlareElementDef(const CLensFlareElementDef& other)
    : CParentDefClassBase(other), Radius(other.Radius), Colour(other.Colour), Position(other.Position), Texture(other.Texture)
{
}