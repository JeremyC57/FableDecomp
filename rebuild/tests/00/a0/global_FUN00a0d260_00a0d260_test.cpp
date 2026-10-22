#include <cstdio>
// Behaviour test for 00a0d260: the reset clears the byte flag at +0x26 only.
struct Owner { unsigned char pad[0x26]; bool flag; unsigned char after; void Reset() { flag = false; } };
int main()
{
    Owner o; o.flag = true; o.after = 9; o.pad[0x25] = 7;
    o.Reset();
    int ok = (!o.flag && o.after == 9 && o.pad[0x25] == 7);
    if (!ok) { std::printf("00a0d260_TEST FAIL\n"); return 1; }
    std::printf("00a0d260_TEST PASS\n");
    return 0;
}