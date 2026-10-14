#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SIcons { unsigned char _pad[0x18]; long m_iconA, m_iconB, m_iconC, m_iconD, m_iconE; }; static SIcons g_icons; struct CGuiDef { unsigned char _pad[0x958]; SIcons* m_pMarkerIcons; }; static CGuiDef g_def; static CGuiDef* PeekGuiDef() { g_def.m_pMarkerIcons = &g_icons; return &g_def; }
struct CMarkerInfo { unsigned char _pad[8]; long m_icon; bool IsRotatedOnMap() { SIcons* icons = PeekGuiDef()->m_pMarkerIcons; if (m_icon == icons->m_iconE || m_icon == icons->m_iconA || m_icon == icons->m_iconB || m_icon == icons->m_iconC || m_icon == icons->m_iconD) return false; return true; } };

int main() {
    g_icons.m_iconA = 1; g_icons.m_iconB = 2; g_icons.m_iconC = 3; g_icons.m_iconD = 4; g_icons.m_iconE = 5; CMarkerInfo m; m.m_icon = 7; if (!m.IsRotatedOnMap()) { std::printf("0082a230_TEST FAIL\n"); return 1; } for (long i = 1; i <= 5; ++i) { m.m_icon = i; if (m.IsRotatedOnMap()) { std::printf("0082a230_TEST FAIL\n"); return 1; } }
    std::printf("0082a230_TEST PASS\n"); return 0;
}
