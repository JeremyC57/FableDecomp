#include <cstdio>
int __fastcall GetLevelBeingLoaded(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 220);
}
int main()
{
    unsigned char object[224] = {0};
    *reinterpret_cast<int*>(object + 220) = 0x12345678;
    if (GetLevelBeingLoaded(object) == 0x12345678) { std::printf("AUTO_TINY_0051e2c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0051e2c0_TEST FAIL\n");
    return 1;
}