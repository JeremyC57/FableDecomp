#include <cstdio>
#include <cstring>
#include <cstdlib>

struct CMessageEvent { long type; long extra; };
struct SNode { SNode* next; SNode* prev; CMessageEvent value; CMessageEvent* GetEvent() { return &value; } };
struct CList { SNode* head; };
static SNode g_head, g_n1, g_n2, g_n3;
static void Link() { g_head.next = &g_n1; g_n1.prev = &g_head; g_n1.next = &g_n2; g_n2.prev = &g_n1; g_n2.next = &g_n3; g_n3.prev = &g_n2; g_n3.next = &g_head; g_head.prev = &g_n3; g_n1.value.type = 1; g_n2.value.type = 2; g_n3.value.type = 2; g_n1.value.extra = 0; g_n2.value.extra = 0; g_n3.value.extra = 1; }

struct IsOfType { long type; bool operator()(CMessageEvent* e) const { return e->type == type; } };
struct SPNode { SPNode* next; SPNode* prev; CMessageEvent* value; };
struct CPtrList { SPNode* head; void push_back(CMessageEvent* const& e) { SPNode* n = new SPNode; n->value = e; SPNode* p = head->prev; n->next = head; n->prev = p; p->next = n; head->prev = n; } };
inline long Distance(SPNode* f, SPNode* l) { long c = 0; for (; f != l; f = f->next) ++c; return c; }
struct CMgr { void* world; CList msgs;
    template<class F> long FindAllMessages(const F& f, CPtrList& found) { for (SNode* n = msgs.head->next; n != msgs.head; n = n->next) { if (f(n->GetEvent())) found.push_back(n->GetEvent()); } return Distance(found.head->next, found.head); } };
int main()
{
    Link(); CMgr m; m.msgs.head = &g_head; IsOfType f; f.type = 2; SPNode h; h.next = &h; h.prev = &h; CPtrList out; out.head = &h;
    if (m.FindAllMessages(f, out) != 2 || h.next->value != &g_n2.value || h.next->next->value != &g_n3.value || h.prev->value != &g_n3.value) { std::printf("00493eab_TEST FAIL\n"); return 1; }
    f.type = 1; if (m.FindAllMessages(f, out) != 3 || h.prev->value != &g_n1.value) { std::printf("00493eab_TEST FAIL\n"); return 1; }
    f.type = 7; if (m.FindAllMessages(f, out) != 3) { std::printf("00493eab_TEST FAIL\n"); return 1; }
    std::printf("00493eab_TEST PASS\n");
    return 0;
}