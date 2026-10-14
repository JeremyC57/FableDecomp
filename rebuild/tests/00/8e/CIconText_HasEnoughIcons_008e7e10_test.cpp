#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SIconEntry { long m_a; long m_b; }; static long g_calls = 0; struct SIconMatcher { long m_state; bool Matches(const SIconEntry& e) { ++g_calls; return e.m_a != 0; } }; struct SIconLimit { unsigned char _pad[0x34]; long m_required; };
struct CIconText { unsigned char _pad[0x80]; SIconEntry* m_pIconsFirst; SIconEntry* m_pIconsLast; unsigned char _pad88[8]; SIconLimit* m_pLimit;
    bool HasEnoughIcons() { long required = 2; if (m_pLimit) required = m_pLimit->m_required; SIconEntry* last = m_pIconsLast; SIconEntry* it = m_pIconsFirst; SIconMatcher matcher; long count = 0; for (; it != last; ++it) { if (matcher.Matches(*it)) ++count; } return count >= required; } };

int main() {
    CIconText t; std::memset(&t, 0, sizeof t); SIconEntry e[4] = { { 1, 0 }, { 0, 0 }, { 1, 0 }, { 1, 0 } }; t.m_pIconsFirst = e; t.m_pIconsLast = e + 4; SIconLimit lim; lim.m_required = 3;
    if (!t.HasEnoughIcons() || g_calls != 4) { std::printf("008e7e10_TEST FAIL\n"); return 1; } t.m_pIconsLast = e + 2; if (t.HasEnoughIcons()) { std::printf("008e7e10_TEST FAIL\n"); return 1; } t.m_pIconsLast = e + 3; t.m_pLimit = &lim; if (t.HasEnoughIcons()) { std::printf("008e7e10_TEST FAIL\n"); return 1; } lim.m_required = 2; if (!t.HasEnoughIcons()) { std::printf("008e7e10_TEST FAIL\n"); return 1; } t.m_pIconsLast = e; g_calls = 0; if (t.HasEnoughIcons() || g_calls) { std::printf("008e7e10_TEST FAIL\n"); return 1; }
    std::printf("008e7e10_TEST PASS\n"); return 0;
}