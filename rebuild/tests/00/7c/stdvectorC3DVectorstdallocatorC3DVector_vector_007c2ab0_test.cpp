// Behaviour test for 0x007C2AB0 std::vector<C3DVector>::vector(size_type n): n elements allocated and filled from one default
// element, start / finish / end-of-storage set (finish == end-of-storage == start + n); n == 0 allocates nothing (all three null).
#include <cstdio>
#include <cstdlib>
#include <new>
struct V { float X, Y, Z; V() { } V(const V& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
struct Vec { V* start; V* finish; V* eos;
  explicit Vec(unsigned int n) : start(0), finish(0), eos(0) {
    start = n != 0 ? (V*)std::malloc(n * sizeof(V)) : 0; finish = start; eos = start + n;
    V t; t.X = 7; t.Y = 8; t.Z = 9;   // stands in for the default T() the retail fill copies
    V* c = start; for (; n > 0; --n, ++c) new (c) V(t); finish = c; } };
int main()
{
    Vec a(4); bool ok = a.finish - a.start == 4 && a.eos == a.finish && a.start[3].Z == 9 && a.start[0].X == 7;
    Vec b(0); ok = ok && b.start == 0 && b.finish == 0 && b.eos == 0;
    std::free(a.start);
    if (!ok) { std::printf("007c2ab0_TEST FAIL\n"); return 1; }
    std::printf("007c2ab0_TEST PASS\n");
    return 0;
}
