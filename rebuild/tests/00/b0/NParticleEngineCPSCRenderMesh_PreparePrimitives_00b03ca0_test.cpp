// Behaviour test for NParticleEngine::CPSCRenderMesh::PreparePrimitives (0x00B03CA0): the row's body verbatim over a controlled model of its
// world -- a recording engine (group set, child set / add with ids), a mesh bank (data type, mesh bounding sphere), a particle system (fade /
// global alpha, world transform, local offset, dimension scale, clear), particles with lifetimes / positions / recorded positions / primitive
// ids, colours, flicker noise and the render-size parameter. Checks: the bank / data-type gate (else ClearAllPrimitives), the group set-up,
// the colour ramp (start / mid / end selection and fade alpha), size lerp over clamped life, fade-in / fade-out alpha, alpha / size fade
// minimums, flicker, the CentredOnPos offset, set v add of the mesh and trail children with SetPrimitiveInfo, short v long trails, the local
// transform of the trail ends, the trail width scale, the zero-alpha skip and the dead-particle clear (only past MaxLife).
#include <stdio.h>
#include <math.h>
#include <list>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("check failed line %d\n", __LINE__); ++g_fail; } } while (0)
static bool feq(float a, float b) { return fabs(a - b) < 1e-3f; }
class C3DVector { public: float X, Y, Z; C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
    C3DVector& operator-=(const C3DVector& v) { X -= v.X; Y -= v.Y; Z -= v.Z; return *this; } };
static bool veq(const C3DVector& a, const C3DVector& b) { return feq(a.X, b.X) && feq(a.Y, b.Y) && feq(a.Z, b.Z); }
class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} };
class CRGBColour { public: unsigned char B, G, R, A; CRGBColour() {} CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) {} };
static int g_interpCalls = 0;
static CRGBColour GFGetInterpolatedColour(CRGBColour a, CRGBColour b, unsigned long f) { ++g_interpCalls;
    return CRGBColour((unsigned char)((a.R * (256 - f) + b.R * f) >> 8), (unsigned char)((a.G * (256 - f) + b.G * f) >> 8),
                      (unsigned char)((a.B * (256 - f) + b.B * f) >> 8), (unsigned char)((a.A * (256 - f) + b.A * f) >> 8)); }
static long GFFloatToLongNear(float x) { return (long)floor(x + 0.5f); }
static float GFUnpackFloat(unsigned long p, unsigned long bits, float lo, float hi, unsigned long) { return lo + (float)p * (hi - lo) / (float)((1 << bits) - 1); }
static float GFSmoothFadeValue(float x) { return x * x; }
static const float DELTA_NOTIONAL_ZERO = 0.0001f;
enum EEnginePrimitiveBlendMode { BLEND_MODE_0 }; enum EEnginePrimitiveBlendOp { BLEND_OP_0 }; enum EEngineInterpolationSet { INTERP_0 };
class CMatrix3x4 { public: float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void TransformVector(const C3DVector& s, C3DVector& d) const {
        d.X = s.X * E11 + s.Y * E21 + s.Z * E31; d.Y = s.X * E12 + s.Y * E22 + s.Z * E32; d.Z = s.X * E13 + s.Y * E23 + s.Z * E33; } };
class C3DBoundingSphere { public: C3DVector Center; float Radius; };
class C3DMeshInfo { public: C3DBoundingSphere S; long Type; C3DBoundingSphere GetBoundingSphere() const { return S; } long GetDataType() const { return Type; } };
static C3DMeshInfo g_mesh;
class CMeshDataBank { public: const C3DMeshInfo* PeekMesh(long) const { return &g_mesh; } const C3DMeshInfo* PeekMeshInfo(long) const { return &g_mesh; } };
static CMeshDataBank g_bank; static const CMeshDataBank& GetMeshBank() { return g_bank; }
static long GetTickRate() { return 30; } static float GetOneOverTickRate() { return 0.5f; }
namespace NFractal { class CPerlinNoise { public: float Value; mutable float LastX, LastY; float GetGaussian2DNoise(const C2DVector& p) const { LastX = p.X; LastY = p.Y; return Value; } }; }
class CParticleEmitterManager { public: NFractal::CPerlinNoise Noise; const NFractal::CPerlinNoise& PeekPerlinNoise() const { return Noise; } };
static CParticleEmitterManager g_manager; static CParticleEmitterManager& GetParticleEmitterManager() { return g_manager; }
class CRightHandedSet { public: C3DVector R, F, U; C3DVector GetRight() const { return R; } C3DVector GetForward() const { return F; } C3DVector GetUp() const { return U; } };
class CQuaternion { public: C3DVector R, F, U; void ToRightHandedSet(CRightHandedSet& s) const { s.R = R; s.F = F; s.U = U; } };
class CEnginePrimitiveHandle { public: int ID; CEnginePrimitiveHandle(int id = 0) : ID(id) {} };
struct Prim { int Kind; long Bank; C3DVector A, B; CRGBColour Colour; float Width; C3DVector Size; Prim() : Kind(0), Bank(0), Width(0) {} };
struct CEnginePrimitiveParticleMeshGroup : Prim { CEnginePrimitiveParticleMeshGroup(long b, const CMatrix3x4&, EEnginePrimitiveBlendMode, EEnginePrimitiveBlendOp) { Kind = 1; Bank = b; } };
struct CEnginePrimitiveParticleLineGroup : Prim { CEnginePrimitiveParticleLineGroup(long b, unsigned short, EEnginePrimitiveBlendMode, EEnginePrimitiveBlendOp) { Kind = 2; Bank = b; } };
struct CEnginePrimitiveParticleMesh : Prim { CEnginePrimitiveParticleMesh(int, const C3DVector& p, const CRightHandedSet&, const C3DVector& s, const CRGBColour& c, EEngineInterpolationSet) { Kind = 3; A = p; Size = s; Colour = c; } };
struct CEnginePrimitiveParticleLine : Prim { CEnginePrimitiveParticleLine(int, const C3DVector& a, const C3DVector& b, const CRGBColour& c, float w, EEngineInterpolationSet) { Kind = 4; A = a; B = b; Colour = c; Width = w; } };
struct Event { int Parent; long Child; bool Added; Prim P; };
class CIEngine { public: std::vector<Event> E; long Next; CIEngine() : Next(100) {}
    void SetChildPrimitive(CEnginePrimitiveHandle& g, long& index, CEnginePrimitiveHandle& out, const Prim& p, int) { Event e; e.Parent = g.ID; e.Child = index; e.Added = false; e.P = p; E.push_back(e); out.ID = 50 + p.Kind; }
    void SetChildPrimitive(CEnginePrimitiveHandle& h, long id, const Prim& p, int) { Event e; e.Parent = h.ID; e.Child = id; e.Added = false; e.P = p; E.push_back(e); }
    long AddChildPrimitive(CEnginePrimitiveHandle& h, const Prim& p, int) { Event e; e.Parent = h.ID; e.Child = Next; e.Added = true; e.P = p; E.push_back(e); return Next++; } };
class CEnginePrimitivePrepareInfo { public: CIEngine* Engine; };
class CParticle { public: bool Active; float Lifetimer, MaxLife; C3DVector Pos, Rec2, RecL; float LastRec; CQuaternion Orient;
    std::vector<std::vector<long> > IDs; std::vector<int> Parents;
    CParticle() : Active(true), Lifetimer(0), MaxLife(10), LastRec(-1) {}
    float GetInterpolatedLifetimer(float t) const { return Lifetimer + t; }
    bool IsActive() const { return Active; }
    float GetMaxLife() const { return MaxLife; }
    float GetInterpolatedLifetimerInSeconds(float t) const { return (Lifetimer + t) * 0.5f; }
    CQuaternion GetInterpolatedOrientation(float) const { return Orient; }
    C3DVector GetInterpolatedPos(float) const { return Pos; }
    C3DVector GetRecordedPos(float d, float) { LastRec = d; return d == 2.0f ? Rec2 : RecL; }
    float GetFlickerTimeOffset() const { return 1.0f; } float GetFlickerIndex() const { return 3.0f; }
    bool GetPrimID(long list, long index, long& id) const { if ((unsigned long)list < IDs.size() && (unsigned long)index < IDs[list].size()) { id = IDs[list][index]; return id >= 0; } return false; }
    void SetPrimitiveInfo(long list, long index, const CEnginePrimitiveHandle& h, long id) { if (IDs.size() <= (unsigned long)list) IDs.resize(list + 1);
        if (IDs[list].size() <= (unsigned long)index) IDs[list].resize(index + 1, -1); IDs[list][index] = id; if (Parents.size() <= (unsigned long)list) Parents.resize(list + 1); Parents[list] = h.ID; } };
class CParticleSystem { public: typedef std::list<CParticle*>::iterator ParticleIterator; std::list<CParticle*> L; float Fade, Alpha, Dim; C3DVector Scale, Offset; bool Local; std::vector<CParticle*> Cleared;
    CParticleSystem() : Fade(1), Alpha(1), Dim(1), Scale(1, 1, 1), Local(false) {}
    float GetFadeAlpha() const { return Fade; } float GetGlobalAlpha() const { return Alpha; } const C3DVector& GetScale() const { return Scale; }
    void GetParticleWorldTransform(CMatrix3x4& m) const { m.E11 = 1; }
    std::list<CParticle*>& GetParticleList() { return L; }
    void TransformPosIfLocal(C3DVector& p) const { if (Local) p = p + Offset; }
    void ScaleParticleDimensions(float& w) const { w *= Dim; }
    void ClearParticle(ParticleIterator& it) { Cleared.push_back(*it); } };
template <class K, class V> class CParticleUpdateParameters { public: bool Has; C3DVector Val; mutable K Asked; CParticleUpdateParameters() : Has(false), Asked(0) {}
    bool GetVectorParam(C3DVector& v, const K& k) const { Asked = k; if (Has) { v = Val; return true; } return false; } };
class CPSCRenderMesh { public:
    long BankIndex, TrailBankIndex; CRGBColour TrailStartColour, TrailMidColour, TrailEndColour, StartColour, MidColour, EndColour;
    unsigned long BlendMode : 3; unsigned long TrailBlendMode : 3; unsigned long BlendOp : 3; unsigned long TrailBlendOp : 3;
    unsigned long FadeInEndInteger : 10; unsigned long FadeOutBeginInteger : 10;
    unsigned long TrailLengthInteger : 12; unsigned long FlickerMinAlphaInteger : 10; unsigned long FlickerMinSizeInteger : 10;
    unsigned long StartRenderSizeX : 11; unsigned long StartRenderSizeY : 11; unsigned long StartRenderSizeZ : 11; unsigned long CentredOnPos : 1;
    unsigned long AlphaFadeEnable : 1; unsigned long EndRenderSizeX : 11; unsigned long EndRenderSizeY : 11; unsigned long EndRenderSizeZ : 11;
    unsigned long SizeFadeEnable : 1; unsigned long FlickerEnable : 1; unsigned long TrailUseStartColour : 1; unsigned long TrailUseMidColour : 1;
    unsigned long TrailUseEndColour : 1; unsigned long UseRenderSizeParam : 1; unsigned long FlickerBias : 8; unsigned long FlickerSpeed : 12;
    unsigned long AlphaFadeMinimum : 7; unsigned long UseStartColour : 1; unsigned long SizeFadeMinimum : 7; unsigned long UseMidColour : 1;
    unsigned long TrailWidth : 10; unsigned long UseEndColour : 1; unsigned long RenderSizeParam;
    CEnginePrimitiveHandle PrimHandle; long ChildPrimIndex; CEnginePrimitiveHandle TrailPrimHandle; long ChildTrailPrimIndex; int Clears;
    void ClearAllPrimitives(CIEngine*, CEnginePrimitiveHandle&) { ++Clears; }
    CPSCRenderMesh() { BankIndex = 0; TrailBankIndex = 0; StartColour = MidColour = EndColour = TrailStartColour = TrailMidColour = TrailEndColour = CRGBColour(0, 0, 0, 0);
        BlendMode = TrailBlendMode = BlendOp = TrailBlendOp = 0; FadeInEndInteger = 0; FadeOutBeginInteger = 1000; TrailLengthInteger = 0;
        FlickerMinAlphaInteger = FlickerMinSizeInteger = 0; StartRenderSizeX = StartRenderSizeY = StartRenderSizeZ = 0; CentredOnPos = 0; AlphaFadeEnable = 0;
        EndRenderSizeX = EndRenderSizeY = EndRenderSizeZ = 0; SizeFadeEnable = FlickerEnable = 0; TrailUseStartColour = TrailUseMidColour = TrailUseEndColour = 0;
        UseRenderSizeParam = 0; FlickerBias = 0; FlickerSpeed = 0; AlphaFadeMinimum = 0; UseStartColour = 0; SizeFadeMinimum = 0; UseMidColour = 0; TrailWidth = 0;
        UseEndColour = 0; RenderSizeParam = 0; PrimHandle.ID = 11; TrailPrimHandle.ID = 12; ChildPrimIndex = 1; ChildTrailPrimIndex = 2; Clears = 0; }
    void PreparePrimitives(const float interpolate, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                           const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& system_group);
};
// ---- the row's body, verbatim ----
void CPSCRenderMesh::PreparePrimitives(const float interpolate, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                       const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& system_group)
{
    if (BankIndex > 0 && GetMeshBank().PeekMesh(BankIndex)->GetDataType() == 4) {
        const float startSizeX = GFUnpackFloat(StartRenderSizeX, 11, 0.0f, 20.0f, 2);
        const float startSizeY = GFUnpackFloat(StartRenderSizeY, 11, 0.0f, 20.0f, 2);
        const float startSizeZ = GFUnpackFloat(StartRenderSizeZ, 11, 0.0f, 20.0f, 2);
        const float endSizeX = GFUnpackFloat(EndRenderSizeX, 11, 0.0f, 20.0f, 2);
        const float endSizeY = GFUnpackFloat(EndRenderSizeY, 11, 0.0f, 20.0f, 2);
        const float endSizeZ = GFUnpackFloat(EndRenderSizeZ, 11, 0.0f, 20.0f, 2);
        const float alphaFadeMinimum = GFUnpackFloat(AlphaFadeMinimum, 7, 0.0f, 1.0f, 2);
        const float sizeFadeMinimum = GFUnpackFloat(SizeFadeMinimum, 7, 0.0f, 1.0f, 2);
        const float flickerSpeed = GFUnpackFloat(FlickerSpeed, 12, 0.0f, 30.0f, 2);
        const float flickerBias = GFUnpackFloat(FlickerBias, 8, -1.0f, 1.0f, 2);
        const float trailWidth = GFUnpackFloat(TrailWidth, 10, 0.0f, 10.0f, 2);
        const float tickRate = (float)GetTickRate();
        const float fadeInEnd = FadeInEndInteger * 0.001f;
        const float fadeOutBegin = FadeOutBeginInteger * 0.001f;
        const float fadeAlpha = system.GetFadeAlpha();
        const float flickerRange = (float)fabs(flickerBias) + 0.5f;
        EEnginePrimitiveBlendMode blendMode = (EEnginePrimitiveBlendMode)BlendMode;
        EEnginePrimitiveBlendOp blendOp = (EEnginePrimitiveBlendOp)BlendOp;
        CRGBColour colour;
#pragma inline_depth(0)
        CRGBColour colours[3];
#pragma inline_depth()
        long trailPrimIndex = 0;
        if (BankIndex) {
            CMatrix3x4 worldTransform;
            system.GetParticleWorldTransform(worldTransform);
            info.Engine->SetChildPrimitive(system_group, ChildPrimIndex, PrimHandle,
                CEnginePrimitiveParticleMeshGroup(BankIndex, worldTransform, blendMode, blendOp), 0);
            trailPrimIndex = 1;
            colours[0] = UseStartColour ? StartColour : UseMidColour ? MidColour : UseEndColour ? EndColour : CRGBColour(255, 255, 255, 255);
            colours[2] = UseEndColour ? EndColour : UseMidColour ? MidColour : UseStartColour ? StartColour : CRGBColour(255, 255, 255, 255);
            colours[1] = UseMidColour ? MidColour : GFGetInterpolatedColour(colours[0], colours[2], 128);
            colours[0].A = (unsigned char)(colours[0].A * fadeAlpha);
            colours[1].A = (unsigned char)(colours[1].A * fadeAlpha);
            colours[2].A = (unsigned char)(colours[2].A * fadeAlpha);
        }
        EEnginePrimitiveBlendMode trailBlendMode = (EEnginePrimitiveBlendMode)TrailBlendMode;
        EEnginePrimitiveBlendOp trailBlendOp = (EEnginePrimitiveBlendOp)TrailBlendOp;
#pragma inline_depth(0)
        CRGBColour trailColours[3];
#pragma inline_depth()
        if (TrailBankIndex) {
            trailColours[0] = TrailUseStartColour ? TrailStartColour : TrailUseMidColour ? TrailMidColour : TrailUseEndColour ? TrailEndColour : CRGBColour(255, 255, 255, 255);
            trailColours[2] = TrailUseEndColour ? TrailEndColour : TrailUseMidColour ? TrailMidColour : TrailUseStartColour ? TrailStartColour : CRGBColour(255, 255, 255, 255);
            trailColours[1] = TrailUseMidColour ? TrailMidColour : GFGetInterpolatedColour(trailColours[0], trailColours[2], 128);
            info.Engine->SetChildPrimitive(system_group, ChildTrailPrimIndex, TrailPrimHandle,
                CEnginePrimitiveParticleLineGroup(TrailBankIndex, 0, trailBlendMode, trailBlendOp), 0);
        }
        for (CParticleSystem::ParticleIterator it = system.GetParticleList().begin(); it != system.GetParticleList().end(); ++it) {
            CParticle* particle = *it;
            float lifetimer = particle->GetInterpolatedLifetimer(interpolate);
            if (particle->IsActive() && particle->GetMaxLife() != 0) {
                float lifeSeconds = GetOneOverTickRate() * lifetimer;
                float life = lifetimer / particle->GetMaxLife();
                if (life < 0.0f)
                    life = 0.0f;
                else if (life > 1.0f)
                    life = 1.0f;
                float sizeLife = life;
                if (sizeLife < 0.0f)
                    sizeLife = 0.0f;
                else {
                    float limit = 1.0f - DELTA_NOTIONAL_ZERO;
                    if (sizeLife > limit)
                        sizeLife = limit;
                }
                unsigned long colourPos = GFFloatToLongNear(life * 511.0f);
                unsigned long colourIndex = colourPos >> 8;
                unsigned long colourFrac = colourPos & 0xff;
                colour = GFGetInterpolatedColour(colours[colourIndex], colours[colourIndex + 1], colourFrac);
                float timeSeconds = particle->GetInterpolatedLifetimerInSeconds(interpolate);   // unused (retail calls 0x00AF85E0 and pops the result)
                CQuaternion orientation = particle->GetInterpolatedOrientation(interpolate);
                CRightHandedSet rhs;
                orientation.ToRightHandedSet(rhs);
                const C3DBoundingSphere meshSphere = GetMeshBank().PeekMeshInfo(BankIndex)->GetBoundingSphere();
                float meshScale = 1.0f / (meshSphere.Radius * 0.01f);
                C3DVector size;
                if (!UseRenderSizeParam || !params.GetVectorParam(size, RenderSizeParam)) {
                    size.X = ((endSizeX - startSizeX) * sizeLife + startSizeX) * meshScale;
                    size.Y = ((endSizeY - startSizeY) * sizeLife + startSizeY) * meshScale;
                    size.Z = ((endSizeZ - startSizeZ) * sizeLife + startSizeZ) * meshScale;
                }
                C3DVector pos = particle->GetInterpolatedPos(interpolate);
                if (CentredOnPos) {
                    C3DVector forward = rhs.GetForward();
                    C3DVector up = rhs.GetUp();
#pragma inline_depth(0)
                    C3DVector right = rhs.GetRight();
#pragma inline_depth()
                    const C3DVector& scale = system.GetScale();
                    right *= size.X * scale.X * 0.01f;
                    forward *= size.Y * scale.Y * 0.01f;
                    up *= size.Z * scale.Z * 0.01f;
                    CMatrix3x4 matrix;
                    matrix.E11 = -right.X; matrix.E12 = -right.Y; matrix.E13 = -right.Z;
                    matrix.E21 = -forward.X; matrix.E22 = -forward.Y; matrix.E23 = -forward.Z;
                    matrix.E31 = up.X; matrix.E32 = up.Y; matrix.E33 = up.Z;
                    C3DVector offset;
                    matrix.TransformVector(meshSphere.Center, offset);
                    pos -= offset;
                }
                float alpha = system.GetGlobalAlpha();
                float sizeScale = 1.0f;
                if (life < fadeInEnd)
                    alpha *= GFSmoothFadeValue(life / fadeInEnd);
                if (life > fadeOutBegin)
                    alpha *= GFSmoothFadeValue((1.0f - life) / (1.0f - fadeOutBegin));
                float alphaScale = 1.0f;
                if (alpha < 1.0f) {
                    if (AlphaFadeEnable)
                        alphaScale = (1.0f - alphaFadeMinimum) * alpha + alphaFadeMinimum;
                    if (SizeFadeEnable)
                        sizeScale = (1.0f - sizeFadeMinimum) * alpha + sizeFadeMinimum;
                }
                if (FlickerEnable) {
                    const NFractal::CPerlinNoise& noise = GetParticleEmitterManager().PeekPerlinNoise();
                    C2DVector noiseLookup(lifeSeconds * flickerSpeed + particle->GetFlickerTimeOffset(), particle->GetFlickerIndex());
                    float flicker = noise.GetGaussian2DNoise(noiseLookup) * flickerRange + flickerBias + 0.5f;
                    if (flicker < 0.0f)
                        flicker = 0.0f;
                    else if (flicker > 1.0f)
                        flicker = 1.0f;
                    float minAlpha = FlickerMinAlphaInteger * 0.001f;
                    alphaScale *= (1.0f - minAlpha) * flicker + minAlpha;
                    float minSize = FlickerMinSizeInteger * meshScale * 0.001f;
                    sizeScale *= (1.0f - minSize) * flicker + minSize;
                }
                size *= sizeScale;
                colour.A = (unsigned char)GFFloatToLongNear(colour.A * alphaScale);
                if (colour.A > 0) {
                    long id;
                    if (particle->GetPrimID(0, 0, id))
                        info.Engine->SetChildPrimitive(PrimHandle, id,
                            CEnginePrimitiveParticleMesh(0, pos, rhs, size, colour, (EEngineInterpolationSet)0), 0);
                    else {
                        id = info.Engine->AddChildPrimitive(PrimHandle,
                            CEnginePrimitiveParticleMesh(0, pos, rhs, size, colour, (EEngineInterpolationSet)0), 0);
                        particle->SetPrimitiveInfo(0, 0, PrimHandle, id);
                    }
                    if (TrailBankIndex) {
                        colour = GFGetInterpolatedColour(trailColours[colourIndex], trailColours[colourIndex + 1], colourFrac);
                        float trailLength = TrailLengthInteger * 0.001f;
                        C3DVector trailStart = pos;
                        C3DVector trailEnd;
                        if (trailLength > 2.0f) {
                            float trailScale = trailLength * 0.5f;
                            trailEnd = particle->GetRecordedPos(2.0f, interpolate);
                            trailEnd = (trailEnd - pos) * trailScale + pos;
                        } else
                            trailEnd = particle->GetRecordedPos(trailLength, interpolate);
                        system.TransformPosIfLocal(trailStart);
                        system.TransformPosIfLocal(trailEnd);
                        float width = sizeScale * meshScale * trailWidth;
                        system.ScaleParticleDimensions(width);
                        if (particle->GetPrimID(trailPrimIndex, 0, id))
                            info.Engine->SetChildPrimitive(TrailPrimHandle, id,
                                CEnginePrimitiveParticleLine(0, trailStart, trailEnd, colour, width, (EEngineInterpolationSet)0), 0);
                        else {
                            id = info.Engine->AddChildPrimitive(TrailPrimHandle,
                                CEnginePrimitiveParticleLine(0, trailStart, trailEnd, colour, width, (EEngineInterpolationSet)0), 0);
                            particle->SetPrimitiveInfo(trailPrimIndex, 0, TrailPrimHandle, id);
                        }
                    }
                }
            } else if (lifetimer > particle->GetMaxLife())
                system.ClearParticle(it);
        }
    } else
        ClearAllPrimitives(info.Engine, system_group);
}
// ---- end of the row's body ----
static CPSCRenderMesh MeshOnly() { CPSCRenderMesh m; m.BankIndex = 3; m.EndRenderSizeX = m.EndRenderSizeY = m.EndRenderSizeZ = 2047;
    m.UseStartColour = 1; m.StartColour = CRGBColour(200, 100, 50, 255); m.UseEndColour = 1; m.EndColour = CRGBColour(0, 0, 0, 255); return m; }
int main()
{
    CEnginePrimitiveHandle group(1); CParticleUpdateParameters<unsigned long, unsigned long> p;
    g_mesh.Type = 4; g_mesh.S.Radius = 100.0f; g_mesh.S.Center = C3DVector(10, 20, 30);
    // 1. gate: no bank or a non-mesh data type -> ClearAllPrimitives only.
    {
        CIEngine e; CEnginePrimitivePrepareInfo info; info.Engine = &e; CParticleSystem s; CParticle a; s.L.push_back(&a);
        CPSCRenderMesh m = MeshOnly(); m.BankIndex = 0; m.PreparePrimitives(1.0f, info, s, p, group);
        CHECK(m.Clears == 1 && e.E.size() == 0);
        m.BankIndex = 3; g_mesh.Type = 2; m.PreparePrimitives(1.0f, info, s, p, group); g_mesh.Type = 4;
        CHECK(m.Clears == 2 && e.E.size() == 0);
    }
    // 2. mesh only: group set, colour ramp with fade alpha, size lerp at life 0.5, add then set on the second frame.
    {
        CIEngine e; CEnginePrimitivePrepareInfo info; info.Engine = &e; CParticleSystem s; s.Fade = 0.5f;
        CParticle a; a.Lifetimer = 4.0f; a.Pos = C3DVector(1, 2, 3); s.L.push_back(&a);
        CPSCRenderMesh m = MeshOnly(); m.PreparePrimitives(1.0f, info, s, p, group);
        CHECK(m.Clears == 0 && e.E.size() == 2);
        CHECK(e.E[0].P.Kind == 1 && e.E[0].Parent == 1 && e.E[0].Child == 1 && e.E[0].P.Bank == 3 && m.PrimHandle.ID == 51);
        CHECK(e.E[1].P.Kind == 3 && e.E[1].Added && e.E[1].Parent == 51 && veq(e.E[1].P.A, C3DVector(1, 2, 3)));
        CHECK(veq(e.E[1].P.Size, C3DVector(10, 10, 10)));
        CHECK(e.E[1].P.Colour.R == 100 && e.E[1].P.Colour.G == 50 && e.E[1].P.Colour.B == 25 && e.E[1].P.Colour.A == 127);
        CHECK(a.IDs.size() == 1 && a.IDs[0][0] == 100 && a.Parents[0] == 51);
        m.PreparePrimitives(1.0f, info, s, p, group);
        CHECK(e.E.size() == 4 && e.E[3].P.Kind == 3 && !e.E[3].Added && e.E[3].Child == 100);
        // the render-size parameter replaces the lerp
        m.UseRenderSizeParam = 1; m.RenderSizeParam = 9; CParticleUpdateParameters<unsigned long, unsigned long> q; q.Has = true; q.Val = C3DVector(1, 2, 3);
        m.PreparePrimitives(1.0f, info, s, q, group);
        CHECK(q.Asked == 9 && veq(e.E.back().P.Size, C3DVector(1, 2, 3)));
    }
    // 3. CentredOnPos: the oriented mesh-centre offset is subtracted from the position.
    {
        CIEngine e; CEnginePrimitivePrepareInfo info; info.Engine = &e; CParticleSystem s;
        CParticle a; a.Lifetimer = 4.0f; a.Pos = C3DVector(1, 2, 3); a.Orient.R = C3DVector(1, 0, 0); a.Orient.F = C3DVector(0, 1, 0); a.Orient.U = C3DVector(0, 0, 1);
        s.L.push_back(&a);
        CPSCRenderMesh m = MeshOnly(); m.CentredOnPos = 1; m.PreparePrimitives(1.0f, info, s, p, group);
        CHECK(e.E.size() == 2 && veq(e.E[1].P.A, C3DVector(2, 4, 0)));
    }
    // 4. fade in / out with the alpha and size fade minimums; zero alpha submits nothing.
    {
        CIEngine e; CEnginePrimitivePrepareInfo info; info.Engine = &e; CParticleSystem s;
        CParticle a; a.Lifetimer = 4.0f; s.L.push_back(&a);
        CPSCRenderMesh m = MeshOnly(); m.FadeInEndInteger = 1000; m.AlphaFadeEnable = 1; m.SizeFadeEnable = 1;
        m.PreparePrimitives(1.0f, info, s, p, group);
        CHECK(e.E.size() == 2 && e.E[1].P.Colour.A == 64 && veq(e.E[1].P.Size, C3DVector(2.5f, 2.5f, 2.5f)));
        m.FadeInEndInteger = 0; m.FadeOutBeginInteger = 0; m.SizeFadeMinimum = 127; m.PreparePrimitives(1.0f, info, s, p, group);
        CHECK(e.E.size() == 4 && e.E[3].P.Colour.A == 64 && veq(e.E[3].P.Size, C3DVector(10, 10, 10)));
        s.Alpha = 0.0f; m.PreparePrimitives(1.0f, info, s, p, group);
        CHECK(e.E.size() == 5 && e.E[4].P.Kind == 1);
    }
    // 5. flicker: noise lookup (seconds * speed + offset, index), clamp, minimum alpha / size.
    {
        CIEngine e; CEnginePrimitivePrepareInfo info; info.Engine = &e; CParticleSystem s;
        CParticle a; a.Lifetimer = 4.0f; s.L.push_back(&a);
        CPSCRenderMesh m = MeshOnly(); m.FlickerEnable = 1; m.FlickerBias = 255; m.FlickerSpeed = 4095; m.FlickerMinAlphaInteger = 500; m.FlickerMinSizeInteger = 500;
        g_manager.Noise.Value = -1.0f; m.PreparePrimitives(1.0f, info, s, p, group);
        CHECK(feq(g_manager.Noise.LastX, 76.0f) && feq(g_manager.Noise.LastY, 3.0f));
        CHECK(e.E.size() == 2 && e.E[1].P.Colour.A == 128 && veq(e.E[1].P.Size, C3DVector(5, 5, 5)));
        g_manager.Noise.Value = 0.0f; m.PreparePrimitives(1.0f, info, s, p, group);
        CHECK(e.E.size() == 4 && e.E[3].P.Colour.A == 255 && veq(e.E[3].P.Size, C3DVector(10, 10, 10)));
    }
    // 6. trails: line group, short trail (recorded position at the trail length), local transform of both ends, width scale; long trail.
    {
        CIEngine e; CEnginePrimitivePrepareInfo info; info.Engine = &e; CParticleSystem s; s.Local = true; s.Offset = C3DVector(100, 0, 0); s.Dim = 2.0f;
        CParticle a; a.Lifetimer = 4.0f; a.Pos = C3DVector(1, 2, 3); a.RecL = C3DVector(1, 2, 7); a.Rec2 = C3DVector(1, 6, 3); s.L.push_back(&a);
        CPSCRenderMesh m = MeshOnly(); m.TrailBankIndex = 5; m.TrailLengthInteger = 1500; m.TrailWidth = 1023;
        m.TrailUseStartColour = 1; m.TrailStartColour = CRGBColour(10, 20, 30, 40); m.TrailUseEndColour = 1; m.TrailEndColour = CRGBColour(50, 60, 70, 80);
        m.PreparePrimitives(1.0f, info, s, p, group);
        CHECK(e.E.size() == 4 && e.E[1].P.Kind == 2 && e.E[1].Parent == 1 && e.E[1].Child == 2 && e.E[1].P.Bank == 5 && m.TrailPrimHandle.ID == 52);
        CHECK(e.E[3].P.Kind == 4 && e.E[3].Added && e.E[3].Parent == 52 && feq(a.LastRec, 1.5f));
        CHECK(veq(e.E[3].P.A, C3DVector(101, 2, 3)) && veq(e.E[3].P.B, C3DVector(101, 2, 7)) && feq(e.E[3].P.Width, 20.0f));
        CHECK(e.E[3].P.Colour.R == 30 && e.E[3].P.Colour.G == 40 && e.E[3].P.Colour.B == 50 && e.E[3].P.Colour.A == 60);
        CHECK(a.IDs.size() == 2 && a.IDs[1][0] == e.E[3].Child && a.Parents[1] == 52);
        m.TrailLengthInteger = 3000; m.PreparePrimitives(1.0f, info, s, p, group);
        CHECK(e.E.size() == 8 && e.E[7].P.Kind == 4 && !e.E[7].Added && feq(a.LastRec, 2.0f) && veq(e.E[7].P.B, C3DVector(101, 8, 3)));
    }
    // 7. dead particles: only an inactive (or zero-life) particle past MaxLife is cleared.
    {
        CIEngine e;
        CEnginePrimitivePrepareInfo info; info.Engine = &e; CParticleSystem s;
        CParticle dead, young, zero; dead.Active = false; dead.Lifetimer = 20.0f; young.Active = false; young.Lifetimer = 2.0f; zero.MaxLife = 0.0f; zero.Lifetimer = 1.0f;
        s.L.push_back(&dead); s.L.push_back(&young); s.L.push_back(&zero);
        CPSCRenderMesh m = MeshOnly(); m.PreparePrimitives(1.0f, info, s, p, group);
        CHECK(s.Cleared.size() == 2 && s.Cleared[0] == &dead && s.Cleared[1] == &zero && e.E.size() == 1);
    }
    if (g_fail) { printf("00b03ca0_TEST FAIL (%d)\n", g_fail); return 1; }
    printf("00b03ca0_TEST PASS\n");
    return 0;
}