#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x004B8C50: exercises the same logic through a copy of the model (compiled separately from the source)
static int g_freed = 0, g_destroyed = 0;
static void FreeNode(void* p) { ++g_freed; std::free(p); }
class CWideString { public: void* m_pData; ~CWideString() { ++g_destroyed; } };
struct L {
    struct node { node* next; node* prev; CWideString value; };
    node* head;
    void clear()
    {
        node* n = head->next;
        while (n != head) { node* victim = n; n = n->next; victim->value.~CWideString(); FreeNode(victim); }
        head->next = head; head->prev = head;
    }
};
static bool Run()
{
    L list; L::node* head = (L::node*)std::malloc(sizeof(L::node)); list.head = head;
    L::node* a = (L::node*)std::malloc(sizeof(L::node)); L::node* b = (L::node*)std::malloc(sizeof(L::node)); L::node* c = (L::node*)std::malloc(sizeof(L::node));
    head->next = a; a->prev = head; a->next = b; b->prev = a; b->next = c; c->prev = b; c->next = head; head->prev = c;
    list.clear();
    bool ok = g_freed == 3 && g_destroyed == 3 && head->next == head && head->prev == head;
    std::free(head);
    return ok;
}
int main()
{
    if (!Run()) { std::printf("004b8c50_TEST FAIL\n"); return 1; }
    std::printf("004b8c50_TEST PASS\n");
    return 0;
}