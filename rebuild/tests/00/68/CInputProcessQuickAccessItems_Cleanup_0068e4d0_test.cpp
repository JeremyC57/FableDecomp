#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static long g_item = -1, g_slot = -1, g_mode = 0; static bool g_hasMode = false; class CPlayer { public: bool HasMode(long mode) { std::strcat(g_seq, "H"); g_mode = mode; return g_hasMode; } };
class CInputProcessQuickAccessItems { public: virtual void v000() { } unsigned char _pad04[0x14]; CPlayer* m_pPlayer; void CancelQuickAccessCharge() { std::strcat(g_seq, "C"); }
    virtual void Cleanup() { if (m_pPlayer->HasMode(0x1c) && g_item != -1) { CancelQuickAccessCharge(); g_item = -1; g_slot = -1; } } };

int main() {
    CInputProcessQuickAccessItems q; CPlayer p; q.m_pPlayer = &p; g_hasMode = true; g_item = 3; g_slot = 4; q.Cleanup(); if (std::strcmp(g_seq, "HC") || g_item != -1 || g_slot != -1 || g_mode != 0x1c) { std::printf("0068e4d0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; q.Cleanup(); if (std::strcmp(g_seq, "H")) { std::printf("0068e4d0_TEST FAIL\n"); return 1; } g_seq[0] = 0; g_hasMode = false; g_item = 5; q.Cleanup(); if (std::strcmp(g_seq, "H") || g_item != 5) { std::printf("0068e4d0_TEST FAIL\n"); return 1; }
    std::printf("0068e4d0_TEST PASS\n"); return 0;
}