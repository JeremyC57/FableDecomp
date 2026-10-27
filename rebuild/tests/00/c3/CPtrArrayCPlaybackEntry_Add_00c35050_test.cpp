#include <cstdio>
#include <cstdlib>
// Behaviour model: a full array grows by one slot before the append.
struct Arr { void** data; unsigned long cap, count; int grows;
    void Grow(unsigned long n) { ++grows; void** d = (void**)std::malloc((cap + n) * sizeof(void*)); for (unsigned long i = 0; i < count; ++i) d[i] = data[i]; std::free(data); data = d; cap += n; }
    void Add(void* v) { if (cap - count == 0) Grow(1); data[count] = v; ++count; } };
int main()
{
    Arr a = { 0, 0, 0, 0 }; int x, y;
    a.Add(&x); a.Add(&y);
    if (a.count != 2 || a.grows != 2 || a.data[0] != &x || a.data[1] != &y) { std::printf("00c35050_TEST FAIL\n"); return 1; }
    std::free(a.data);
    std::printf("00c35050_TEST PASS\n");
    return 0;
}