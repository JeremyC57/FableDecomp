// CEngineInternalPrimitiveDecal::Update(const CEnginePrimitive& primitive, unsigned long flags) -- copies a decal descriptor into the internal decal: primitive flags, landscape-only flag, decal index/frame, position, size, colour, orientation set, blend mode/op, stencil height and the Lit flag bit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBA290, length 157 bytes (`ret 8`). No callees.
// Layout: descriptor = PDB CEnginePrimitiveDecal (88 bytes over the 12-byte CEnginePrimitive), target = PDB
// CEngineInternalPrimitiveDecal (148 bytes; EnginePrimitiveFlags +0x0C of the base); all offsets agree with the PDB.
// Shape: the struct members (WorldPos C3DVector, RHSet CRightHandedSet) copy through `lea` + edi, the C2DVector and colour word by
// word; the Lit flag is bit 7 of the descriptor's Flags word read as an UNSIGNED SHORT bit field (`mov al,[eax+54h] ; shr al,7 ;
// and al,1`): an unsigned char unit drops the `and`, an unsigned long unit loads the whole dword.
struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CRightHandedSet { C3DVector Forward; C3DVector Up; };   // 24 bytes

class CEnginePrimitive {                           // PDB, 12 bytes
public:
    long          Type;
    long          InterpolationSet;
    unsigned long ThemeChannelDesc;
};

class CEnginePrimitiveDecal : public CEnginePrimitive {   // PDB, 88 bytes
public:
    bool            LandscapeOnlyDecal;            // +0x0c
    long            DecalIndex;                    // +0x10
    long            DecalFrame;                    // +0x14
    C3DVector       WorldPos;                      // +0x18
    C2DVector       Size;                          // +0x24
    CRGBColour      Colour;                        // +0x2c
    CRightHandedSet RHSet;                         // +0x30
    long            BlendMode;                     // +0x48 EEnginePrimitiveBlendMode
    long            BlendOp;                       // +0x4c EEnginePrimitiveBlendOp
    float           StencilCubeHeight;             // +0x50
    unsigned short  FlagBits0 : 7;                 // +0x54 Flags (low bits)
    unsigned short  Lit : 1;                       //       bit 7
};

class CEngineInternalPrimitiveDecal {              // PDB, 148 bytes (over the 0x48-byte CEngineInternalPrimitiveBase)
public:
    virtual void v000();
    unsigned char   _pad04[8];
    unsigned long   EnginePrimitiveFlags;          // +0x0c (base)
    unsigned char   _pad10[0x38];
    bool            LandscapeOnlyDecal;            // +0x48
    long            DecalIndex;                    // +0x4c
    long            DecalFrame;                    // +0x50
    C3DVector       WorldPos;                      // +0x54
    C2DVector       Size;                          // +0x60
    CRGBColour      Colour;                        // +0x68
    CRightHandedSet RHSet;                         // +0x6c
    long            BlendMode;                     // +0x84
    long            BlendOp;                       // +0x88
    bool            Lit;                           // +0x8c
    float           StencilCubeHeight;             // +0x90
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitiveDecal::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    const CEnginePrimitiveDecal& decal = static_cast<const CEnginePrimitiveDecal&>(primitive);
    EnginePrimitiveFlags = flags;
    LandscapeOnlyDecal = decal.LandscapeOnlyDecal;
    DecalIndex = decal.DecalIndex;
    DecalFrame = decal.DecalFrame;
    WorldPos = decal.WorldPos;
    Size = decal.Size;
    Colour = decal.Colour;
    RHSet = decal.RHSet;
    BlendMode = decal.BlendMode;
    BlendOp = decal.BlendOp;
    StencilCubeHeight = decal.StencilCubeHeight;
    Lit = decal.Lit;
}