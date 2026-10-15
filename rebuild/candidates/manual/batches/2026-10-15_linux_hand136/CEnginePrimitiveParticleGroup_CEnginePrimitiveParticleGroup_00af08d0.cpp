// CEnginePrimitiveParticleGroup::CEnginePrimitiveParticleGroup(float, const C3DBoundingSphere&, bool, bool, EEngineEffectWaterStage, bool, bool, bool) -- primitive type 0x18: camera distance, render flags, water stage and bounding sphere
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF08D0, length 101 bytes. No callees.
// Signature from Ego_r.pdb: ??0CEnginePrimitiveParticleGroup@@QAE@MABVC3DBoundingSphere@@_N1W4EEngineEffectWaterStage@@111@Z
// (8 stack words, `ret 20h`). Layout = PDB CEnginePrimitiveParticleGroup (0x2c): CEnginePrimitive base
// {Type +0, InterpolationSet +4, ThemeChannelDesc +8}, DistanceFromCam +0xc, ScreenDisplacement +0x10,
// ReadZBuffer +0x11, WaterStage +0x14, WaterEffect +0x18 (left uninitialised: no parameter feeds it),
// ZBufferWritable +0x19, ClipEffectToWeatherMask +0x1a, EnableDithering +0x1b, BSphere +0x1c.
// Shape: the base's inline constructor (default interpolation set 0) stores the zero word and the three channel
// bytes from one zero register; the bounding sphere is copied memberwise (C3DVector has a user copy
// constructor, so the implicit sphere copy goes member by member through one register per dword).

struct C3DVector {                                       // memberwise user copy constructor: one register per dword
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct C2DVector {
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
struct CRGBColour {                                      // the union's channel view: D3DCOLOR byte order
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

struct CEnginePrimitiveThemeChannelDesc {                // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
struct CEnginePrimitive {                                // 12 bytes (PDB)
    long                             Type;               // +0x00 EEnginePrimitiveType
    long                             InterpolationSet;   // +0x04 EEngineInterpolationSet
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    CEnginePrimitive(long type, long interpolationSet = 0) : Type(type), InterpolationSet(interpolationSet) { }
};

struct C3DBoundingSphere { C3DVector Center; float Radius; };

struct CEnginePrimitiveParticleGroup : CEnginePrimitive {
    float             DistanceFromCam;                   // +0x0c
    bool              ScreenDisplacement;                // +0x10
    bool              ReadZBuffer;                       // +0x11
    long              WaterStage;                        // +0x14 EEngineEffectWaterStage
    bool              WaterEffect;                       // +0x18
    bool              ZBufferWritable;                   // +0x19
    bool              ClipEffectToWeatherMask;           // +0x1a
    bool              EnableDithering;                   // +0x1b
    C3DBoundingSphere BSphere;                           // +0x1c   sizeof == 0x2c
    CEnginePrimitiveParticleGroup(float distanceFromCam, const C3DBoundingSphere& bSphere, bool screenDisplacement,
                                  bool readZBuffer, long waterStage, bool zBufferWritable,
                                  bool clipEffectToWeatherMask, bool enableDithering);
};

CEnginePrimitiveParticleGroup::CEnginePrimitiveParticleGroup(float distanceFromCam, const C3DBoundingSphere& bSphere,
                                                             bool screenDisplacement, bool readZBuffer, long waterStage,
                                                             bool zBufferWritable, bool clipEffectToWeatherMask,
                                                             bool enableDithering)
    : CEnginePrimitive(0x18)                             // ENGINE_PRIMITIVE_PARTICLE_GROUP
    , DistanceFromCam(distanceFromCam)
    , ScreenDisplacement(screenDisplacement)
    , ReadZBuffer(readZBuffer)
    , WaterStage(waterStage)
    , ZBufferWritable(zBufferWritable)
    , ClipEffectToWeatherMask(clipEffectToWeatherMask)
    , EnableDithering(enableDithering)
    , BSphere(bSphere)
{
}
