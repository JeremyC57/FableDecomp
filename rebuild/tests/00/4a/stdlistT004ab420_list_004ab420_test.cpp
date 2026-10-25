// Behaviour test for std::list<T_004ab420>::~list() (0x004AB420): every node and then the sentinel are freed once;
// an empty list frees only the sentinel.
#include <cstdio>
#include <cstdlib>
static int g_freed = 0;
struct Node { Node* next; Node* prev; long value; };
struct List {
    Node* head;
    void Destroy()
    {
        Node* cur = head->next;
        while (cur != head) { Node* tmp = cur; cur = cur->next; std::free(tmp); ++g_freed; }
        head->next = head; head->prev = head;
        if (head != 0) { std::free(head); ++g_freed; }
    }
};
static Node* NewNode() { Node* n = (Node*)std::malloc(sizeof(Node)); n->next = n->prev = n; n->value = 0; return n; }
static void PushBack(List& l, long v)
{
    Node* n = NewNode(); n->value = v;
    n->prev = l.head->prev; n->next = l.head; l.head->prev->next = n; l.head->prev = n;
}
int main()
{
    List a; a.head = NewNode();
    PushBack(a, 1); PushBack(a, 2); PushBack(a, 3);
    a.Destroy();
    bool ok = g_freed == 4;
    List b; b.head = NewNode();
    b.Destroy();
    ok = ok && g_freed == 5;
    if (!ok) { std::printf("004ab420_TEST FAIL\n"); return 1; }
    std::printf("004ab420_TEST PASS\n");
    return 0;
}
