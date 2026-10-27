#include <cstdio>
// Behaviour model: equality of a value with an object's getter value through a member pointer.
struct O { unsigned long v; const unsigned long& Get() const { return v; } };
template <const unsigned long& (O::*G)() const> struct C { int EqualToValue(const unsigned long& v, const O* b) const { return v == (b->*G)(); } };
int main()
{
    O b = { 5 }; C<&O::Get> cmp; unsigned long five = 5, six = 6;
    if (cmp.EqualToValue(five, &b) != 1 || cmp.EqualToValue(six, &b) != 0) { std::printf("00c0a060_TEST FAIL\n"); return 1; }
    std::printf("00c0a060_TEST PASS\n");
    return 0;
}