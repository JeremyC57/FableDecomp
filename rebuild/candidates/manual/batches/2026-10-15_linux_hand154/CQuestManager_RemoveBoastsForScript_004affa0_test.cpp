// Behaviour test for 0x004affa0 (CQuestManager::RemoveBoastsForScript): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Boast { long ID; char Script[32]; bool Failed, Completed; };
struct BNode { BNode* next; BNode* prev; Boast value; };
struct BList { BNode head; int n;
    BList() : n(0) { head.next = head.prev = &head; }
    void push(long id, const char* s) { BNode* p = new BNode; p->value.ID = id; std::strcpy(p->value.Script, s); p->value.Failed = p->value.Completed = false;
        p->next = &head; p->prev = head.prev; head.prev->next = p; head.prev = p; ++n; }
    ~BList() { BNode* p = head.next; while (p != &head) { BNode* q = p->next; delete p; p = q; } } };
static bool Match(const Boast& b, long id, const char* s) { return b.ID == id && (s[0] == 0 || std::strcmp(s, b.Script) == 0); }
static Boast* Find(BList& l, long id, const char* s) { for (BNode* p = l.head.next; p != &l.head; p = p->next) if (Match(p->value, id, s)) return &p->value; return 0; }
static BList* MakeList() { BList* l = new BList; l->push(1, "A"); l->push(2, "B"); l->push(2, "C"); l->push(3, "A"); return l; }
static void RemoveForScript(BList& l, const char* s)
{ BNode* p = l.head.next; while (p != &l.head) { if (std::strcmp(p->value.Script, s) == 0) { BNode* nx = p->next; p->prev->next = nx; nx->prev = p->prev; delete p; --l.n; p = nx; } else p = p->next; } }

int main()
{
    BList* l = MakeList();
    RemoveForScript(*l, "B"); CHECK(l->n == 3 && Find(*l, 2, "B") == 0 && Find(*l, 2, "C") != 0); delete l;
    if (g_fail) { std::printf("004affa0_TEST FAIL\n"); return 1; }
    std::printf("004affa0_TEST PASS\n");
    return 0;
}
