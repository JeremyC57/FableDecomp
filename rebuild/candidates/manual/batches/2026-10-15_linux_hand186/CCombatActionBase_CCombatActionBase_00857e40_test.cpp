#include <cstdio>
#include <new>
struct Thing { int id; };
struct Owner { int id; };
struct IP { Thing* item; IP() : item(0) {} IP(Thing* t) : item(0) { Set(t); } Thing* Get() const { return item; } void Set(Thing* t) { item = t; }
  IP& operator=(const IP& o) { Set(o.Get()); return *this; } };
struct Node { Node* next; Node* prev; void* a; void* b; };
struct List { Node* node; List() : node(0) { Node* n = static_cast<Node*>(::operator new(sizeof(Node))); n->next = n; n->prev = n; node = n; }
  ~List() { ::operator delete(node); } };
struct Vec { void* s; void* f; void* e; Vec() : s(0), f(0), e(0) {} };
struct CreatureAction { long priority; Owner* owner; CreatureAction(long p, Owner* o) : priority(p), owner(o) {} };
struct Combat : CreatureAction {
  IP target, camera; void* def; List events; bool sentMissed, hasReset; Vec anims; long active; IP lastWeapon; int inits;
  Combat(Owner* o) : CreatureAction(100, o), def(0), sentMissed(false), hasReset(false), active(-1), lastWeapon(0), inits(0)
  { target = IP(0); Init(); }
  void Init() { ++inits; } };
int main()
{
    Owner o;
    Combat c(&o);
    bool ok = c.priority == 100 && c.owner == &o && c.target.Get() == 0 && c.def == 0 && c.events.node->next == c.events.node &&
              c.events.node->prev == c.events.node && !c.sentMissed && !c.hasReset && c.anims.s == 0 && c.active == -1 &&
              c.lastWeapon.Get() == 0 && c.inits == 1;
    if (!ok) { std::printf("00857e40_TEST FAIL\n"); return 1; }
    std::printf("00857e40_TEST PASS\n");
    return 0;
}
