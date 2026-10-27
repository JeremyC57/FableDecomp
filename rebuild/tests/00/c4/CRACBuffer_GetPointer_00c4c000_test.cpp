// Test for 0x00C4C000 CRACBuffer::GetPointer: base plus offset.
#include <cstdio>
struct CRACBuffer { unsigned char* m_pBase; unsigned char* GetPointer(unsigned int off) const { return m_pBase + off; } };
int main()
{
    unsigned char buf[16]; CRACBuffer b; b.m_pBase = buf;
    if (b.GetPointer(0) != buf || b.GetPointer(5) != buf + 5) { std::printf("00c4c000_TEST FAIL\n"); return 1; }
    std::printf("00c4c000_TEST PASS\n");
    return 0;
}