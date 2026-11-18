// Behaviour test for row 0x007243C0 CTCPhysicsStandard::UpdateVelocityForImpassability (links the row's object). The test
// supplies the row's callees: the ground height, the world map and its impassability factors (one per neighbour block), the
// nine neighbour offsets, the vector reserve, the magnitude-and-normalise and the IsOnMeshSurface vtable slot (+0xDC), then
// checks the velocity against an independent float model for: high above ground (untouched), a push towards a wall
// (dot > 0, X/Y changed, Z refit when not on a mesh surface, Z kept on a mesh surface), moving away (dot <= 0, untouched), a
// centre block exactly on the cell centre (mag == 0 skipped), blocks beyond 1.4 skipped and the 1.0..1.4 fade-out band.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class C2DCoordI { public: long X, Y; };
class C3DVector {
public:
    float X, Y, Z;
    float GetMagnitudeAndNormalise();
};
float C3DVector::GetMagnitudeAndNormalise()
{
    float m = (float)sqrt(X * X + Y * Y + Z * Z);
    if (m != 0.0f) { X /= m; Y /= m; Z /= m; }
    return m;
}
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    void reserve(unsigned int n);
};
}
static float g_factors[9];
template<> void std::vector<float, std::allocator<float> >::reserve(unsigned int n)
{
    _M_start = (float*)malloc(n * sizeof(float)); _M_finish = _M_start; _M_end_of_storage = _M_start + n;
}
extern const C2DCoordI g_BlockNeighbourOffsets[9] = { {-1,-1}, {0,-1}, {1,-1}, {-1,0}, {0,0}, {1,0}, {-1,1}, {0,1}, {1,1} };
class CWorldMap {
public:
    void GetImpassabilityFactorAround(const C3DVector& pos, std::vector<float>& factors, bool navigation) const;
};
static bool g_nav_arg;
void CWorldMap::GetImpassabilityFactorAround(const C3DVector&, std::vector<float>& factors, bool navigation) const
{
    g_nav_arg = navigation;
    for (int i = 0; i < 9; ++i) factors._M_start[i] = g_factors[i];
    factors._M_finish = factors._M_start + 9;
}
static CWorldMap g_map;
static float g_ground_base;                 // ground height = base + 0.25 * x
class CTCPhysicsBase {
public:
    void** VTable;
    float GetGroundHeight(const C3DVector& pos);
protected:
    CWorldMap& GetWorldMap();
};
float CTCPhysicsBase::GetGroundHeight(const C3DVector& pos) { return g_ground_base + 0.25f * pos.X; }
CWorldMap& CTCPhysicsBase::GetWorldMap() { return g_map; }
class CTCPhysicsStandard : public CTCPhysicsBase {
protected:
    void UpdateVelocityForImpassability(const C3DVector& current_pos, C3DVector& velocity);
public:
    void Run(const C3DVector& p, C3DVector& v) { UpdateVelocityForImpassability(p, v); }
};
static bool g_on_mesh;
static bool __fastcall IsOnMeshSurfaceStub(void*, void*) { return g_on_mesh; }
static void* g_vtable[64];

static void Model(const C3DVector& p, C3DVector& v)          // independent reference
{
    float map_z = g_ground_base + 0.25f * p.X;
    float gap = p.Z - map_z;
    if (!(gap < 0.7f)) return;
    float cx = 0.5f - (float)fmod((double)p.X, 1.0), cy = 0.5f - (float)fmod((double)p.Y, 1.0);
    float fx = 0, fy = 0, fz = 0;
    for (int i = 0; i < 9; ++i) {
        float tx = g_BlockNeighbourOffsets[i].X + cx, ty = g_BlockNeighbourOffsets[i].Y + cy;
        float m = (float)sqrt(tx * tx + ty * ty);
        if (m == 0.0f || !(m < 1.4f)) continue;
        tx /= m; ty /= m;
        float fade = (m > 1.0f) ? (1.4f - m) * 2.5f : 1.0f;
        float s = fade * g_factors[i] / (m * m);
        fx += tx * s; fy += ty * s;
    }
    float mag = (float)sqrt(fx * fx + fy * fy + fz * fz);
    if (mag != 0.0f) { fx /= mag; fy /= mag; fz /= mag; }
    float dot = fx * v.X + fy * v.Y + fz * v.Z;
    float k = -(dot * mag);
    if (mag > 0.0f && dot > 0.0f) {
        v.X += fx * k; v.Y += fy * k; v.Z += fz * k;
        if (!g_on_mesh) {
            C3DVector q; q.X = p.X + v.X; q.Y = p.Y + v.Y; q.Z = p.Z + v.Z;
            v.Z = (g_ground_base + 0.25f * q.X) + gap - p.Z;
        }
    }
}
static int g_fail;
static void Case(const char* name, float px, float py, float pz, float vx, float vy, float vz, bool mesh, int expect_change)
{
    C3DVector p; p.X = px; p.Y = py; p.Z = pz;
    C3DVector v; v.X = vx; v.Y = vy; v.Z = vz;
    C3DVector m = v;
    g_on_mesh = mesh;
    CTCPhysicsStandard thing; thing.VTable = g_vtable;
    thing.Run(p, v);
    Model(p, m);
    int changed = (v.X != vx || v.Y != vy || v.Z != vz);
    if (fabs(v.X - m.X) > 1e-4f || fabs(v.Y - m.Y) > 1e-4f || fabs(v.Z - m.Z) > 1e-4f || changed != expect_change || v.X != v.X) {
        printf("%s: got %f %f %f want %f %f %f (changed %d)\n", name, v.X, v.Y, v.Z, m.X, m.Y, m.Z, changed);
        g_fail = 1;
    }
}
int main()
{
    for (int i = 0; i < 64; ++i) g_vtable[i] = 0;
    g_vtable[0xDC / 4] = (void*)&IsOnMeshSurfaceStub;
    g_ground_base = 1.0f;
    for (int i = 0; i < 9; ++i) g_factors[i] = 0.0f;
    g_factors[5] = 1.0f;                                  // wall on the +X side
    // cell centre (10.5, 20.5): centre block mag == 0 (skipped), side blocks mag 1.0, corners 1.414 (beyond 1.4)
    Case("high", 10.5f, 20.5f, 1.0f + 2.625f + 0.75f, 1.0f, 0.5f, 0.0f, false, 0);
    Case("push", 10.5f, 20.5f, 1.0f + 2.625f + 0.2f, 2.0f, 0.5f, 0.0f, false, 1);
    Case("push_mesh", 10.5f, 20.5f, 1.0f + 2.625f + 0.2f, 2.0f, 0.5f, 0.3f, true, 1);
    Case("away", 10.5f, 20.5f, 1.0f + 2.625f + 0.2f, -1.0f, 0.5f, 0.0f, false, 0);
    // off-centre cell (10.2, 20.7): side blocks land inside the 1.0..1.4 fade band, corners split around 1.4
    for (int i = 0; i < 9; ++i) g_factors[i] = 0.1f * (float)(i + 1);
    Case("fade", 10.2f, 20.7f, 1.0f + 2.55f + 0.1f, 0.3f, -0.8f, 0.1f, false, 1);
    Case("fade2", 10.8f, 20.1f, 1.0f + 2.7f + 0.1f, -0.6f, 0.9f, 0.0f, false, 1);
    Case("edge", 10.5f, 20.5f, 1.0f + 2.625f + 0.6999f, 1.0f, 0.5f, 0.0f, false, 1);
    if (g_nav_arg != false) g_fail = 1;
    if (g_fail) { printf("007243c0_TEST FAIL\n"); return 1; }
    printf("007243c0_TEST PASS\n");
    return 0;
}