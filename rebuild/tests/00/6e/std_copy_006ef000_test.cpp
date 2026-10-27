// Behaviour test for std::__copy<pair<C3DVector,bool>*> (0x006EF000): forward element copy of 13-byte packed pairs, returns result + n.
#include <cstdio>
#pragma pack(push, 1)
class C3DVector { public: float X, Y, Z; };
struct Pair { C3DVector first; bool second; };
#pragma pack(pop)
static Pair* copy_pairs(Pair* first, Pair* last, Pair* result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    if (sizeof(Pair) != 13) { std::printf("006ef000_TEST FAIL\n"); return 1; }
    Pair s[3], d[3];
    for (int i = 0; i < 3; ++i) { s[i].first.X = (float)i; s[i].first.Y = i + 0.5f; s[i].first.Z = -(float)i; s[i].second = i == 1; d[i].second = true; }
    if (copy_pairs(s, s + 3, d) != d + 3) { std::printf("006ef000_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (d[i].first.X != (float)i || d[i].first.Y != i + 0.5f || d[i].first.Z != -(float)i || d[i].second != (i == 1)) { std::printf("006ef000_TEST FAIL\n"); return 1; }
    std::printf("006ef000_TEST PASS\n");
    return 0;
}