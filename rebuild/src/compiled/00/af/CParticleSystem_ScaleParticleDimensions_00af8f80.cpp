// CParticleSystem::ScaleParticleDimensions(float*) -- when ScaleParticles is set, multiplies the particle size by the largest component of the system's Scale vector
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF8F80, 93 bytes (`ret 4`). The two-component twin is 0x00AF8F00.
// Callees: none.
// Shape: ScaleParticles is bit 7 of the flag byte +0x50 (PDB +0x6c; `test al,al ; jns`). The largest component is
// the macro form MAX(MAX(x, y), z): the inner `x > y` is evaluated twice (once for the outer compare against z,
// once for the selected value) -- a function or reference-returning max would compare once. Every arm of the
// select keeps its own `fld ; fmul [dims] ; fstp ; ret 4` (the multiply is tail-duplicated into the ternary).
// Scale is the C3DVector at +0x60 (PDB +0x7c).
#define PS_MAX(a, b) ((a) > (b) ? (a) : (b))

class C3DVector { public:
    float X;
    float Y;
    float Z;
};

struct CParticleSystem {
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
    void ScaleParticleDimensions(float* size);
};

void CParticleSystem::ScaleParticleDimensions(float* size)
{
    if (ScaleParticles)
        *size *= PS_MAX(PS_MAX(Scale.X, Scale.Y), Scale.Z);
}