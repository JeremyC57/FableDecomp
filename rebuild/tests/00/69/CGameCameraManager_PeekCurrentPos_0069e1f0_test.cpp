// behaviour test for 0x0069E1F0: the getter returns a copy of the member vector
#include <cstdio>
struct V { float X, Y, Z; };
struct Obj { char pad[4]; V member; V Get() const { return member; } };
int main()
{
    Obj o; o.member.X = 1.5f; o.member.Y = -2.0f; o.member.Z = 3.25f;
    V v = o.Get();
    if (v.X != 1.5f || v.Y != -2.0f || v.Z != 3.25f) { std::printf("0069e1f0_TEST FAIL\n"); return 1; }
    std::printf("0069e1f0_TEST PASS\n");
    return 0;
}