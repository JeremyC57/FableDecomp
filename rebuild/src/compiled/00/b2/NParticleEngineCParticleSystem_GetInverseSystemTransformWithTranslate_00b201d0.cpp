// NParticleEngine::CParticleSystem::GetInverseSystemTransformWithTranslate(CMatrix3x4*) -- the inverse of the system transform including its translation (minus the inverse rotation applied to the forward translation), with every rotation row scaled by the system's Scale
// Grade: RELOCATION_MATCH -- re-graded unchanged against the registered retry oracle (the earlier x87-order DIFFER no longer reproduces)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B201D0, length 240 bytes (`ret 4`; manifest label correct).
// Callees: CParticleSystem::GetSystemTransform @ 0x00AF3D80 (&PParentEmitter->Transform) and GetInverseSystemTransform
//          @ 0x00AF3DA0 (&PParentEmitter->InverseTransform, emitter +0xe0); both labelled "CVertexBufferWin32::DoSizeof"
//          (propagated).
// Layout: CMatrix3x4 as the landed GetUnscaledWorldTransform 0x00B05840 (twelve floats: rows, then the translation);
// the retail CParticleSystem's Scale is at +0x60.
// Shape: the forward transform is copied to a local (rep movsd), the inverse straight into the result; each
// translation component is the negated dot product of the forward translation with a column of the inverse
// rotation; the scale multiplies in place, element by element.
struct C3DVector { float X, Y, Z; };

struct CMatrix3x4 {
    float m[12];                                   // 3x3 rotation (rows) then the translation
};

namespace NParticleEngine {
struct CParticleSystem {
    unsigned char m_pad00[0x60];
    C3DVector     Scale;                           // +0x60
    const CMatrix3x4* GetSystemTransform() const;           // 0x00AF3D80
    const CMatrix3x4* GetInverseSystemTransform() const;    // 0x00AF3DA0
    void GetInverseSystemTransformWithTranslate(CMatrix3x4* result);
};

void CParticleSystem::GetInverseSystemTransformWithTranslate(CMatrix3x4* result)
{
    CMatrix3x4 transform = *GetSystemTransform();
    *result = *GetInverseSystemTransform();
    result->m[9]  = -(result->m[0] * transform.m[9] + result->m[3] * transform.m[10] + result->m[6] * transform.m[11]);
    result->m[10] = -(result->m[1] * transform.m[9] + result->m[4] * transform.m[10] + result->m[7] * transform.m[11]);
    result->m[11] = -(result->m[2] * transform.m[9] + result->m[5] * transform.m[10] + result->m[8] * transform.m[11]);
    result->m[0] *= Scale.X;
    result->m[1] *= Scale.Y;
    result->m[2] *= Scale.Z;
    result->m[3] *= Scale.X;
    result->m[4] *= Scale.Y;
    result->m[5] *= Scale.Z;
    result->m[6] *= Scale.X;
    result->m[7] *= Scale.Y;
    result->m[8] *= Scale.Z;
}
}