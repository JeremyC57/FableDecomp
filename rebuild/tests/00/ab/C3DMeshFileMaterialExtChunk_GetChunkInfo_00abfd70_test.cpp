#include <cstdio>

#include <cstring>
#include <cstdlib>
struct Str { char s[64]; Str(const char* t) { std::strcpy(s, t); } };
Str Plus(const Str& a, const Str& b) { Str r(a.s); std::strcat(r.s, b.s); return r; }
Str IntToStr(long v) { char b[32]; std::sprintf(b, "%ld", v); return Str(b); }
struct Ext { unsigned long SelfIllumination; Str GetChunkInfo() const { return Plus(Str("Self Illumination: "), IntToStr(SelfIllumination)); } };
int main()
{
    Ext e; e.SelfIllumination = 42; if (std::strcmp(e.GetChunkInfo().s, "Self Illumination: 42") != 0) { std::printf("00abfd70_TEST FAIL\n"); return 1; }
    std::printf("00abfd70_TEST PASS\n");
    return 0;
}