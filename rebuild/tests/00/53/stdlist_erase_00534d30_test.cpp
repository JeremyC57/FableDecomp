// Behaviour test for 0x00534D30 list<CComponent*>::erase(first, last): the run is unlinked and freed, last is returned.
#include <cstdio>
#include <cstdlib>
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { void* data; };
static int g_frees = 0;
static NodeBase* EraseRange(NodeBase* first, NodeBase* last)
{
    while (first != last) {
        NodeBase* n = first;
        first = first->next;
        NodeBase* next = n->next;
        NodeBase* prev = n->prev;
        prev->next = next;
        next->prev = prev;
        if (n) { std::free(n); ++g_frees; }
    }
    return last;
}
int main()
{
    NodeBase* s = (NodeBase*)std::malloc(sizeof(Node));
    s->next = s; s->prev = s;
    Node* nodes[4];
    for (int i = 0; i < 4; ++i) {
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->data = (void*)(long)(i + 1);
        n->next = s; n->prev = s->prev; s->prev->next = n; s->prev = n;
        nodes[i] = n;
    }
    NodeBase* r = EraseRange(nodes[1], nodes[3]);
    if (r != nodes[3] || g_frees != 2 || s->next != nodes[0] || nodes[0]->next != nodes[3] || nodes[3]->prev != nodes[0]) {
        std::printf("00534d30_TEST FAIL\n"); return 1;
    }
    r = EraseRange(s->next, s);
    if (r != s || g_frees != 4 || s->next != s || s->prev != s) { std::printf("00534d30_TEST FAIL\n"); return 1; }
    std::free(s);
    std::printf("00534d30_TEST PASS\n");
    return 0;
}