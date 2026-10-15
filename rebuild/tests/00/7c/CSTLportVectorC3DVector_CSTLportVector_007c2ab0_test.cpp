// Behaviour test for 0x007c2ab0: vector(n[, value]) allocates n 12-byte elements, fills them and sets start/finish/end.
#include <cstdio>
#include <cstdlib>
#include <new>
struct V { float X, Y, Z; };
struct Vec { V* start; V* finish; V* eos;
  Vec(unsigned int n, const V& v) : start(0), finish(0), eos(0) {
    start = n != 0 ? (V*)std::malloc(n * sizeof(V)) : 0; finish = start; eos = start + n;
    V* c = start; for (; n > 0; --n, ++c) new (c) V(v); finish = c; } };
int main()
{
    V v = {1, 2, 3}; Vec a(4, v); bool ok = a.finish - a.start == 4 && a.eos == a.finish && a.start[3].Z == 3 && a.start[0].X == 1;
    Vec b(0, v); ok = ok && b.start == 0 && b.finish == 0 && b.eos == 0;
    std::free(a.start);
    if (!ok) { std::printf("007c2ab0_TEST FAIL\n"); return 1; }
    std::printf("007c2ab0_TEST PASS\n");
    return 0;
}