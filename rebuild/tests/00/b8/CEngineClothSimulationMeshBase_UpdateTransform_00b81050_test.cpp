// 00b81050 behaviour test: CEngineClothSimulationMeshBase::UpdateTransform (match.cpp body, host-modelled)
#include <stdio.h>
#include <math.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
#define __forceinline inline
class CMatrix3x4 { public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    __forceinline void GetInverseRotation(CMatrix3x4& out) const
    {
        float det = E11 * (E22 * E33 - E23 * E32) + E21 * (E32 * E13 - E33 * E12) + E31 * (E23 * E12 - E22 * E13);
        float inv = 1.0f / det;
        out.E11 = (E22 * E33 - E23 * E32) * inv;
        out.E21 = (E23 * E31 - E21 * E33) * inv;
        out.E31 = (E21 * E32 - E22 * E31) * inv;
        out.E12 = (E32 * E13 - E33 * E12) * inv;
        out.E22 = (E33 * E11 - E13 * E31) * inv;
        out.E32 = (E31 * E12 - E32 * E11) * inv;
        out.E13 = (E23 * E12 - E22 * E13) * inv;
        out.E23 = (E21 * E13 - E23 * E11) * inv;
        out.E33 = (E22 * E11 - E21 * E12) * inv;
        out.E41 = 0.0f;
        out.E42 = 0.0f;
        out.E43 = 0.0f;
    }
};


class CEngineInternalPrimitiveMeshBase { public:
    CMatrix3x4 last, cur; int calls;
    void GetLastTransform(CMatrix3x4& out) { out = last; ++calls; }
    void GetTransform(CMatrix3x4& out) { out = cur; ++calls; }
};
struct CEngineClothSimulationMeshBase {
    CMatrix3x4 CurrentTransform, LastTransform, InverseTransform;
    CEngineInternalPrimitiveMeshBase* Owner;
    void UpdateTransform()
    {
        float u0 = 0.0f;
        Owner->GetLastTransform(LastTransform);
        Owner->GetTransform(CurrentTransform);
        CurrentTransform.GetInverseRotation(InverseTransform);
    }
};
static bool near(float a, float b) { return fabs(a - b) < 1e-4f; }
int main()
{
    CEngineInternalPrimitiveMeshBase owner;
    CMatrix3x4 L = { 1,2,3, 4,5,6, 7,8,9, 10,11,12 };
    CMatrix3x4 C = { 2,0,1, 1,3,0, 0,1,4, 5,6,7 };
    owner.last = L; owner.cur = C; owner.calls = 0;
    CEngineClothSimulationMeshBase m; m.Owner = &owner;
    m.InverseTransform.E41 = 9.0f;
    m.UpdateTransform();
    CHECK(owner.calls == 2);
    CHECK(m.LastTransform.E11 == 1 && m.LastTransform.E43 == 12);
    CHECK(m.CurrentTransform.E22 == 3 && m.CurrentTransform.E41 == 5);
    const CMatrix3x4& A = m.CurrentTransform; const CMatrix3x4& B = m.InverseTransform;
    // rows-as-vectors convention: v' = v * A; check A3x3 * B3x3 == I
    float a[3][3] = { {A.E11,A.E12,A.E13},{A.E21,A.E22,A.E23},{A.E31,A.E32,A.E33} };
    float b[3][3] = { {B.E11,B.E12,B.E13},{B.E21,B.E22,B.E23},{B.E31,B.E32,B.E33} };
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) {
        float s = 0; for (int k = 0; k < 3; ++k) s += a[i][k] * b[k][j];
        CHECK(near(s, i == j ? 1.0f : 0.0f));
    }
    CHECK(B.E41 == 0.0f && B.E42 == 0.0f && B.E43 == 0.0f);
    if (!g_fail) printf("00b81050_TEST PASS\n");
    return g_fail;
}