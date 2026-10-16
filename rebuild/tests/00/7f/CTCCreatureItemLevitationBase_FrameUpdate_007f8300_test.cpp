#include <cstdio>
// Behaviour model of CTCCreatureItemLevitationBase::FrameUpdate (0x007F8300).
struct V { float x, y, z; };
static V g_off = { 0.0f, 1.0f, 2.0f };
struct Phys { int awake; bool moving; V pos; };
struct Item { bool drawable; Phys phys; };
struct Node { Node* next; Item* value; };
struct Lev {
    Node* head; V owner; int targetUpdates;
    void FrameUpdate()
    {
        ++targetUpdates;
        const V& p = owner;
        for (Node* n = head->next; n != head; n = n->next) {
            Item* it = n->value;
            V q; q.x = g_off.x + p.x; q.y = g_off.y + p.y; q.z = g_off.z + p.z;
            it->drawable = true; it->phys.awake++; it->phys.moving = true; it->phys.pos = q;
        }
    }
};
int main()
{
    Item a = { false, { 0, false, { 0, 0, 0 } } }, b = a;
    Node h, n1, n2; h.next = &n1; n1.next = &n2; n2.next = &h; n1.value = &a; n2.value = &b;
    Lev l; l.head = &h; l.owner.x = 3; l.owner.y = 4; l.owner.z = 5; l.targetUpdates = 0;
    l.FrameUpdate();
    if (l.targetUpdates != 1 || !a.drawable || !b.phys.moving || a.phys.pos.y != 5.0f || b.phys.pos.z != 7.0f || b.phys.awake != 1) { std::printf("007f8300_TEST FAIL\n"); return 1; }
    std::printf("007f8300_TEST PASS\n");
    return 0;
}