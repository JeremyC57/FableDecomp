#include <cstdio>
// Behaviour model: equality of the getter values through a member pointer.
struct O { unsigned long v; const unsigned long& Get() const { return v; } };
template <const unsigned long& (O::*G)() const> struct C { int Equal(const O* a, const O* b) const { unsigned long x = (a->*G)(); unsigned long y = (b->*G)(); return x == y; } };
int main()
{
    O a = { 5 }, b = { 5 }, c = { 6 }; C<&O::Get> cmp;
    if (cmp.Equal(&a, &b) != 1 || cmp.Equal(&a, &c) != 0) { std::printf("00c24e70_TEST FAIL\n"); return 1; }
    std::printf("00c24e70_TEST PASS\n");
    return 0;
}