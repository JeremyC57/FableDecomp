#include <cstdio>

#include <cstring>
struct Str { char s[64]; Str(const char* t) { std::strcpy(s, t); } };
Str Plus(const char* a, const Str& b) { Str r(a); std::strcat(r.s, b.s); return r; }
struct Mat { Str Name; Mat() : Name("stone") {} Str GetName() const { return Name; }
    Str GetChunkInfo() const { Str info = Plus("Name: ", GetName()); return info; } };
int main()
{
    Mat m; if (std::strcmp(m.GetChunkInfo().s, "Name: stone") != 0) { std::printf("00ac51d0_TEST FAIL\n"); return 1; }
    std::printf("00ac51d0_TEST PASS\n");
    return 0;
}
