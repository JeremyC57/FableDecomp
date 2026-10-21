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
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
struct C4DVector {
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