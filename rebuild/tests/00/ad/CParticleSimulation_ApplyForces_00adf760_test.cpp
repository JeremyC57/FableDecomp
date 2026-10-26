// Behaviour test for 00adf760: src/00adf760.cpp copied verbatim, the force source modelled below.
// CParticleSimulation::ApplyForces(const CParticleForceSource*)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static bool Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-4f; }
// ---- source (verbatim) ----
// CParticleSimulation::ApplyForces(const CParticleForceSource* forces) -- one Verlet step: with acceleration enabled the
// position buffers swap and every particle moves to last + (last - previous) * damping + force * dt^2, where force is
// gravity * GravityStrength + wind * WindStrength and damping is (1 - GlobalDamping)^dt * TimestepMultiplier; otherwise
// the current positions are copied into the last-position buffer. Either way the timestep flags reset.
// Leaf: ApplyForces
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADF760, 0x1FB bytes (padded to 512), `ret 4`, `and esp,-16` frame.
// Callees: the source's virtual slot 1 (gravity) and slot 2 (wind), both returning a C3DVector; pow (_CIpow 0x00BFEB40).
// Layout: CParticleSimulation per the Ego_r PDB (Timestep +0xC, TimestepChanged +0x10, TimestepMultiplier +0x14, Size
//   +0x18, Positions +0x1C, LastPositions +0x20, GravityStrength +0x34, WindStrength +0x38, AccelerationEnable +0x44,
//   GlobalDamping +0x48). C4DVector is NOT 16-byte aligned (Ego_r.pdb embeds it at +8 in C2DAlphaMaskInfo and +0x14 in
//   CLandscapeSettings); only the acceleration local is `__declspec(align(16))`, which gives the `and esp,-16` frame. The
//   force-source class is a local model.
// Shape: the position-buffer swap is STLport `std::swap` (an inline by-reference template; a hand-written three-line swap
//   stores both pointers two instructions earlier); with only the acceleration local aligned, the per-particle difference
//   and sum temporaries are plain 4-aligned and pack up against the wind result as in retail (an aligned C4DVector type
//   puts them 8 bytes lower).
#include <math.h>
#include <string.h>
namespace std {
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b) { _Tp __tmp = __a; __a = __b; __b = __tmp; }   // STLport <stl/_algobase.h>
}
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
    C4DVector(const C3DVector& v, float w) : X(v.X), Y(v.Y), Z(v.Z), W(w) {}
    C4DVector operator+(const C4DVector& o) const { return C4DVector(X + o.X, Y + o.Y, Z + o.Z, W + o.W); }
    C4DVector operator-(const C4DVector& o) const { return C4DVector(X - o.X, Y - o.Y, Z - o.Z, W - o.W); }
    C4DVector operator*(float s) const { return C4DVector(X * s, Y * s, Z * s, W * s); }
};
class CParticleForceSource {
public:
    virtual void _slot0();
    virtual C3DVector GetGravity() const;
    virtual C3DVector GetWind() const;
};
class CParticleSimulation {
public:
    int            _padding_;                       // +0x00
    void*          RenderData[2];                   // +0x04
    float          Timestep;                        // +0x0C
    bool           TimestepChanged;                 // +0x10
    float          TimestepMultiplier;              // +0x14
    unsigned long  Size;                            // +0x18
    C4DVector*     Positions;                       // +0x1C
    C4DVector*     LastPositions;                   // +0x20
    C4DVector*     SavedPositions;                  // +0x24
    C4DVector*     Accelerations;                   // +0x28
    float*         SimulationAlphas;                // +0x2C
    float          MaximumAdjustment;               // +0x30
    float          GravityStrength;                 // +0x34
    float          WindStrength;                    // +0x38
    bool           DraggingEnable;                  // +0x3C
    bool           DraggingRotational;              // +0x3D
    float          DraggingStrength;                // +0x40
    bool           AccelerationEnable;              // +0x44
    float          GlobalDamping;                   // +0x48
    void ApplyForces(const CParticleForceSource* forces);
};
void CParticleSimulation::ApplyForces(const CParticleForceSource* forces)
{
    if (AccelerationEnable) {
        C3DVector gravity = forces->GetGravity();
        C3DVector wind = forces->GetWind();
        C3DVector force = gravity * GravityStrength + wind * WindStrength;
        std::swap(Positions, LastPositions);
        float damping = (float)pow(1.0f - GlobalDamping, Timestep) * TimestepMultiplier;
        __declspec(align(16)) C4DVector acceleration = C4DVector(force, 0.0f) * (Timestep * Timestep);
        for (unsigned long i = 0; i < Size; ++i)
            Positions[i] = LastPositions[i] + (LastPositions[i] - Positions[i]) * damping + acceleration;
    } else {
        memcpy(LastPositions, Positions, Size * sizeof(C4DVector));
    }
    TimestepChanged = false;
    TimestepMultiplier = 1.0f;
}

// ---- stubs ----
static int g_grav = 0, g_wind = 0;
void CParticleForceSource::_slot0() {}
C3DVector CParticleForceSource::GetGravity() const { ++g_grav; return C3DVector(0.0f, 0.0f, -10.0f); }
C3DVector CParticleForceSource::GetWind() const { ++g_wind; return C3DVector(2.0f, 1.0f, 0.0f); }
int main()
{
    static C4DVector a[3], b[3];
    CParticleSimulation sim; memset(&sim, 0, sizeof(sim));
    CParticleForceSource src;
    for (int i = 0; i < 3; ++i) { a[i] = C4DVector((float)i, 1.0f, 2.0f, 1.0f); b[i] = C4DVector((float)i + 0.5f, 1.5f, 2.0f, 1.0f); }
    // disabled: last positions take a copy of the current positions, flags reset
    sim.Size = 3; sim.Positions = a; sim.LastPositions = b; sim.TimestepChanged = true; sim.TimestepMultiplier = 3.0f;
    sim.ApplyForces(&src);
    CHECK(g_grav == 0 && g_wind == 0);
    CHECK(sim.Positions == a && sim.LastPositions == b);
    for (int i = 0; i < 3; ++i) CHECK(b[i].X == a[i].X && b[i].Y == a[i].Y && b[i].Z == a[i].Z && b[i].W == a[i].W);
    CHECK(sim.TimestepChanged == false && sim.TimestepMultiplier == 1.0f);
    // enabled: swap, then Verlet step with damping (1 - 0.5)^1 * 2 = 1 and dt^2 = 1
    for (int i = 0; i < 3; ++i) { a[i] = C4DVector((float)i, 1.0f, 2.0f, 1.0f); b[i] = C4DVector((float)i + 0.5f, 1.5f, 2.0f, 1.0f); }
    sim.AccelerationEnable = true; sim.GlobalDamping = 0.5f; sim.Timestep = 1.0f; sim.TimestepMultiplier = 2.0f;
    sim.GravityStrength = 2.0f; sim.WindStrength = 0.5f; sim.TimestepChanged = true;
    sim.ApplyForces(&src);
    CHECK(g_grav == 1 && g_wind == 1);
    CHECK(sim.Positions == b && sim.LastPositions == a);
    // force = (0,0,-20) + (1,0.5,0) = (1,0.5,-20); acc = force * 1 = (1,0.5,-20,0)
    // new = last + (last - prev) * 1 + acc where last = a[i], prev = b[i]
    for (int i = 0; i < 3; ++i) {
        CHECK(Near(b[i].X, (float)i + ((float)i - ((float)i + 0.5f)) + 1.0f));
        CHECK(Near(b[i].Y, 1.0f + (1.0f - 1.5f) + 0.5f));
        CHECK(Near(b[i].Z, 2.0f + 0.0f - 20.0f));
        CHECK(Near(b[i].W, 1.0f));
    }
    CHECK(sim.TimestepChanged == false && sim.TimestepMultiplier == 1.0f);
    // enabled, damping exponent: (1 - 0.75)^2 * 1 = 0.0625, dt^2 = 4, zero particles still swaps
    sim.Size = 0; sim.GlobalDamping = 0.75f; sim.Timestep = 2.0f; sim.TimestepMultiplier = 1.0f;
    sim.ApplyForces(&src);
    CHECK(sim.Positions == a && sim.LastPositions == b);
    // one particle with that damping: new = last + (last - prev) * 0.0625 + force * 4
    sim.Size = 1; a[0] = C4DVector(0.0f, 0.0f, 0.0f, 0.0f); b[0] = C4DVector(16.0f, 0.0f, 0.0f, 0.0f);
    sim.ApplyForces(&src);   // swap: Positions = b, LastPositions = a; last = a[0] = 0, prev = b[0] = 16
    CHECK(sim.Positions == b);
    CHECK(Near(b[0].X, 0.0f + (0.0f - 16.0f) * 0.0625f + 1.0f * 4.0f));
    CHECK(Near(b[0].Y, 0.5f * 4.0f)); CHECK(Near(b[0].Z, -20.0f * 4.0f)); CHECK(Near(b[0].W, 0.0f));
    if (fails == 0) printf("00adf760_TEST PASS\n"); else printf("00adf760_TEST FAIL\n");
    return fails;
}