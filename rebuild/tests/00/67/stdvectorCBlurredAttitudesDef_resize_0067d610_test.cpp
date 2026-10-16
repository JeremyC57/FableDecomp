#include <cstdio>
// Behaviour model of vector<CBlurredAttitudesDef>::resize(n): forwards to resize(n, T()) with a default record whose
// def index is 0, flags clear, InstantiationName -1 and both vectors empty.
struct Rec { long defIndex; unsigned char flags; long instName; void* mapFirst; bool dirty; long* attFirst;
    Rec() : defIndex(0), flags(0), instName(-1), mapFirst(0), dirty(false), attFirst(0) {} };
struct Vec {
    unsigned int n; Rec last;
    void resize(unsigned int count, const Rec& v) { n = count; last = v; }
    void resize(unsigned int count) { resize(count, Rec()); }
};
int main()
{
    Vec v; v.n = 0; v.last.instName = 5;
    v.resize(7);
    if (v.n != 7 || v.last.instName != -1 || v.last.defIndex || v.last.flags || v.last.mapFirst || v.last.dirty || v.last.attFirst) {
        std::printf("0067d610_TEST FAIL\n"); return 1;
    }
    std::printf("0067d610_TEST PASS\n");
    return 0;
}