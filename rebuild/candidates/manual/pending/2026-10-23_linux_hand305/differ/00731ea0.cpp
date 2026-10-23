// Grade: DIFFER(553v575) -- retail keeps the component this in edi with the full object in esi=edi-0xc and ebx as a zero register (frame 0x68, flicker spilled into the info parameter slot); ours uses ebp for the appearance param, frame 0x78, and loads the colour bytes with movzx instead of cl/dl/bl/al
// CTCSpotLight::OnAppearanceDraw(CTCGraphicAppearance&, const CGraphicAppearanceDrawInfo&) -- create the light's appearance node on first draw, then hand it a particle-line primitive from the owner along the light direction, coloured by the light colour scaled by the interpolated flicker
// Leaf: OnAppearanceDraw
// Retail: Fable.exe (TLC, VC7.1) @ 0x00731EA0, 576 bytes, `ret 8`; runs with `this` at the CAGraphicAppearanceComponent
//   subobject (+0x0c), so the CTCSpotLight is `this - 0xc`.
// Callees: CBase() 0x0099A2D0, CTCGraphicAppearance::InsertAppearanceNode 0x004C0050 (catalog
//   "CArray_InsertAppearanceNode"), the node spec's destructor 0x004C4160 (catalog "~CObject"), __ftol2 0x00BFEA70,
//   CThing::GetPosition 0x004C73D0, CTCSpotLight::GetDirection 0x00731CA0 (landed), CEnginePrimitiveParticleLine ctor
//   0x007324A0, CTCGraphicAppearance::SetPrimitiveGraphics 0x004BD620.
// Floats: 1/255 0x01231724, 1.0f 0x0122DED8, 255.0f 0x01230014, 0.5f 0x0122F59C.
// Layout: Ego_r.pdb CTCSpotLight (PLightInfo.Data +0x10, PLightDef +0x18, Overridden +0x1c, LightPrimitive +0x20, the two
//   flicker factors +0x24/+0x28); CThing's 64-bit id at +0x18.
struct C3DVector { float X, Y, Z; };
struct CRGBColour {
    unsigned char B, G, R, A;
};
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour(const CRGBColour& c)
        : R(c.R * (1.0f / 255.0f)), G(c.G * (1.0f / 255.0f)), B(c.B * (1.0f / 255.0f)), A(c.A * (1.0f / 255.0f)) { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float f) const { return CRGBFloatColour(R * f, G * f, B * f, A * f); }
};

struct CBase {
    CBase();                                                // 0x0099A2D0
    virtual void v00();
};
struct CAppearanceNodeData : CBase {                        // vtable 0x0122F598
    long a, b;
    CAppearanceNodeData() : a(0), b(0) { }
    virtual void v00();
};
struct CAppearanceNodeSpec {
    long Type;
    CAppearanceNodeData Data;
    CAppearanceNodeSpec(long type) : Type(type) { }
    ~CAppearanceNodeSpec();                                 // 0x004C4160
};
struct CPrimitive;
struct CEnginePrimitive;
struct CEnginePrimitiveParticleLine {
    unsigned char _body[0x38];
    CEnginePrimitiveParticleLine(unsigned __int64 id, const C3DVector& start, const C3DVector& direction, const CRGBColour& colour,
                                 float a, float b, float c, long d, long interpolation);   // 0x007324A0
};
struct CTCGraphicAppearance {
    CPrimitive* InsertAppearanceNode(const CAppearanceNodeSpec& spec);   // 0x004C0050
    void SetPrimitiveGraphics(CPrimitive* primitive, CEnginePrimitive* engine, unsigned short flags);   // 0x004BD620
};
struct CGraphicAppearanceDrawInfo {
    float Interpolation;                                    // +0x00
};

struct CThing {
    unsigned char _pad00[0x18];
    unsigned __int64 ID;                                    // +0x18
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
struct CSpotLightInfo {
    unsigned char _pad00[4];
    float A;                                                // +0x04
    float B;                                                // +0x08
    float C;                                                // +0x0c
    unsigned char _pad10[4];
    CRGBColour Colour;                                      // +0x14
};
struct CSpotLightDef {
    unsigned char _pad00[0x28];
    CRGBColour Colour;                                      // +0x28
    float B;                                                // +0x2c
    float C;                                                // +0x30
    float A;                                                // +0x34
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};
struct CAGraphicAppearanceComponent {
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info);
};
struct CTCSpotLight : CTCBase, CAGraphicAppearanceComponent {
    CSpotLightInfo* PLightInfo;                             // +0x10 (CCountedPointer data)
    void*           PLightInfoBlock;                        // +0x14
    const CSpotLightDef* PLightDef;                         // +0x18
    bool            Overridden;                             // +0x1c
    CPrimitive*     LightPrimitive;                         // +0x20
    float           AccumilativeFlickerFactor;              // +0x24
    float           OldAccumilativeFlickerFactor;           // +0x28
    C3DVector GetDirection() const;                         // 0x00731CA0
    const CRGBColour& GetColour() const { return Overridden ? PLightInfo->Colour : PLightDef->Colour; }
    float GetA() const { return Overridden ? PLightInfo->A : PLightDef->A; }
    float GetB() const { return Overridden ? PLightInfo->B : PLightDef->B; }
    float GetC() const { return Overridden ? PLightInfo->C : PLightDef->C; }
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info);
};

bool CTCSpotLight::OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info)
{
    if (!LightPrimitive)
        LightPrimitive = appearance.InsertAppearanceNode(CAppearanceNodeSpec(3));
    CRGBFloatColour baseColour(GetColour());
    float flicker = (1.0f - info.Interpolation) * OldAccumilativeFlickerFactor + AccumilativeFlickerFactor * info.Interpolation;
    CRGBFloatColour colour = baseColour * flicker;
    CRGBColour drawColour;
    drawColour.R = (unsigned char)(colour.R * 255.0f);
    drawColour.G = (unsigned char)(colour.G * 255.0f);
    drawColour.B = (unsigned char)(colour.B * 255.0f);
    drawColour.A = (unsigned char)(colour.A * 255.0f);
    float a = GetA();
    float b = GetB();
    float c = GetC();
    C3DVector pos = ParentThing->GetPosition();
    CEnginePrimitiveParticleLine line(ParentThing->ID, pos, GetDirection(), drawColour, c, b * flicker, a * 0.5f, 0, -1);
    appearance.SetPrimitiveGraphics(LightPrimitive, (CEnginePrimitive*)&line, 0);
    return true;
}
