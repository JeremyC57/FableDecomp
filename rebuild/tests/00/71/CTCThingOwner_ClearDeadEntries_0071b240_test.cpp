#include <cstdio>
#include <cstdlib>
#include <new>
struct CThing { unsigned char _pad[0x91]; unsigned char m_flags; bool IsDead() const { return (m_flags & 1) != 0; } };
static int g_destroyed = 0, g_freed = 0;
struct CIntelligentPointer {
    CThing* m_pItem;
    virtual ~CIntelligentPointer() { ++g_destroyed; }
    CThing* GetPItem() const { return m_pItem; }
};
struct SThingListNode { SThingListNode* next; SThingListNode* prev; CIntelligentPointer value; };
static void FreeNode(void* p) { ++g_freed; std::free(p); }
struct CTCThingOwner {
    SThingListNode* ObjectList;
    void ClearDeadEntries();
};
static bool IsEntryDead(const CIntelligentPointer& p)
{
    if (p.GetPItem() != 0) { if (!p.GetPItem()->IsDead()) return false; }
    return true;
}
void CTCThingOwner::ClearDeadEntries()
{
    SThingListNode* n = ObjectList->next;
    while (n != ObjectList) {
        if (IsEntryDead(n->value)) {
            SThingListNode* prev = n->prev; SThingListNode* next = n->next;
            prev->next = next; next->prev = prev;
            n->value.~CIntelligentPointer();
            FreeNode(n);
            n = next;
        } else n = n->next;
    }
}
static SThingListNode* MakeNode(CThing* t)
{
    SThingListNode* n = (SThingListNode*)std::malloc(sizeof(SThingListNode));
    new (&n->value) CIntelligentPointer();
    n->value.m_pItem = t;
    return n;
}
int main()
{
    CThing alive, dead; alive.m_flags = 0; dead.m_flags = 1;
    SThingListNode head; head.next = head.prev = &head;
    CThing* items[5] = { &alive, 0, &dead, &alive, &dead };
    for (int i = 0; i < 5; ++i) {
        SThingListNode* n = MakeNode(items[i]);
        n->prev = head.prev; n->next = &head; head.prev->next = n; head.prev = n;
    }
    CTCThingOwner o; o.ObjectList = &head;
    o.ClearDeadEntries();
    int count = 0; bool ok = true;
    for (SThingListNode* n = head.next; n != &head; n = n->next) { ++count; if (n->value.m_pItem != &alive) ok = false; if (n->next->prev != n) ok = false; }
    if (!ok || count != 2 || g_destroyed != 3 || g_freed != 3) { std::printf("0071b240_TEST FAIL\n"); return 1; }
    std::printf("0071b240_TEST PASS\n");
    return 0;
}