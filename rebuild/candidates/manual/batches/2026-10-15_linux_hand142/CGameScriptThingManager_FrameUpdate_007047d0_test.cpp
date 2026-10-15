#include <cstdio>
#include <cstdlib>
// Host model of CGameScriptThingManager::FrameUpdate: a circular list of counted script things; entries whose count
// block has only the manager's reference are erased (object destroyed, block and node freed), the rest updated.
struct Thing { int updates; bool destroyed; Thing() : updates(0), destroyed(false) {} void FrameUpdate() { ++updates; } };
struct Block { long refs; Thing* obj; };
static int g_blocksFreed = 0;
struct Node { Node* next; Node* prev; Thing* data; Block* info; };
struct Manager {
    Node* head;
    Manager() { head = (Node*)std::malloc(sizeof(Node)); head->next = head->prev = head; }
    void Add(Thing* t, Block* b) { Node* n = (Node*)std::malloc(sizeof(Node)); n->data = t; n->info = b; n->prev = head->prev; n->next = head; head->prev->next = n; head->prev = n; }
    static long RefCount(const Node* n) { return n->info ? n->info->refs : 0; }
    Node* Erase(Node* n)
    {
        Node* next = n->next; Node* prev = n->prev;
        prev->next = next; next->prev = prev;
        if (n->info) { --n->info->refs; if (n->info->refs == 0) { n->info->obj->destroyed = true; ++g_blocksFreed; } }
        n->data = 0; n->info = 0;
        std::free(n);
        return next;
    }
    void FrameUpdate()
    {
        for (Node* it = head->next; it != head; ) {
            if (RefCount(it) == 1) it = Erase(it);
            else it = it->next;
        }
        for (Node* it = head->next; it != head; it = it->next) it->data->FrameUpdate();
    }
};
int main()
{
    int fail = 0;
    Thing a, b, c; Block ba = { 1, &a }; Block bb = { 2, &b }; Block bc = { 1, &c };
    Manager m; m.Add(&a, &ba); m.Add(&b, &bb); m.Add(&c, &bc);
    m.FrameUpdate();
    if (!a.destroyed || b.destroyed || !c.destroyed || g_blocksFreed != 2) fail = 1;
    if (a.updates != 0 || b.updates != 1 || c.updates != 0) fail = 1;
    if (m.head->next->data != &b || m.head->next->next != m.head) fail = 1;
    m.FrameUpdate();
    if (b.updates != 2) fail = 1;
    if (fail) { std::printf("007047d0_TEST FAIL\n"); return 1; }
    std::printf("007047d0_TEST PASS\n");
    return 0;
}
