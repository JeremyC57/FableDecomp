// Behaviour test for 0x007119C0 list<CAnimSet>::_M_insert_dispatch(pos, first, last, __false_type): copies of [first, last)
// inserted before pos, in order, through the CAnimSet copy constructor.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
class CAnimSet { public:
    int Priority;
    unsigned char rest[0x50];
    CAnimSet(int p) : Priority(p) {}
    CAnimSet(const CAnimSet& o) : Priority(o.Priority) { ++g_copies; }
};
struct SNodeBase { SNodeBase* next; SNodeBase* prev; };
struct SNode : SNodeBase { CAnimSet data; };

static SNodeBase* MakeList()
{
    SNodeBase* s = (SNodeBase*)std::malloc(sizeof(SNode));
    s->next = s; s->prev = s;
    return s;
}
static void InsertDispatch(SNodeBase* pos, SNodeBase* first, SNodeBase* last)
{
    for (; first != last; first = first->next) {
        SNode* tmp = (SNode*)std::malloc(sizeof(SNode));
        new (&tmp->data) CAnimSet(((SNode*)first)->data);
        SNodeBase* prev = pos->prev;
        tmp->next = pos; tmp->prev = prev; prev->next = tmp; pos->prev = tmp;
    }
}
static void PushBack(SNodeBase* list, int p)
{
    SNode* tmp = (SNode*)std::malloc(sizeof(SNode));
    new (&tmp->data) CAnimSet(p);
    SNodeBase* prev = list->prev;
    tmp->next = list; tmp->prev = prev; prev->next = tmp; list->prev = tmp;
}

int main()
{
    SNodeBase* src = MakeList();
    PushBack(src, 1); PushBack(src, 2); PushBack(src, 3);
    SNodeBase* dst = MakeList();
    PushBack(dst, 10); PushBack(dst, 20);
    InsertDispatch(dst->next->next, src->next, src);    // before the 20
    int expect[] = { 10, 1, 2, 3, 20 };
    bool ok = g_copies == 3;
    int i = 0;
    for (SNodeBase* n = dst->next; n != dst; n = n->next, ++i)
        ok = ok && i < 5 && ((SNode*)n)->data.Priority == expect[i] && n->next->prev == n;
    ok = ok && i == 5;
    InsertDispatch(dst, src, src);                       // empty range: nothing copied
    ok = ok && g_copies == 3;
    if (!ok) { std::printf("007119c0_TEST FAIL\n"); return 1; }
    std::printf("007119c0_TEST PASS\n");
    return 0;
}