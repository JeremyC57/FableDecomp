// Behaviour test for 0x006A0E80 CCreatureAction_UnsheatheItemFromInventory copy constructor: the intelligent pointer is
// re-pointed at the same item, the counted follow-up pointer shares the object and bumps its count (null count untouched).
#include <cstdio>
struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    void* GetPItem() const { return m_pItem; }
    void SetPItem(void* p) { m_pItem = p; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() {}
    CIntelligentPointer(const CIntelligentPointer& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
};
struct CCountedPointer {
    void* Object; int* RefCount;
    CCountedPointer() : Object(0), RefCount(0) {}
    CCountedPointer(const CCountedPointer& o) : Object(o.Object), RefCount(o.RefCount) { if (RefCount) ++*RefCount; }
};
struct CBase { int m_base; CBase() : m_base(7) {} CBase(const CBase& o) : m_base(o.m_base) {} };
struct CUnsheathe : CBase {
    CIntelligentPointer PUnsheatheItem;
    CCountedPointer PFollowUpAction;
    CUnsheathe() {}
    CUnsheathe(const CUnsheathe& o) : CBase(o), PUnsheatheItem(o.PUnsheatheItem), PFollowUpAction(o.PFollowUpAction) {}
};
int main()
{
    int item = 0, action = 0, count = 1;
    CUnsheathe a; a.m_base = 3; a.PUnsheatheItem.SetPItem(&item); a.PFollowUpAction.Object = &action; a.PFollowUpAction.RefCount = &count;
    CUnsheathe b(a);
    if (b.m_base != 3 || b.PUnsheatheItem.GetPItem() != &item || b.PFollowUpAction.Object != &action || count != 2) { std::printf("006a0e80_TEST FAIL\n"); return 1; }
    CUnsheathe c; CUnsheathe d(c);
    if (d.PFollowUpAction.RefCount != 0 || d.PUnsheatheItem.GetPItem() != 0) { std::printf("006a0e80_TEST FAIL\n"); return 1; }
    std::printf("006a0e80_TEST PASS\n");
    return 0;
}