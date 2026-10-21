// Behaviour test for 008a1cc0 CGameScriptInterface::SetResidency: eviction loop over the village resident list.
#include <cstdio>
struct Thing { bool dead; bool available; Thing* home; Thing* other; bool killed; };
struct Node { Node* next; Thing* data; };
static Node* g_head;   // sentinel->next
static void RemoveKilled() { Node** p = &g_head; while (*p) { if ((*p)->data->killed) *p = (*p)->next; else p = &(*p)->next; } }
static Thing* Evict(Thing* house, bool resident, bool& locking)
{
    Thing* evictedTo = 0;
    locking = !resident;
    house->available = resident;
    if (!resident) {
        Node* it = g_head;
        while (it) {
            Thing* r = it->data;
            if (!r || r->dead) continue;
            if (r->home == house) {
                if (r->other) { r->other->available = false; evictedTo = r->other; }
                r->killed = true; RemoveKilled();
                it = g_head;
            } else
                it = it->next;
        }
    }
    return evictedTo;
}
int main()
{
    Thing house = { false, true, 0, 0, false }, other = { false, true, 0, 0, false }, elsewhere = { false, true, 0, 0, false };
    Thing a = { false, false, &house, &other, false }, b = { false, false, &elsewhere, 0, false }, c = { false, false, &house, 0, false };
    Node n3 = { 0, &c }, n2 = { &n3, &b }, n1 = { &n2, &a };
    g_head = &n1;
    bool locking = false;
    Thing* r = Evict(&house, false, locking);
    if (r != &other || other.available || !a.killed || b.killed || !c.killed || house.available || !locking || g_head != &n2 || n2.next != 0) {
        std::printf("008a1cc0_TEST FAIL\n"); return 1; }
    Thing* r2 = Evict(&house, true, locking);
    if (r2 != 0 || !house.available || locking) { std::printf("008a1cc0_TEST FAIL\n"); return 1; }
    std::printf("008a1cc0_TEST PASS\n");
    return 0;
}