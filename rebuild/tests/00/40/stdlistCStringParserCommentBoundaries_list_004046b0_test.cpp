// Behaviour test for 0x004046B0 std::list<CStringParserCommentBoundaries>::~list: every node and the sentinel are freed once.
#include <cstdio>
#include <cstdlib>

static int g_frees = 0;
static void* g_freed[16];
static void TestFree(void* p) { g_freed[g_frees++] = p; std::free(p); }

class CStringParserCommentBoundaries { public: long Start; long End; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { CStringParserCommentBoundaries data; };

struct List {
    NodeBase* sentinel;
    void clear()
    {
        Node* cur = (Node*)sentinel->next;
        while (cur != sentinel) {
            Node* tmp = cur;
            cur = (Node*)cur->next;
            TestFree(tmp);
        }
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
    }
    void destroy()
    {
        clear();
        if (sentinel)
            TestFree(sentinel);
    }
};

int main()
{
    List l;
    l.sentinel = (NodeBase*)std::malloc(sizeof(Node));
    l.sentinel->next = l.sentinel; l.sentinel->prev = l.sentinel;
    void* nodes[3];
    for (int i = 0; i < 3; ++i) {
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->data.Start = i; n->data.End = i + 10;
        n->next = l.sentinel; n->prev = l.sentinel->prev;
        l.sentinel->prev->next = n; l.sentinel->prev = n;
        nodes[i] = n;
    }
    void* s = l.sentinel;
    l.destroy();
    if (g_frees != 4 || g_freed[0] != nodes[0] || g_freed[1] != nodes[1] || g_freed[2] != nodes[2] || g_freed[3] != s) {
        std::printf("004046b0_TEST FAIL\n"); return 1;
    }
    // empty list: only the sentinel is freed
    g_frees = 0;
    l.sentinel = (NodeBase*)std::malloc(sizeof(Node));
    l.sentinel->next = l.sentinel; l.sentinel->prev = l.sentinel;
    s = l.sentinel;
    l.destroy();
    if (g_frees != 1 || g_freed[0] != s) { std::printf("004046b0_TEST FAIL\n"); return 1; }
    std::printf("004046b0_TEST PASS\n");
    return 0;
}