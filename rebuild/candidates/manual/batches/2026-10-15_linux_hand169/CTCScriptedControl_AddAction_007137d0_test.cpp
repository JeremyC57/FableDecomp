#include <cstdio>
// Behaviour model of CTCScriptedControl::AddAction (0x007137D0): the action is attached, notified once, appended at the
// list end, and the list node holds the only remaining reference.
struct Control;
struct Action { Control* owner; int added; };
struct Block { long refs; Action* item; };
struct Node { Node* next; Node* prev; Action* data; Block* info; };
struct Control {
    Node head;
    void Add(Action* a, Node* n, Block* b)
    {
        a->owner = this; a->added++;
        b->refs = 1; b->item = a;              // temporary
        n->data = a; n->info = b; ++b->refs;   // node copy
        Node* prev = head.prev; n->next = &head; n->prev = prev; prev->next = n; head.prev = n;
        --b->refs;                             // temporary released
    }
};
int main()
{
    Control c; c.head.next = c.head.prev = &c.head;
    Action a1 = { 0, 0 }, a2 = { 0, 0 }; Node n1, n2; Block b1, b2;
    c.Add(&a1, &n1, &b1); c.Add(&a2, &n2, &b2);
    if (a1.owner != &c || a2.added != 1 || c.head.next != &n1 || n1.next != &n2 || c.head.prev != &n2 || n2.prev != &n1 || b1.refs != 1) { std::printf("007137d0_TEST FAIL\n"); return 1; }
    std::printf("007137d0_TEST PASS\n");
    return 0;
}
