#include <cstdio>

#include <cstring>
struct Str { char s[64]; Str(const char* t) { std::strcpy(s, t); } };
Str Plus(const Str& a, const Str& b) { Str r(a.s); std::strcat(r.s, b.s); return r; }
struct CV { Str Name; CV() : Name("box") {} Str GetChunkInfo() const { return Plus(Str("Name: "), Name); } };
int main()
{
    CV c; if (std::strcmp(c.GetChunkInfo().s, "Name: box") != 0) { std::printf("00aa9ad0_TEST FAIL\n"); return 1; }
    std::printf("00aa9ad0_TEST PASS\n");
    return 0;
}