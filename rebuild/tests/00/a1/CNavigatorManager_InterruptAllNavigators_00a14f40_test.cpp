#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_calls = 0; struct IObj { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void v005() { } virtual void v006() { } virtual void v007() { } virtual void v008() { } virtual void v009() { } virtual void v010() { } virtual void v011() { } virtual void v012() { } virtual void v013() { } virtual void v014() { } virtual void v015() { } virtual void v016() { } virtual void v017() { } virtual void Act() { ++g_calls; } }; struct SNode { SNode* next; SNode* prev; IObj* obj; };
struct COwner { unsigned char _pad[0x18]; SNode* list; void All() { for (SNode* n = list->next; n != list; n = n->next) n->obj->Act(); } };

int main() {
    SNode head, n1, n2; IObj o1, o2; head.next = &n1; n1.next = &n2; n2.next = &head; n1.obj = &o1; n2.obj = &o2; COwner* c = new COwner; c->list = &head; c->All(); if (g_calls != 2) { std::printf("00a14f40_TEST FAIL\n"); return 1; } head.next = &head; c->All(); if (g_calls != 2) { std::printf("00a14f40_TEST FAIL\n"); return 1; } delete c;
    std::printf("00a14f40_TEST PASS\n"); return 0;
}