// Behaviour test for CEngineInternalAttachedPrimitive::RedoMatrixConversion @ 0x00b38e30
#include <cstdio>
struct CMatrix3x4 { float m_elements[12]; };
struct CEngineInternalAttachedPrimitive { void RedoMatrixConversion(CMatrix3x4* matrix); };
void CEngineInternalAttachedPrimitive::RedoMatrixConversion(CMatrix3x4* matrix)
{
    matrix->m_elements[6] *= 0.01f;
    matrix->m_elements[7] *= 0.01f;
    matrix->m_elements[8] *= 0.01f;
    matrix->m_elements[0] *= -0.01f;
    matrix->m_elements[1] *= -0.01f;
    matrix->m_elements[2] *= -0.01f;
    matrix->m_elements[3] *= -0.01f;
    matrix->m_elements[4] *= -0.01f;
    matrix->m_elements[5] *= -0.01f;
}
int main()
{
    CMatrix3x4 m;
    int i;
    for (i = 0; i < 12; ++i) m.m_elements[i] = (float)(i + 1);
    CEngineInternalAttachedPrimitive p;
    p.RedoMatrixConversion(&m);
    for (i = 0; i < 12; ++i) {
        float want = (float)(i + 1);
        if (i < 6) want *= -0.01f; else if (i < 9) want *= 0.01f;
        if (m.m_elements[i] != want) { std::printf("00b38e30_TEST FAIL\n"); return 1; }
    }
    std::printf("00b38e30_TEST PASS\n");
    return 0;
}
