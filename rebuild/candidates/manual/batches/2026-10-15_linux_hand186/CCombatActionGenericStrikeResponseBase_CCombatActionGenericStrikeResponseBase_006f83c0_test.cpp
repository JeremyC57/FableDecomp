#include <cstdio>
struct Thing { int id; };
struct IP { Thing* item; IP() : item(0) {} IP(const IP& o) : item(0) { Set(o.Get()); } Thing* Get() const { return item; } void Set(Thing* t) { item = t; } };
struct V { float X, Y, Z; V() {} V(const V& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct Base { int body; Base() : body(0) {} Base(const Base& o) : body(o.body) {} };
struct GSR : Base { IP hitter; V point, dir; bool move, dust; long frame; V dustDir;
  GSR() {}
  GSR(const GSR& o) : Base(o), hitter(o.hitter), point(o.point), dir(o.dir), move(o.move), dust(o.dust), frame(o.frame), dustDir(o.dustDir) {} };
int main()
{
    Thing t; GSR a; a.body = 4; a.hitter.Set(&t); a.point.X = 1; a.point.Y = 2; a.point.Z = 3; a.dir.X = 4; a.dir.Y = 5; a.dir.Z = 6;
    a.move = true; a.dust = false; a.frame = 9; a.dustDir.X = 7; a.dustDir.Y = 8; a.dustDir.Z = 9;
    GSR b(a);
    bool ok = b.body == 4 && b.hitter.Get() == &t && b.point.Y == 2 && b.dir.Z == 6 && b.move && !b.dust && b.frame == 9 && b.dustDir.X == 7;
    if (!ok) { std::printf("006f83c0_TEST FAIL\n"); return 1; }
    std::printf("006f83c0_TEST PASS\n");
    return 0;
}
