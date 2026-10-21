// NParticleEngine::CPSCSingleSprite::CPSCSingleSprite() -- construct the single-sprite render component: white start/mid/end colours, unit sizes and animation time, default blend modes, fade/trail settings and empty primitive state
// Leaf: ??0CPSCSingleSprite
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B09360, length 321 bytes (padded to 336); vtable 0x0129FB84.
// Callees: NParticleEngine::CPSCSplineBase::CPSCSplineBase() @ 0x00B21530 (base), CEnginePrimitiveSpriteTrail ctor @ 0x00B09F90
//          (manifest label "C3DPrimitive2"), CParticleEmitterManager::Get() @ 0x00AEAAF0 (returns the instance @ 0x013D2E1C;
//          manifest label "GFGetPreMainMemoryUsed" is a byte twin) four times with the result unused.
// Layout: PDB (Ego_r) CPSCSingleSprite (0x148) members sit 4 bytes lower in retail (retail CPSCSplineBase is 0x34, its spline
//   4 bytes smaller): SpriteBankIndex +0x34, Start/Mid/EndColour +0x38/+0x3c/+0x40, AnimationTimeSecs +0x44 ... StayWithEmitter
//   bit +0x94, UsePosition +0x95, LastUpdateFrame +0x98, Lifetimer +0x9c, Offset +0xa0, TrailUpdatePrimitive +0xac (0x80 in
//   retail), PrimIDList +0x12c (12-byte CArray). No engine/ header exists for the class.
// Shape: the init list builds the colours (CRGBColour ctor storing R, G, B, A), the three Use*Colour bools, the zeroed Offset,
//   the trail primitive and the empty CArray; the body then assigns defaults. The blend-mode/alignment defaults are read as
//   enumerators through the emitter manager instance (`CParticleEmitterManager::Get()->X`), which keeps each Get() call even
//   though the value is a constant; the calls split the body into the store groups retail shows.
class CParticleEmitterManager {
public:
    enum { SPRITE_ALIGN_DEFAULT = 0, BLEND_MODE_DEFAULT = 2, BLEND_OP_DEFAULT = 0, TRAIL_BLEND_OP_DEFAULT = 0 };
    static CParticleEmitterManager* Get();          // 0x00AEAAF0
};

struct CRGBColour {
    unsigned char B;                                // +0
    unsigned char G;                                // +1
    unsigned char R;                                // +2
    unsigned char A;                                // +3
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct CEnginePrimitiveSpriteTrail {
    unsigned char _body[0x80];
    CEnginePrimitiveSpriteTrail();                  // 0x00B09F90
};

template <class T> struct CArray {
    T*   Data;
    long Size;
    long Capacity;
    CArray() : Data(0), Size(0), Capacity(0) {}
};

namespace NParticleEngine {

struct CPSCSplineBase {
    virtual ~CPSCSplineBase();
    unsigned char _base[0x30];                      // +0x04 CPSCBase part, SplineAnimSpeed, Spline, counted pointers
    CPSCSplineBase();                               // 0x00B21530
};

struct CPSCSingleSprite : CPSCSplineBase {          // vtable 0x0129FB84
    long          SpriteBankIndex;                  // +0x34
    CRGBColour    StartColour;                      // +0x38
    CRGBColour    MidColour;                        // +0x3c
    CRGBColour    EndColour;                        // +0x40
    float         AnimationTimeSecs;                // +0x44
    float         StartRenderSize;                  // +0x48
    float         EndRenderSize;                    // +0x4c
    long          SpriteAlignment;                  // +0x50
    long          NoCrossedSprites;                 // +0x54
    float         InitialAngle;                     // +0x58
    long          FadeInEndInteger;                 // +0x5c
    long          FadeOutBeginInteger;              // +0x60
    float         AlphaFadeMinimum;                 // +0x64
    float         SizeFadeMinimum;                  // +0x68
    unsigned long PositionParam;                    // +0x6c
    long          BlendMode;                        // +0x70
    long          BlendOp;                          // +0x74
    long          TrailBlendMode;                   // +0x78
    long          TrailBlendOp;                     // +0x7c
    long          TrailBankIndex;                   // +0x80
    float         TrailWidth;                       // +0x84
    long          MaxTrailLength;                   // +0x88
    long          StayWithEmitterIntFactor;         // +0x8c
    bool          UseStartColour;                   // +0x90
    bool          UseMidColour;                     // +0x91
    bool          UseEndColour;                     // +0x92
    bool          SelfIlluminating : 1;             // +0x93 bit 0
    bool          AlphaFadeEnable : 1;              //        bit 1
    bool          RotateAroundCentre : 1;           //        bit 2
    bool          FaceMe2D : 1;                     //        bit 3
    bool          FaceMe3D : 1;                     //        bit 4
    bool          CrossedSprites : 1;               //        bit 5
    bool          SizeFadeEnable : 1;               //        bit 6
    bool          ForceAnimationTime : 1;           //        bit 7
    bool          StayWithEmitter : 1;              // +0x94 bit 0
    bool          UsePosition;                      // +0x95
    bool          UseSplinePoints;                  // +0x96
    bool          EnteringTimelineEnd;              // +0x97
    unsigned long LastUpdateFrame;                  // +0x98
    long          Lifetimer;                        // +0x9c
    C3DVector     Offset;                           // +0xa0
    CEnginePrimitiveSpriteTrail TrailUpdatePrimitive; // +0xac
    CArray<long>  PrimIDList;                       // +0x12c

    CPSCSingleSprite();
    virtual ~CPSCSingleSprite();
};

CPSCSingleSprite::CPSCSingleSprite()
    : StartColour(255, 255, 255, 255),
      MidColour(255, 255, 255, 255),
      EndColour(255, 255, 255, 255),
      UseStartColour(true),
      UseMidColour(false),
      UseEndColour(false),
      Offset(0.0f, 0.0f, 0.0f)
{
    SpriteBankIndex = 0;
    AnimationTimeSecs = 1.0f;
    StartRenderSize = 1.0f;
    EndRenderSize = 1.0f;
    SelfIlluminating = false;
    FaceMe2D = true;
    FaceMe3D = false;
    CrossedSprites = false;
    ForceAnimationTime = false;
    SpriteAlignment = CParticleEmitterManager::Get()->SPRITE_ALIGN_DEFAULT;
    RotateAroundCentre = false;
    NoCrossedSprites = 2;
    InitialAngle = 0.0f;
    BlendMode = CParticleEmitterManager::Get()->BLEND_MODE_DEFAULT;
    BlendOp = CParticleEmitterManager::Get()->BLEND_OP_DEFAULT;
    TrailBlendMode = 4;
    TrailBlendOp = CParticleEmitterManager::Get()->TRAIL_BLEND_OP_DEFAULT;
    TrailWidth = 1.0f;
    FadeInEndInteger = 0;
    AlphaFadeMinimum = 0.0f;
    SizeFadeMinimum = 0.0f;
    TrailBankIndex = 0;
    UseSplinePoints = false;
    EnteringTimelineEnd = false;
    Lifetimer = 0;
    FadeOutBeginInteger = 1000;
    AlphaFadeEnable = true;
    SizeFadeEnable = true;
    MaxTrailLength = 10;
    UsePosition = true;
    StayWithEmitter = false;
}

} // namespace NParticleEngine
