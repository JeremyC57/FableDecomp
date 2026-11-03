// NParticleEngine::CParticleSystem::ScaleParticleDimensions(unsigned long&) const -- when ScaleParticles is set, scale an integer dimension by the largest component of the system scale (truncated)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B05B80, length 117 bytes (padded to 128).
// Leaf: ?ScaleParticleDimensions@CParticleSystem@NParticleEngine@@QBEXAAK@Z
// Callees: _ftol @ 0x00BFEA70.
// Identity: the integer twin of 0x00AF8F00 (same layout and MAX(MAX(x, y), z) shape); its only caller is 0x00B09241. The
// overload is FableWin.pdb / Ego_r.pdb ?ScaleParticleDimensions@CParticleSystem@NParticleEngine@@QBEXAAK@Z (the AAVC2DVector / AAK /
// AAM / AAVC3DVector set); the caller passes &TrailUpdatePrimitive.MaxLength, an unsigned long (+0xC0). `imul ecx,[eax] ; mov [eax],ecx`.
// Shape: `dimension *= (long)MAX(...)` -- the float maximum truncated through _ftol, then multiplied into the reference.
#define MAX(a, b) ((a) > (b) ? (a) : (b))
class C3DVector { public:
    float X;
    float Y;
    float Z;
};
namespace NParticleEngine {
class CParticleSystem { public:
    unsigned char _system[0x50];
    bool          Initialised           : 1;   // +0x50 bit 0
    bool          Enabled               : 1;
    bool          Dead                  : 1;
    bool          NormalisedTimeEnabled : 1;
    bool          UseAllAttractors      : 1;
    bool          Visible               : 1;
    bool          StayWithEmitter       : 1;
    bool          ScaleParticles        : 1;   //       bit 7
    unsigned char _system51[0x03];
    float         GlobalAlpha;                 // +0x54
    float         FadeAlpha;                   // +0x58
    float         NormalisedTime;              // +0x5c
    C3DVector     Scale;                       // +0x60
    void ScaleParticleDimensions(unsigned long& dimension) const;
};
void CParticleSystem::ScaleParticleDimensions(unsigned long& dimension) const
{
    if (ScaleParticles)
        dimension *= (long)MAX(MAX(Scale.X, Scale.Y), Scale.Z);
}
}