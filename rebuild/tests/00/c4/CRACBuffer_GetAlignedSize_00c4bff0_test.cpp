// Test for 0x00C4BFF0 CRACBuffer::GetAlignedSize: alignment times count.
#include <cstdio>
struct CRACBuffer { unsigned int m_alignment; unsigned int GetAlignedSize(unsigned int n) const { return m_alignment * n; } };
int main()
{
    CRACBuffer b; b.m_alignment = 0x800;
    if (b.GetAlignedSize(3) != 0x1800 || b.GetAlignedSize(0) != 0) { std::printf("00c4bff0_TEST FAIL\n"); return 1; }
    std::printf("00c4bff0_TEST PASS\n");
    return 0;
}