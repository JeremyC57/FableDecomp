#include <cstdio>

struct Mat { const char* type; long MaterialIndex; const char* Name; float Shiny; void Reset() { Shiny = 1.0f; }
    Mat() : type("MTRL"), MaterialIndex(-1), Name("") { Reset(); } };
int main()
{
    Mat m; if (m.type[0] != 'M' || m.MaterialIndex != -1 || m.Shiny != 1.0f) { std::printf("00ac0ab0_TEST FAIL\n"); return 1; }
    std::printf("00ac0ab0_TEST PASS\n");
    return 0;
}