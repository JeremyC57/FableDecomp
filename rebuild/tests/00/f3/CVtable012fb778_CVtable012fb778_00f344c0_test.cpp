#include <cstdio>
#include <cstddef>
// Behaviour model of CVtable_012fb778::CVtable_012fb778 @ 0x00F344C0: six indices start at -1.
struct CPSCBase { virtual void v00() {} CPSCBase() {} };
struct SComponentIndices { long m_trackIndexA, m_trackIndexB; unsigned char _pad1[0x10]; long m_paramIndexA; unsigned char _pad2[0x10]; long m_paramIndexB, m_paramIndexC, m_pad5c, m_paramIndexD;
    SComponentIndices() : m_trackIndexA(-1), m_trackIndexB(-1), m_paramIndexA(-1), m_paramIndexB(-1), m_paramIndexC(-1), m_paramIndexD(-1) {} };
struct CVtable_012fb778 : CPSCBase { unsigned char _pad0[0x24]; SComponentIndices m_indices; virtual void v00() {} CVtable_012fb778() : CPSCBase(), m_indices() {} };
int main()
{
    CVtable_012fb778 c;
    SComponentIndices& i = c.m_indices;
    bool ok = i.m_trackIndexA == -1 && i.m_trackIndexB == -1 && i.m_paramIndexA == -1 && i.m_paramIndexB == -1 && i.m_paramIndexC == -1 && i.m_paramIndexD == -1;
    if (sizeof(void*) == 4) ok = ok && offsetof(CVtable_012fb778, m_indices) == 0x28 && offsetof(SComponentIndices, m_paramIndexD) == 0x38;
    if (!ok) { std::printf("00f344c0_TEST FAIL\n"); return 1; }
    std::printf("00f344c0_TEST PASS\n");
    return 0;
}