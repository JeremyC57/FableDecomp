#include <cstdio>
#include <cstring>
#include <cstdlib>

struct CMessageEvent { long type; long extra; };
struct SNode { SNode* next; SNode* prev; CMessageEvent value; CMessageEvent* GetEvent() { return &value; } };
struct CList { SNode* head; };
static SNode g_head, g_n1, g_n2, g_n3;
static void Link() { g_head.next = &g_n1; g_n1.prev = &g_head; g_n1.next = &g_n2; g_n2.prev = &g_n1; g_n2.next = &g_n3; g_n3.prev = &g_n2; g_n3.next = &g_head; g_head.prev = &g_n3; g_n1.value.type = 1; g_n2.value.type = 2; g_n3.value.type = 2; g_n1.value.extra = 0; g_n2.value.extra = 0; g_n3.value.extra = 1; }

static char g_seq[32] = "";
struct IsOfType { long type; bool operator()(CMessageEvent* e) const { std::strcat(g_seq, "T"); return e->type == type; } };
struct HasExtra { bool operator()(CMessageEvent* e) const { std::strcat(g_seq, "X"); return e->extra != 0; } };
struct Between { long a, b; bool operator()(CMessageEvent* e) const { std::strcat(g_seq, "B"); return e->extra >= a && e->extra <= b; } };
template<class A, class B, class C> struct Bind3 { const A* a; const B* b; const C* c; bool operator()(CMessageEvent* e) const { return (*a)(e) && (*b)(e) && (*c)(e); } };

struct CMgr { void* world; CList msgs;
    template<class F> CMessageEvent* FindMostRecentMessage(const F& f) { for (SNode* n = msgs.head->next; n != msgs.head; n = n->next) { CMessageEvent* e = n->GetEvent(); if (f(e)) return e; } return 0; } };

int main()
{
    Link(); CMgr m; m.msgs.head = &g_head; IsOfType f; f.type = 2; HasExtra x; Between w; w.a = 1; w.b = 1; Bind3<IsOfType, HasExtra, Between> b; b.a = &f; b.b = &x; b.c = &w;
    if (m.FindMostRecentMessage(b) != &g_n3.value || std::strcmp(g_seq, "TTXTXB")) { std::printf("0091a5b0_TEST FAIL\n"); return 1; }
    w.a = 2; g_seq[0] = 0; if (m.FindMostRecentMessage(b) != 0 || std::strcmp(g_seq, "TTXTXB")) { std::printf("0091a5b0_TEST FAIL\n"); return 1; }
    std::printf("0091a5b0_TEST PASS\n");
    return 0;
}