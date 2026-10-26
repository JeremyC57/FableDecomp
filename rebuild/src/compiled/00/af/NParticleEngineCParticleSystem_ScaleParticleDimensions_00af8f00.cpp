// NParticleEngine::CParticleSystem::ScaleParticleDimensions(C2DVector&) const -- when ScaleParticles is set, scale the 2D size by the largest component of the system scale
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8F00, length 117 bytes (padded to 128).
// Leaf: ScaleParticleDimensions
// Callees: none.
// Layout: Ego_r.pdb CParticleSystem minus 0x1c (retail 12-byte CVectorMap / 4-byte STLport lists / 0x24-byte update
// parameters): the bool bit-field byte +0x50 (ScaleParticles = bit 7, tested with `test al,al ; jns`), Scale (C3DVector) +0x60.
// Shape: the largest scale component comes from the nested macro MAX(MAX(x, y), z) (`(a) > (b) ? (a) : (b)`), which
// re-evaluates the inner MAX on the taken arm -- hence the second x/y compare; the size is scaled by an inline
// C2DVector::operator*=(float) (`fld st(0) ; fmul [eax] ; fstp [eax] ; fmul [eax+4] ; fstp [eax+4]`), tail-duplicated per arm.
#define MAX(a, b) ((a) > (b) ? (a) : (b))
class C3DVector { public:
    float X;
    float Y;
    float Z;
};
class C2DVector { public:
    float X;
    float Y;
    C2DVector& operator*=(float s) { X *= s; Y *= s; return *this; }
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
    void ScaleParticleDimensions(C2DVector& dimensions) const;
};
void CParticleSystem::ScaleParticleDimensions(C2DVector& dimensions) const
{
    if (ScaleParticles)
        dimensions *= MAX(MAX(Scale.X, Scale.Y), Scale.Z);
}
}