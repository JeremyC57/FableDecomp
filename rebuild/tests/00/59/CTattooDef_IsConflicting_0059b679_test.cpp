#include <cstdio>

struct D { unsigned char pad[0x40]; unsigned long Covers; unsigned long x; unsigned long Specific;
    bool Is(D* o) { unsigned long c = o->Covers & Covers; if (c == 0) return false; if (c != 8) return true;
        if (o->Specific & Specific) return true; if (Specific == 0) return true; if (o->Specific != 0) return false; return true; } };
int main()
{
    D a, b; a.Covers = 1; b.Covers = 2; if (a.Is(&b)) { std::printf("0059b679_TEST FAIL\n"); return 1; }
    b.Covers = 1; if (!a.Is(&b)) { std::printf("0059b679_TEST FAIL\n"); return 1; }
    a.Covers = b.Covers = 8; a.Specific = 1; b.Specific = 2; if (a.Is(&b)) { std::printf("0059b679_TEST FAIL\n"); return 1; }
    b.Specific = 0; if (!a.Is(&b)) { std::printf("0059b679_TEST FAIL\n"); return 1; }
    b.Specific = 3; if (!a.Is(&b)) { std::printf("0059b679_TEST FAIL\n"); return 1; }
    std::printf("0059b679_TEST PASS\n");
    return 0;
}