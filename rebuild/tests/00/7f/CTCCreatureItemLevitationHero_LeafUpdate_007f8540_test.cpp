// Test for 007f8540 CTCCreatureItemLevitationHero::LeafUpdate: pop the oldest item when over the limit or 30 frames stale.
#include <cstdio>
#include <stdlib.h>
struct SThingListNode;
class CThing;
struct SThingListNode { SThingListNode* Next; SThingListNode* Prev; CThing* Data; };
struct CThingList {
    SThingListNode* Header;
    SThingListNode* begin() const { return Header->Next; }
    SThingListNode* end() const { return Header; }
    void init() { Header = (SThingListNode*)malloc(sizeof(SThingListNode)); Header->Next = Header->Prev = Header; }
    void push_back(CThing* t)
    {
        SThingListNode* n = (SThingListNode*)malloc(sizeof(SThingListNode));
        n->Data = t; n->Next = Header; n->Prev = Header->Prev; Header->Prev->Next = n; Header->Prev = n;
    }
    void erase(SThingListNode* node)
    {
        SThingListNode* next = node->Next;
        SThingListNode* prev = node->Prev;
        prev->Next = next;
        next->Prev = prev;
        free(node);
    }
    int size() const { int n = 0; for (SThingListNode* p = Header->Next; p != Header; p = p->Next) ++n; return n; }
};
class CThing { public: int KillCalls; void Kill(bool) { ++KillCalls; } };
static long g_frame = 0;
long GetWorldFrame() { return g_frame; }
class CTCCreatureItemLevitationBase { public:
    CThingList LevitatingItems;
    long NumItems;
    long LastAddFrame;
    long MaxItems;
    virtual ~CTCCreatureItemLevitationBase() {}
    virtual long GetMaxNumItems() { return MaxItems; }
    virtual void LeafUpdate() {}
    bool HasTimedOut() { long lastAdd = LastAddFrame; return GetWorldFrame() > lastAdd + 30; }
    void PopItem()
    {
        SThingListNode* front = LevitatingItems.begin();
        front->Data->Kill(true);
        LevitatingItems.erase(front);
        --NumItems;
    }
};
struct CTCCreatureItemLevitationHero : CTCCreatureItemLevitationBase {
    virtual void LeafUpdate()
    {
        long numItems = NumItems;
        if (numItems > 0) {
            if (numItems > GetMaxNumItems() || HasTimedOut())
                PopItem();
        }
    }
};
int main()
{
    CThing t[3] = { { 0 }, { 0 }, { 0 } };
    CTCCreatureItemLevitationHero h; h.LevitatingItems.init(); h.NumItems = 0; h.LastAddFrame = 100; h.MaxItems = 2;
    CTCCreatureItemLevitationBase* b = &h;
    g_frame = 200; b->LeafUpdate();                         // empty: nothing
    for (int i = 0; i < 3; ++i) { h.LevitatingItems.push_back(&t[i]); ++h.NumItems; }
    g_frame = 110; b->LeafUpdate();                         // 3 > 2: pop t[0]
    if (t[0].KillCalls != 1 || h.NumItems != 2) { std::printf("007f8540_TEST FAIL\n"); return 1; }
    b->LeafUpdate();                                        // 2 items, frame 110 <= 130: nothing
    g_frame = 130; b->LeafUpdate();                         // 130 > 130 is false: nothing
    if (t[1].KillCalls != 0 || h.NumItems != 2) { std::printf("007f8540_TEST FAIL\n"); return 1; }
    g_frame = 131; b->LeafUpdate();                         // timed out: pop t[1]
    if (t[1].KillCalls != 1 || h.NumItems != 1 || t[2].KillCalls != 0) { std::printf("007f8540_TEST FAIL\n"); return 1; }
    std::printf("007f8540_TEST PASS\n");
    return 0;
}