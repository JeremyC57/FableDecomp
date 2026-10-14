#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x0070BA00: exercises the same logic through a copy of the model (compiled separately from the source)
static int g_freed = 0, g_destroyed = 0;
static void FreeNode(void* p) { ++g_freed; std::free(p); }
static int g_innerCleared = 0;
struct CInner {
    void* m_pHead;
    void clear() { ++g_innerCleared; }
    ~CInner() { clear(); if (m_pHead) FreeNode(m_pHead); }
};
struct L {
    struct node { node* next; node* prev; CInner value; };
    node* head;
    void clear()
    {
        node* n = head->next;
        while (n != head) { node* victim = n; n = n->next; victim->value.~CInner(); FreeNode(victim); }
        head->next = head; head->prev = head;
    }
};
static bool Run()
{
    L list; L::node* head = (L::node*)std::malloc(sizeof(L::node)); list.head = head;
    L::node* a = (L::node*)std::malloc(sizeof(L::node)); L::node* b = (L::node*)std::malloc(sizeof(L::node));
    a->value.m_pHead = std::malloc(16); b->value.m_pHead = 0;   // one inner list with a head node, one without
    head->next = a; a->prev = head; a->next = b; b->prev = a; b->next = head; head->prev = b;
    list.clear();
    bool ok = g_innerCleared == 2 && g_freed == 3 && head->next == head && head->prev == head;
    std::free(head);
    return ok;
}
int main()
{
    if (!Run()) { std::printf("0070ba00_TEST FAIL\n"); return 1; }
    std::printf("0070ba00_TEST PASS\n");
    return 0;
}