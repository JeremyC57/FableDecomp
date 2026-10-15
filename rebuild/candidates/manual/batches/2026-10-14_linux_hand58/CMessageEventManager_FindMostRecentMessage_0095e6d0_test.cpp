#include <cstdio>
#include <cstring>
#include <cstdlib>

struct CMessageEvent { long type; long extra; };
struct SNode { SNode* next; SNode* prev; CMessageEvent value; CMessageEvent* GetEvent() { return &value; } };
struct CList { SNode* head; };
static SNode g_head, g_n1, g_n2, g_n3;
static void Link() { g_head.next = &g_n1; g_n1.prev = &g_head; g_n1.next = &g_n2; g_n2.prev = &g_n1; g_n2.next = &g_n3; g_n3.prev = &g_n2; g_n3.next = &g_head; g_head.prev = &g_n3; g_n1.value.type = 1; g_n2.value.type = 2; g_n3.value.type = 2; g_n1.value.extra = 0; g_n2.value.extra = 0; g_n3.value.extra = 1; }

static int g_calls = 0;
struct IsMyDeadLeader { long type; bool operator()(CMessageEvent* e) const { ++g_calls; return e->type == type; } };

struct CMgr { void* world; CList msgs;
    template<class F> CMessageEvent* FindMostRecentMessage(const F& f) { for (SNode* n = msgs.head->next; n != msgs.head; n = n->next) { CMessageEvent* e = n->GetEvent(); if (f(e)) return e; } return 0; } };

int main()
{
    Link(); CMgr m; m.msgs.head = &g_head; IsMyDeadLeader f; f.type = 2;
    if (m.FindMostRecentMessage(f) != &g_n2.value || g_calls != 2) { std::printf("0095e6d0_TEST FAIL\n"); return 1; }
    f.type = 9; g_calls = 0; if (m.FindMostRecentMessage(f) != 0 || g_calls != 3) { std::printf("0095e6d0_TEST FAIL\n"); return 1; }
    g_head.next = &g_head; g_head.prev = &g_head; g_calls = 0; if (m.FindMostRecentMessage(f) != 0 || g_calls != 0) { std::printf("0095e6d0_TEST FAIL\n"); return 1; }
    std::printf("0095e6d0_TEST PASS\n");
    return 0;
}
