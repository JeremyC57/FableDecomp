#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_mode = 0; struct CBase { virtual void v00() { } unsigned char _body[0x18]; CBase(long mode) { g_mode = mode; } }; struct IProv { virtual void v0() { } virtual void v1() { } virtual long GetValue() { return 42; } }; struct CAlloc { CAlloc() { } };
template<unsigned int N> struct CRetailList { struct node { node* next; node* prev; unsigned char value[N - 8]; }; node* head; CRetailList(CAlloc) : head(0) { node* s = (node*)std::malloc(sizeof(node)); s->next = s; s->prev = s; head = s; } ~CRetailList() { std::free(head); } };
struct CMgr : CBase { void* m_pOwner; long m_capacity; CRetailList<0x28> m_maps; long m_28; CRetailList<0x10> m_navigators; CMgr(void*, void* owner, IProv* p) : CBase(5), m_pOwner(owner), m_capacity(800), m_maps(CAlloc()), m_navigators(CAlloc()) { m_28 = p->GetValue(); } };

int main() {
    IProv p; int owner; CMgr* m = new CMgr(0, &owner, &p); if (g_mode != 5 || m->m_pOwner != &owner || m->m_capacity != 800 || m->m_28 != 42 || !m->m_maps.head || m->m_maps.head->next != m->m_maps.head || m->m_navigators.head->prev != m->m_navigators.head) { std::printf("00a15670_TEST FAIL\n"); return 1; } delete m;
    std::printf("00a15670_TEST PASS\n"); return 0;
}