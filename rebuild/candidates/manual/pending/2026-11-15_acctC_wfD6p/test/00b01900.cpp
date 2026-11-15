// Behaviour test for NParticleEngine::CPSCRenderMesh::AddToBoundingSphere (0x00B01900): the row's body verbatim over a controlled model of
// its world -- a recording bounding sphere, a mesh bank with one mesh sphere, a particle system (local transform = offset, dimension
// scale), particles with explicit active flags / positions / recorded positions / lifetimes / orientation frames, and an update-parameter
// map. Checks: the trail segment sphere (short v long trail, width floor), the mesh sphere (size lerp over life with both clamps, the
// render-size parameter v its absence, the dimension scale, the oriented mesh-centre offset v CentredOnPos), the dead-particle clear
// (only past MaxLife), the return value and the primitive-handle clear when nothing was added.
#include <stdio.h>
#include <math.h>
#include <list>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("check failed line %d\n", __LINE__); ++g_fail; } } while (0)
static bool feq(float a, float b) { return fabs(a - b) < 1e-3f; }
#define FABLE_RETAIL_ASSERT(c) ((void)0)
class C3DVector { public: float X, Y, Z; C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); } };
static bool veq(const C3DVector& a, const C3DVector& b) { return feq(a.X, b.X) && feq(a.Y, b.Y) && feq(a.Z, b.Z); }
static float GFUnpackFloat(unsigned long p, unsigned long bits, float lo, float hi, unsigned long) { return lo + (float)p * (hi - lo) / (float)((1 << bits) - 1); }
static const float DELTA_NOTIONAL_ZERO = 0.0001f;
struct Add { C3DVector c; float r; };
class C3DBoundingSphere { public: C3DVector Center; float Radius; std::vector<Add> Adds; C3DBoundingSphere() : Radius(0) {}
    void AddToBounds(const C3DVector& c, float r) { Add a; a.c = c; a.r = r; Adds.push_back(a); } };
class C3DMeshInfo { public: C3DBoundingSphere S; C3DBoundingSphere GetBoundingSphere() const { return S; } };
static C3DMeshInfo g_mesh; static long g_peekIndex = -1;
class CMeshDataBank { public: const C3DMeshInfo* PeekMeshInfo(long i) const { g_peekIndex = i; return &g_mesh; } };
static CMeshDataBank g_bank; static const CMeshDataBank& GetMeshBank() { return g_bank; }
class CRightHandedSet { public: C3DVector R, F, U; C3DVector GetRight() const { return R; } const C3DVector& GetForward() const { return F; } const C3DVector& GetUp() const { return U; } };
class CQuaternion { public: C3DVector R, F, U; void ToRightHandedSet(CRightHandedSet& s) const { s.R = R; s.F = F; s.U = U; } };
class CMatrix3x4 { public: float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void TransformVector(const C3DVector& pSource, C3DVector& destination) const {
        destination.X = pSource.X * E11 + pSource.Y * E21 + pSource.Z * E31;
        destination.Y = pSource.X * E12 + pSource.Y * E22 + pSource.Z * E32;
        destination.Z = pSource.X * E13 + pSource.Y * E23 + pSource.Z * E33; } };
class CParticle { public: bool Active, ActiveAt; C3DVector Pos, Rec2, RecL; float Lifetimer, CreationTime; long MaxLife; CQuaternion Orient; float lastRecLen;
    CParticle() : Active(true), ActiveAt(true), Lifetimer(0), CreationTime(0), MaxLife(10), lastRecLen(-1) {}
    bool IsActive() const { return Active; }
    bool IsActive(float) const { return ActiveAt; }
    C3DVector GetInterpolatedPos(float) const { return Pos; }
    C3DVector GetRecordedPos(float len, float) { lastRecLen = len; return len == 2.0f ? Rec2 : RecL; }
    float GetInterpolatedLifetimer(float t) const { return Lifetimer - CreationTime + t; }
    long GetMaxLife() const { return MaxLife; }
    CQuaternion GetInterpolatedOrientation(float) const { return Orient; } };
class CParticleSystem { public: std::list<CParticle*> L; C3DVector Offset; float Scale; std::vector<CParticle*> Cleared;
    CParticleSystem() : Scale(1) {}
    std::list<CParticle*>& GetParticleList() { return L; }
    void TransformPosIfLocal(C3DVector& p) const { p += Offset; }
    void ScaleParticleDimensions(C3DVector& s) const { s *= Scale; }
    void ClearParticle(std::list<CParticle*>::iterator& it) { Cleared.push_back(*it); } };
template <class K, class V> class CParticleUpdateParameters { public: bool Has; K Key; C3DVector Val; mutable K asked;
    CParticleUpdateParameters() : Has(false), Key(0), asked(0) {}
    bool GetVectorParam(C3DVector& v, const K& k) const { asked = k; if (Has && k == Key) { v = Val; return true; } return false; } };
class CEnginePrimitiveHandle { public: int Cleared; CEnginePrimitiveHandle() : Cleared(0) {} void Clear() { ++Cleared; } };
class CPSCRenderMesh { public:
    long BankIndex, TrailBankIndex; unsigned long TrailLengthInteger, StartRenderSizeX, StartRenderSizeY, StartRenderSizeZ, CentredOnPos,
        EndRenderSizeX, EndRenderSizeY, EndRenderSizeZ, UseRenderSizeParam, TrailWidth, RenderSizeParam;
    CEnginePrimitiveHandle PrimHandle, TrailPrimHandle;
    CPSCRenderMesh() : BankIndex(0), TrailBankIndex(0), TrailLengthInteger(0), StartRenderSizeX(0), StartRenderSizeY(0), StartRenderSizeZ(0),
        CentredOnPos(0), EndRenderSizeX(0), EndRenderSizeY(0), EndRenderSizeZ(0), UseRenderSizeParam(0), TrailWidth(0), RenderSizeParam(0) {}
    bool AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params);
};
// ---- the row's body, verbatim ----
bool CPSCRenderMesh::AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params)
{
    const float startSizeX = GFUnpackFloat(StartRenderSizeX, 11, 0.0f, 20.0f, 2);
    const float startSizeY = GFUnpackFloat(StartRenderSizeY, 11, 0.0f, 20.0f, 2);
    const float startSizeZ = GFUnpackFloat(StartRenderSizeZ, 11, 0.0f, 20.0f, 2);
    const float endSizeX = GFUnpackFloat(EndRenderSizeX, 11, 0.0f, 20.0f, 2);
    const float endSizeY = GFUnpackFloat(EndRenderSizeY, 11, 0.0f, 20.0f, 2);
    const float endSizeZ = GFUnpackFloat(EndRenderSizeZ, 11, 0.0f, 20.0f, 2);
    float trailWidth = GFUnpackFloat(TrailWidth, 10, 0.0f, 10.0f, 2);
    float trailLength = (float)TrailLengthInteger * 0.001f;
    bool longTrail;
    float recordedLength;
    if (trailLength > 2.0f) {
        recordedLength = 2.0f;
        longTrail = true;
    } else {
        recordedLength = trailLength;
        longTrail = false;
    }
    FABLE_RETAIL_ASSERT(trailWidth >= 0.0f);
    FABLE_RETAIL_ASSERT(recordedLength <= 2.0f);
    float trailScale = recordedLength * 0.5f;
    float meshScale = 0.0f;
    C3DBoundingSphere meshSphere;
    if (BankIndex) {
        meshSphere = GetMeshBank().PeekMeshInfo(BankIndex)->GetBoundingSphere();
        FABLE_RETAIL_ASSERT(meshSphere.Radius > 0.0f);
        meshScale = 1.0f / meshSphere.Radius;
    }
    bool added = false;
    for (std::list<CParticle*>::iterator it = system.GetParticleList().begin(); it != system.GetParticleList().end(); ++it) {
        CParticle* particle = *it;
        if (particle->IsActive() && particle->IsActive(timeOffset)) {
            C3DVector pos = particle->GetInterpolatedPos(timeOffset);
            system.TransformPosIfLocal(pos);
            if (TrailBankIndex) {
                C3DVector trailEnd;
                if (longTrail) {
                    trailEnd = particle->GetRecordedPos(2.0f, timeOffset);
                    system.TransformPosIfLocal(trailEnd);
                    trailEnd = (trailEnd - pos) * trailScale + pos;
                } else {
                    trailEnd = particle->GetRecordedPos(trailLength, timeOffset);
                    system.TransformPosIfLocal(trailEnd);
                }
                C3DVector trail;
                trail = trailEnd - pos;
                float length = trail.GetMagnitude();
                if (trailWidth > length)
                    length = trailWidth;
                sphere.AddToBounds(trail * 0.5f + pos, length * 0.5f);
                added = true;
            }
            if (BankIndex) {
                C3DVector size;
                if (!UseRenderSizeParam || !params.GetVectorParam(size, RenderSizeParam)) {
                    float life = particle->GetInterpolatedLifetimer(timeOffset) / particle->GetMaxLife();
                    if (life < 0.0f)
                        life = 0.0f;
                    else {
                        float limit = 1.0f - DELTA_NOTIONAL_ZERO;
                        if (life > limit)
                            life = limit;
                    }
                    size.X = (endSizeX - startSizeX) * life + startSizeX;
                    size.Y = (endSizeY - startSizeY) * life + startSizeY;
                    size.Z = (endSizeZ - startSizeZ) * life + startSizeZ;
                }
                system.ScaleParticleDimensions(size);
                if (!CentredOnPos) {
                    CQuaternion orientation = particle->GetInterpolatedOrientation(timeOffset);
                    CRightHandedSet rhs;
                    orientation.ToRightHandedSet(rhs);
                    C3DVector right = rhs.GetRight();
                    right *= size.X * meshScale;
                    C3DVector forward = rhs.GetForward() * (size.Y * meshScale);
                    C3DVector up = rhs.GetUp();
                    up *= size.Z * meshScale;
                    CMatrix3x4 matrix;
                    matrix.E11 = -right.X; matrix.E12 = -right.Y; matrix.E13 = -right.Z;
                    matrix.E21 = -forward.X; matrix.E22 = -forward.Y; matrix.E23 = -forward.Z;
                    matrix.E31 = up.X; matrix.E32 = up.Y; matrix.E33 = up.Z;
                    C3DVector offset;
                    matrix.TransformVector(meshSphere.Center, offset);
                    pos += offset;
                }
                sphere.AddToBounds(pos, size.GetMagnitude() * 0.5f);
                added = true;
            }
        } else if (particle->GetInterpolatedLifetimer(timeOffset) > particle->GetMaxLife()) {
            system.ClearParticle(it);
        }
    }
    if (!added) {
        PrimHandle.Clear();
        TrailPrimHandle.Clear();
    }
    return added;
}
// ---- end of the row's body ----
static float U11(unsigned long p) { return (float)p * 20.0f / 2047.0f; }
int main()
{
    // 1. nothing active: no adds, both handles cleared, false; only the particle past MaxLife is cleared.
    {
        CPSCRenderMesh m; m.BankIndex = 3; m.TrailBankIndex = 1; m.TrailLengthInteger = 500;
        CParticle dead, young, half; dead.Active = false; dead.Lifetimer = 12; young.Active = false; young.Lifetimer = 4;
        half.ActiveAt = false; half.Lifetimer = 11;
        CParticleSystem s; s.L.push_back(&dead); s.L.push_back(&young); s.L.push_back(&half);
        CParticleUpdateParameters<unsigned long, unsigned long> p; C3DBoundingSphere sp; g_mesh.S.Radius = 2.0f;
        CHECK(!m.AddToBoundingSphere(sp, 0.5f, s, p));
        CHECK(sp.Adds.size() == 0); CHECK(m.PrimHandle.Cleared == 1 && m.TrailPrimHandle.Cleared == 1);
        CHECK(s.Cleared.size() == 2 && s.Cleared[0] == &dead && s.Cleared[1] == &half);
        CHECK(g_peekIndex == 3);
    }
    // 2. short trail only (no mesh): segment midpoint, half length; width floor; local offset applied to both ends.
    {
        CPSCRenderMesh m; m.TrailBankIndex = 1; m.TrailLengthInteger = 1500; m.TrailWidth = 0;
        CParticle a; a.Pos = C3DVector(1, 2, 3); a.RecL = C3DVector(1, 2, 7);
        CParticleSystem s; s.Offset = C3DVector(10, 0, 0); s.L.push_back(&a);
        CParticleUpdateParameters<unsigned long, unsigned long> p; C3DBoundingSphere sp;
        CHECK(m.AddToBoundingSphere(sp, 0.0f, s, p));
        CHECK(sp.Adds.size() == 1); CHECK(veq(sp.Adds[0].c, C3DVector(11, 2, 5))); CHECK(feq(sp.Adds[0].r, 2.0f));
        CHECK(feq(a.lastRecLen, 1.5f)); CHECK(m.PrimHandle.Cleared == 0);
        // width wins over a short segment
        m.TrailWidth = 1023; sp.Adds.clear();
        CHECK(m.AddToBoundingSphere(sp, 0.0f, s, p)); CHECK(feq(sp.Adds[0].r, 5.0f));
    }
    // 3. long trail (> 2.0): recorded position at 2.0, scaled by recordedLength / 2.
    {
        CPSCRenderMesh m; m.TrailBankIndex = 1; m.TrailLengthInteger = 3000;
        CParticle a; a.Pos = C3DVector(0, 0, 0); a.Rec2 = C3DVector(0, 6, 0);
        CParticleSystem s; s.L.push_back(&a);
        CParticleUpdateParameters<unsigned long, unsigned long> p; C3DBoundingSphere sp;
        CHECK(m.AddToBoundingSphere(sp, 0.0f, s, p));
        CHECK(feq(a.lastRecLen, 2.0f)); CHECK(veq(sp.Adds[0].c, C3DVector(0, 3, 0))); CHECK(feq(sp.Adds[0].r, 3.0f));
    }
    // 4. mesh, centred: size lerp over life (start 1023/2047*20.., end ..), dimension scale, radius |size| / 2.
    {
        CPSCRenderMesh m; m.BankIndex = 5; m.CentredOnPos = 1;
        m.StartRenderSizeX = 100; m.StartRenderSizeY = 200; m.StartRenderSizeZ = 300; m.EndRenderSizeX = 500; m.EndRenderSizeY = 600; m.EndRenderSizeZ = 700;
        CParticle a; a.Pos = C3DVector(4, 5, 6); a.Lifetimer = 2.0f; a.CreationTime = 0.5f; a.MaxLife = 10;   // life (2 - 0.5 + 1) / 10 = 0.25
        CParticleSystem s; s.Scale = 2.0f; s.L.push_back(&a);
        CParticleUpdateParameters<unsigned long, unsigned long> p; C3DBoundingSphere sp; g_mesh.S.Radius = 4.0f;
        CHECK(m.AddToBoundingSphere(sp, 1.0f, s, p));
        C3DVector e((U11(100) + (U11(500) - U11(100)) * 0.25f) * 2, (U11(200) + (U11(600) - U11(200)) * 0.25f) * 2, (U11(300) + (U11(700) - U11(300)) * 0.25f) * 2);
        CHECK(sp.Adds.size() == 1); CHECK(veq(sp.Adds[0].c, C3DVector(4, 5, 6))); CHECK(feq(sp.Adds[0].r, e.GetMagnitude() * 0.5f));
        CHECK(p.asked == 0);
        // life below 0 clamps to the start size; above 1 clamps to just under the end size
        a.Lifetimer = -5.0f; sp.Adds.clear(); m.AddToBoundingSphere(sp, 0.0f, s, p);
        CHECK(feq(sp.Adds[0].r, C3DVector(U11(100) * 2, U11(200) * 2, U11(300) * 2).GetMagnitude() * 0.5f));
        a.Lifetimer = 50.0f; sp.Adds.clear(); m.AddToBoundingSphere(sp, 0.0f, s, p);
        float l = 1.0f - DELTA_NOTIONAL_ZERO;
        CHECK(feq(sp.Adds[0].r, C3DVector((U11(100) + (U11(500) - U11(100)) * l) * 2, (U11(200) + (U11(600) - U11(200)) * l) * 2, (U11(300) + (U11(700) - U11(300)) * l) * 2).GetMagnitude() * 0.5f));
        CHECK(s.Cleared.size() == 0);
        // render-size parameter present: used instead of the lerp (still scaled); absent key falls back to the lerp
        m.UseRenderSizeParam = 1; m.RenderSizeParam = 77; p.Has = true; p.Key = 77; p.Val = C3DVector(3, 4, 0);
        sp.Adds.clear(); m.AddToBoundingSphere(sp, 0.0f, s, p); CHECK(p.asked == 77); CHECK(feq(sp.Adds[0].r, 5.0f));
        p.Key = 78; sp.Adds.clear(); m.AddToBoundingSphere(sp, 0.0f, s, p); CHECK(!feq(sp.Adds[0].r, 5.0f));
    }
    // 5. mesh, oriented: centre offset = -right * cx - forward * cy + up * cz, each axis scaled by size / mesh radius.
    {
        CPSCRenderMesh m; m.BankIndex = 1; m.CentredOnPos = 0; m.UseRenderSizeParam = 1; m.RenderSizeParam = 9;
        CParticleUpdateParameters<unsigned long, unsigned long> p; p.Has = true; p.Key = 9; p.Val = C3DVector(2, 4, 8);
        CParticle a; a.Pos = C3DVector(1, 1, 1);
        a.Orient.R = C3DVector(0, 1, 0); a.Orient.F = C3DVector(0, 0, 1); a.Orient.U = C3DVector(1, 0, 0);
        CParticleSystem s; s.L.push_back(&a);
        C3DBoundingSphere sp; g_mesh.S.Radius = 2.0f; g_mesh.S.Center = C3DVector(1, 2, 3);
        CHECK(m.AddToBoundingSphere(sp, 0.0f, s, p));
        // right (0,1,0)*1, forward (0,0,1)*2, up (1,0,0)*4: offset = -(0,1,0)*1 - (0,0,2)*2 + (4,0,0)*3 = (12,-1,-4)
        CHECK(veq(sp.Adds[0].c, C3DVector(13, 0, -3))); CHECK(feq(sp.Adds[0].r, C3DVector(2, 4, 8).GetMagnitude() * 0.5f));
    }
    if (g_fail == 0) printf("00b01900_TEST PASS\n");
    return g_fail;
}
