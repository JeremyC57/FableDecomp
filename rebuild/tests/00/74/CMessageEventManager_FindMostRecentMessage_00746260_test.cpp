#include <cstdio>
#include <cstring>
#include <cstdlib>

class CMessageEvent { public: long type; long extra; };
struct SNode { SNode* next; SNode* prev; CMessageEvent value; CMessageEvent* GetEvent() { return &value; } };
struct CList { SNode* head; };
static SNode g_head, g_n1, g_n2, g_n3;
static void Link() { g_head.next = &g_n1; g_n1.prev = &g_head; g_n1.next = &g_n2; g_n2.prev = &g_n1; g_n2.next = &g_n3; g_n3.prev = &g_n2; g_n3.next = &g_head; g_head.prev = &g_n3; g_n1.value.type = 1; g_n2.value.type = 2; g_n3.value.type = 2; g_n1.value.extra = 0; g_n2.value.extra = 0; g_n3.value.extra = 1; }

static char g_seq[32] = "";
struct IsOfType { long type; bool operator()(CMessageEvent* e) const { std::strcat(g_seq, "T"); return e->type == type; } };
struct Seen { void* t; bool operator()(CMessageEvent* e) const { std::strcat(g_seq, "S"); return e->extra == 1; } };
template<class A, class B> struct Bind2 { const A* a; const B* b; Bind2(const A& x, const B& y) : a(&x), b(&y) { } bool operator()(CMessageEvent* e) const { return (*a)(e) && (*b)(e); } };

struct CMgr { void* world; CList msgs;
    template<class F> CMessageEvent* FindMostRecentMessage(const F& f) { for (SNode* n = msgs.head->next; n != msgs.head; n = n->next) { CMessageEvent* e = n->GetEvent(); if (f(e)) return e; } return 0; } };

int main()
{
    Link(); CMgr m; m.msgs.head = &g_head; IsOfType f; f.type = 2; Seen s; Bind2<IsOfType, Seen> b(f, s);
    if (m.FindMostRecentMessage(b) != &g_n3.value || std::strcmp(g_seq, "TTSTS")) { std::printf("00746260_TEST FAIL\n"); return 1; }
    f.type = 1; g_seq[0] = 0; if (m.FindMostRecentMessage(b) != 0 || std::strcmp(g_seq, "TSTT")) { std::printf("00746260_TEST FAIL\n"); return 1; }
    std::printf("00746260_TEST PASS\n");
    return 0;
}