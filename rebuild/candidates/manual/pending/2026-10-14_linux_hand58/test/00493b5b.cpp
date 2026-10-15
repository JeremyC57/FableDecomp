#include <cstdio>
#include <cstring>
#include <cstdlib>

struct CMessageEvent { long type; long extra; };
struct SNode { SNode* next; SNode* prev; CMessageEvent value; CMessageEvent* GetEvent() { return &value; } };
struct CList { SNode* head; };
static SNode g_head, g_n1, g_n2, g_n3;
static void Link() { g_head.next = &g_n1; g_n1.prev = &g_head; g_n1.next = &g_n2; g_n2.prev = &g_n1; g_n2.next = &g_n3; g_n3.prev = &g_n2; g_n3.next = &g_head; g_head.prev = &g_n3; g_n1.value.type = 1; g_n2.value.type = 2; g_n3.value.type = 2; g_n1.value.extra = 0; g_n2.value.extra = 0; g_n3.value.extra = 1; }

static SNode* g_erased = 0; static int g_calls = 0;
struct CIt { SNode* n; CIt(SNode* p) : n(p) { } CIt(const CIt& o) : n(o.n) { } };
struct CEList : CList { CIt erase(CIt w) { ++g_calls; g_erased = w.n; return CIt(w.n->next); } };
struct CMgr { void* world; CEList msgs; void RemoveMessage(const CMessageEvent* e) { for (SNode* n = msgs.head->next; n != msgs.head; n = n->next) { if (n->GetEvent() == e) { msgs.erase(CIt(n)); return; } } } };
int main()
{
    Link(); CMgr m; m.msgs.head = &g_head; CMessageEvent other;
    m.RemoveMessage(&g_n2.value); if (g_calls != 1 || g_erased != &g_n2) { std::printf("00493b5b_TEST FAIL\n"); return 1; }
    m.RemoveMessage(&other); if (g_calls != 1) { std::printf("00493b5b_TEST FAIL\n"); return 1; }
    m.RemoveMessage(&g_n3.value); if (g_calls != 2 || g_erased != &g_n3) { std::printf("00493b5b_TEST FAIL\n"); return 1; }
    std::printf("00493b5b_TEST PASS\n");
    return 0;
}
