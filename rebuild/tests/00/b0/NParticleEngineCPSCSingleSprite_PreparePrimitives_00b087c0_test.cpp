// Behaviour test for NParticleEngine::CPSCSingleSprite::PreparePrimitives (0x00B087C0): the row's body verbatim over a controlled model of
// its world -- a recording engine (child set / clear events), a graphic bank (4 frames, 16x8), an update-parameter map with one position
// callback, a scaling particle system -- checking the empty early return, the spline-trail refresh, start / mid / end colour selection with
// the fade alpha, flags, the position lookup v spline fallback, the transform, fades (alpha and size), frame clamp, single v crossed sprites
// (angles, prim ids), the clear path, and the trail primitive fields (length, width scaling, skip points, colour, blend, flags).
#include <stdio.h>
#include <math.h>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("check failed line %d\n", __LINE__); ++g_fail; } } while (0)
static bool feq(float a, float b) { return fabs(a - b) < 1e-4f; }
class C3DVector { public: float X, Y, Z; C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {} };
class C2DVector { public: float X, Y; C2DVector(float x = 0, float y = 0) : X(x), Y(y) {} };
class CRGBColour { public: unsigned char B, G, R, A;
    CRGBColour() {} CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) {} };
static long GFFloatToLongNear(float x) { return (long)floor(x + 0.5f); }
static long GFFloatToLongFloor(float x) { return (long)floor(x); }
class CRGBFloatColour { public: float R, G, B, A;
    CRGBFloatColour() {} CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
    CRGBFloatColour(const CRGBColour& c) : R(c.R * (1.0f / 255.0f)), G(c.G * (1.0f / 255.0f)), B(c.B * (1.0f / 255.0f)), A(c.A * (1.0f / 255.0f)) {}
    CRGBFloatColour operator+(const CRGBFloatColour& c) const { return CRGBFloatColour(R + c.R, G + c.G, B + c.B, A + c.A); }
    CRGBFloatColour operator*(float f) const { return CRGBFloatColour(R * f, G * f, B * f, A * f); }
    CRGBFloatColour Interpolate(const CRGBFloatColour& c, float t) const { return *this * (1.0f - t) + c * t; }
    CRGBColour GetRGBColour() const { return CRGBColour((unsigned char)(R * 255.0f), (unsigned char)(G * 255.0f), (unsigned char)(B * 255.0f), (unsigned char)(A * 255.0f)); }
    operator CRGBColour() const { return GetRGBColour(); }
    CRGBColour GetRGBColourClamped() const {
        long c[4] = { GFFloatToLongNear(R * 255.0f), GFFloatToLongNear(G * 255.0f), GFFloatToLongNear(B * 255.0f), GFFloatToLongNear(A * 255.0f) };
        for (int i = 0; i < 4; ++i) { if (c[i] < 0) c[i] = 0; else if (c[i] > 255) c[i] = 255; }
        return CRGBColour((unsigned char)c[0], (unsigned char)c[1], (unsigned char)c[2], (unsigned char)c[3]); } };
class CMatrix3x4 { public: float E[12];
    void TransformPoint(C3DVector& p) const { p = C3DVector(p.X * E[0] + p.Y * E[3] + p.Z * E[6] + E[9], p.X * E[1] + p.Y * E[4] + p.Z * E[7] + E[10], p.X * E[2] + p.Y * E[5] + p.Z * E[8] + E[11]); } };
static const float DELTA_NOTIONAL_ZERO = 0.0001f;
enum EEnginePrimitiveBlendMode { BM0 }; enum EEnginePrimitiveBlendOp { BO0 }; enum EEngineInterpolationSet { IS0 };
class CEnginePrimitiveHandle { public: int ID; };
class CEnginePrimitive { public: int Kind; CEnginePrimitive(int k = 0) : Kind(k) {} };
class CEnginePrimitiveSprite : public CEnginePrimitive { public: C3DVector Pos; unsigned long Bank, Frame; unsigned short Flags; float Angle, SX, SY; CEnginePrimitiveSprite() {}
    long Mode, Op; unsigned char a, r, g, b;
    CEnginePrimitiveSprite(const C3DVector& p, unsigned long bank, unsigned long frame, unsigned short flags, float angle, float sx, float sy,
        EEnginePrimitiveBlendMode m, unsigned char A, unsigned char R, unsigned char G, unsigned char B, float, EEngineInterpolationSet, EEnginePrimitiveBlendOp o)
        : CEnginePrimitive(7), Pos(p), Bank(bank), Frame(frame), Flags(flags), Angle(angle), SX(sx), SY(sy), Mode(m), Op(o), a(A), r(R), g(G), b(B) {} };
class CEnginePrimitiveSpriteTrail : public CEnginePrimitive { public: bool UpdateExisting; unsigned long MaxLength, GraphicIndex, GraphicFrame;
    float SkipPoints, Width; CRGBColour HeadColour, MidColour, TailColour; unsigned short Flags; EEnginePrimitiveBlendMode BlendMode;
    EEnginePrimitiveBlendOp BlendOp; CMatrix3x4 WorldTransform; CEnginePrimitiveSpriteTrail() : CEnginePrimitive(9) {} };
struct Event { long Id; bool Clear; int Kind; CEnginePrimitiveSprite S; CEnginePrimitiveSpriteTrail T; };
class CIEngine { public: std::vector<Event> Ev;
    void SetChildPrimitive(CEnginePrimitiveHandle&, long id, const CEnginePrimitive& p, int) {
        Event e; e.Id = id; e.Clear = false; e.Kind = p.Kind;
        if (p.Kind == 7) e.S = (const CEnginePrimitiveSprite&)p; else e.T = (const CEnginePrimitiveSpriteTrail&)p; Ev.push_back(e); }
    void ClearChildPrimitive(CEnginePrimitiveHandle&, long id) { Event e; e.Id = id; e.Clear = true; e.Kind = 0; Ev.push_back(e); } };
// Event needs default-constructible members
class CEnginePrimitivePrepareInfo { public: CIEngine* Engine; };
class CGraphicInfoFrame { public: unsigned long GetHeight() const { return 8; } unsigned long GetWidth() const { return 16; } };
class CGraphicInfo { public: CGraphicInfoFrame F[4]; const CGraphicInfoFrame* PeekInfoFrame(unsigned long i) const { return i < 4 ? &F[i] : 0; } };
class CGraphicDataBank { public: CGraphicInfo G;
    unsigned long GetNoFramesInGraphic(unsigned long id) const { return id ? 4 : 0; }
    const CGraphicInfo* PeekGraphic(unsigned long id) const { return id ? &G : 0; } };
static CGraphicDataBank g_bank;
static long GetTickRate() { return 10; }
static const CGraphicDataBank& GetGraphicBank() { return g_bank; }
class CParticleSystem { public: float NormalisedTime, FadeAlpha, GlobalAlpha, Scale; bool ScaleParticles;
    float GetNormalisedTime() const { return NormalisedTime; } float GetFadeAlpha() const { return FadeAlpha; } float GetGlobalAlpha() const { return GlobalAlpha; }
    void GetSystemTransform(CMatrix3x4& m) const { for (int i = 0; i < 12; ++i) m.E[i] = 0; m.E[0] = m.E[4] = m.E[8] = 1; m.E[9] = 10; m.E[10] = 20; m.E[11] = 30; }
    void GetUnscaledWorldTransform(CMatrix3x4& m) const { GetSystemTransform(m); }
    void ScaleParticleDimensions(float& d) const { if (ScaleParticles) d *= Scale; }
    void ScaleParticleDimensions(C2DVector& d) const { if (ScaleParticles) { d.X *= Scale; d.Y *= Scale; } }
    void ScaleParticleDimensions(unsigned long& d) const { if (ScaleParticles) d *= (long)Scale; } };
class CParticleUpdateParameters { public: unsigned long Key; C3DVector Value;
    bool GetUpdateParam(C3DVector& out, const unsigned long& key, float, bool, unsigned long) const { if (key && key == Key) { out = Value; return true; } return false; } };
struct CPointer { void* P; bool IsNull() const { return P == 0; } };
class CPSCSingleSprite { public:
    CPointer SplineRandomOffsetData; long SplineCount; int TrailCalls; float TrailCallWidth;
    long SpriteBankIndex; CRGBColour StartColour, MidColour, EndColour; float AnimationTimeSecs, StartRenderSize, EndRenderSize;
    long SpriteAlignment, NoCrossedSprites; float InitialAngle; long FadeInEndInteger, FadeOutBeginInteger; float AlphaFadeMinimum, SizeFadeMinimum;
    unsigned long PositionParam; long BlendMode, BlendOp, TrailBlendMode, TrailBlendOp, TrailBankIndex; float TrailWidth; long MaxTrailLength;
    bool UseStartColour, UseMidColour, UseEndColour, SelfIlluminating, AlphaFadeEnable, RotateAroundCentre, FaceMe3D, CrossedSprites, SizeFadeEnable,
         ForceAnimationTime, UsePosition, UseSplinePoints;
    long Lifetimer; CEnginePrimitiveSpriteTrail TrailUpdatePrimitive; std::vector<long> PrimIDList;
    long GetIndexCount() { return SplineCount; }
    void GetPointAtIndex(const CParticleUpdateParameters&, C3DVector& p, long, float) { p = C3DVector(1, 2, 3); }
    void SetTrailPositionsFromSpline(CParticleSystem&, const CParticleUpdateParameters&, float w, float) { ++TrailCalls; TrailCallWidth = w; }
    float SmoothFadeValue(float t) const { return 0.5f - (float)cos(t * 0.5f * 6.2831853f) * 0.5f; }
    long GetPrimID(long index) const { return PrimIDList.size() > (unsigned long)index ? PrimIDList[index] : -1; }
    void PreparePrimitives(const float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system, const CParticleUpdateParameters& params, CEnginePrimitiveHandle& handle);
};
void CPSCSingleSprite::PreparePrimitives(const float time, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system, const CParticleUpdateParameters& params, CEnginePrimitiveHandle& handle)
{
    if (!SpriteBankIndex && !TrailBankIndex) return;
    if (!SplineRandomOffsetData.IsNull())
        SetTrailPositionsFromSpline(system, params, TrailWidth, time);
    float fadeAlpha = 0.0f;
    float tickRate = (float)GetTickRate();
    float fadeInEnd = FadeInEndInteger * 0.001f;
    float fadeOutBegin = FadeOutBeginInteger * 0.001f;
    float lifeTime = Lifetimer + time;
    float lifeFraction = system.GetNormalisedTime();
    if (lifeFraction < 0.0f) lifeFraction = 0.0f;
    else if (lifeFraction > 1.0f) lifeFraction = 1.0f;
    float animFraction;
    if (!ForceAnimationTime) {
        animFraction = lifeFraction;
        if (animFraction < 0.0f) animFraction = 0.0f;
        else {
            float limit = 1.0f - DELTA_NOTIONAL_ZERO;
            if (animFraction > limit) animFraction = limit;
        }
    } else
        animFraction = (float)fmod((double)(lifeTime * (1.0f / (tickRate * AnimationTimeSecs))), 1.0);
    CRGBFloatColour startColour(UseStartColour ? StartColour : UseMidColour ? MidColour
                                : UseEndColour ? EndColour : CRGBColour(255, 255, 255, 255));
    startColour.A = StartColour.A * (1.0f / 255.0f);
    CRGBFloatColour endColour(UseEndColour ? EndColour : UseMidColour ? MidColour
                              : UseStartColour ? StartColour : CRGBColour(255, 255, 255, 255));
    endColour.A = EndColour.A * (1.0f / 255.0f);
    CRGBFloatColour midColour(UseMidColour ? MidColour : (startColour * 0.5f + endColour * 0.5f).GetRGBColour());
    midColour.A = MidColour.A * (1.0f / 255.0f);
    fadeAlpha = system.GetFadeAlpha();
    startColour.A *= fadeAlpha;
    endColour.A *= fadeAlpha;
    midColour.A *= fadeAlpha;
    unsigned short flags = (unsigned short)SpriteAlignment;
    if (!SelfIlluminating) flags |= 0x80;
    if (CrossedSprites) flags |= 0x40;
    else {
        if (FaceMe3D) flags |= 0x01;
        if (RotateAroundCentre) flags |= 0x20;
    }
    C3DVector position;
    if (!UsePosition || !params.GetUpdateParam(position, PositionParam, time, false, 1))
        GetPointAtIndex(params, position, 0, 0.0f);
    {
        CMatrix3x4 transform;
        system.GetSystemTransform(transform);
        transform.TransformPoint(position);
    }
    const CGraphicDataBank& bank = GetGraphicBank();
    float angle = InitialAngle;
    CRGBFloatColour colour;
    if (lifeFraction < 0.5f)
        colour = startColour.Interpolate(midColour, lifeFraction * 2.0f);
    else
        colour = midColour.Interpolate(endColour, (lifeFraction - 0.5f) * 2.0f);
    float alpha = system.GetGlobalAlpha();
    float sizeScale = 1.0f;
    if (lifeFraction < fadeInEnd)
        alpha *= SmoothFadeValue(lifeFraction / fadeInEnd);
    if (lifeFraction > fadeOutBegin)
        alpha *= SmoothFadeValue((1.0f - lifeFraction) / (1.0f - fadeOutBegin));
    if (alpha < 1.0f) {
        if (AlphaFadeEnable)
            colour.A *= (1.0f - AlphaFadeMinimum) * alpha + AlphaFadeMinimum;
        if (SizeFadeEnable)
            sizeScale = (1.0f - SizeFadeMinimum) * alpha + SizeFadeMinimum;
    }
    CRGBColour spriteColour = colour.GetRGBColourClamped();
    long primIndex = 0;
    if (SpriteBankIndex) {
        long blendMode = BlendMode;
        long blendOp = BlendOp;
        long noFrames = bank.GetNoFramesInGraphic(SpriteBankIndex);
        long frame = GFFloatToLongFloor(noFrames * animFraction);
        if (frame < 0) frame = 0;
        else if (frame > noFrames - 1) frame = noFrames - 1;
        const CGraphicInfo* graphic = bank.PeekGraphic(SpriteBankIndex);
        C2DVector size;
        float startSize = StartRenderSize;
        size.X = ((EndRenderSize - startSize) * animFraction + startSize) * sizeScale;
        const CGraphicInfoFrame* graphicFrame = graphic->PeekInfoFrame(frame);
        size.Y = (float)graphicFrame->GetHeight() / graphicFrame->GetWidth() * size.X;
        system.ScaleParticleDimensions(size);
        if (CrossedSprites) {
            float angleScale = 0.5f;
            if (SpriteAlignment == 2 || SpriteAlignment == 4) angleScale = 1.0f;
            for (long i = 0; i < NoCrossedSprites; ++i) {
                float spriteAngle = (i + 0.5f) * angleScale / NoCrossedSprites + angle;
                info.Engine->SetChildPrimitive(handle, GetPrimID(i),
                    CEnginePrimitiveSprite(position, SpriteBankIndex, frame, flags, spriteAngle, size.X, size.Y,
                                           (EEnginePrimitiveBlendMode)blendMode, spriteColour.A, spriteColour.R, spriteColour.G,
                                           spriteColour.B, 0.0f, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)blendOp), 0);
            }
            primIndex = NoCrossedSprites;
        } else {
            long primID = GetPrimID(0);
            if (size.X > 0.0f && spriteColour.A) {
                CIEngine* engine = info.Engine;
                engine->SetChildPrimitive(handle, primID,
                    CEnginePrimitiveSprite(position, SpriteBankIndex, frame, flags, angle, size.X, size.Y,
                                           (EEnginePrimitiveBlendMode)blendMode, spriteColour.A, spriteColour.R, spriteColour.G,
                                           spriteColour.B, 0.0f, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)blendOp), 0);
            } else
                info.Engine->ClearChildPrimitive(handle, primID);
            primIndex = 1;
        }
    }
    if (TrailBankIndex) {
        long blendMode = TrailBlendMode;
        long blendOp = TrailBlendOp;
        long noFrames = bank.GetNoFramesInGraphic(TrailBankIndex);
        long frame = GFFloatToLongFloor(noFrames * animFraction);
        if (frame < 0) frame = 0;
        else if (frame > noFrames - 1) frame = noFrames - 1;
        TrailUpdatePrimitive.GraphicIndex = TrailBankIndex;
        TrailUpdatePrimitive.GraphicFrame = frame;
        float skip;
        if (UsePosition) {
            skip = 1.0f - time;
            float limit = 1.0f - DELTA_NOTIONAL_ZERO;
            if (!(skip < limit)) skip = limit;
        } else
            skip = 0.0f;
        TrailUpdatePrimitive.SkipPoints = skip;
        if (!UsePosition && UseSplinePoints)
            TrailUpdatePrimitive.MaxLength = GetIndexCount();
        else {
            TrailUpdatePrimitive.MaxLength = MaxTrailLength;
            system.ScaleParticleDimensions(TrailUpdatePrimitive.MaxLength);
        }
        TrailUpdatePrimitive.Width = TrailWidth;
        system.ScaleParticleDimensions(TrailUpdatePrimitive.Width);
        CRGBColour trailColour = colour * colour.A;
        TrailUpdatePrimitive.BlendMode = (EEnginePrimitiveBlendMode)blendMode;
        TrailUpdatePrimitive.HeadColour = trailColour;
        TrailUpdatePrimitive.MidColour = trailColour;
        TrailUpdatePrimitive.TailColour = trailColour;
        TrailUpdatePrimitive.Flags = flags;
        TrailUpdatePrimitive.BlendOp = (EEnginePrimitiveBlendOp)blendOp;
        system.GetUnscaledWorldTransform(TrailUpdatePrimitive.WorldTransform);
        TrailUpdatePrimitive.UpdateExisting = false;
        info.Engine->SetChildPrimitive(handle, GetPrimID(primIndex), TrailUpdatePrimitive, 0);
    }
}
static CPSCSingleSprite Make()
{
    CPSCSingleSprite s; s.SplineRandomOffsetData.P = 0; s.SplineCount = 7; s.TrailCalls = 0; s.TrailCallWidth = 0;
    s.SpriteBankIndex = 5; s.StartColour = CRGBColour(200, 100, 50, 255); s.MidColour = CRGBColour(100, 100, 100, 128); s.EndColour = CRGBColour(0, 0, 0, 0);
    s.AnimationTimeSecs = 1; s.StartRenderSize = 2; s.EndRenderSize = 6; s.SpriteAlignment = 1; s.NoCrossedSprites = 2; s.InitialAngle = 0.3f;
    s.FadeInEndInteger = 0; s.FadeOutBeginInteger = 1000; s.AlphaFadeMinimum = 0.2f; s.SizeFadeMinimum = 0.5f; s.PositionParam = 7;
    s.BlendMode = 2; s.BlendOp = 3; s.TrailBlendMode = 4; s.TrailBlendOp = 5; s.TrailBankIndex = 0; s.TrailWidth = 1.5f; s.MaxTrailLength = 3;
    s.UseStartColour = s.UseMidColour = s.UseEndColour = true; s.SelfIlluminating = false; s.AlphaFadeEnable = true; s.RotateAroundCentre = true;
    s.FaceMe3D = true; s.CrossedSprites = false; s.SizeFadeEnable = true; s.ForceAnimationTime = false; s.UsePosition = true; s.UseSplinePoints = true;
    s.Lifetimer = 3; s.PrimIDList.push_back(42); s.PrimIDList.push_back(43); s.PrimIDList.push_back(44);
    s.TrailUpdatePrimitive.UpdateExisting = true; s.TrailUpdatePrimitive.MaxLength = 0; s.TrailUpdatePrimitive.SkipPoints = -1;
    return s;
}
int main()
{
    CIEngine engine; CEnginePrimitivePrepareInfo info; info.Engine = &engine; CEnginePrimitiveHandle h; h.ID = 1;
    CParticleSystem sys; sys.NormalisedTime = 0.25f; sys.FadeAlpha = 0.5f; sys.GlobalAlpha = 1.0f; sys.Scale = 2; sys.ScaleParticles = true;
    CParticleUpdateParameters params; params.Key = 7; params.Value = C3DVector(4, 5, 6);
    { CPSCSingleSprite s = Make(); s.SpriteBankIndex = 0; s.SplineRandomOffsetData.P = &s;           // nothing to draw: early return
      s.PreparePrimitives(0.5f, info, sys, params, h); CHECK(engine.Ev.empty() && s.TrailCalls == 0); }
    { CPSCSingleSprite s = Make(); s.SplineRandomOffsetData.P = &s; engine.Ev.clear();                // single sprite
      s.PreparePrimitives(0.5f, info, sys, params, h);
      CHECK(s.TrailCalls == 1 && feq(s.TrailCallWidth, 1.5f)); CHECK(engine.Ev.size() == 1);
      if (engine.Ev.size() == 1) { const Event& e = engine.Ev[0]; const CEnginePrimitiveSprite& p = e.S;
        CHECK(!e.Clear && e.Id == 42 && e.Kind == 7 && p.Bank == 5 && p.Frame == 1 && p.Flags == 0xA1 && feq(p.Angle, 0.3f));
        CHECK(feq(p.Pos.X, 14) && feq(p.Pos.Y, 25) && feq(p.Pos.Z, 36) && feq(p.SX, 6) && feq(p.SY, 3) && p.Mode == 2 && p.Op == 3);
        CHECK(p.r == 150 && p.g == 100 && p.b == 75 && p.a == 96); } }
    { CPSCSingleSprite s = Make(); engine.Ev.clear(); sys.FadeAlpha = 0;                               // invisible: clear
      s.PreparePrimitives(0.5f, info, sys, params, h);
      CHECK(engine.Ev.size() == 1 && engine.Ev[0].Clear && engine.Ev[0].Id == 42); sys.FadeAlpha = 0.5f; }
    { CPSCSingleSprite s = Make(); engine.Ev.clear(); s.CrossedSprites = true; s.SpriteAlignment = 2; s.TrailBankIndex = 9;
      s.UsePosition = false; s.ForceAnimationTime = true; sys.NormalisedTime = 0.75f; sys.FadeAlpha = 1; sys.GlobalAlpha = 0.5f; sys.ScaleParticles = false;
      s.PreparePrimitives(0.5f, info, sys, params, h);                                                  // crossed sprites + trail
      CHECK(engine.Ev.size() == 3);
      if (engine.Ev.size() == 3) {
        for (int i = 0; i < 2; ++i) { const CEnginePrimitiveSprite& p = engine.Ev[i].S;
          CHECK(engine.Ev[i].Id == 42 + i && p.Flags == 0xC2 && p.Frame == 1 && feq(p.Angle, (i + 0.5f) / 2 + 0.3f));
          CHECK(feq(p.Pos.X, 11) && feq(p.Pos.Y, 22) && feq(p.Pos.Z, 33) && feq(p.SX, 2.55f) && feq(p.SY, 1.275f));
          CHECK(p.r == 50 && p.g == 50 && p.b == 50 && p.a == 38); }
        const Event& e = engine.Ev[2]; const CEnginePrimitiveSpriteTrail& t = e.T;
        CHECK(e.Id == 44 && e.Kind == 9 && t.GraphicIndex == 9 && t.GraphicFrame == 1 && feq(t.SkipPoints, 0) && t.MaxLength == 7);
        CHECK(feq(t.Width, 1.5f) && t.BlendMode == 4 && t.BlendOp == 5 && t.Flags == 0xC2 && !t.UpdateExisting && feq(t.WorldTransform.E[9], 10));
        CHECK(t.HeadColour.R == 7 && t.HeadColour.A == 5 && t.MidColour.G == 7 && t.TailColour.B == 7); }
      engine.Ev.clear(); s.SpriteAlignment = 3; s.PreparePrimitives(0.5f, info, sys, params, h);   // other alignments: half-turn spread
      CHECK(engine.Ev.size() == 3 && feq(engine.Ev[0].S.Angle, 0.425f) && feq(engine.Ev[1].S.Angle, 0.675f) && engine.Ev[0].S.Flags == 0xC3); }
    { CPSCSingleSprite s = Make(); engine.Ev.clear(); s.SpriteBankIndex = 0; s.TrailBankIndex = 9; params.Key = 8;   // trail only, positioned
      sys.ScaleParticles = true; s.PreparePrimitives(0.25f, info, sys, params, h);
      CHECK(engine.Ev.size() == 1 && engine.Ev[0].Id == 42);
      if (engine.Ev.size() == 1) { const CEnginePrimitiveSpriteTrail& t = engine.Ev[0].T;
        CHECK(feq(t.SkipPoints, 0.75f) && t.MaxLength == 6 && feq(t.Width, 3)); }
      engine.Ev.clear(); s.PreparePrimitives(0.0f, info, sys, params, h);
      CHECK(engine.Ev.size() == 1 && feq(engine.Ev[0].T.SkipPoints, 1.0f - DELTA_NOTIONAL_ZERO)); }
    if (!g_fail) printf("00b087c0_TEST PASS\n");
    return g_fail;
}