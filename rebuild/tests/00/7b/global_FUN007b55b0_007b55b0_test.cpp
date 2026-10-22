#include <cstdio>
// Behaviour test for 007b55b0: the setter stores its byte argument at +0x241 and leaves the neighbours alone.
struct Owner { unsigned char pad[0x241]; unsigned char value; unsigned char after;
    void Set(unsigned char v) { value = v; } };
int main()
{
    Owner o; o.pad[0x240] = 0x11; o.value = 0; o.after = 0x22;
    o.Set(0xAB);
    int ok = (o.value == 0xAB && o.pad[0x240] == 0x11 && o.after == 0x22);
    o.Set(0); ok = ok && o.value == 0;
    if (!ok) { std::printf("007b55b0_TEST FAIL\n"); return 1; }
    std::printf("007b55b0_TEST PASS\n");
    return 0;
}