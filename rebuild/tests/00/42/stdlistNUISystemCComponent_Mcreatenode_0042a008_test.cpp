// Behaviour test for 0x0042a008: std::list<NUISystem::CComponent*>::_M_create_node -- one 12-byte node holding a copy of x.
#include <cstdio>
#include <cstdlib>
#include <new>
struct C { int v; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { C* data; };
static int g_allocs; static unsigned g_size;
static void* my_malloc(unsigned n) { ++g_allocs; g_size = n; return std::malloc(n); }
static Node* create(C* const& x) { Node* p = (Node*)my_malloc(sizeof(Node)); new (&p->data) C*(x); return p; }
int main()
{
    C c; C* x = &c; Node* n = create(x);
    bool ok = g_allocs == 1 && g_size == 12 && n && n->data == &c && (char*)&n->data - (char*)n == 8;
    std::free(n);
    if (!ok) { std::printf("0042a008_TEST FAIL\n"); return 1; }
    std::printf("0042a008_TEST PASS\n"); return 0;
}
