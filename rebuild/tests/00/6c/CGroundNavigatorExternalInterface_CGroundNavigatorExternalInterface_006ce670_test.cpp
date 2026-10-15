#include <cstdio>
#include <cstdlib>
// Host model of the CGroundNavigatorExternalInterface copy constructor: copy the creature and radius, register the
// creature's obstacle record in the global list unless already present.
struct Node { Node* next; Node* prev; void* data; };
static Node g_head;
static int Count() { int n = 0; for (Node* i = g_head.next; i != &g_head; i = i->next) ++n; return n; }
struct Creature { void* obstacles; };
struct Interface {
    Creature* creature; float radius;
    Interface(Creature* c, float r) : creature(c), radius(r) {}
    Interface(const Interface& o) : creature(o.creature), radius(o.radius)
    {
        void* obstacles = creature->obstacles;
        Node* it = g_head.next;
        while (it != &g_head && it->data != obstacles) it = it->next;
        if (it == &g_head) {
            Node* n = (Node*)std::malloc(sizeof(Node)); n->data = obstacles;
            Node* prev = g_head.prev; n->next = &g_head; n->prev = prev; prev->next = n; g_head.prev = n;
        }
    }
};
int main()
{
    int fail = 0;
    g_head.next = g_head.prev = &g_head;
    int recA, recB;
    Creature a = { &recA }, b = { &recB };
    Interface ia(&a, 2.0f), ib(&b, 3.0f);
    Interface c1(ia);
    if (Count() != 1 || c1.creature != &a || c1.radius != 2.0f) fail = 1;
    Interface c2(ia);
    if (Count() != 1) fail = 1;
    Interface c3(ib);
    if (Count() != 2 || g_head.prev->data != &recB || g_head.next->data != &recA) fail = 1;
    if (fail) { std::printf("006ce670_TEST FAIL\n"); return 1; }
    std::printf("006ce670_TEST PASS\n");
    return 0;
}