// Test for 0x00C547A0 CDecodedStream::Open: clears +4, copies desc+0x58, 0 when the initialiser succeeds, else -9.
#include <cstdio>
struct SDesc { int m_58; };
struct CStream { bool m_04; int m_220; bool m_initResult;
    bool Initialise() { return m_initResult; }
    long Open(const SDesc* d) { m_04 = false; m_220 = d->m_58; return Initialise() ? 0 : -9; } };
int main()
{
    SDesc d = {77}; CStream s; s.m_04 = true; s.m_220 = 0; s.m_initResult = true;
    bool ok = s.Open(&d) == 0 && !s.m_04 && s.m_220 == 77;
    s.m_initResult = false; ok = ok && s.Open(&d) == -9;
    if (!ok) { std::printf("00c547a0_TEST FAIL\n"); return 1; }
    std::printf("00c547a0_TEST PASS\n");
    return 0;
}