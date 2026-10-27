#include <cstdio>
#include <cstring>
// Behaviour model: with an implementation the call is forwarded unchanged; without one the error line is logged first.
static char logged[96]; static long got_a, got_b;
struct Impl { void Call(long a, long b) { got_a = a; got_b = b; } };
struct Holder { Impl* impl; void Call(long a, long b) { if (!impl) std::sprintf(logged, ".\\PKCTRMorphable.cpp(%d) : Error: NULL implimentation\n", 98); else impl->Call(a, b); } };
int main()
{
    Impl i; Holder h = { &i }; h.Call(3, 4);
    Holder e = { 0 }; e.Call(1, 2);
    if (got_a != 3 || got_b != 4 || !std::strstr(logged, "(98) : Error: NULL implimentation")) { std::printf("00c180d0_TEST FAIL\n"); return 1; }
    std::printf("00c180d0_TEST PASS\n");
    return 0;
}