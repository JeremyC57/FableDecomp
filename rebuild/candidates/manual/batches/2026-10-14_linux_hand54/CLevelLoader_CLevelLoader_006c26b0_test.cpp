#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_a = -1, g_size = -1; static float g_f = -1; struct CBaseLoader { virtual void v00() { } unsigned char _body[0x10]; CBaseLoader(long a, long s, float f) { g_a = a; g_size = s; g_f = f; } };
struct CAlloc { CAlloc() { } }; struct CList { struct node { node* next; node* prev; unsigned char value[8]; }; node* head; CList(CAlloc) : head(0) { node* s = (node*)std::malloc(sizeof(node)); s->next = s; s->prev = s; head = s; } ~CList() { std::free(head); } };
struct CLevelLoader : CBaseLoader { CList m_pending; void* m_18; void* m_1c; void* m_20; CLevelLoader(void* a, void* b, void* c) : CBaseLoader(0, 0xfa00, 0.1f), m_pending(CAlloc()), m_18(a), m_1c(b), m_20(c) { } };

int main() {
    CLevelLoader l((void*)1, (void*)2, (void*)3); if (g_a != 0 || g_size != 0xfa00 || g_f != 0.1f || l.m_pending.head->next != l.m_pending.head || l.m_pending.head->prev != l.m_pending.head || l.m_18 != (void*)1 || l.m_1c != (void*)2 || l.m_20 != (void*)3) { std::printf("006c26b0_TEST FAIL\n"); return 1; }
    std::printf("006c26b0_TEST PASS\n"); return 0;
}
