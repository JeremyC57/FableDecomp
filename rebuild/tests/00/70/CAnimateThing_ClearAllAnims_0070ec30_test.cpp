// Behaviour test for 0x0070EC30 CAnimateThing::ClearAllAnims(): main set cleared, both anim lists and the delayed queue
// emptied, the queue refilled with exactly three empty delayed animations, the current delayed animation emptied.
#include <cstdio>
#include <cstdlib>

struct SNodeBase { SNodeBase* next; SNodeBase* prev; };
static SNodeBase* MakeList() { SNodeBase* s = (SNodeBase*)std::malloc(0x5C); s->next = s; s->prev = s; return s; }
static void Link(SNodeBase* list, SNodeBase* n) { SNodeBase* prev = list->prev; n->next = list; n->prev = prev; prev->next = n; list->prev = n; }
static void ClearList(SNodeBase* list)
{
    for (SNodeBase* n = list->next; n != list; ) { SNodeBase* v = n; n = n->next; std::free(v); }
    list->next = list; list->prev = list;
}
static unsigned int Size(SNodeBase* list) { unsigned int c = 0; for (SNodeBase* n = list->next; n != list; n = n->next) ++c; return c; }

struct SDelayedNode : SNodeBase { SNodeBase* anims; };
class CAnimateThing { public:
    int MainAnimCleared;
    SNodeBase* Transition;
    SNodeBase* Combination;
    SNodeBase* DelayedQueue;
    SNodeBase* DelayedAnim;
    void ClearAllAnims()
    {
        MainAnimCleared = 1;
        ClearList(Transition);
        ClearList(Combination);
        for (SNodeBase* n = DelayedQueue->next; n != DelayedQueue; ) {
            SNodeBase* v = n; n = n->next;
            ClearList(((SDelayedNode*)v)->anims); std::free(((SDelayedNode*)v)->anims); std::free(v);
        }
        DelayedQueue->next = DelayedQueue; DelayedQueue->prev = DelayedQueue;
        while (Size(DelayedQueue) != 3) {
            SNodeBase* temp = MakeList();
            SDelayedNode* node = (SDelayedNode*)std::malloc(sizeof(SDelayedNode));
            node->anims = MakeList();
            Link(DelayedQueue, node);
            ClearList(temp); std::free(temp);
        }
        ClearList(DelayedAnim);
    }
};

int main()
{
    CAnimateThing t;
    t.MainAnimCleared = 0;
    t.Transition = MakeList(); Link(t.Transition, (SNodeBase*)std::malloc(0x5C));
    t.Combination = MakeList(); Link(t.Combination, (SNodeBase*)std::malloc(0x5C)); Link(t.Combination, (SNodeBase*)std::malloc(0x5C));
    t.DelayedQueue = MakeList();
    SDelayedNode* old = (SDelayedNode*)std::malloc(sizeof(SDelayedNode)); old->anims = MakeList(); Link(old->anims, (SNodeBase*)std::malloc(0x5C));
    Link(t.DelayedQueue, old);
    t.DelayedAnim = MakeList(); Link(t.DelayedAnim, (SNodeBase*)std::malloc(0x5C));
    t.ClearAllAnims();
    bool ok = t.MainAnimCleared == 1 && Size(t.Transition) == 0 && Size(t.Combination) == 0 && Size(t.DelayedQueue) == 3
        && Size(t.DelayedAnim) == 0;
    for (SNodeBase* n = t.DelayedQueue->next; n != t.DelayedQueue; n = n->next)
        ok = ok && Size(((SDelayedNode*)n)->anims) == 0;
    t.ClearAllAnims();                                    // idempotent: still three
    ok = ok && Size(t.DelayedQueue) == 3;
    if (!ok) { std::printf("0070ec30_TEST FAIL\n"); return 1; }
    std::printf("0070ec30_TEST PASS\n");
    return 0;
}