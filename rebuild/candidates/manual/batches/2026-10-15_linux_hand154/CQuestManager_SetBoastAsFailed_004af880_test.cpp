// Behaviour test for 0x004af880 (CQuestManager::SetBoastAsFailed): the reconstructed logic run over a host model.
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
static void SetFailed(BList& l, long id, const char* s) { Boast* b = Find(l, id, s); if (b) b->Failed = true; }

int main()
{
    BList* l = MakeList();
    SetFailed(*l, 3, "A"); CHECK(l->head.prev->value.Failed && !l->head.prev->value.Completed && !l->head.next->value.Failed);
    SetFailed(*l, 2, ""); CHECK(l->head.next->next->value.Failed && !l->head.next->next->next->value.Failed); delete l;
    if (g_fail) { std::printf("004af880_TEST FAIL\n"); return 1; }
    std::printf("004af880_TEST PASS\n");
    return 0;
}
