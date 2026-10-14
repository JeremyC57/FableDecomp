#include <cstdio>
struct CIndexTriple { long m_count, m_index0, m_index1, m_index2; CIndexTriple() : m_count(0), m_index0(-1), m_index1(-1), m_index2(-1) { } };
int main()
{
    CIndexTriple t;
    if (t.m_count != 0 || t.m_index0 != -1 || t.m_index1 != -1 || t.m_index2 != -1) { std::printf("0046abc4_TEST FAIL\n"); return 1; }
    std::printf("0046abc4_TEST PASS\n"); return 0;
}