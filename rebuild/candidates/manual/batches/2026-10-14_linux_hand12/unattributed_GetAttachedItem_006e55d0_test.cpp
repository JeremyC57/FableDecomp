// Behaviour test for GetAttachedItem @ 0x006e55d0: current slot -> slot's item index -> pointer array element -> GetPItem; -1 at either level gives null.
#include <cstdio>
struct CBaseObject { int id; };
struct CBaseIntelligentPointer { virtual void v0() { } CBaseObject* m_pItem; CBaseObject* GetPItem() const { return m_pItem; } };
struct SAttachmentSlot { long m_type; long m_itemIndex; long m_reserved8; long m_reservedC; };
struct CAttachmentHolder_006e55d0 {
    unsigned char _pad0[8]; CBaseIntelligentPointer* m_pItems; unsigned char _pad1[8]; SAttachmentSlot* m_pSlots; unsigned char _pad2[8]; long m_currentSlot;
    CBaseObject* GetAttachedItem() const
    {
        long slot = m_currentSlot;
        if (slot > -1) { long item = m_pSlots[slot].m_itemIndex; if (item > -1) return m_pItems[item].GetPItem(); }
        return 0;
    }
};
int main()
{
    CBaseObject objects[2] = { { 1 }, { 2 } };
    CBaseIntelligentPointer items[2]; items[0].m_pItem = &objects[0]; items[1].m_pItem = &objects[1];
    SAttachmentSlot slots[3]; slots[0].m_itemIndex = 1; slots[1].m_itemIndex = -1; slots[2].m_itemIndex = 0;
    CAttachmentHolder_006e55d0 h; h.m_pItems = items; h.m_pSlots = slots;
    h.m_currentSlot = 0; bool ok = h.GetAttachedItem() == &objects[1];
    h.m_currentSlot = 2; if (h.GetAttachedItem() != &objects[0]) ok = false;
    h.m_currentSlot = 1; if (h.GetAttachedItem() != 0) ok = false;
    h.m_currentSlot = -1; if (h.GetAttachedItem() != 0) ok = false;
    if (!ok) { std::printf("006e55d0_TEST FAIL\n"); return 1; }
    std::printf("006e55d0_TEST PASS\n");
    return 0;
}
