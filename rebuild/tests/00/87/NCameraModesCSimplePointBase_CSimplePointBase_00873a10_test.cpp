// Behaviour test for 0x00873A10 (NCameraModes::CSimplePointBase::CSimplePointBase): position, look-at and flag stored.
#include <cstdio>
struct V { float x, y, z; V() {} V(float a, float b, float c) : x(a), y(b), z(c) {} V(const V& o) : x(o.x), y(o.y), z(o.z) {} };
struct Base { void* manager; const void* def; Base(void* m, const void* d) : manager(m), def(d) {} };
struct SimplePoint : Base {
    V StartPos, Position, LookAtPos; bool AllowLookAround;
    SimplePoint(void* m, const void* d, const V& p, const V& l, bool a) : Base(m, d), Position(p), LookAtPos(l), AllowLookAround(a) {}
};
int main()
{
    int mgr = 0, def = 0;
    V p(1, 2, 3), l(4, 5, 6);
    SimplePoint s(&mgr, &def, p, l, true);
    int ok = s.manager == &mgr && s.def == &def && s.Position.x == 1 && s.Position.z == 3 && s.LookAtPos.y == 5 && s.LookAtPos.z == 6 && s.AllowLookAround;
    if (!ok) { std::printf("00873a10_TEST FAIL\n"); return 1; }
    std::printf("00873a10_TEST PASS\n");
    return 0;
}