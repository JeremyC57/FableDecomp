#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x004B8C00: exercises the same logic through a copy of the model (compiled separately from the source)
static int g_freed = 0, g_destroyed = 0;
static void FreeNode(void* p) { ++g_freed; std::free(p); }
class CCharString { public: void* m_pData; ~CCharString() { ++g_destroyed; } };
class CWideString { public: void* m_pData; ~CWideString() { g_destroyed += 10; } };
struct SStringRecord_004b8c00 { long m_a, m_b, m_c; CCharString m_name; CWideString m_text; CWideString m_tooltip; };
struct L {
    struct node { node* next; node* prev; SStringRecord_004b8c00 value; };
    node* head;
    void clear()
    {
        node* n = head->next;
        while (n != head) { node* victim = n; n = n->next; victim->value.~SStringRecord_004b8c00(); FreeNode(victim); }
        head->next = head; head->prev = head;
    }
};
static bool Run()
{
    L list; L::node* head = (L::node*)std::malloc(sizeof(L::node)); list.head = head;
    L::node* a = (L::node*)std::malloc(sizeof(L::node)); L::node* b = (L::node*)std::malloc(sizeof(L::node));
    head->next = a; a->prev = head; a->next = b; b->prev = a; b->next = head; head->prev = b;
    list.clear();
    if (g_freed != 2 || g_destroyed != 42 || head->next != head || head->prev != head) return false;
    list.clear();                                  // empty: nothing more
    bool ok = g_freed == 2 && g_destroyed == 42;
    std::free(head);
    return ok;
}
int main()
{
    if (!Run()) { std::printf("004b8c00_TEST FAIL\n"); return 1; }
    std::printf("004b8c00_TEST PASS\n");
    return 0;
}