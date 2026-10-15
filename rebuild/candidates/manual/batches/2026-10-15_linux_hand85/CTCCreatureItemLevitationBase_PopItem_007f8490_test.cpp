// Test for 007f8490 CTCCreatureItemLevitationBase::PopItem: oldest item killed, its node unlinked, count decremented.
#include <cstdio>
#include <stdlib.h>
struct SThingListNode;
struct CThing;
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
struct CThing { int KillCalls; bool LastCheck; void Kill(bool c) { ++KillCalls; LastCheck = c; } };
struct CTCCreatureItemLevitationBase {
    CThingList LevitatingItems;
    long NumItems;
    void PopItem()
    {
        SThingListNode* front = LevitatingItems.begin();
        front->Data->Kill(true);
        LevitatingItems.erase(front);
        --NumItems;
    }
};
int main()
{
    CThing t[2] = { { 0, false }, { 0, false } };
    CTCCreatureItemLevitationBase l; l.LevitatingItems.init(); l.NumItems = 2;
    l.LevitatingItems.push_back(&t[0]); l.LevitatingItems.push_back(&t[1]);
    l.PopItem();
    if (t[0].KillCalls != 1 || !t[0].LastCheck || t[1].KillCalls != 0 || l.NumItems != 1 || l.LevitatingItems.size() != 1
        || l.LevitatingItems.begin()->Data != &t[1]) { std::printf("007f8490_TEST FAIL\n"); return 1; }
    l.PopItem();
    if (t[1].KillCalls != 1 || l.NumItems != 0 || l.LevitatingItems.size() != 0) { std::printf("007f8490_TEST FAIL\n"); return 1; }
    std::printf("007f8490_TEST PASS\n");
    return 0;
}
