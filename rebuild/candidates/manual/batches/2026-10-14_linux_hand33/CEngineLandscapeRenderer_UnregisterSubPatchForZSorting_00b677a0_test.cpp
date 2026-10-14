#include <cstdio>
struct CEntry { void* m_pSubPatch; unsigned long m_flags; };
struct CRenderer { unsigned char _pad[0x5e4]; CEntry* m_pSortedSubPatches; void Unregister(long* slot) { m_pSortedSubPatches[*slot].m_pSubPatch = 0; m_pSortedSubPatches[*slot].m_flags &= ~1; *slot = -1; } };
int main()
{
    int sp; CEntry e[3] = { { &sp, 3 }, { &sp, 0xff }, { &sp, 1 } }; CRenderer r; r.m_pSortedSubPatches = e; long slot = 1; r.Unregister(&slot);
    if (slot != -1 || e[1].m_pSubPatch != 0 || e[1].m_flags != 0xfe || e[0].m_pSubPatch != &sp || e[0].m_flags != 3 || e[2].m_flags != 1) { std::printf("00b677a0_TEST FAIL\n"); return 1; }
    std::printf("00b677a0_TEST PASS\n"); return 0;
}
