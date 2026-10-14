#include <cstdio>

struct Sub { const char* type; const char* Name; long a, b, c, d;
    Sub() : type("SUBM"), Name(""), a(-1), b(-1), c(-1), d(-1) {} };
int main()
{
    Sub s; if (s.type[0] != 'S' || s.Name[0] || s.a != -1 || s.b != -1 || s.c != -1 || s.d != -1) { std::printf("00ab9a70_TEST FAIL\n"); return 1; }
    std::printf("00ab9a70_TEST PASS\n");
    return 0;
}