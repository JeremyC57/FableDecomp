#include <cstdio>

struct F { bool Active; unsigned char pad[0xb]; bool Closing; bool Opening;
    long IsOpen() { if (Active && !Opening && !Closing) return 1; return 0; } };
int main()
{
    F f; f.Active = true; f.Closing = false; f.Opening = false; if (f.IsOpen() != 1) { std::printf("0048da30_TEST FAIL\n"); return 1; }
    f.Opening = true; if (f.IsOpen() != 0) { std::printf("0048da30_TEST FAIL\n"); return 1; }
    f.Opening = false; f.Active = false; if (f.IsOpen() != 0) { std::printf("0048da30_TEST FAIL\n"); return 1; }
    std::printf("0048da30_TEST PASS\n");
    return 0;
}