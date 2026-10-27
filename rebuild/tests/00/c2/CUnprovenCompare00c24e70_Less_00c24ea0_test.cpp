#include <cstdio>
// Behaviour model: unsigned less-than of the getter values through a member pointer.
struct O { unsigned long v; const unsigned long& Get() const { return v; } };
template <const unsigned long& (O::*G)() const> struct C { int Less(const O* a, const O* b) const { unsigned long x = (a->*G)(); return x < (b->*G)(); } };
int main()
{
    O a = { 5 }, b = { 5 }, c = { 0xfffffff0UL }; C<&O::Get> cmp;
    if (cmp.Less(&a, &b) != 0 || cmp.Less(&a, &c) != 1 || cmp.Less(&c, &a) != 0) { std::printf("00c24ea0_TEST FAIL\n"); return 1; }
    std::printf("00c24ea0_TEST PASS\n");
    return 0;
}