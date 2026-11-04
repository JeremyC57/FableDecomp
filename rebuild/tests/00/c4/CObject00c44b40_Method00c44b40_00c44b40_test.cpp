// Behaviour test for the libpk forwarder 0x00C44B40: it clears the member at +0x80 (whose Clear 0x00C61C50 zeroes its word at +8)
// and nothing else.
#include <cstdio>
struct C00c61c50 { unsigned long a, b, f; void M() { f = 0u; } };
struct CObject_00c44b40 { unsigned long head[0x20]; C00c61c50 Member_0x80; void Method_00c44b40() { Member_0x80.M(); } };
int main()
{
    CObject_00c44b40 o; for (int i = 0; i < 0x20; ++i) o.head[i] = i; o.Member_0x80.a = 1; o.Member_0x80.b = 2; o.Member_0x80.f = 99;
    o.Method_00c44b40();
    if (o.Member_0x80.f != 0 || o.Member_0x80.a != 1 || o.Member_0x80.b != 2 || o.head[5] != 5) { std::printf("00c44b40_TEST FAIL\n"); return 1; }
    std::printf("00c44b40_TEST PASS\n");
    return 0;
}