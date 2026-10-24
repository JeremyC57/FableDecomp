// Behaviour test for 0x00A825D0 pair<unsigned __int64, list<C2DLineF> > copy constructor: the copy holds the same
// 64-bit key and its own list (fresh sentinel, new nodes) with the source's lines in order; the source is unchanged.
#include <cstdio>
#include <cstdlib>
struct SLine { float x0, y0, x1, y1; };
struct SNode { SNode* next; SNode* prev; SLine data; };
struct SList { SNode* sentinel; };
static int g_live = 0;
static void* Alloc() { ++g_live; return std::malloc(sizeof(SNode)); }
static void Free(void* p) { --g_live; std::free(p); }
static void InsertRange(SNode* pos, const SNode* first, const SNode* last)   // the out-of-line _M_insert_dispatch
{
    for (; first != last; first = first->next) {
        SNode* tmp = (SNode*)Alloc();
        tmp->data = first->data;
        SNode* p = pos->prev;
        tmp->next = pos; tmp->prev = p; p->next = tmp; pos->prev = tmp;
    }
}
static void CopyConstruct(SList* self, const SList& x)                       // list's copy ctor as inlined
{
    self->sentinel = 0;
    SNode* n = (SNode*)Alloc();
    n->next = n; n->prev = n;
    self->sentinel = n;
    InsertRange(self->sentinel->next, x.sentinel->next, x.sentinel);
}
static void Destroy(SList* self)                                             // ~_List_base as inlined
{
    SNode* cur = self->sentinel->next;
    while (cur != self->sentinel) { SNode* tmp = cur; cur = cur->next; Free(tmp); }
    self->sentinel->next = self->sentinel;
    self->sentinel->prev = self->sentinel;
    if (self->sentinel) Free(self->sentinel);
}
static void PushBack(SList* self, float v)
{
    SNode* tmp = (SNode*)Alloc();
    tmp->data.x0 = v; tmp->data.y0 = v + 1; tmp->data.x1 = v + 2; tmp->data.y1 = v + 3;
    SNode* p = self->sentinel->prev;
    tmp->next = self->sentinel; tmp->prev = p; p->next = tmp; self->sentinel->prev = tmp;
}
static bool Holds(const SList& l, const float* v, int n)
{
    const SNode* c = l.sentinel->next;
    for (int i = 0; i < n; ++i, c = c->next) {
        if (c == l.sentinel || c->data.x0 != v[i] || c->data.y1 != v[i] + 3 || c->next->prev != c) return false;
    }
    return c == l.sentinel && l.sentinel->prev->next == l.sentinel;
}
struct SEntry { unsigned long long first; SList second; };
// the function under test (same shape as the source)
static void CopyEntry(SEntry* self, const SEntry& o) { self->first = o.first; CopyConstruct(&self->second, o.second); }
int main()
{
    SEntry a; a.first = 0x123456789ULL;
    SNode* s = (SNode*)Alloc(); s->next = s; s->prev = s; a.second.sentinel = s;
    float v[3] = { 1.0f, 5.0f, -2.0f };
    for (int i = 0; i < 3; ++i) PushBack(&a.second, v[i]);
    SEntry b;
    CopyEntry(&b, a);
    if (b.first != 0x123456789ULL || !Holds(b.second, v, 3) || !Holds(a.second, v, 3) || b.second.sentinel == a.second.sentinel
        || b.second.sentinel->next == a.second.sentinel->next) { std::printf("00a825d0_TEST FAIL\n"); return 1; }
    std::printf("00a825d0_TEST PASS\n");
    return 0;
}