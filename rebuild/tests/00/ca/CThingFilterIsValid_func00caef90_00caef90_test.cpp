#include <cstdio>
// Behaviour of func_00caef90 (0x00CAEF90): returns this.
struct CSelfAccessor_00caef90 { long m_pad; CSelfAccessor_00caef90* func_00caef90() { return this; } };
int main()
{
    CSelfAccessor_00caef90 o;
    if (o.func_00caef90() != &o) { std::printf("00caef90_TEST FAIL\n"); return 1; }
    std::printf("00caef90_TEST PASS\n"); return 0;
}