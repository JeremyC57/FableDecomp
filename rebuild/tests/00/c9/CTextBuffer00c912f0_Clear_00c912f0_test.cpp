#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CTextBuffer { long m_0; long m_4; char* m_pBuffer; char* m_pWrite; void Clear() { m_pWrite = m_pBuffer; m_pBuffer[0] = 0; m_0 = 0; m_4 = 0; } };

int main() {
    char buf[8] = "abc"; CTextBuffer t; t.m_0 = 5; t.m_4 = 6; t.m_pBuffer = buf; t.m_pWrite = buf + 3; t.Clear(); if (t.m_pWrite != buf || buf[0] != 0 || buf[1] != 'b' || t.m_0 || t.m_4) { std::printf("00c912f0_TEST FAIL\n"); return 1; }
    std::printf("00c912f0_TEST PASS\n"); return 0;
}