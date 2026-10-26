// Behaviour test for 0x0042ab8a: std::list<NUISystem::CComponent*>::insert(pos, x) -- the new node is linked in before
// pos and returned.
#include <cstdio>
#include <cstdlib>
struct C { int v; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { C* data; };
static Node* create(C* const& x) { Node* p = (Node*)std::malloc(sizeof(Node)); p->data = x; return p; }
static NodeBase* insert(NodeBase* n, C* const& x)
{
    Node* tmp = create(x); NodeBase* p = n->prev;
    tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp; return tmp;
}
int main()
{
    NodeBase s; s.next = &s; s.prev = &s; C a, b; C* pa = &a; C* pb = &b;
    NodeBase* ia = insert(&s, pa);          // push_back a
    NodeBase* ib = insert(ia, pb);          // before a
    bool ok = s.next == ib && ib->next == ia && ia->next == &s && s.prev == ia && ia->prev == ib && ib->prev == &s
              && ((Node*)ia)->data == &a && ((Node*)ib)->data == &b;
    std::free(ia); std::free(ib);
    if (!ok) { std::printf("0042ab8a_TEST FAIL\n"); return 1; }
    std::printf("0042ab8a_TEST PASS\n"); return 0;
}
