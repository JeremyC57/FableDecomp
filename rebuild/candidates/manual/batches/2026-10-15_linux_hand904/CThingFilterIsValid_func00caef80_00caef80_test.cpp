#include <cstdio>
// Behaviour of func_00caef80 (0x00CAEF80): returns this.
struct CSelfAccessor_00caef80 { long m_pad; CSelfAccessor_00caef80* func_00caef80() { return this; } };
int main()
{
    CSelfAccessor_00caef80 o;
    if (o.func_00caef80() != &o) { std::printf("00caef80_TEST FAIL\n"); return 1; }
    std::printf("00caef80_TEST PASS\n"); return 0;
}
