#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct IR { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void v005() { } virtual void v006() { } virtual void v007() { } virtual void v008() { } virtual void v009() { } virtual void v010() { } virtual void v011() { } virtual void v012() { } virtual void v013() { } virtual void Reset() { std::strcat(g_seq, "R"); } }; struct SNode { SNode* next; SNode* prev; IR* obj; };
struct COwner { unsigned char _pad[0x44]; IR* main; long m_48; SNode* list; void Prepare() { std::strcat(g_seq, "P"); } void ResetAll() { Prepare(); main->Reset(); for (SNode* n = list->next; n != list; n = n->next) n->obj->Reset(); } };

int main() {
    IR main, o1, o2; SNode head, n1, n2; head.next = &n1; n1.next = &n2; n2.next = &head; n1.obj = &o1; n2.obj = &o2; COwner* c = new COwner; c->main = &main; c->list = &head; c->ResetAll(); if (std::strcmp(g_seq, "PRRR")) { std::printf("00a3eb20_TEST FAIL\n"); return 1; } delete c;
    std::printf("00a3eb20_TEST PASS\n"); return 0;
}