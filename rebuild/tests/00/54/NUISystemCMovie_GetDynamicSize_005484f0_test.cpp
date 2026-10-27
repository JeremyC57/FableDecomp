// behaviour test for 0x005484F0: GetDynamicSize returns whatever the virtual GetSize returns
#include <cstdio>
struct V { float X, Y; };
struct B { virtual V GetSize() const = 0; V GetDynamicSize() const { return GetSize(); } };
struct D : B { V GetSize() const { V v; v.X = 3.0f; v.Y = 4.5f; return v; } };
int main()
{
    D d; V v = d.GetDynamicSize();
    if (v.X != 3.0f || v.Y != 4.5f) { std::printf("005484f0_TEST FAIL\n"); return 1; }
    std::printf("005484f0_TEST PASS\n");
    return 0;
}