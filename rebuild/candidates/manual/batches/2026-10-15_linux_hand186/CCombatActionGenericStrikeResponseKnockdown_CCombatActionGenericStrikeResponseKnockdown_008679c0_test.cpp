#include <cstdio>
struct Thing { int id; };
struct Owner : Thing { };
struct Pair { long a, b; Pair() : a(0), b(0) {} };
struct IP { Thing* item; IP(Thing* t) : item(0) { Set(t); } void Set(Thing* t) { item = t; } };
struct V { float X, Y, Z; V(const V& o) : X(o.X), Y(o.Y), Z(o.Z) {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct Base { Owner* owner; long code; long s0, s1; Base(Owner* o, long c, const Pair& p) : owner(o), code(c), s0(p.a), s1(p.b) {} };
struct KD : Base { const char* anim; IP hitter; V point; bool gotPoint; V dir; bool gotDir;
  KD(Owner* o, Thing* t, const V& p, const V& d) : Base(o, 250, Pair()), anim(""), hitter(t), point(p), gotPoint(true), dir(d), gotDir(true) {} };
int main()
{
    Owner o; Thing t; V p(1, 2, 3), d(4, 5, 6);
    KD k(&o, &t, p, d);
    bool ok = k.owner == &o && k.code == 250 && k.s0 == 0 && k.s1 == 0 && k.hitter.item == &t && k.point.Y == 2 && k.gotPoint && k.dir.Z == 6 && k.gotDir;
    if (!ok) { std::printf("008679c0_TEST FAIL\n"); return 1; }
    std::printf("008679c0_TEST PASS\n");
    return 0;
}
