// Behaviour test for 0x00A80E90 list<CCountedPointer<T> >::_M_insert_dispatch(pos, first, last, __false_type): every
// counted pointer of [first, last) is copied (reference count bumped, null pointers skipped) into a new node linked in
// before pos, in source order; an empty range changes nothing.
#include <cstdio>
#include <cstdlib>

struct SCountBlock { long refs; };
struct SPtr { void* obj; SCountBlock* count; };
struct SNode { SNode* next; SNode* prev; SPtr data; };

static SNode* MakeList() { SNode* h = (SNode*)std::malloc(sizeof(SNode)); h->next = h; h->prev = h; return h; }
// the function under test (same shape as the source)
static void InsertRange(SNode* pos, SNode* first, SNode* last)
{
    for (; first != last; first = first->next) {
        SNode* tmp = (SNode*)std::malloc(sizeof(SNode));
        tmp->data.obj = first->data.obj;
        tmp->data.count = first->data.count;
        if (tmp->data.count) ++tmp->data.count->refs;
        SNode* n = pos;
        SNode* p = n->prev;
        tmp->next = n;
        tmp->prev = p;
        p->next = tmp;
        n->prev = tmp;
    }
}

int main()
{
    SCountBlock c[3] = { { 1 }, { 1 }, { 5 } };
    int objs[3] = { 10, 20, 30 };
    SNode* src = MakeList();
    for (int i = 0; i < 3; ++i) {
        SNode* tail = src;
        SPtr p; p.obj = &objs[i]; p.count = (i == 1) ? 0 : &c[i];
        SNode* one = MakeList(); one->data = p;
        InsertRange(tail, one, one); // empty range: nothing
        SNode* n = (SNode*)std::malloc(sizeof(SNode)); n->data = p;
        n->next = src; n->prev = src->prev; src->prev->next = n; src->prev = n;
        std::free(one);
    }
    SNode* dst = MakeList();
    SPtr x; x.obj = 0; x.count = 0;
    SNode* sentinel = (SNode*)std::malloc(sizeof(SNode)); sentinel->data = x;
    sentinel->next = dst; sentinel->prev = dst; dst->next = sentinel; dst->prev = sentinel;   // one existing node
    InsertRange(sentinel, src->next, src);                                                   // insert before it
    int order[4] = { 10, 20, 30, -1 };
    SNode* n = dst->next;
    for (int i = 0; i < 4; ++i, n = n->next) {
        int v = n->data.obj ? *(int*)n->data.obj : -1;
        if (v != order[i] || n->next->prev != n) { std::printf("00a80e90_TEST FAIL\n"); return 1; }
    }
    if (n != dst || c[0].refs != 2 || c[2].refs != 6 || c[1].refs != 1) { std::printf("00a80e90_TEST FAIL\n"); return 1; }
    std::printf("00a80e90_TEST PASS\n");
    return 0;
}