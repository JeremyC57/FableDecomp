#include <cstdio>

static unsigned long g_type = 0;
struct Bank { unsigned long GetEntryDataType(unsigned long) { return g_type; } };
struct A { void* vt; Bank* bank; unsigned long entry;
    bool Is() { unsigned long t = bank->GetEntryDataType(entry) & 0xF; if (t == 7 || t == 8 || t == 9) return true; return false; } };
int main()
{
    Bank b; A a; a.bank = &b; g_type = 0x18; if (!a.Is()) { std::printf("00a23f00_TEST FAIL\n"); return 1; }
    g_type = 0x16; if (a.Is()) { std::printf("00a23f00_TEST FAIL\n"); return 1; }
    std::printf("00a23f00_TEST PASS\n");
    return 0;
}