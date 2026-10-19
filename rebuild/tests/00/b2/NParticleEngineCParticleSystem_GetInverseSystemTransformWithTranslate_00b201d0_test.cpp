#include <stdio.h>
#include <math.h>
// Behaviour test for NParticleEngine::CParticleSystem::GetInverseSystemTransformWithTranslate (0x00B201D0).
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 { float m[12]; };
namespace NParticleEngine {
struct CParticleSystem {
    unsigned char m_pad00[0x60];
    C3DVector Scale;
    CMatrix3x4 fwd, inv;
    const CMatrix3x4* GetSystemTransform() const { return &fwd; }
    const CMatrix3x4* GetInverseSystemTransform() const { return &inv; }
    void GetInverseSystemTransformWithTranslate(CMatrix3x4* result);
};
void CParticleSystem::GetInverseSystemTransformWithTranslate(CMatrix3x4* result)
{
    CMatrix3x4 transform = *GetSystemTransform();
    *result = *GetInverseSystemTransform();
    result->m[9]  = -(result->m[0] * transform.m[9] + result->m[3] * transform.m[10] + result->m[6] * transform.m[11]);
    result->m[10] = -(result->m[1] * transform.m[9] + result->m[4] * transform.m[10] + result->m[7] * transform.m[11]);
    result->m[11] = -(result->m[2] * transform.m[9] + result->m[5] * transform.m[10] + result->m[8] * transform.m[11]);
    result->m[0] *= Scale.X; result->m[1] *= Scale.Y; result->m[2] *= Scale.Z;
    result->m[3] *= Scale.X; result->m[4] *= Scale.Y; result->m[5] *= Scale.Z;
    result->m[6] *= Scale.X; result->m[7] *= Scale.Y; result->m[8] *= Scale.Z;
}
}
int main()
{
    int ok = 1;
    NParticleEngine::CParticleSystem ps;
    for (int i = 0; i < 12; ++i) { ps.fwd.m[i] = 0; ps.inv.m[i] = 0; }
    // forward: rotate 90 degrees about Z (rows) then translate (1,2,3); inverse rotation is the transpose
    ps.fwd.m[1] = 1; ps.fwd.m[3] = -1; ps.fwd.m[8] = 1; ps.fwd.m[9] = 1; ps.fwd.m[10] = 2; ps.fwd.m[11] = 3;
    ps.inv.m[3] = 1; ps.inv.m[1] = -1; ps.inv.m[8] = 1; ps.inv.m[9] = 99;
    ps.Scale.X = 2; ps.Scale.Y = 3; ps.Scale.Z = 4;
    CMatrix3x4 r; ps.GetInverseSystemTransformWithTranslate(&r);
    // t' = -(t * Rinv): x = -(r0*1 + r3*2 + r6*3) = -(0 + 2 + 0) = -2; y = -(r1*1 + r4*2 + r7*3) = 1; z = -(r8*3) = -3
    if (r.m[9] != -2.0f || r.m[10] != 1.0f || r.m[11] != -3.0f) ok = 0;
    if (r.m[1] != -3.0f || r.m[3] != 2.0f || r.m[8] != 4.0f || r.m[0] != 0.0f) ok = 0;
    if (ps.fwd.m[9] != 1.0f) ok = 0;
    if (ok) { printf("00b201d0_TEST PASS\n"); return 0; }
    printf("00b201d0_TEST FAIL\n"); return 1;
}