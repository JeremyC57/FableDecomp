// Behaviour test for CThingSearchTools::GetPFirstLeafThing<Bind2<OverlappingSphere, PosInContainmentVolume>, CThingBuilding> (006698d0).
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct Thing { unsigned char state; C3DVector centre; float radius; bool contains; };
struct Node { Node* next; Node* prev; Thing* value; };
static bool Visible(const Thing* t) { unsigned char f = t->state; unsigned char nf = ~f; return (f & 4) && (f & 8) && (nf & 1) && !(f & 0x20); }
struct Sphere { C3DVector P; float R;
    bool operator()(const Thing* t) const { float dx = t->centre.X - P.X, dy = t->centre.Y - P.Y, dz = t->centre.Z - P.Z; float r = t->radius + R; return dx * dx + dy * dy + dz * dz < r * r; } };
struct Volume { bool operator()(const Thing* t) const { return t->contains; } };
static bool GetPFirstLeafThing(Node* head, const Sphere& a, const Volume& b, Thing** out)
{
    for (Node* n = head->next; n != head; n = n->next) {
        Thing* t = n->value;
        if (Visible(t) && a(t) && b(t)) { *out = t; return true; }
    }
    return false;
}
int main()
{
    Thing t1 = { 0x0c, { 100, 0, 0 }, 1, true };    // too far
    Thing t2 = { 0x0d, { 1, 0, 0 }, 1, true };      // dead
    Thing t3 = { 0x0c, { 1, 0, 0 }, 1, false };     // not containing
    Thing t4 = { 0x0c, { 1, 1, 0 }, 1, true };      // hit
    Node head, n1, n2, n3, n4;
    head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &n4; n4.next = &head;
    n1.value = &t1; n2.value = &t2; n3.value = &t3; n4.value = &t4;
    Sphere s = { { 0, 0, 0 }, 2 }; Volume v; Thing* out = 0;
    bool ok = GetPFirstLeafThing(&head, s, v, &out) && out == &t4;
    t4.state = 0x2c; out = 0;
    ok = ok && !GetPFirstLeafThing(&head, s, v, &out) && out == 0;
    if (!ok) { std::printf("006698d0_TEST FAIL\n"); return 1; }
    std::printf("006698d0_TEST PASS\n");
    return 0;
}