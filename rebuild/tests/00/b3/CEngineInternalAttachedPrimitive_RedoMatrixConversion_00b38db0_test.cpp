// Behaviour test for CEngineInternalAttachedPrimitive::RedoMatrixConversion @ 0x00b38db0
#include <cstdio>
class CMatrix3x4 { public: float m_elements[12]; };
class CEngineInternalAttachedPrimitive { public: void RedoMatrixConversion(CMatrix3x4* matrix); };
void CEngineInternalAttachedPrimitive::RedoMatrixConversion(CMatrix3x4* matrix)
{
    matrix->m_elements[6] *= 100.0f;
    matrix->m_elements[7] *= 100.0f;
    matrix->m_elements[8] *= 100.0f;
    matrix->m_elements[0] *= -100.0f;
    matrix->m_elements[1] *= -100.0f;
    matrix->m_elements[2] *= -100.0f;
    matrix->m_elements[3] *= -100.0f;
    matrix->m_elements[4] *= -100.0f;
    matrix->m_elements[5] *= -100.0f;
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
        if (i < 6) want *= -100.0f; else if (i < 9) want *= 100.0f;
        if (m.m_elements[i] != want) { std::printf("00b38db0_TEST FAIL\n"); return 1; }
    }
    std::printf("00b38db0_TEST PASS\n");
    return 0;
}