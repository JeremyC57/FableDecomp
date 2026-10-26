// CEngineInternalPrimitiveFlareSprite::Update(const CEnginePrimitive& primitive, unsigned long flags) -- applies a flare-sprite descriptor: builds the equivalent plain sprite descriptor (additive blend 4, white with the flare's alpha) and runs the sprite update on it, then keeps the light-source radius and minimum light scale
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCDFB0, length 123 bytes (`ret 8`).
// Callees: CEnginePrimitiveSprite::CEnginePrimitiveSprite(15 arguments) @ 0x006558CE (on the 72-byte frame local),
// CEngineInternalPrimitiveSprite::Update(const CEnginePrimitive&, unsigned long) @ 0x00BAD670 (qualified, non-virtual call).
// Layout: descriptor = PDB CEnginePrimitiveFlareSprite (68 bytes: WorldPos +0x0C, BankIndex +0x18, Frame +0x1C, LightSourceRadius
// +0x20, SizeX/SizeY +0x24/+0x28, MinScale +0x2C, Angle +0x34, Alpha +0x40); target PDB CEngineInternalPrimitiveFlareSprite
// (LightSourceRadius +0x88, MinLightScale +0x8C); EnginePrimitiveFlags +0x0C of the primitive base. The sprite constructor's
// parameter names are by position (manifest types: ref, ulong, ulong, ushort, float x3, blend mode, uchar x4, float, set, op).
// Shape: arguments pushed right to left with the float zero as `push 0`, the alpha byte zero-extended (`xor eax,eax ; mov al,..`).
// Leaf: ?Update@CEngineInternalPrimitiveFlareSprite
class C3DVector { public: float X, Y, Z; };

class CEnginePrimitive {                           // PDB, 12 bytes
public:
    long          Type;
    long          InterpolationSet;
    unsigned long ThemeChannelDesc;
};

class CEnginePrimitiveSprite : public CEnginePrimitive {   // PDB, 72 bytes
public:
    unsigned char _body[72 - 12];
    CEnginePrimitiveSprite(const C3DVector& position, unsigned long bankIndex, unsigned long frame, unsigned short flags,
                           float angle, float sizeX, float sizeY, long blendMode,
                           unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue,
                           float fadeStart, long interpolationSet, long blendOp);   // 0x006558CE
};

class CEnginePrimitiveFlareSprite : public CEnginePrimitive {   // PDB, 68 bytes
public:
    C3DVector     WorldPos;                        // +0x0c
    unsigned long BankIndex;                       // +0x18
    unsigned long Frame;                           // +0x1c
    float         LightSourceRadius;               // +0x20
    float         SizeX;                           // +0x24
    float         SizeY;                           // +0x28
    float         MinScale;                        // +0x2c
    float         FadeStart;                       // +0x30
    float         Angle;                           // +0x34
    long          BlendMode;                       // +0x38
    long          Identifier;                      // +0x3c
    unsigned char Alpha;                           // +0x40
};

class CEngineInternalPrimitiveSprite {
public:
    virtual void v000();
    unsigned char _pad04[8];
    unsigned long EnginePrimitiveFlags;            // +0x0c (CEngineInternalPrimitiveBase)
    unsigned char _pad10[0x78];
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);   // 0x00BAD670
};

class CEngineInternalPrimitiveFlareSprite : public CEngineInternalPrimitiveSprite {   // PDB, 156 bytes
public:
    float         LightSourceRadius;               // +0x88
    float         MinLightScale;                   // +0x8c
    float         FadeIn;                          // +0x90
    unsigned long FlareVisibilityTestHandle;       // +0x94
    unsigned long FlareVisibilityTestFrame;        // +0x98
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitiveFlareSprite::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    const CEnginePrimitiveFlareSprite& flare = static_cast<const CEnginePrimitiveFlareSprite&>(primitive);
    EnginePrimitiveFlags = flags;
    CEnginePrimitiveSprite sprite(flare.WorldPos, flare.BankIndex, flare.Frame, 0, flare.Angle, flare.SizeX, flare.SizeY, 4,
                                  flare.Alpha, 0xff, 0xff, 0xff, 0.0f, 0, 0);
    CEngineInternalPrimitiveSprite::Update(sprite, flags);
    LightSourceRadius = flare.LightSourceRadius;
    MinLightScale = flare.MinScale;
}