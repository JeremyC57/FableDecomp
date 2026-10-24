// Behaviour test for CAnimFilterOf3Groups::~CAnimFilterOf3Groups (0x00739DF0): Group3, Group2, Group1 destroyed, then the CAnimFilterBase root.
#include <cstdio>
static int g_log[16]; static int g_n = 0; static int g_base = 0;
class CCharString { public: int id; ~CCharString() { g_log[g_n++] = id; } };
class CAnimFilterBase { public: virtual ~CAnimFilterBase() { g_log[g_n++] = 100; } };
class CAnimFilterOf3Groups : public CAnimFilterBase { public: CCharString Group1, Group2, Group3; unsigned long Group1CRC, Group2CRC, Group3CRC; };
int main()
{
    { CAnimFilterOf3Groups f; f.Group1.id = 1; f.Group2.id = 2; f.Group3.id = 3; }
    if (!(g_n == 4 && g_log[0] == 3 && g_log[1] == 2 && g_log[2] == 1 && g_log[3] == 100)) { std::printf("00739df0_TEST FAIL\n"); return 1; }
    std::printf("00739df0_TEST PASS\n");
    return 0;
}