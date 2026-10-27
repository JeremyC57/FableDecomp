// Behaviour test for 0x00BD9ED0 CEngineLandscapeMemoryManager::~CEngineLandscapeMemoryManager (disposal lists freed, sentinels released)
#include <cstdio>
#include <cstdlib>

static int g_frees = 0;
static int g_disposed = 0;
struct Node { Node* next; Node* prev; int data; };
struct List {
    Node* head;
    List() { head = (Node*)std::malloc(sizeof(Node)); head->next = head; head->prev = head; }
    void push(int v) { Node* n = (Node*)std::malloc(sizeof(Node)); n->data = v; n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
    ~List() {
        Node* cur = head->next;
        while (cur != head) { Node* t = cur; cur = cur->next; std::free(t); ++g_frees; }
        head->next = head; head->prev = head;
        if (head) { std::free(head); ++g_frees; }
    }
};
struct Mgr {
    List Tex[3], Vb[3], Ib[3];
    void FreeDisposalList() { ++g_disposed; }
    ~Mgr() { FreeDisposalList(); }
};

int main()
{
    {
        Mgr m;
        m.Tex[0].push(1); m.Tex[2].push(2); m.Vb[1].push(3); m.Ib[0].push(4); m.Ib[0].push(5);
    }
    if (g_disposed != 1 || g_frees != 9 + 5) { std::printf("00bd9ed0_TEST FAIL\n"); return 1; }
    std::printf("00bd9ed0_TEST PASS\n");
    return 0;
}