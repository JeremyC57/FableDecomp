// Behaviour test for 0x00664490 CThingCreatureBase::AddQueuedAction (model copy)
#include <cstdio>
#include <cstdlib>
#include <new>
struct Action { int id; virtual Action* Clone() const { Action* a = new Action(*this); return a; } virtual ~Action() { } };
struct Counted { Action* Data; void* Info; Counted() : Data(0), Info(0) { } void Reset(Action* a) { Data = a; } };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { Counted data; };
struct List {
    NodeBase* node;
    void push_back(const Counted& x)
    {
        Node* tmp = (Node*)malloc(sizeof(Node)); new (&tmp->data) Counted(x);
        NodeBase* n = node; NodeBase* p = n->prev;
        tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp;
    }
    Counted& back() { return ((Node*)node->prev)->data; }
};
struct Creature { List PQueuedActions; void AddQueuedAction(const Action& a) { PQueuedActions.push_back(Counted()); PQueuedActions.back().Reset(a.Clone()); } };
int main()
{
    NodeBase s; s.next = s.prev = &s;
    Creature c; c.PQueuedActions.node = &s;
    Action a; a.id = 4; Action b; b.id = 9;
    c.AddQueuedAction(a);
    c.AddQueuedAction(b);
    Node* first = (Node*)s.next; Node* second = (Node*)first->next;
    if (first->data.Data == 0 || first->data.Data->id != 4 || first->data.Data == &a || second->data.Data->id != 9 || second->next != &s || s.prev != second || first->prev != &s)
        { std::printf("00664490_TEST FAIL\n"); return 1; }
    std::printf("00664490_TEST PASS\n");
    return 0;
}