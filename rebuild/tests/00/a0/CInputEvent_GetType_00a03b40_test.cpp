#include <cstdio>
int __fastcall DrawGetEnvironment(const unsigned char* event)
{
    return *reinterpret_cast<const int*>(event + 40);
}
int main()
{
    unsigned char object[44] = {0};
    *reinterpret_cast<int*>(object + 40) = 0x12345678;
    if (DrawGetEnvironment(object) == 0x12345678) { std::printf("AUTO_TINY_00a03b40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a03b40_TEST FAIL\n");
    return 1;
}