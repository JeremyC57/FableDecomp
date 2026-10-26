// behaviour test for 0x00A5E0A0 std::vector<CVertexStreamFormat::CDataElement >::vector(const vector&): the copy owns a fresh block of exactly
// x.size() 8-byte elements (end_of_storage == finish), every element is copied, and an empty source yields null pointers
#include <cstdio>
#include <cstdlib>
struct Elem { unsigned int A; unsigned int B; };
struct Vec { Elem* Start; Elem* Finish; Elem* EndOfStorage; };
static void CopyConstruct(Vec& self, const Vec& x)
{
    unsigned int n = (unsigned int)(x.Finish - x.Start);
    self.Start = n != 0 ? (Elem*)std::malloc(n * sizeof(Elem)) : 0;
    self.Finish = self.Start;
    self.EndOfStorage = self.Start + n;
    for (const Elem* p = x.Start; p != x.Finish; ++p, ++self.Finish) *self.Finish = *p;
}
int main()
{
    int fail = 0;
    Elem src[3] = { { 1, 10 }, { 2, 20 }, { 3, 30 } };
    Vec x; x.Start = src; x.Finish = src + 3; x.EndOfStorage = src + 3;
    Vec v; CopyConstruct(v, x);
    if (v.Start == src || v.Finish != v.Start + 3 || v.EndOfStorage != v.Finish) fail |= 1;
    for (int i = 0; i < 3; ++i) if (v.Start[i].A != src[i].A || v.Start[i].B != src[i].B) fail |= 2;
    std::free(v.Start);
    Vec e; e.Start = e.Finish = e.EndOfStorage = 0;
    Vec w; CopyConstruct(w, e);
    if (w.Start != 0 || w.Finish != 0 || w.EndOfStorage != 0) fail |= 4;
    if (fail) { std::printf("00a5e0a0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00a5e0a0_TEST PASS\n");
    return 0;
}