// Behaviour test for 0x006EFF00 (std::__uninitialized_fill_n over 13-byte pair<C3DVector,bool>).
#include <cstdio>
#include <cstring>
#include <new>
#pragma pack(push, 1)
struct C3DVector { float X, Y, Z; C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct SProngVector { C3DVector first; bool second; SProngVector(const SProngVector& o) : first(o.first), second(o.second) { } SProngVector(const C3DVector& v, bool b) : first(v), second(b) { } };
#pragma pack(pop)
struct __false_type { };
static SProngVector* FillN(SProngVector* first, unsigned int n, const SProngVector& x, const __false_type&)
{
    SProngVector* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SProngVector(x);
    return cur;
}
#define FAIL { std::printf("006eff00_TEST FAIL\n"); return 1; }
int main()
{
    if (sizeof(SProngVector) != 13u) FAIL;
    unsigned char raw[13 * 5];
    std::memset(raw, 0xAB, sizeof raw);
    SProngVector* base = reinterpret_cast<SProngVector*>(raw);
    SProngVector x(C3DVector(1.0f, -2.0f, 3.5f), true);
    if (FillN(base, 0, x, __false_type()) != base || raw[0] != 0xAB) FAIL;   // n == 0: untouched, returns first
    SProngVector* end = FillN(base, 4, x, __false_type());
    if (end != base + 4) FAIL;
    for (int i = 0; i < 4; ++i)
        if (base[i].first.X != 1.0f || base[i].first.Y != -2.0f || base[i].first.Z != 3.5f || !base[i].second) FAIL;
    if (raw[13 * 4] != 0xAB) FAIL;                                             // the fifth slot untouched
    std::printf("006eff00_TEST PASS\n");
    return 0;
}