// Behaviour test for CMessageEventManager::AddMessage(EMessageEventType, ...) (0x00687540): a model of the row -- the event takes the given type, lifespan, radii, extra data and position, the next id, the UIDs (0 for null) and the target faction.
#include <stdio.h>
#include <list>
struct Th { unsigned long long UID; int* faction; };
struct Ev { int type; unsigned long life; float sight, hearing; long extra; unsigned long id; unsigned long long o, t; int* faction; };
static unsigned long g_id = 1;
static Ev* Add(std::list<Ev>& l, int type, unsigned long life, Th* o, Th* t, float s, float h, long x)
{ Ev e = { type, life, s, h, x, g_id++, o ? o->UID : 0, t ? t->UID : 0, t ? t->faction : 0 }; l.push_front(e); return &*l.begin(); }
int main() { std::list<Ev> l; int f = 4; Th a = { 5, 0 }, b = { 6, &f }; Ev* e = Add(l, 3, 10, &a, &b, 2.0f, 4.0f, 9);
  Ev* e2 = Add(l, 1, 1, 0, 0, 0, 0, 0);
  bool ok = e->type == 3 && e->o == 5 && e->t == 6 && e->faction == &f && e->id == 1 && e2->id == 2 && e2->o == 0 && e2->faction == 0;
  printf(ok ? "00687540_TEST PASS\n" : "00687540_TEST FAIL\n");
  return ok ? 0 : 1;
}