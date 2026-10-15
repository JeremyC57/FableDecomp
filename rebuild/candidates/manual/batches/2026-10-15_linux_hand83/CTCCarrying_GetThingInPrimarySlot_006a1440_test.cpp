// Behaviour test for 006a1440 (CTCCarrying::GetThingInPrimarySlot): a 0x4000000-flagged thing wins (unless its +0x91 bit 0 is set, then the first plain thing so far); otherwise the first live thing.
#include <cstdio>

struct CThing {
    unsigned long m_flags;
    unsigned char m_flags91;
};

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};

struct SThingListNode {
    SThingListNode*         _M_next;
    SThingListNode*         _M_prev;
    CBaseIntelligentPointer _M_data;
};

struct CThingList { SThingListNode* _M_node; };

struct CTCCarrying {
    CThingList ThingsInPrimarySlots;
    CThing* GetThingInPrimarySlot()
    {
        CThing* result = 0;
        for (SThingListNode* node = ThingsInPrimarySlots._M_node->_M_next; node != ThingsInPrimarySlots._M_node; node = node->_M_next) {
            CThing* thing = node->_M_data.GetPItem();
            if (thing) {
                if (thing->m_flags & 0x4000000) {
                    if (thing->m_flags91 & 1)
                        return result;
                    return thing;
                }
                if (!result)
                    result = thing;
            }
        }
        return result;
    }
};

int main()
{
    CThing plain1 = { 0, 0 }, plain2 = { 0, 0 }, flagged = { 0x4000000, 0 };
    SThingListNode head, n0, n1, n2, n3;
    head._M_next = &n0; n0._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &head;
    n0._M_data.m_pItem = 0; n1._M_data.m_pItem = &plain1; n2._M_data.m_pItem = &plain2; n3._M_data.m_pItem = &flagged;
    CTCCarrying c; c.ThingsInPrimarySlots._M_node = &head;
    bool ok = c.GetThingInPrimarySlot() == &flagged;
    flagged.m_flags91 = 1;
    ok = ok && c.GetThingInPrimarySlot() == &plain1;
    n3._M_data.m_pItem = 0;
    ok = ok && c.GetThingInPrimarySlot() == &plain1;
    head._M_next = &head;
    ok = ok && c.GetThingInPrimarySlot() == 0;
    if (!ok) { std::printf("006a1440_TEST FAIL\n"); return 1; }
    std::printf("006a1440_TEST PASS\n");
    return 0;
}
