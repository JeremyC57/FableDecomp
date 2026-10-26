// Behaviour test for 0x0042abaf: std::list<NUISystem::CComponent*>::list(const allocator&) -- one self-linked 12-byte
// sentinel; the list is that one word.
#include <cstdio>
#include <cstdlib>
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { void* data; };
static unsigned g_size;
struct List { Node* node; List() : node(0) { g_size = sizeof(Node); Node* n = (Node*)std::malloc(sizeof(Node)); n->next = n; n->prev = n; node = n; } };
int main()
{
    List l;
    bool ok = sizeof(List) == 4 && g_size == 12 && l.node && l.node->next == l.node && l.node->prev == l.node;
    std::free(l.node);
    if (!ok) { std::printf("0042abaf_TEST FAIL\n"); return 1; }
    std::printf("0042abaf_TEST PASS\n"); return 0;
}
