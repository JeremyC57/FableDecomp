// NParticleEngine::CParticleSystem::ScaleParticleDimensions(float&) const -- when ScaleParticles is set, multiplies the particle size by the largest component of the system's Scale vector
// Leaf: ?ScaleParticleDimensions@CParticleSystem@NParticleEngine@@QBEXAAM@Z
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ?ScaleParticleDimensions@CParticleSystem@NParticleEngine@@QBEXAAM@Z (const, the
//   size by reference, in NParticleEngine; engine/CParticleSystem.h names it the same). Was spelled (float*) non-const at global scope.
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF8F80, 93 bytes (`ret 4`). The two-component twin is 0x00AF8F00.
// Callees: none.
// Shape: ScaleParticles is bit 7 of the flag byte +0x50 (PDB +0x6c; `test al,al ; jns`). The largest component is
// the macro form MAX(MAX(x, y), z): the inner `x > y` is evaluated twice (once for the outer compare against z,
// once for the selected value) -- a function or reference-returning max would compare once. Every arm of the
// select keeps its own `fld ; fmul [dims] ; fstp ; ret 4` (the multiply is tail-duplicated into the ternary).
// Scale is the C3DVector at +0x60 (PDB +0x7c).
#define PS_MAX(a, b) ((a) > (b) ? (a) : (b))

#include "engine/C3DVector.h"

namespace NParticleEngine {
class CParticleSystem { public:
    unsigned char m_pad00[0x50];
    bool          Initialised : 1;                   // +0x50 bit 0
    bool          Enabled : 1;
    bool          Dead : 1;
    bool          NormalisedTimeEnabled : 1;
    bool          UseAllAttractors : 1;
    bool          Visible : 1;
    bool          StayWithEmitter : 1;
    bool          ScaleParticles : 1;                // bit 7
    float         GlobalAlpha;                       // +0x54
    float         FadeAlpha;                         // +0x58
    float         NormalisedTime;                    // +0x5c
    C3DVector     Scale;                             // +0x60
    void ScaleParticleDimensions(float& size) const;
};
}

void NParticleEngine::CParticleSystem::ScaleParticleDimensions(float& size) const
{
    if (ScaleParticles)
        size *= PS_MAX(PS_MAX(Scale.X, Scale.Y), Scale.Z);
}