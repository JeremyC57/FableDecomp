// Behaviour test for CThingSearchTools::GetPBestThing<Bind3<...>, CThingCompare_Nearest, CThingObject> (00965e80).
#include <cstdio>
struct Thing { unsigned char state; unsigned long typeFlags; bool p1, p2, p3; float dist; };
struct Node { Node* next; Node* prev; Thing* value; };
static bool Visible(const Thing* t) { unsigned char f = t->state; unsigned char nf = ~f; return (f & 4) && (f & 8) && (nf & 1) && !(f & 0x20); }
static Thing* Nearest(Thing* best, Thing* c) { return (!best || c->dist < best->dist) ? c : best; }
static bool GetPBestThing(Node* head, Thing** out)
{
    Thing* best = 0;
    for (Node* n = head->next; n != head; n = n->next) {
        Thing* t = n->value;
        if (Visible(t) && (t->typeFlags & 0x807) == 0x807) {
            if (t && t->p1 && t->p2 && t->p3) best = Nearest(best, t);
        }
    }
    *out = best;
    return best != 0;
}
int main()
{
    Thing a = { 0x0c, 0x807, true, true, true, 5 };
    Thing b = { 0x0c, 0x807, true, true, true, 2 };
    Thing c = { 0x0c, 0x007, true, true, true, 1 };   // not an object
    Thing d = { 0x0c, 0x807, true, false, true, 0 };  // fails a filter
    Node head, n1, n2, n3, n4;
    head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &n4; n4.next = &head;
    n1.value = &a; n2.value = &b; n3.value = &c; n4.value = &d;
    Thing* out = 0;
    bool ok = GetPBestThing(&head, &out) && out == &b;
    head.next = &head; ok = ok && !GetPBestThing(&head, &out) && out == 0;
    if (!ok) { std::printf("00965e80_TEST FAIL\n"); return 1; }
    std::printf("00965e80_TEST PASS\n");
    return 0;
}