// Behaviour test for 0x0121F850, the dynamic initializer of LHNetworkLib::LSocket::GlobalList: an empty STLport list whose malloc'd
// sentinel links to itself; destruction frees every node and then the sentinel.
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
struct NodeBase { NodeBase* Next; NodeBase* Prev; };
struct Node : NodeBase { void* Data; };
struct ListModel {
    Node* Sentinel;
    ListModel()
    {
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->Next = n;
        n->Prev = n;
        Sentinel = n;
    }
    void PushBack(void* d)
    {
        Node* t = (Node*)std::malloc(sizeof(Node));
        t->Data = d; t->Next = Sentinel; t->Prev = Sentinel->Prev; Sentinel->Prev->Next = t; Sentinel->Prev = t;
    }
    ~ListModel()
    {
        Node* cur = (Node*)Sentinel->Next;
        while (cur != Sentinel) { Node* tmp = cur; cur = (Node*)cur->Next; std::free(tmp); ++g_frees; }
        std::free(Sentinel); ++g_frees;
    }
};
int main()
{
    {
        ListModel list;
        if (list.Sentinel->Next != list.Sentinel || list.Sentinel->Prev != list.Sentinel) { std::printf("0121f850_TEST FAIL\n"); return 1; }
        list.PushBack(0);
        list.PushBack(0);
    }
    if (g_frees != 3) { std::printf("0121f850_TEST FAIL\n"); return 1; }
    std::printf("0121f850_TEST PASS\n");
    return 0;
}