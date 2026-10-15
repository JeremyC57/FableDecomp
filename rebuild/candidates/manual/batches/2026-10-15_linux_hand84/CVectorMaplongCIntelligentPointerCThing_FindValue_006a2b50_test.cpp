#include <cstdio>
struct CBaseObject { int id; };
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const { return m_pItem; }
};
struct SCarrySlot { long first; CBaseIntelligentPointer second; };
struct CVectorMap_LongThingPointer {
    SCarrySlot* First; SCarrySlot* Last;
    SCarrySlot* FindValue(const CBaseIntelligentPointer& value)
    {
        for (SCarrySlot* it = First; it != Last; ++it)
            if (it->second.GetPItem() == value.GetPItem())
                return it;
        return Last;
    }
};
int main()
{
    CBaseObject a = { 1 }, b = { 2 }, c = { 3 };
    SCarrySlot slots[3];
    slots[0].first = 10; slots[0].second.m_pItem = &a;
    slots[1].first = 20; slots[1].second.m_pItem = &b;
    slots[2].first = 30; slots[2].second.m_pItem = &b;
    CVectorMap_LongThingPointer map; map.First = slots; map.Last = slots + 3;
    CBaseIntelligentPointer pb; pb.m_pItem = &b;
    CBaseIntelligentPointer pc; pc.m_pItem = &c;
    CVectorMap_LongThingPointer empty; empty.First = slots; empty.Last = slots;
    if (map.FindValue(pb) != &slots[1] || map.FindValue(pc) != map.Last || empty.FindValue(pb) != slots) { std::printf("006a2b50_TEST FAIL\n"); return 1; }
    std::printf("006a2b50_TEST PASS\n");
    return 0;
}
