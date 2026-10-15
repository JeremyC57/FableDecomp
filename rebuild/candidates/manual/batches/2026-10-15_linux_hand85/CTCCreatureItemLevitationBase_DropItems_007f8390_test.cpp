// Test for 007f8390 CTCCreatureItemLevitationBase::DropItems: every item's physics re-activated, list emptied, count 0.
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
struct CTCPhysicsBase { int ActiveCalls; bool Active; void SetPhysicsActive(bool a) { ++ActiveCalls; Active = a; } };
struct CThing { CTCPhysicsBase* PhysicsTC; };
struct CTCCreatureItemLevitationBase {
    CThingList LevitatingItems;
    long NumItems;
    void DropItems()
    {
        for (SThingListNode* it = LevitatingItems.begin(); it != LevitatingItems.end(); it = it->Next)
            it->Data->PhysicsTC->SetPhysicsActive(true);
        SThingListNode* cur = LevitatingItems.Header->Next;
        while (cur != LevitatingItems.Header) {
            SThingListNode* tmp = cur;
            cur = cur->Next;
            free(tmp);
        }
        LevitatingItems.Header->Next = LevitatingItems.Header;
        LevitatingItems.Header->Prev = LevitatingItems.Header;
        NumItems = 0;
    }
};
int main()
{
    CTCPhysicsBase p[3] = { { 0, false }, { 0, false }, { 0, false } };
    CThing t[3] = { { &p[0] }, { &p[1] }, { &p[2] } };
    CTCCreatureItemLevitationBase l; l.LevitatingItems.init(); l.NumItems = 3;
    for (int i = 0; i < 3; ++i) l.LevitatingItems.push_back(&t[i]);
    l.DropItems();
    for (int i = 0; i < 3; ++i)
        if (p[i].ActiveCalls != 1 || !p[i].Active) { std::printf("007f8390_TEST FAIL\n"); return 1; }
    if (l.NumItems != 0 || l.LevitatingItems.size() != 0) { std::printf("007f8390_TEST FAIL\n"); return 1; }
    l.DropItems();                          // empty list: no calls
    if (p[0].ActiveCalls != 1) { std::printf("007f8390_TEST FAIL\n"); return 1; }
    std::printf("007f8390_TEST PASS\n");
    return 0;
}
