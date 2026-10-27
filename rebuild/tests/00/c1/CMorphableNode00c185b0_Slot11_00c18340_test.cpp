#include <cstdio>
#include <cstring>
// Behaviour model: with an implementation the call is forwarded unchanged; without one the error line is logged first.
static char logged[96]; static int calls;
struct Impl { void Call() { ++calls; } };
struct Holder { Impl* impl; void Call() { if (!impl) std::sprintf(logged, ".\\PKCTRMorphable.cpp(%d) : Error: NULL implimentation\n", 116); else impl->Call(); } };
int main()
{
    Impl i; Holder h = { &i }; h.Call();
    Holder e = { 0 }; e.Call();
    if (calls != 1 || !std::strstr(logged, "(116) : Error: NULL implimentation")) { std::printf("00c18340_TEST FAIL\n"); return 1; }
    std::printf("00c18340_TEST PASS\n");
    return 0;
}