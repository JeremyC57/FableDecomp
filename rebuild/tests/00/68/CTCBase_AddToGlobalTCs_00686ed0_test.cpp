// Behaviour test for 0x00686ED0 CTCBase::AddToGlobalTCs: appended once to the global list, flag set.
#include <cstdio>
#include <cstdlib>
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct TC;
struct Node : NodeBase { TC* data; };
static NodeBase* g_sentinel;
struct TC {
    void* vtbl; void* parent; bool toBeRemoved; bool isInGlobal;
    void AddToGlobalTCs()
    {
        if (!isInGlobal) {
            NodeBase* pos = g_sentinel;
            Node* n = (Node*)std::malloc(sizeof(Node));
            n->data = this;
            NodeBase* prev = pos->prev;
            n->next = pos; n->prev = prev; prev->next = n; pos->prev = n;
            isInGlobal = true;
        }
    }
};
int main()
{
    g_sentinel = (NodeBase*)std::malloc(sizeof(Node)); g_sentinel->next = g_sentinel; g_sentinel->prev = g_sentinel;
    TC a = { 0, 0, false, false }, b = { 0, 0, false, false };
    a.AddToGlobalTCs(); b.AddToGlobalTCs(); a.AddToGlobalTCs();
    int n = 0; for (NodeBase* p = g_sentinel->next; p != g_sentinel; p = p->next) ++n;
    if (n != 2 || !a.isInGlobal || !b.isInGlobal || ((Node*)g_sentinel->next)->data != &a || ((Node*)g_sentinel->prev)->data != &b) {
        std::printf("00686ed0_TEST FAIL\n"); return 1;
    }
    std::printf("00686ed0_TEST PASS\n");
    return 0;
}