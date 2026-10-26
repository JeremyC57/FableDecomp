// behaviour test for 0x0081BFE0 vector<std::pair<EHeroMorphType, CParticleMorphs::CEntry> >::erase(first, last): the tail [last, finish) is copied down onto first word
// for word (10-dword records), finish moves back by last - first, first is returned; an empty range changes nothing
#include <cstdio>
struct Rec { unsigned int w[10]; };
struct Vec { Rec* s; Rec* f; Rec* e; };
static Rec* Erase(Vec& v, Rec* first, Rec* last)
{
    Rec* r = first;
    for (int n = (int)(v.f - last); n > 0; --n) { *r = *last; ++r; ++last; }
    v.f = r;
    return first;
}
int main()
{
    int fails = 0;
    Rec buf[8];
    for (int k = 0; k < 8; ++k) for (int j = 0; j < 10; ++j) buf[k].w[j] = (unsigned int)(k * 100 + j);
    Vec v; v.s = buf; v.f = buf + 6; v.e = buf + 8;
    Rec* r = Erase(v, buf + 1, buf + 3);
    if (r != buf + 1 || v.f != buf + 4) fails |= 1;
    if (buf[1].w[0] != 300 || buf[2].w[10 - 1] != (unsigned int)(400 + 10 - 1) || buf[3].w[0] != 500 || buf[0].w[0] != 0) fails |= 2;
    r = Erase(v, buf + 2, buf + 2);
    if (r != buf + 2 || v.f != buf + 4 || buf[2].w[0] != 400) fails |= 4;
    r = Erase(v, v.s, v.f);
    if (r != buf || v.f != buf) fails |= 8;
    if (sizeof(Rec) != (unsigned int)40) fails |= 16;
    if (fails) { std::printf("0081bfe0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0081bfe0_TEST PASS\n");
    return 0;
}