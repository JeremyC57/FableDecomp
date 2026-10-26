#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CTCBase { public: bool m_free; bool IsActionPointFree() { std::strcat(g_seq, "F"); return m_free; } }; static CTCBase g_tc, g_end; static bool g_free = true; static unsigned long g_flags = 0;
struct STCEntry { long m_kind; CTCBase* m_pTC; };
struct CTCMap { STCEntry* m_pFirst; STCEntry* m_pLast; STCEntry* m_pEnd; STCEntry* LowerBound(const unsigned long& kind) const { STCEntry* it = m_pFirst; while (it != m_pLast && it->m_kind < (long)kind) ++it; return it; } CTCBase* Find(long kind) const { STCEntry* it = LowerBound((unsigned long)kind); if (it == m_pLast || kind < it->m_kind) it = m_pLast; return it->m_pTC; } };
class CThing { public: virtual unsigned long GetStateFlags() { return g_flags; } unsigned char _pad04[0x20]; unsigned long m_flags24; unsigned char _pad28[0x1c]; CTCMap m_tcs; };
class CIsFreeHopscotchBoard { public: bool operator()(CThing* thing) { g_tc.m_free = g_free; if ((unsigned char)(thing->GetStateFlags() >> 15) & 1) { if (thing->m_flags24 & 0x2000) { if (thing->m_tcs.Find(0x2d)->IsActionPointFree()) return true; } } return false; } };

int main() {
    CThing t; STCEntry entries[2]; entries[0].m_kind = 0x2d; entries[0].m_pTC = &g_tc; entries[1].m_kind = 0; entries[1].m_pTC = &g_end; t.m_tcs.m_pFirst = entries; t.m_tcs.m_pLast = entries + 1; t.m_tcs.m_pEnd = entries + 2; CIsFreeHopscotchBoard f;
    g_flags = 0x8000; t.m_flags24 = 0x2000; g_free = true; if (!f(&t) || std::strcmp(g_seq, "F")) { std::printf("00941be0_TEST FAIL\n"); return 1; } g_free = false; if (f(&t)) { std::printf("00941be0_TEST FAIL\n"); return 1; } g_free = true; t.m_flags24 = 0; g_seq[0] = 0; if (f(&t) || g_seq[0]) { std::printf("00941be0_TEST FAIL\n"); return 1; } t.m_flags24 = 0x2000; g_flags = 0x7fff; if (f(&t)) { std::printf("00941be0_TEST FAIL\n"); return 1; }
    g_flags = 0x8000; entries[0].m_kind = 0x2e; g_end.m_free = false; if (f(&t)) { std::printf("00941be0_TEST FAIL\n"); return 1; }
    std::printf("00941be0_TEST PASS\n"); return 0;
}