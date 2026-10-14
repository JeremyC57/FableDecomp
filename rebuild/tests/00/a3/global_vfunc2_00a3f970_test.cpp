#include <cstdio>
static void __stdcall F(void* b) { delete static_cast<char*>(b); }
int main()
{
    F(new char[4]);

    std::printf("00a3f970_TEST PASS\n");
    return 0;
}