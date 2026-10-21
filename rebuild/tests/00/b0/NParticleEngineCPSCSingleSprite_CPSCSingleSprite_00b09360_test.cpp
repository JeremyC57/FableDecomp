// Behaviour test for NParticleEngine::CPSCSingleSprite::CPSCSingleSprite @ 0x00B09360: every default the ctor sets.
#include <cstdio>

static int g_getCalls = 0;
class CParticleEmitterManager {
public:
    enum { SPRITE_ALIGN_DEFAULT = 0, BLEND_MODE_DEFAULT = 2, BLEND_OP_DEFAULT = 0, TRAIL_BLEND_OP_DEFAULT = 0 };
    static CParticleEmitterManager* Get() { ++g_getCalls; static CParticleEmitterManager m; return &m; }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static int g_trailCtors = 0;
struct CEnginePrimitiveSpriteTrail { CEnginePrimitiveSpriteTrail() { ++g_trailCtors; } };
template <class T> struct CArray { T* Data; long Size; long Capacity; CArray() : Data(0), Size(0), Capacity(0) {} };

struct CPSCSingleSprite {
    long SpriteBankIndex; CRGBColour StartColour, MidColour, EndColour;
    float AnimationTimeSecs, StartRenderSize, EndRenderSize; long SpriteAlignment, NoCrossedSprites; float InitialAngle;
    long FadeInEndInteger, FadeOutBeginInteger; float AlphaFadeMinimum, SizeFadeMinimum; unsigned long PositionParam;
    long BlendMode, BlendOp, TrailBlendMode, TrailBlendOp, TrailBankIndex; float TrailWidth; long MaxTrailLength;
    long StayWithEmitterIntFactor; bool UseStartColour, UseMidColour, UseEndColour;
    bool SelfIlluminating : 1; bool AlphaFadeEnable : 1; bool RotateAroundCentre : 1; bool FaceMe2D : 1;
    bool FaceMe3D : 1; bool CrossedSprites : 1; bool SizeFadeEnable : 1; bool ForceAnimationTime : 1;
    bool StayWithEmitter : 1; bool UsePosition, UseSplinePoints, EnteringTimelineEnd;
    unsigned long LastUpdateFrame; long Lifetimer; C3DVector Offset; CEnginePrimitiveSpriteTrail TrailUpdatePrimitive;
    CArray<long> PrimIDList;
    CPSCSingleSprite();
};

CPSCSingleSprite::CPSCSingleSprite()
    : StartColour(255, 255, 255, 255), MidColour(255, 255, 255, 255), EndColour(255, 255, 255, 255),
      UseStartColour(true), UseMidColour(false), UseEndColour(false), Offset(0.0f, 0.0f, 0.0f)
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

int main()
{
    CPSCSingleSprite s;
    int ok = g_getCalls == 4 && g_trailCtors == 1 && s.StartColour.R == 255 && s.MidColour.A == 255 && s.EndColour.B == 255
          && s.UseStartColour && !s.UseMidColour && !s.UseEndColour && s.Offset.Y == 0.0f && s.PrimIDList.Data == 0
          && s.SpriteBankIndex == 0 && s.AnimationTimeSecs == 1.0f && s.EndRenderSize == 1.0f && !s.SelfIlluminating
          && s.FaceMe2D && !s.FaceMe3D && !s.CrossedSprites && !s.ForceAnimationTime && !s.RotateAroundCentre
          && s.SpriteAlignment == 0 && s.NoCrossedSprites == 2 && s.BlendMode == 2 && s.BlendOp == 0 && s.TrailBlendMode == 4
          && s.TrailBlendOp == 0 && s.TrailWidth == 1.0f && s.FadeOutBeginInteger == 1000 && s.AlphaFadeEnable
          && s.SizeFadeEnable && s.MaxTrailLength == 10 && s.UsePosition && !s.StayWithEmitter && s.Lifetimer == 0;
    if (!ok) { std::printf("00b09360_TEST FAIL\n"); return 1; }
    std::printf("00b09360_TEST PASS\n");
    return 0;
}