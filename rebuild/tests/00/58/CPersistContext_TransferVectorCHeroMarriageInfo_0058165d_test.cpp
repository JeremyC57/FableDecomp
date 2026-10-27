// behaviour test for 0x0058165D: TransferVector dispatches on the persist mode
#include <cstdio>
static int g_last = -1;
struct Ctx {
    long Mode;
    void CheckCRC(const char*) { }
    void Transfer(const char* name, int& value)
    {
        CheckCRC(name);
        switch (Mode) {
        case 0: g_last = 0; break;
        case 1: g_last = 1; break;
        case 2: g_last = 2; break;
        case 3: g_last = 3; break;
        default: value = 0; g_last = 4; break;
        }
    }
};
int main()
{
    int v = 5; Ctx c;
    for (long m = 0; m < 4; ++m) { c.Mode = m; c.Transfer("k", v); if (g_last != m || v != 5) { std::printf("0058165d_TEST FAIL\n"); return 1; } }
    c.Mode = 4; c.Transfer("k", v);
    if (g_last != 4 || v != 0) { std::printf("0058165d_TEST FAIL\n"); return 1; }
    std::printf("0058165d_TEST PASS\n");
    return 0;
}