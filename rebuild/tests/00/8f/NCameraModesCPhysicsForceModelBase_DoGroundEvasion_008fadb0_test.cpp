// Behaviour test for 008fadb0: src/008fadb0.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// NCameraModes::CPhysicsForceModelBase::DoGroundEvasion()
#include <cstdio>
#include <cmath>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NCameraModes::CPhysicsForceModelBase::DoGroundEvasion() -- rescans the ground points within 50 units of the camera, sums a 5/distance potential at four samples and returns the gradient, flipped to point down when it points up and with its height change clamped to [-1000, 0]
// Leaf: ?DoGroundEvasion@CPhysicsForceModelBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FADB0, 446 bytes, `ret 4` (C3DVector by hidden pointer).
// Callees: the camera manager's (+0xA8) world (+0x74) slot 13 (+0x34, the world map), UpdateScan<const CWorldMap> 0x008FC430,
//   0x008FA9F0 on Ground (manifest "LinkedList_ClearAndReinit"). Constants: offsets 0x013893AC, 0.1f 0x0122EE58, 5.0f
//   0x012462A8, 10.0f 0x01237F0C, 0.0f 0x0122DEDC, -1.0f 0x0122DEE0, -1000.0f 0x012448F8.
// Shape: C3DVector has a user member-wise copy ctor (the evasion loop then spills the sample to frame temporaries and
//   adds the position from memory, as the landed 0x008FAA80); the world map is fetched into a named reference before the
//   scan call; the flip is three in-place `*= -1.0f` (retail keeps X/Y in the result's frame words and Z on the x87 stack);
//   UpdateScan<const CWorldMap> (a template instantiation in retail's TU) has a same-TU noinline body taking the centre by
//   value, so VC7.1 knows &position does not escape and sinks the position stores past the world-map loads.
// Layout: retail Ground +0x50 (PDB +0x80), CameraManager +0xA8 (PDB +0xF0).
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double);
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float Length() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
inline float Magnitude(const C3DVector& v) { return (float)sqrt(v.Z * v.Z + v.Y * v.Y + v.X * v.X); }
struct SVectorNode { SVectorNode* _M_next; SVectorNode* _M_prev; C3DVector _M_data; };
extern const C3DVector g_EvasionSampleOffsets[4];  // 0x013893AC: 0, X, Y, Z

struct CWorldMap;
struct CWorldMapOwner {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual const CWorldMap& GetWorldMap() const;  // slot 13 (+0x34)
};
struct CGameCameraManager {
    unsigned char   _pad00[0x74];
    CWorldMapOwner* World;                         // +0x74
};
namespace NCameraModes {
struct CPhysicsState {
    C3DVector Position;                            // +0x00
};
struct CPhysicsCollisionPointsObject {             // retail 0x24 bytes
    SVectorNode* Points;                           // +0x00 list<C3DVector> header
    unsigned char _pad04[0x20];
    void Finalise();                               // 0x008FA9F0
};
struct CPhysicsForceModelBase {
    virtual void v00();
    CPhysicsState*                LinkedState;     // +0x04
    unsigned char                 _pad08[0x50 - 0x08];
    CPhysicsCollisionPointsObject Ground;          // +0x50 (PDB +0x80)
    unsigned char                 _pad74[0xa8 - 0x74];
    CGameCameraManager*           CameraManager;   // +0xa8 (PDB +0xF0)
    void UpdateScan(CPhysicsCollisionPointsObject& points, const CWorldMap& map, const C3DVector& centre, long flags, float radius);   // 0x008FC430
    C3DVector DoGroundEvasion();
};

void ScanGroundPoints_X(CPhysicsForceModelBase* model, CPhysicsCollisionPointsObject& points, const CWorldMap& map,
                        float x, float y, float z, long flags, float radius);
#pragma auto_inline(off)
__declspec(noinline) void CPhysicsForceModelBase::UpdateScan(CPhysicsCollisionPointsObject& points, const CWorldMap& map,
                                                            const C3DVector& centre, long flags, float radius)
{
    ScanGroundPoints_X(this, points, map, centre.X, centre.Y, centre.Z, flags, radius);
}
#pragma auto_inline(on)

C3DVector CPhysicsForceModelBase::DoGroundEvasion()
{
    C3DVector position = LinkedState->Position;
    const CWorldMap& map = CameraManager->World->GetWorldMap();
    UpdateScan(Ground, map, position, 0, 50.0f);
    Ground.Finalise();
    SVectorNode* head = Ground.Points;
    float potential[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (SVectorNode* it = head->_M_next; it != head; it = it->_M_next) {
        for (long i = 0; i < 4; ++i) {
            C3DVector sample = g_EvasionSampleOffsets[i] * 0.1f + position;
            potential[i] += 5.0f / Magnitude(sample - it->_M_data);
        }
    }
    C3DVector result((potential[1] - potential[0]) * 10.0f, (potential[2] - potential[0]) * 10.0f, (potential[3] - potential[0]) * 10.0f);
    if (result.Z > 0.0f) {
        result.X *= -1.0f;
        result.Y *= -1.0f;
        result.Z *= -1.0f;
    }
    if (result.Z < -1000.0f)
        result.Z = -1000.0f;
    else if (result.Z > 0.0f)
        result.Z = 0.0f;
    return result;
}
}

// ---- stubs and driver ----
const C3DVector g_EvasionSampleOffsets[4] = { C3DVector(0.0f, 0.0f, 0.0f), C3DVector(1.0f, 0.0f, 0.0f),
                                              C3DVector(0.0f, 1.0f, 0.0f), C3DVector(0.0f, 0.0f, 1.0f) };
struct CWorldMap { int id; };
static CWorldMap g_map;
static int g_getMap = 0, g_scan = 0, g_final = 0, g_order = 0, g_scanAt = 0, g_finalAt = 0;
static float g_cx, g_cy, g_cz, g_radius; static long g_flags; static const void* g_points; static const void* g_model; static const void* g_mapSeen;
void CWorldMapOwner::v00() {} void CWorldMapOwner::v01() {} void CWorldMapOwner::v02() {} void CWorldMapOwner::v03() {}
void CWorldMapOwner::v04() {} void CWorldMapOwner::v05() {} void CWorldMapOwner::v06() {} void CWorldMapOwner::v07() {}
void CWorldMapOwner::v08() {} void CWorldMapOwner::v09() {} void CWorldMapOwner::v10() {} void CWorldMapOwner::v11() {}
void CWorldMapOwner::v12() {}
const CWorldMap& CWorldMapOwner::GetWorldMap() const { ++g_getMap; return g_map; }
void NCameraModes::ScanGroundPoints_X(NCameraModes::CPhysicsForceModelBase* model, NCameraModes::CPhysicsCollisionPointsObject& points,
                        const CWorldMap& map, float x, float y, float z, long flags, float radius)
{
    ++g_scan; g_scanAt = ++g_order; g_model = model; g_points = &points; g_mapSeen = &map;
    g_cx = x; g_cy = y; g_cz = z; g_flags = flags; g_radius = radius;
}
void NCameraModes::CPhysicsCollisionPointsObject::Finalise() { ++g_final; g_finalAt = ++g_order; }
void NCameraModes::CPhysicsForceModelBase::v00() {}
static bool Near(float a, float b) { return fabs(a - b) <= 1e-3 * (fabs(b) + 1.0); }
static float Pot(const C3DVector& pos, int i, const C3DVector& c)
{
    const C3DVector& o = g_EvasionSampleOffsets[i];
    double x = pos.X + o.X * 0.1 - c.X, y = pos.Y + o.Y * 0.1 - c.Y, z = pos.Z + o.Z * 0.1 - c.Z;
    return (float)(5.0 / sqrt(x * x + y * y + z * z));
}
static C3DVector Run(const C3DVector& pos, const C3DVector* pts, int n)
{
    static SVectorNode head; static SVectorNode nodes[4];
    head._M_next = head._M_prev = &head;
    for (int i = 0; i < n; ++i) {               // append
        nodes[i]._M_data = pts[i];
        nodes[i]._M_prev = head._M_prev; nodes[i]._M_next = &head;
        head._M_prev->_M_next = &nodes[i]; head._M_prev = &nodes[i];
    }
    NCameraModes::CPhysicsState state; state.Position = pos;
    CWorldMapOwner world; CGameCameraManager mgr; mgr.World = &world;
    NCameraModes::CPhysicsForceModelBase model;
    model.LinkedState = &state; model.CameraManager = &mgr; model.Ground.Points = &head;
    g_getMap = g_scan = g_final = g_order = 0;
    C3DVector r = model.DoGroundEvasion();
    CHECK(g_getMap == 1 && g_scan == 1 && g_final == 1 && g_scanAt == 1 && g_finalAt == 2);
    CHECK(g_model == &model && g_points == &model.Ground && g_mapSeen == &g_map);
    CHECK(g_cx == pos.X && g_cy == pos.Y && g_cz == pos.Z && g_flags == 0 && g_radius == 50.0f);
    return r;
}
static void Expect(const C3DVector& pos, const C3DVector* pts, int n, float ex, float ey, float ez)
{
    C3DVector r = Run(pos, pts, n);
    CHECK(Near(r.X, ex)); CHECK(Near(r.Y, ey)); CHECK(Near(r.Z, ez));
}
int main()
{
    C3DVector pos(10.0f, 20.0f, 30.0f);
    // 1: no ground points -> zero gradient
    Expect(pos, 0, 0, 0.0f, 0.0f, 0.0f);
    // 2: one point below -> gradient points down already (Z < 0): returned as is
    {
        C3DVector p[1] = { C3DVector(11.0f, 19.0f, 28.0f) };
        float p0 = Pot(pos, 0, p[0]), p1 = Pot(pos, 1, p[0]), p2 = Pot(pos, 2, p[0]), p3 = Pot(pos, 3, p[0]);
        CHECK((p3 - p0) * 10.0f < 0.0f);
        Expect(pos, p, 1, (p1 - p0) * 10.0f, (p2 - p0) * 10.0f, (p3 - p0) * 10.0f);
    }
    // 3: two points above -> summed gradient points up (Z > 0): all three components negated
    {
        C3DVector p[2] = { C3DVector(9.0f, 21.0f, 33.0f), C3DVector(10.5f, 20.0f, 32.0f) };
        float s[4];
        for (int i = 0; i < 4; ++i) s[i] = Pot(pos, i, p[0]) + Pot(pos, i, p[1]);
        CHECK((s[3] - s[0]) * 10.0f > 0.0f);
        Expect(pos, p, 2, -(s[1] - s[0]) * 10.0f, -(s[2] - s[0]) * 10.0f, -(s[3] - s[0]) * 10.0f);
    }
    // 4: a point just under the camera -> huge downward gradient, Z clamped to -1000
    {
        C3DVector p[1] = { C3DVector(10.0f, 20.0f, 29.99f) };
        float p0 = Pot(pos, 0, p[0]), p1 = Pot(pos, 1, p[0]), p2 = Pot(pos, 2, p[0]), p3 = Pot(pos, 3, p[0]);
        CHECK((p3 - p0) * 10.0f < -1000.0f);
        Expect(pos, p, 1, (p1 - p0) * 10.0f, (p2 - p0) * 10.0f, -1000.0f);
    }
    if (fails == 0) printf("008fadb0_TEST PASS\n");
    return fails;
}