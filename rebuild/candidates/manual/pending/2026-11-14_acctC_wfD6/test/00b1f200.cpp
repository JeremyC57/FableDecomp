// Behaviour test for NParticleEngine::CPSCDecalRenderer::PreparePrimitives (0x00B1F200): the row's body verbatim over a controlled
// model of its world -- a recording engine (group / child set / child add events), a decal list, a graphic bank (4 frames, 8x4),
// a Perlin noise stub -- checking the empty-bank clear, both decal groups, the [MinLife, 1] clamp written back, set v add (and the
// SetPrimitiveInfo bookkeeping), dead-decal clearing, colour fades, flicker clamp, the three pooling overrides and frame clamp.
#include <stdio.h>
#include <math.h>
#include <vector>
#include <list>
#if defined(_MSC_VER)
namespace std { template<class T> inline const T& min(const T& a, const T& b) { return b < a ? b : a; } }
#else
#include <algorithm>
#endif
#define FABLE_FASTCALL
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("check failed line %d\n", __LINE__); ++g_fail; } } while (0)
class C3DVector { public: float X, Y, Z; C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {} };
class C2DVector { public: float X, Y; C2DVector(float x = 0, float y = 0) : X(x), Y(y) {} };
class CRightHandedSet { public: C3DVector Up, Forward; };
class CRGBColour { public: unsigned char R, G, B, A;
    CRGBColour(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 0) : R(r), G(g), B(b), A(a) {} };
static CRGBColour GFGetInterpolatedColour(CRGBColour a, CRGBColour b, unsigned long f)
{
    return CRGBColour((unsigned char)((a.R * (256 - f) + b.R * f) / 256), (unsigned char)((a.G * (256 - f) + b.G * f) / 256),
                      (unsigned char)((a.B * (256 - f) + b.B * f) / 256), (unsigned char)((a.A * (256 - f) + b.A * f) / 256));
}
static long GFFloatToLongNear(float x) { return (long)floor(x + 0.5f); }
static long GFFloatToLongFloor(float x) { return (long)floor(x); }
static float GFUnpackFloat(unsigned long p, unsigned long bits, float low, float high, unsigned long places)
{
    float scale = 1; while (places--) scale *= 10;
    return GFFloatToLongNear((p / ((float)(1UL << bits) - 1) * (high - low) + low) * scale) / scale;
}
enum EEnginePrimitiveBlendOp { BlendOpZero, BlendOpOne };
enum EEnginePrimitiveBlendMode { BlendModeZero, BlendModeOne, BlendModeTwo };
class CEnginePrimitiveHandle { public: int ID; CEnginePrimitiveHandle(int n = 0) : ID(n) {} };
class CEnginePrimitive { public: int Kind; long Bank, Frame; bool Landscape; unsigned long Flags; float Stencil; int Mode, Op;
    C3DVector Pos; C2DVector Size; CRGBColour Colour;
    CEnginePrimitive(int k) : Kind(k), Bank(0), Frame(0), Landscape(false), Flags(0), Stencil(0), Mode(0), Op(0) {} };
struct CEnginePrimitiveParticleDecalGroup : CEnginePrimitive {
    CEnginePrimitiveParticleDecalGroup(long bank, bool landscape, EEnginePrimitiveBlendMode m, EEnginePrimitiveBlendOp o, float stencil,
                                       unsigned long flags) : CEnginePrimitive(0x1e)
    { Bank = bank; Landscape = landscape; Mode = m; Op = o; Stencil = stencil; Flags = flags; } };
struct CEnginePrimitiveParticleDecal : CEnginePrimitive {
    CEnginePrimitiveParticleDecal(long frame, const C3DVector& p, const C2DVector& s, const CRGBColour& c, const CRightHandedSet&)
        : CEnginePrimitive(0x19) { Frame = frame; Pos = p; Size = s; Colour = c; } };
struct Event { int Parent; long Child; bool Added; CEnginePrimitive Prim; };
class CIEngine { public: std::vector<Event> Events; long Next; CIEngine() : Next(100) {}
    void SetChildPrimitive(CEnginePrimitiveHandle& parent, long index, CEnginePrimitiveHandle& out, const CEnginePrimitive& p, int)
    { Event e = { parent.ID * 1000 + out.ID, index, false, p }; Events.push_back(e); }
    void SetChildPrimitive(CEnginePrimitiveHandle& parent, long id, const CEnginePrimitive& p, int)
    { Event e = { parent.ID, id, false, p }; Events.push_back(e); }
    long AddChildPrimitive(CEnginePrimitiveHandle& parent, const CEnginePrimitive& p, int)
    { Event e = { parent.ID, Next, true, p }; Events.push_back(e); return Next++; } };
class CEnginePrimitivePrepareInfo { public: CIEngine* Engine; };
class CGraphicInfoFrame { public: unsigned long GetHeight() const { return 4; } unsigned long GetWidth() const { return 8; } };
class CGraphicInfo { public: CGraphicInfoFrame Frame; const CGraphicInfoFrame* PeekInfoFrame(int i) const { return i == 0 ? &Frame : 0; } };
class CGraphicDataBank { public: CGraphicInfo Graphic;
    long GetNoFramesInGraphic(long id) const { return id == 7 ? 4 : 0; }
    const CGraphicInfo* PeekGraphic(long id) const { return id == 7 ? &Graphic : 0; } };
namespace NFractal { class CPerlinNoise { public: float Value; mutable int Calls; mutable C2DVector Last;
    CPerlinNoise() : Value(0), Calls(0) {}
    float GetGaussian2DNoise(const C2DVector& p) const { Last = p; ++Calls; return Value; } }; }
namespace NParticleEngine {
class CParticleEmitterManager { public: NFractal::CPerlinNoise Noise; const NFractal::CPerlinNoise& PeekPerlinNoise() const { return Noise; } };
static CGraphicDataBank Bank; static CParticleEmitterManager Manager; static int TickCalls;
static long GetTickRate() { ++TickCalls; return 15; }
static float GetOneOverTickRate() { return 0.5f; }
static const CGraphicDataBank& GetGraphicBank() { return Bank; }
static CParticleEmitterManager& GetParticleEmitterManager() { return Manager; }
static float GFSmoothFadeValue(float x) { return x; }
template<class A, class B> class CParticleUpdateParameters { public: };
class CDecal { public:
    long Lifetimer, MaxLife; float MinLife, AlphaFactor[2], SizeFactor[2], GraphicFrame; bool Active, Landscape;
    unsigned char FlickerIndex, FlickerTimeOffset; C3DVector Position; CRightHandedSet RHSet;
    std::vector<std::vector<long> > PrimIDList; std::vector<CEnginePrimitiveHandle> ParentHandleList; int Stores;
    CDecal() : Lifetimer(2), MaxLife(4), MinLife(0), GraphicFrame(2.6f), Active(true), Landscape(false), FlickerIndex(3), FlickerTimeOffset(5),
        Position(1, 2, 3), Stores(0) { AlphaFactor[0] = AlphaFactor[1] = 1; SizeFactor[0] = SizeFactor[1] = 1; }
    float GetInterpolatedLifetimer(float f) const { return Lifetimer + f; }
    long GetMaxLife() const { return MaxLife; }
    bool IsActive() const { return Active; }
    float GetMinLife() const { return MinLife; }
    void SetMinLife(float m) { MinLife = m; }
    float GetAlphaFactor(float t) const { return (AlphaFactor[0] - AlphaFactor[1]) * t + AlphaFactor[1]; }
    float GetSizeFactor(float t) const { return (SizeFactor[0] - SizeFactor[1]) * t + SizeFactor[1]; }
    unsigned char GetFlickerIndex() const { return FlickerIndex; }
    unsigned char GetFlickerTimeOffset() const { return FlickerTimeOffset; }
    long GetNearestGraphicFrame() const { return (long)floor(GraphicFrame + 0.5f); }
    bool IsLandscapeOnlyDecal() const { return Landscape; }
    const C3DVector& GetPosition() const { return Position; }
    const CRightHandedSet& GetRightHandedSet() const { return RHSet; }
    bool GetPrimID(long g, long i, long& id) const
    { if (PrimIDList.size() > (unsigned long)g && PrimIDList[g].size() > (unsigned long)i) { id = PrimIDList[g][i]; return id >= 0; } return false; }
    void SetPrimitiveInfo(long l, long i, const CEnginePrimitiveHandle& parent, long id)
    { ++Stores; if (PrimIDList.size() <= (unsigned long)l) PrimIDList.resize(l + 1); if (PrimIDList[l].size() <= (unsigned long)i) PrimIDList[l].resize(i + 1);
      PrimIDList[l][i] = id; if (ParentHandleList.size() <= (unsigned long)l) ParentHandleList.resize(l + 1); ParentHandleList[l] = parent; } };
class CParticleSystem { public: std::list<CDecal*> Decals; float Fade, Alpha, Scale; int Clears;
    CParticleSystem() : Fade(1), Alpha(1), Scale(1), Clears(0) {}
    float GetFadeAlpha() const { return Fade; }
    float GetGlobalAlpha() const { return Alpha; }
    std::list<CDecal*>& GetDecalList() { return Decals; }
    void ScaleParticleDimensions(C2DVector& d) const { d.X *= Scale; d.Y *= Scale; }
    void ClearDecal(std::list<CDecal*>::iterator&) { ++Clears; } };
class CPSCDecalRenderer { public:
    long DecalBankIndex; CRGBColour StartColour, MidColour, EndColour;
    unsigned long BlendMode, BlendOp, Flags, FadeInEndInteger, FadeOutBeginInteger, StartRenderSize, AlphaFadeMinimum, EndRenderSize,
        AnimationTimeSecs, SizeFadeMinimum, FlickerBias, FlickerMinAlphaInteger, FlickerMinSizeInteger, FlickerSpeed;
    bool UseStartColour, UseMidColour, UseEndColour, AlphaFadeEnable, SizeFadeEnable, ForceAnimationTime, FlickerEnable, PoolingEnable,
        PoolIncreaseAlphaEnable, PoolIncreaseSizeEnable, PoolIncreaseLifeEnable, PoolIncreaseFrameEnable;
    float MaxPoolSize, MaxPoolAlpha, StencilCubeWidth;
    CEnginePrimitiveHandle LandscapeDecalsPrimHandle; long LandscapeDecalsChildPrimIndex; CEnginePrimitiveHandle MeshDecalsPrimHandle;
    long MeshDecalsChildPrimIndex; int Clears;
    CPSCDecalRenderer() : DecalBankIndex(7), StartColour(64, 32, 16, 128), MidColour(64, 32, 16, 128), EndColour(64, 32, 16, 128),
        BlendMode(2), BlendOp(1), Flags(0x55), FadeInEndInteger(0), FadeOutBeginInteger(1000), StartRenderSize(2047),
        AlphaFadeMinimum(0), EndRenderSize(2047), AnimationTimeSecs(16383), SizeFadeMinimum(0), FlickerBias(255), FlickerMinAlphaInteger(0),
        FlickerMinSizeInteger(0), FlickerSpeed(0), UseStartColour(true), UseMidColour(false), UseEndColour(false), AlphaFadeEnable(false),
        SizeFadeEnable(false), ForceAnimationTime(false), FlickerEnable(false), PoolingEnable(false), PoolIncreaseAlphaEnable(false),
        PoolIncreaseSizeEnable(false), PoolIncreaseLifeEnable(false), PoolIncreaseFrameEnable(false), MaxPoolSize(5), MaxPoolAlpha(0.25f),
        StencilCubeWidth(3.5f), LandscapeDecalsPrimHandle(2), LandscapeDecalsChildPrimIndex(11), MeshDecalsPrimHandle(3),
        MeshDecalsChildPrimIndex(12), Clears(0) {}
    void ClearAllPrimitives(CIEngine*, CEnginePrimitiveHandle& h) { if (h.ID == 1) ++Clears; }
    void PreparePrimitives(const float, const CEnginePrimitivePrepareInfo&, CParticleSystem&,
                           const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&); };
// ---- the row's body, verbatim ----
void CPSCDecalRenderer::PreparePrimitives(const float interpolate, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                          const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& system_group)
{
    if (!DecalBankIndex) {
        ClearAllPrimitives(info.Engine, system_group);
        return;
    }
    const float alpha_fade = GFUnpackFloat(AlphaFadeMinimum, 7, 0.0f, 1.0f, 2);
    const float size_fade = GFUnpackFloat(SizeFadeMinimum, 7, 0.0f, 1.0f, 2);
    const float anim_time_secs = GFUnpackFloat(AnimationTimeSecs, 14, 0.1f, 100.0f, 2);
    const float start_render_size = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
    const float end_render_size = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
    const float flicker_speed = GFUnpackFloat(FlickerSpeed, 12, 0.0f, 30.0f, 2);
    const float this_flicker_bias = GFUnpackFloat(FlickerBias, 8, -1.0f, 1.0f, 2);
    const float tick_rate = (float)GetTickRate();       // unused: retail still calls it (0x00B1F482)
    const float one_on_tick_rate = GetOneOverTickRate();
    const CGraphicDataBank& bank = GetGraphicBank();
    const float fade_in_end = FadeInEndInteger * 0.001f;
    const float fade_out_begin = FadeOutBeginInteger * 0.001f;
    const float fade_alpha = system.GetFadeAlpha();
    const float one_on_amin_time_secs = 1.0f / anim_time_secs;
    const float one_over_fade_in_end = 1.0f / fade_in_end;
    const float one_over_one_sub_fade_out_begin = 1.0f / (1.0f - fade_out_begin);
    const float min_alpha = FlickerMinAlphaInteger * 0.001f;
    const float min_size = FlickerMinSizeInteger * 0.001f;
    const float flicker_scale = (float)fabs(this_flicker_bias) + 0.5f;
    const float flicker_bias = this_flicker_bias + 0.5f;
    const float flicker_over_tick = one_on_tick_rate * flicker_speed;
    float alpha_fade_min, one_sub_alpha_fade_min;
    if (AlphaFadeEnable) {
        one_sub_alpha_fade_min = 1.0f - alpha_fade;
        alpha_fade_min = alpha_fade;
    } else {
        one_sub_alpha_fade_min = 0.0f;
        alpha_fade_min = 1.0f;
    }
    float size_fade_min, one_sub_size_fade_min;
    if (SizeFadeEnable) {
        one_sub_size_fade_min = 1.0f - size_fade;
        size_fade_min = size_fade;
    } else {
        one_sub_size_fade_min = 0.0f;
        size_fade_min = 1.0f;
    }
    long no_frames_in_graphic = 0;
    float no_frames_in_graphic_f = 0.0f;
    float base_size_y = 0.0f;
    EEnginePrimitiveBlendOp blend_op = (EEnginePrimitiveBlendOp)BlendOp;
    EEnginePrimitiveBlendMode blend_mode = (EEnginePrimitiveBlendMode)BlendMode;
    const NFractal::CPerlinNoise& perlin_noise = GetParticleEmitterManager().PeekPerlinNoise();
    const float one_sub_min_alpha = 1.0f - min_alpha;
    const float one_sub_min_size = 1.0f - min_size;
    if (DecalBankIndex) {
        no_frames_in_graphic = bank.GetNoFramesInGraphic(DecalBankIndex);
        no_frames_in_graphic_f = (float)no_frames_in_graphic;
        const CGraphicInfoFrame* graphic_frame = bank.PeekGraphic(DecalBankIndex)->PeekInfoFrame(0);
        base_size_y = (float)graphic_frame->GetHeight() / graphic_frame->GetWidth();
        info.Engine->SetChildPrimitive(system_group, LandscapeDecalsChildPrimIndex, LandscapeDecalsPrimHandle,
            CEnginePrimitiveParticleDecalGroup(DecalBankIndex, true, blend_mode, blend_op, StencilCubeWidth, Flags), 0);
        info.Engine->SetChildPrimitive(system_group, MeshDecalsChildPrimIndex, MeshDecalsPrimHandle,
            CEnginePrimitiveParticleDecalGroup(DecalBankIndex, false, blend_mode, blend_op, StencilCubeWidth, Flags), 0);
    }
    const long no_frames_in_graphic_sub_one = no_frames_in_graphic - 1;
    CRGBColour colours[3];
    colours[0] = UseStartColour ? StartColour : UseMidColour ? MidColour : UseEndColour ? EndColour : CRGBColour(255, 255, 255, 255);
    colours[2] = UseEndColour ? EndColour : UseMidColour ? MidColour : UseStartColour ? StartColour : CRGBColour(255, 255, 255, 255);
    colours[1] = UseMidColour ? MidColour : GFGetInterpolatedColour(colours[0], colours[2], 128);
    colours[0].A = (unsigned char)(colours[0].A * fade_alpha);
    colours[1].A = (unsigned char)(colours[1].A * fade_alpha);
    colours[2].A = (unsigned char)(colours[2].A * fade_alpha);
    CRGBColour colour;
    C2DVector noise_lookup;
    long child_prim_id;
    const unsigned long colour_intervals = 512;
    for (std::list<CDecal*>::iterator it_decal = system.GetDecalList().begin(); it_decal != system.GetDecalList().end(); ++it_decal) {
        CDecal& decal = **it_decal;
        const float life_timer = decal.GetInterpolatedLifetimer(interpolate);
        if (decal.GetMaxLife() > -1 && decal.IsActive()) {
            float normalised_time;
            if (life_timer / decal.GetMaxLife() < decal.GetMinLife())
                normalised_time = decal.GetMinLife();
            else if (life_timer / decal.GetMaxLife() > 1.0f)
                normalised_time = 1.0f;
            else
                normalised_time = life_timer / decal.GetMaxLife();
            decal.SetMinLife(normalised_time);
            unsigned long colour_time = GFFloatToLongNear(normalised_time * (colour_intervals - 1));
            unsigned long colour_index = colour_time >> 8;
            unsigned long colour_interp = colour_time & 0xff;
            colour = GFGetInterpolatedColour(colours[colour_index], colours[colour_index + 1], colour_interp);
            float fade_value = system.GetGlobalAlpha();
            if (normalised_time < fade_in_end)
                fade_value *= GFSmoothFadeValue(normalised_time * one_over_fade_in_end);
            if (normalised_time > fade_out_begin)
                fade_value *= GFSmoothFadeValue((1.0f - normalised_time) * one_over_one_sub_fade_out_begin);
            float colour_fade = fade_value * one_sub_alpha_fade_min + alpha_fade_min;
            float scale = fade_value * one_sub_size_fade_min + size_fade_min;
            if (FlickerEnable) {
                noise_lookup.X = life_timer * flicker_over_tick + decal.GetFlickerTimeOffset();
                noise_lookup.Y = decal.GetFlickerIndex();
                float val = perlin_noise.GetGaussian2DNoise(noise_lookup);
                val = val * flicker_scale + flicker_bias;
                if (val < 0.0f) val = 0.0f; else if (val > 1.0f) val = 1.0f;
                colour_fade *= one_sub_min_alpha * val + min_alpha;
                scale *= one_sub_min_size * val + min_size;
            }
            if (PoolingEnable && PoolIncreaseAlphaEnable) {
                const float max_alpha = MaxPoolAlpha * 255.0f;
                const float alpha = decal.GetAlphaFactor(interpolate) * colour.A * colour_fade;
                colour.A = (unsigned char)GFFloatToLongNear(std::min(alpha, max_alpha));
            } else
                colour.A = (unsigned char)GFFloatToLongNear(colour.A * colour_fade);
            if (colour.A) {
                float anim_percent = normalised_time;
                if (ForceAnimationTime)
                    anim_percent = (float)fmod((double)((one_on_amin_time_secs * one_on_tick_rate) * life_timer), 1.0);
                long frame;
                if (PoolingEnable && PoolIncreaseFrameEnable)
                    frame = decal.GetNearestGraphicFrame();
                else
                    frame = GFFloatToLongFloor(anim_percent * no_frames_in_graphic_f);
                frame = std::min(frame, no_frames_in_graphic_sub_one);
                C2DVector size;
                if (PoolingEnable && PoolIncreaseSizeEnable) {
                    const float size_factor = decal.GetSizeFactor(interpolate);
                    const float pool_size = size_factor * ((end_render_size - start_render_size) * anim_percent + start_render_size) * scale;
                    size.X = std::min(pool_size, MaxPoolSize);
                } else
                    size.X = ((end_render_size - start_render_size) * anim_percent + start_render_size) * scale;
                size.Y = size.X * base_size_y;
                system.ScaleParticleDimensions(size);
                if (size.X > 0.0f && size.Y > 0.0f) {
                    CEnginePrimitiveHandle& parent = decal.IsLandscapeOnlyDecal() ? LandscapeDecalsPrimHandle : MeshDecalsPrimHandle;
                    if (decal.GetPrimID(0, 0, child_prim_id))
                        info.Engine->SetChildPrimitive(parent, child_prim_id,
                            CEnginePrimitiveParticleDecal(frame, decal.GetPosition(), size, colour, decal.GetRightHandedSet()), 0);
                    else {
                        child_prim_id = info.Engine->AddChildPrimitive(parent,
                            CEnginePrimitiveParticleDecal(frame, decal.GetPosition(), size, colour, decal.GetRightHandedSet()), 0);
                        decal.SetPrimitiveInfo(0, 0, parent, child_prim_id);
                    }
                }
            }
        } else if (life_timer > decal.GetMaxLife())
            system.ClearDecal(it_decal);
    }
}
}
using namespace NParticleEngine;
struct Fixture { CPSCDecalRenderer R; CIEngine E; CParticleSystem S; CDecal D; CEnginePrimitiveHandle Group; CEnginePrimitivePrepareInfo I;
    CParticleUpdateParameters<unsigned long, unsigned long> P;
    Fixture() : Group(1) { I.Engine = &E; S.Decals.push_back(&D); Manager = CParticleEmitterManager(); TickCalls = 0; }
    void Run(float t = 0.0f) { R.PreparePrimitives(t, I, S, P, Group); }
    int Count(int kind) const { int n = 0; for (size_t i = 0; i < E.Events.size(); ++i) n += E.Events[i].Prim.Kind == kind; return n; }
    const Event& Decal() const { for (size_t i = 0; i < E.Events.size(); ++i) if (E.Events[i].Prim.Kind == 0x19) return E.Events[i]; return E.Events[0]; } };
int main()
{
    { Fixture f; f.R.DecalBankIndex = 0; f.Run(); CHECK(f.R.Clears == 1 && f.E.Events.empty() && TickCalls == 0); }
    { Fixture f; f.Run(); CHECK(TickCalls == 1 && f.Count(0x1e) == 2 && f.Count(0x19) == 1);
      CHECK(f.E.Events[0].Parent == 1002 && f.E.Events[0].Child == 11 && f.E.Events[0].Prim.Landscape && f.E.Events[0].Prim.Bank == 7
            && f.E.Events[0].Prim.Mode == 2 && f.E.Events[0].Prim.Op == 1 && f.E.Events[0].Prim.Flags == 0x55 && f.E.Events[0].Prim.Stencil == 3.5f);
      CHECK(f.E.Events[1].Parent == 1003 && f.E.Events[1].Child == 12 && !f.E.Events[1].Prim.Landscape);
      const Event& e = f.Decal(); CHECK(e.Added && e.Parent == 3 && e.Child == 100 && f.D.Stores == 1 && f.D.PrimIDList[0][0] == 100
            && f.D.ParentHandleList[0].ID == 3);
      CHECK(f.D.MinLife == 0.5f && e.Prim.Frame == 2 && e.Prim.Size.X == 20.0f && e.Prim.Size.Y == 10.0f && e.Prim.Colour.A == 128 && e.Prim.Pos.Z == 3); }
    { Fixture f; f.D.Landscape = true; f.D.PrimIDList.resize(1); f.D.PrimIDList[0].push_back(42); f.Run();
      const Event& e = f.Decal(); CHECK(!e.Added && e.Parent == 2 && e.Child == 42 && f.D.Stores == 0); }
    { Fixture f; f.D.PrimIDList.resize(1); f.D.PrimIDList[0].push_back(-1); f.Run(); CHECK(f.Decal().Added && f.D.Stores == 1); }
    { Fixture f; f.D.MinLife = 0.75f; f.Run(); CHECK(f.D.MinLife == 0.75f && f.Decal().Prim.Frame == 3); }
    { Fixture f; f.D.Lifetimer = 9; f.Run(); CHECK(f.D.MinLife == 1.0f && f.Decal().Prim.Frame == 3); }
    { Fixture f; f.D.Active = false; f.D.Lifetimer = 9; f.Run(); CHECK(f.S.Clears == 1 && f.Count(0x19) == 0); }
    { Fixture f; f.D.Active = false; f.Run(); CHECK(f.S.Clears == 0 && f.Count(0x19) == 0); }
    { Fixture f; f.D.MaxLife = -1; f.Run(); CHECK(f.S.Clears == 1 && f.Count(0x19) == 0); }
    { Fixture f; f.S.Fade = 0.5f; f.Run(); CHECK(f.Decal().Prim.Colour.A == 64); }
    { Fixture f; f.S.Fade = 0.0f; f.Run(); CHECK(f.Count(0x19) == 0 && f.D.Stores == 0); }
    { Fixture f; f.R.AlphaFadeEnable = true; f.S.Alpha = 0.5f; f.Run(); CHECK(f.Decal().Prim.Colour.A == 64); }
    { Fixture f; f.R.SizeFadeEnable = true; f.S.Alpha = 0.5f; f.Run(); CHECK(f.Decal().Prim.Size.X == 10.0f); }
    { Fixture f; f.R.FlickerEnable = true; Manager.Noise.Value = -2.0f; f.R.FlickerMinAlphaInteger = 500; f.R.FlickerMinSizeInteger = 500; f.R.FlickerBias = 0; f.Run();
      CHECK(Manager.Noise.Calls == 1 && Manager.Noise.Last.Y == 3.0f && f.Decal().Prim.Colour.A == 64 && f.Decal().Prim.Size.X == 10.0f); }
    { Fixture f; f.R.PoolingEnable = true; f.R.PoolIncreaseAlphaEnable = true; f.Run(); CHECK(f.Decal().Prim.Colour.A == 64); }
    { Fixture f; f.R.PoolIncreaseAlphaEnable = true; f.Run(); CHECK(f.Decal().Prim.Colour.A == 128); }
    { Fixture f; f.R.PoolingEnable = true; f.R.PoolIncreaseSizeEnable = true; f.Run(); CHECK(f.Decal().Prim.Size.X == 5.0f && f.Decal().Prim.Size.Y == 2.5f && f.Decal().Prim.Frame == 2); }
    { Fixture f; f.R.PoolingEnable = true; f.R.PoolIncreaseFrameEnable = true; f.D.GraphicFrame = 1.4f; f.Run(); CHECK(f.Decal().Prim.Frame == 1); }
    { Fixture f; f.R.PoolingEnable = true; f.R.PoolIncreaseFrameEnable = true; f.D.GraphicFrame = 9.0f; f.Run(); CHECK(f.Decal().Prim.Frame == 3); }
    { Fixture f; f.S.Scale = 0.0f; f.Run(); CHECK(f.Count(0x19) == 0 && f.D.Stores == 0); }
    { Fixture f; f.R.UseStartColour = false; f.Run(); CHECK(f.Decal().Prim.Colour.R == 255 && f.Decal().Prim.Colour.A == 255); }
    { Fixture f; CDecal d2; d2.Lifetimer = 9; d2.Active = false; f.S.Decals.push_back(&d2); f.Run(); CHECK(f.S.Clears == 1 && f.Count(0x19) == 1); }
    printf(g_fail ? "00b1f200_TEST FAIL\n" : "00b1f200_TEST PASS\n");
    return g_fail ? 1 : 0;
}
