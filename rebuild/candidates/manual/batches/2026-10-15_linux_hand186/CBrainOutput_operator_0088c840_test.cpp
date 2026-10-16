#include <cstdio>
struct Obj { int x; };
int g_destroyed = 0;
void Destroy(Obj*) { ++g_destroyed; }
struct Block { long refs; void (*destroy)(Obj*); Obj* obj; };
struct CP { Obj* o; Block* c;
  CP& operator=(const CP& other) {
    Block* count = other.c; Obj* object = other.o;
    if (c != count) {
      if (c) { --c->refs; if (c->refs == 0) { c->destroy(c->obj); delete c; } }
      o = object; c = count; if (count) ++count->refs;
    }
    return *this; } };
struct V { float x, y, z; };
struct IP { Obj* item; Obj* Get() const { return item; } void Set(Obj* p) { item = p; } IP& operator=(const IP& r) { Set(r.Get()); return *this; } };
struct BO { unsigned long Types; long LookFlags; V dest; float prox; long mt; float speed; CP act; V move; V look; IP thing;
  BO& operator=(const BO& r) { Types = r.Types; LookFlags = r.LookFlags; dest = r.dest; prox = r.prox; mt = r.mt; speed = r.speed;
    act = r.act; move = r.move; look = r.look; thing = r.thing; return *this; } };
int main()
{
    Obj a, b, t;
    Block* old = new Block; old->refs = 1; old->destroy = Destroy; old->obj = &a;
    Block nb = { 1, Destroy, &b };
    BO x = {}, y = {};
    x.act.o = &a; x.act.c = old;
    y.Types = 3; y.LookFlags = 5; y.dest.x = 1.5f; y.speed = 2.0f; y.act.o = &b; y.act.c = &nb; y.move.z = 7.0f; y.look.y = 9.0f; y.thing.item = &t;
    x = y;
    bool ok = x.Types == 3 && x.LookFlags == 5 && x.dest.x == 1.5f && x.speed == 2.0f && x.act.o == &b && nb.refs == 2 &&
              g_destroyed == 1 && x.move.z == 7.0f && x.look.y == 9.0f && x.thing.item == &t;
    x = y;  // same block: no change
    ok = ok && nb.refs == 2 && g_destroyed == 1;
    if (!ok) { std::printf("0088c840_TEST FAIL\n"); return 1; }
    std::printf("0088c840_TEST PASS\n");
    return 0;
}
