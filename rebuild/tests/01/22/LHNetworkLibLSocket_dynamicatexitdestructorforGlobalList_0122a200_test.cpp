// Behaviour test for 0x0122A200, the dynamic atexit destructor of LHNetworkLib::LSocket::GlobalList: destroying the STLport list frees
// every node, then the malloc'd sentinel (the out-of-line _List_base destructor 0x008F9F50 the thunk tail-jumps to).
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
struct NodeBase { NodeBase* Next; NodeBase* Prev; };
struct Node : NodeBase { void* Data; };
struct SocketListModel {
    Node* Sentinel;
    SocketListModel() { Node* n = (Node*)std::malloc(sizeof(Node)); n->Next = n; n->Prev = n; Sentinel = n; }
    void PushBack(void* d)
    {
        Node* t = (Node*)std::malloc(sizeof(Node));
        t->Data = d; t->Next = Sentinel; t->Prev = Sentinel->Prev; Sentinel->Prev->Next = t; Sentinel->Prev = t;
    }
    void Destroy()   // ~_List_base: clear(), then deallocate the sentinel
    {
        Node* cur = (Node*)Sentinel->Next;
        while (cur != Sentinel) { Node* tmp = cur; cur = (Node*)cur->Next; std::free(tmp); ++g_frees; }
        Sentinel->Next = Sentinel; Sentinel->Prev = Sentinel;
        std::free(Sentinel); ++g_frees;
    }
};
static SocketListModel g_GlobalList;
static void AtexitDestructor() { g_GlobalList.Destroy(); }
int main()
{
    g_GlobalList.PushBack(0);
    g_GlobalList.PushBack(0);
    AtexitDestructor();
    if (g_frees == 3) { std::printf("AUTO_TINY_0122a200_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a200_TEST FAIL\n");
    return 1;
}
