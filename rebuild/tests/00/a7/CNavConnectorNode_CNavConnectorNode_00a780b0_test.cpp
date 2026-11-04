// Behaviour test for CNavConnectorNode::~CNavConnectorNode (0x00A780B0), g++ model of the retail logic.
#include <cstdio>
#include <algorithm>
struct Node { const Node* n[8]; int size; int region; };
static int removedFrom = -1, removedTo = -1;
static void Unlink(Node* a, Node* b)
{
    const Node** it1 = std::find(a->n, a->n + a->size, b);
    const Node** it2 = std::find(b->n, b->n + b->size, a);
    if (it1 - a->n < a->size - 1) std::swap(*it1, a->n[a->size - 1]);
    if (it2 - b->n < b->size - 1) std::swap(*it2, b->n[b->size - 1]);
    --a->size; --b->size;
    removedFrom = a->region; removedTo = b->region;
}
int main()
{
    Node x = { {0}, 0, 1 }, y = { {0}, 0, 2 }, z = { {0}, 0, 3 }, w = { {0}, 0, 4 };
    x.n[0] = &y; x.n[1] = &z; x.n[2] = &w; x.size = 3;
    y.n[0] = &z; y.n[1] = &x; y.size = 2;
    Unlink(&x, &y);
    int ok = x.size == 2 && x.n[0] == &w && x.n[1] == &z && y.size == 1 && y.n[0] == &z && removedFrom == 1 && removedTo == 2;
    std::printf(ok ? "00a780b0_TEST PASS\n" : "00a780b0_TEST FAIL\n");
    return ok ? 0 : 1;
}