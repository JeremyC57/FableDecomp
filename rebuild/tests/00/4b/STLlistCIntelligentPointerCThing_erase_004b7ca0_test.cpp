#include <cstdio>
#include <cstdlib>
#include <new>

// Behaviour model of list<CIntelligentPointer<CThing> >::erase(iterator) (0x004B7CA0): unlink the node, run the
// element's virtual destructor, free the node, return the iterator to the next node.
static int g_dtors = 0;
struct CThing { int id; };
struct CBaseIntelligentPointer { virtual ~CBaseIntelligentPointer() { ++g_dtors; } CThing* m_pItem; };
struct CIntelligentPointer : CBaseIntelligentPointer { };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { CIntelligentPointer data; };
struct Iterator { NodeBase* node; };
struct List {
    NodeBase* node;
    Iterator erase(Iterator pos)
    {
        NodeBase* next = pos.node->next; NodeBase* prev = pos.node->prev;
        Node* n = (Node*)pos.node;
        prev->next = next; next->prev = prev;
        n->data.~CIntelligentPointer();
        std::free(n);
        Iterator r = { next };
        return r;
    }
};
static Node* MakeNode(CThing* t) { Node* n = (Node*)std::malloc(sizeof(Node)); new (&n->data) CIntelligentPointer(); n->data.m_pItem = t; return n; }

int main()
{
    CThing a = { 1 }, b = { 2 }, c = { 3 };
    NodeBase head;
    Node* n1 = MakeNode(&a); Node* n2 = MakeNode(&b); Node* n3 = MakeNode(&c);
    head.next = n1; n1->prev = &head; n1->next = n2; n2->prev = n1; n2->next = n3; n3->prev = n2; n3->next = &head; head.prev = n3;
    List l = { &head };
    Iterator mid = { n2 };
    Iterator r = l.erase(mid);
    if (r.node != n3 || n1->next != n3 || n3->prev != n1 || g_dtors != 1) { std::printf("004b7ca0_TEST FAIL\n"); return 1; }
    Iterator last = { n3 };
    r = l.erase(last);
    if (r.node != &head || n1->next != &head || head.prev != n1 || g_dtors != 2) { std::printf("004b7ca0_TEST FAIL\n"); return 1; }
    Iterator only = { n1 };
    r = l.erase(only);
    if (r.node != &head || head.next != &head || head.prev != &head || g_dtors != 3) { std::printf("004b7ca0_TEST FAIL\n"); return 1; }
    std::printf("004b7ca0_TEST PASS\n");
    return 0;
}