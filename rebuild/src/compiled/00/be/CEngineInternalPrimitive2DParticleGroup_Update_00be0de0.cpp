// CEngineInternalPrimitive2DParticleGroup::Update(const CEnginePrimitive&, unsigned long) -- copy the 2D particle-group description (flags, layer, inverse transform, 2D position) into the internal primitive, place it at the layer's depth, centre a unit bounding sphere on it and reset the camera distance, water stage and z-write flag
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE0DE0, 196 bytes (+ int3 padding), `ret 8`. Mangled
//   ?Update@CEngineInternalPrimitive2DParticleGroup@@UAEXABVCEnginePrimitive@@K@Z (the key argument is unused here).
// Callees: the 2D renderer (global 0x01436E7C) layer-depth getter 0x00B4A6B0 (`fld [this + layer*4 + 88h]`, ret 4; the
//   manifest calls it Layer2DToDistance, a name the landed code gives to the neighbouring 0x00B4A6A0 table at +0x1d4).
// Layout: the argument is a CEnginePrimitive2DParticleGroup (Ego_r.pdb: Layer +0x0c, ScreenDisplacement +0x10, ReadZBuffer +0x11,
//   InverseTransform +0x14, C2DVector Position +0x44). `this` (retail, 0x14 below the PDB from the particle-group lists on,
//   as noted for 0x00BBBE60): CEngineInternalPrimitiveBase Alpha +0x27 (PDB agrees); CEngineInternalPrimitiveParticleGroup
//   BSphere +0x5c, DistanceFromCam +0x6c, ScreenDisplacement +0xa0, ReadZBuffer +0xa1, WaterStage +0xa4, ZBufferWritable +0xa8;
//   CEngineInternalPrimitive2DParticleGroup Layer +0xac, InverseTransform +0xb0, Pos +0xe0 (PDB +0xc0/+0xc4/+0xf4).
//   engine/CEngineInternalPrimitiveParticleGroup.h carries the PDB offsets, so `this` is a local model with those names.
// Shape: the matrix copies as one 48-byte block (`rep movsd`); the sphere is built as a temporary C3DBoundingSphere(Pos, 1.0f)
//   in the frame and assigned (integer copies through `lea eax,[ebx+5Ch]`); C3DVector needs its memberwise user copy ctor
//   for the centre to go through the frame temporary (the implicit copy forwards Pos straight into BSphere); plain stores
//   in source order otherwise.
struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CMatrix3x4 { float E[12]; };

struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere() { }
    C3DBoundingSphere(const C3DVector& centre, float radius) : Center(centre), Radius(radius) { }
};

struct CEnginePrimitive {
    long Type;                                         // +0x00
    long InterpolationSet;                             // +0x04
    unsigned char ThemeChannelDesc[4];                 // +0x08
};
struct CEnginePrimitive2DParticleGroup : CEnginePrimitive {
    long       Layer;                                  // +0x0c (EEngine2DLayer)
    bool       ScreenDisplacement;                     // +0x10
    bool       ReadZBuffer;                            // +0x11
    CMatrix3x4 InverseTransform;                       // +0x14
    C2DVector  Position;                               // +0x44
};

struct CEnginePrimitiveRenderer2D {
    float GetLayerDepth(long layer);                   // 0x00B4A6B0
};
extern CEnginePrimitiveRenderer2D* g_pPrimitiveRenderer2D;   // 0x01436E7C

struct CEngineInternalPrimitive2DParticleGroup {
    virtual void Update(const CEnginePrimitive& primitive, unsigned long key);
    unsigned char     _pad04[0x27 - 4];
    unsigned char     Alpha;                           // +0x27
    unsigned char     _pad28[0x5c - 0x28];
    C3DBoundingSphere BSphere;                         // +0x5c
    float             DistanceFromCam;                 // +0x6c
    CMatrix3x4        WorldMat;                        // +0x70
    bool              ScreenDisplacement;              // +0xa0
    bool              ReadZBuffer;                     // +0xa1
    long              WaterStage;                      // +0xa4
    bool              ZBufferWritable;                 // +0xa8
    bool              ClipEffectToWeatherMask;         // +0xa9
    bool              EnableDithering;                 // +0xaa
    long              Layer;                           // +0xac
    CMatrix3x4        InverseTransform;                // +0xb0
    C3DVector         Pos;                             // +0xe0
};

void CEngineInternalPrimitive2DParticleGroup::Update(const CEnginePrimitive& primitive, unsigned long key)
{
    const CEnginePrimitive2DParticleGroup& group = static_cast<const CEnginePrimitive2DParticleGroup&>(primitive);
    ScreenDisplacement = group.ScreenDisplacement;
    ReadZBuffer = group.ReadZBuffer;
    Alpha = 0xff;
    Layer = group.Layer;
    InverseTransform = group.InverseTransform;
    Pos.X = group.Position.X;
    Pos.Y = group.Position.Y;
    Pos.Z = g_pPrimitiveRenderer2D->GetLayerDepth(group.Layer);
    BSphere = C3DBoundingSphere(Pos, 1.0f);
    DistanceFromCam = 0.0f;
    WaterStage = 2;
    ZBufferWritable = false;
}