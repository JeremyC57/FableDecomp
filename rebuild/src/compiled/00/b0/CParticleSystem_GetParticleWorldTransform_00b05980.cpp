// NParticleEngine::CParticleSystem::GetParticleWorldTransform(CMatrix3x4& result) const -- the unscaled world transform, with the three diagonal terms multiplied by Scale when ScaleParticles is set
// Leaf: ?GetParticleWorldTransform@CParticleSystem@NParticleEngine@@QBEXAAVCMatrix3x4@@@Z
// Name: FableWin.pdb ?GetParticleWorldTransform@CParticleSystem@NParticleEngine@@QBEXAAVCMatrix3x4@@@Z (namespace, const, reference;
//   one stack word, `ret 4`); its callee is FableWin ?GetUnscaledWorldTransform@CParticleSystem@NParticleEngine@@QBEXAAVCMatrix3x4@@@Z.
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
    void GetUnscaledWorldTransform(CMatrix3x4& result) const;   // 0x00B05840
    void GetParticleWorldTransform(CMatrix3x4& result) const;
};

void CParticleSystem::GetParticleWorldTransform(CMatrix3x4& result) const
{
    GetUnscaledWorldTransform(result);
    if (ScaleParticles) {
        result.m[0] *= Scale.X;
        result.m[4] *= Scale.Y;
        result.m[8] *= Scale.Z;
    }
}
}
