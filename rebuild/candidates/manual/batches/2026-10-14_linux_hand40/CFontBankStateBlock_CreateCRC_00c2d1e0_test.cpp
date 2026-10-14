#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_n = 0; static void* g_bank[4]; static long g_status[4];
struct CBankFile_00c2d1e0 { unsigned char _body[0x180]; void SetBankUpdateStatus(long s) { g_bank[g_n] = this; g_status[g_n++] = s; } };
struct CFontBankStateBlock { unsigned char _pad[0x18]; CBankFile_00c2d1e0 m_banks[2]; void SetBanksUpdateStatus(long s) { for (int i = 0; i < 2; ++i) m_banks[i].SetBankUpdateStatus(s); } };
int main() {
    CFontBankStateBlock b; b.SetBanksUpdateStatus(9); if (g_n != 2 || g_bank[0] != &b.m_banks[0] || g_bank[1] != &b.m_banks[1] || g_status[0] != 9 || g_status[1] != 9) { std::printf("00c2d1e0_TEST FAIL\n"); return 1; }
    std::printf("00c2d1e0_TEST PASS\n"); return 0;
}
