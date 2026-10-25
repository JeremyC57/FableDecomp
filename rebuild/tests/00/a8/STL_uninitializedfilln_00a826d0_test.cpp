// Behaviour test for 0x00A826D0 std::__uninitialized_fill_n of list<C2DLineF>: n lists are copy-constructed from x
// into raw storage (each with its own sentinel and its own copies of x's lines), the end is returned, n == 0 builds none.
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
// the function under test (same shape as the source)
static SList* FillN(SList* first, unsigned int n, const SList& x)
{
    SList* cur = first;
    for (; n > 0; --n, ++cur)
        if (cur) CopyConstruct(cur, x);
    return cur;
}
int main()
{
    SList x; SNode* s = (SNode*)Alloc(); s->next = s; s->prev = s; x.sentinel = s;
    float v[2] = { 3.0f, 9.0f };
    PushBack(&x, v[0]); PushBack(&x, v[1]);
    int before = g_live;
    SList raw[4];
    if (FillN(raw, 0, x) != raw || g_live != before) { std::printf("00a826d0_TEST FAIL\n"); return 1; }
    if (FillN(raw, 4, x) != raw + 4 || g_live != before + 4 * 3) { std::printf("00a826d0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (!Holds(raw[i], v, 2) || raw[i].sentinel == x.sentinel || (i && raw[i].sentinel == raw[i - 1].sentinel)) { std::printf("00a826d0_TEST FAIL\n"); return 1; }
    std::printf("00a826d0_TEST PASS\n");
    return 0;
}