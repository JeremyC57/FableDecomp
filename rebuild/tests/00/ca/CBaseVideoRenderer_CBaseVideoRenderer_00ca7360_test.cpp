// Behaviour test for 0x00CA7360: base built from the arguments in order, three words zeroed, then the initialiser runs.
#include <cstdio>
struct Base { void* a; void* b; void* c; void* d; Base(void* a_, void* b_, void* c_, void* d_) : a(a_), b(b_), c(c_), d(d_) {} };
struct Derived : Base { int ec, w120, w124, inits;
  Derived(void* a, void* b, void* c, void* d) : Base(a, b, c, d), ec(0), w120(0), w124(0), inits(0) { Initialise(); }
  void Initialise() { if (ec == 0 && w120 == 0 && w124 == 0) ++inits; } };
int main()
{
    int x[4]; Derived v(&x[0], &x[1], &x[2], &x[3]);
    bool ok = v.a == &x[0] && v.d == &x[3] && v.inits == 1;
    if (!ok) { std::printf("00ca7360_TEST FAIL\n"); return 1; }
    std::printf("00ca7360_TEST PASS\n");
    return 0;
}