#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SEntry44 { unsigned char _body[0x2c]; }; struct SEntry16 { unsigned char _body[0x10]; };
struct CGraphicDataBank { unsigned char _pad[0x1e0]; SEntry44* m_pEntriesFirst; SEntry44* m_pEntriesLast; SEntry44* m_pEntriesEnd; SEntry16* m_pSlotsFirst; SEntry16* m_pSlotsLast; SEntry16* m_pSlotsEnd;
    long Sizeof() { return (long)(m_pEntriesEnd - m_pEntriesFirst) * 0x2c + (long)(m_pSlotsEnd - m_pSlotsFirst) * 0x10 + 0x324; } };

int main() {
    CGraphicDataBank b; SEntry44 e[3]; SEntry16 s[5]; b.m_pEntriesFirst = e; b.m_pEntriesLast = e + 2; b.m_pEntriesEnd = e + 3; b.m_pSlotsFirst = s; b.m_pSlotsLast = s + 1; b.m_pSlotsEnd = s + 5;
    if (b.Sizeof() != 3 * 0x2c + 5 * 0x10 + 0x324) { std::printf("009fd8d0_TEST FAIL\n"); return 1; } b.m_pEntriesEnd = e; b.m_pSlotsEnd = s; if (b.Sizeof() != 0x324) { std::printf("009fd8d0_TEST FAIL\n"); return 1; }
    std::printf("009fd8d0_TEST PASS\n"); return 0;
}
