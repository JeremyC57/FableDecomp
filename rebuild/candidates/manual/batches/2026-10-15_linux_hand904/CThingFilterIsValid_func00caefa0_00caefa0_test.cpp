#include <cstdio>
// Behaviour of func_00caefa0 (0x00CAEFA0): returns this.
struct CSelfAccessor_00caefa0 { long m_pad; CSelfAccessor_00caefa0* func_00caefa0() { return this; } };
int main()
{
    CSelfAccessor_00caefa0 o;
    if (o.func_00caefa0() != &o) { std::printf("00caefa0_TEST FAIL\n"); return 1; }
    std::printf("00caefa0_TEST PASS\n"); return 0;
}
