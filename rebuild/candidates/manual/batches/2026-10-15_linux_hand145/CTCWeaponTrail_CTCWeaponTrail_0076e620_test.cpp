// Behaviour test for 0x0076E620 (CTCWeaponTrail::CTCWeaponTrail): initial state of the weapon-trail component.
#include <cstdio>
#include <cstdlib>
struct Thing { int id; };
struct Base { Thing* ParentThing; bool ToBeRemoved, IsInGlobalUpdateTCs; Base(Thing* t) : ParentThing(t), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {} };
struct Handle { void* Object; Handle() : Object(0) {} };
template <class T> struct Vec { T* f; T* l; T* e; Vec() : f(0), l(0), e(0) {} };
struct Node { Node* next; Node* prev; };
struct List { Node* head; List() : head(0) { Node* s = (Node*)std::malloc(sizeof(Node)); s->next = s; s->prev = s; head = s; } ~List() { std::free(head); } };
struct WeaponTrail : Base {
    long NoTrails, NoProjectileTrails, WeaponTrailGraphicIndex; Handle ProjectileWeaponTrailHandle;
    Vec<Handle> WeaponTrailHandles; long TrailType; Vec<unsigned int> ActiveTrail; Vec<long> InactiveTicks; Vec<List> Trail; Vec<void*> HelperNames;
    List ProjectileWeaponTrail; long MinProjectileTrailTimer; bool WeaponTrailActive;
    WeaponTrail(Thing* t) : Base(t), NoTrails(-1), WeaponTrailActive(false) {}
};
int main()
{
    Thing t = { 7 };
    WeaponTrail w(&t);
    int ok = w.ParentThing == &t && w.NoTrails == -1 && w.ProjectileWeaponTrailHandle.Object == 0 && w.WeaponTrailHandles.f == 0
          && w.ActiveTrail.e == 0 && w.InactiveTicks.l == 0 && w.Trail.f == 0 && w.HelperNames.e == 0 && !w.WeaponTrailActive
          && w.ProjectileWeaponTrail.head != 0 && w.ProjectileWeaponTrail.head->next == w.ProjectileWeaponTrail.head;
    if (!ok) { std::printf("0076e620_TEST FAIL\n"); return 1; }
    std::printf("0076e620_TEST PASS\n");
    return 0;
}
