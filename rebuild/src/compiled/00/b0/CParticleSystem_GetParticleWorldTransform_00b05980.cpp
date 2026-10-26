// CParticleSystem::GetParticleWorldTransform(CMatrix3x4*) -- the unscaled world transform, with the three diagonal terms multiplied by Scale when ScaleParticles is set
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B05980, 51 bytes (`ret 4`).
// Callees: CParticleSystem::GetUnscaledWorldTransform @ 0x00B05840.
// Shape: ScaleParticles is bit 7 of the flag byte +0x50 (`test al,al ; jns`), Scale the C3DVector at +0x60.
// Each diagonal term is `m *= s` (`fld [scale] ; fmul [m] ; fstp [m]`: the right operand is loaded first).
class C3DVector { public:
    float X;
    float Y;
    float Z;
};

class CMatrix3x4 { public:
    float m[12];                                     // 3x3 rotation (rows) then the translation
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
    void GetUnscaledWorldTransform(CMatrix3x4* result);   // 0x00B05840
    void GetParticleWorldTransform(CMatrix3x4* result);
};

void CParticleSystem::GetParticleWorldTransform(CMatrix3x4* result)
{
    GetUnscaledWorldTransform(result);
    if (ScaleParticles) {
        result->m[0] *= Scale.X;
        result->m[4] *= Scale.Y;
        result->m[8] *= Scale.Z;
    }
}