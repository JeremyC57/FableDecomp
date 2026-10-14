#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_calls = 0; struct IObj { virtual void v000() { } virtual void Act() { ++g_calls; } }; struct SNode { SNode* next; SNode* prev; IObj* obj; };
struct COwner { unsigned char _pad[0x18]; SNode* list; void All() { for (SNode* n = list->next; n != list; n = n->next) n->obj->Act(); } };

int main() {
    SNode head, n1, n2; IObj o1, o2; head.next = &n1; n1.next = &n2; n2.next = &head; n1.obj = &o1; n2.obj = &o2; COwner* c = new COwner; c->list = &head; c->All(); if (g_calls != 2) { std::printf("00a36fe0_TEST FAIL\n"); return 1; } head.next = &head; c->All(); if (g_calls != 2) { std::printf("00a36fe0_TEST FAIL\n"); return 1; } delete c;
    std::printf("00a36fe0_TEST PASS\n"); return 0;
}