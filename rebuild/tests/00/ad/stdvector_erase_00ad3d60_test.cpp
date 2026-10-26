// behaviour test for 0x00AD3D60 vector<QuadricOptimiserInternals::COptimisedFace >::erase(first, last): the tail [last, finish) is copied down onto first word
// for word (18-dword records), finish moves back by last - first, first is returned; an empty range changes nothing
#include <cstdio>
struct Rec { unsigned int w[18]; };
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
    for (int k = 0; k < 8; ++k) for (int j = 0; j < 18; ++j) buf[k].w[j] = (unsigned int)(k * 100 + j);
    Vec v; v.s = buf; v.f = buf + 6; v.e = buf + 8;
    Rec* r = Erase(v, buf + 1, buf + 3);
    if (r != buf + 1 || v.f != buf + 4) fails |= 1;
    if (buf[1].w[0] != 300 || buf[2].w[18 - 1] != (unsigned int)(400 + 18 - 1) || buf[3].w[0] != 500 || buf[0].w[0] != 0) fails |= 2;
    r = Erase(v, buf + 2, buf + 2);
    if (r != buf + 2 || v.f != buf + 4 || buf[2].w[0] != 400) fails |= 4;
    r = Erase(v, v.s, v.f);
    if (r != buf || v.f != buf) fails |= 8;
    if (sizeof(Rec) != (unsigned int)72) fails |= 16;
    if (fails) { std::printf("00ad3d60_TEST FAIL %d\n", fails); return 1; }
    std::printf("00ad3d60_TEST PASS\n");
    return 0;
}