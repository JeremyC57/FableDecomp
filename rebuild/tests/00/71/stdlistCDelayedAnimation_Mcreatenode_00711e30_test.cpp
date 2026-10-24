// Behaviour test for 0x00711E30 list<CDelayedAnimation>::_M_create_node(x): a 12-byte node whose payload is a fresh copy
// of x's anim-set list (own sentinel, same elements in order).
#include <cstdio>
#include <cstdlib>
#include <new>

struct SNodeBase { SNodeBase* next; SNodeBase* prev; };
struct SAnimNode : SNodeBase { int Priority; unsigned char rest[0x50]; };
struct CDelayedAnimation { SNodeBase* CombinationAnims; };
struct SDelayedNode : SNodeBase { CDelayedAnimation data; };

static SNodeBase* MakeList()
{
    SNodeBase* s = (SNodeBase*)std::malloc(sizeof(SAnimNode));
    s->next = s; s->prev = s;
    return s;
}
static void PushBack(SNodeBase* list, int p)
{
    SAnimNode* tmp = (SAnimNode*)std::malloc(sizeof(SAnimNode));
    tmp->Priority = p;
    SNodeBase* prev = list->prev;
    tmp->next = list; tmp->prev = prev; prev->next = tmp; list->prev = tmp;
}
static SDelayedNode* CreateNode(const CDelayedAnimation& x)
{
    SDelayedNode* p = (SDelayedNode*)std::malloc(sizeof(SDelayedNode));
    CDelayedAnimation* d = &p->data;
    d->CombinationAnims = 0;
    d->CombinationAnims = MakeList();
    for (SNodeBase* n = x.CombinationAnims->next; n != x.CombinationAnims; n = n->next)
        PushBack(d->CombinationAnims, ((SAnimNode*)n)->Priority);
    return p;
}

int main()
{
    CDelayedAnimation x; x.CombinationAnims = MakeList();
    PushBack(x.CombinationAnims, 7); PushBack(x.CombinationAnims, 8);
    SDelayedNode* node = CreateNode(x);
    SNodeBase* l = node->data.CombinationAnims;
    bool ok = sizeof(SDelayedNode) == 3 * sizeof(void*) && l != x.CombinationAnims && l->next != x.CombinationAnims->next
        && ((SAnimNode*)l->next)->Priority == 7 && ((SAnimNode*)l->next->next)->Priority == 8 && l->next->next->next == l
        && l->prev->prev->prev == l;
    CDelayedAnimation empty; empty.CombinationAnims = MakeList();
    SDelayedNode* node2 = CreateNode(empty);
    ok = ok && node2->data.CombinationAnims->next == node2->data.CombinationAnims;
    if (!ok) { std::printf("00711e30_TEST FAIL\n"); return 1; }
    std::printf("00711e30_TEST PASS\n");
    return 0;
}