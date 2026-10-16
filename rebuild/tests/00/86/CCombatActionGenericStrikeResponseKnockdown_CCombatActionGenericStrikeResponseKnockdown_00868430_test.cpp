#include <cstdio>
#include <cstring>
struct Thing { int id; };
struct Str { char text[16]; Str() { text[0] = 0; } Str(const Str& o) { std::strcpy(text, o.text); } };
struct IP { Thing* item; IP() : item(0) {} IP(const IP& o) : item(0) { Set(o.Get()); } Thing* Get() const { return item; } void Set(Thing* t) { item = t; } };
struct V { float X, Y, Z; V() {} V(const V& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct Base { int body; Base() : body(0) {} Base(const Base& o) : body(o.body) {} };
struct KD : Base { Str anim; IP hitter; V point; bool gotPoint; V dir; bool gotDir, flip;
  KD() {}
  KD(const KD& o) : Base(o), anim(o.anim), hitter(o.hitter), point(o.point), gotPoint(o.gotPoint), dir(o.dir), gotDir(o.gotDir), flip(o.flip) {} };
int main()
{
    Thing t; KD a; a.body = 2; std::strcpy(a.anim.text, "KNOCK"); a.hitter.Set(&t); a.point.X = 1; a.point.Y = 2; a.point.Z = 3;
    a.gotPoint = true; a.dir.X = 4; a.dir.Y = 5; a.dir.Z = 6; a.gotDir = false; a.flip = true;
    KD b(a);
    bool ok = b.body == 2 && std::strcmp(b.anim.text, "KNOCK") == 0 && b.hitter.Get() == &t && b.point.Z == 3 && b.gotPoint && b.dir.Y == 5 && !b.gotDir && b.flip;
    if (!ok) { std::printf("00868430_TEST FAIL\n"); return 1; }
    std::printf("00868430_TEST PASS\n");
    return 0;
}