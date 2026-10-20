// Behaviour test for CTCActionUseBase::IsThisTheft (0x007EACB0).
#include <cstdio>

class CTCBase {};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
    const SEntry* LowerBound(const long* type) const { const SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_stateFlags;
    CInterfaceMap::SEntry entries[8];
    bool HasTC(long t) const { if (m_presentBits[t >> 5] & (1UL << (t & 31))) return true; return false; }
    template <class T> T* GetTC(long t)
    {
        long type = t; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || t < it->type) ? map->last : it)->component;
    }
    template<class T> bool GetInterface(T** ppOut, long t) const
    {
        if (m_presentBits[t >> 5] & (1UL << (t & 31))) {
            long type = t; const CInterfaceMap* map = &m_interfaces; const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || t < it->type) it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void Init() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_interfaces.last = entries; m_stateFlags = 0; }
    void Add(long t, CTCBase* c)   // types added in increasing order
    {
        m_presentBits[t >> 5] |= 1UL << (t & 31);
        m_interfaces.last->type = t; m_interfaces.last->component = c; ++m_interfaces.last;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CTCOwnedEntity : CTCBase { CBaseIntelligentPointer POwner; };
struct CTCVillage {};
struct CTCVillageMember : CTCBase { CTCVillage* village; CTCVillage* GetPVillage() const { return village; } };
struct CTCFreeUseMarker : CTCBase { bool m_freeToUse; };
struct CTCActionUseBase {
    const CThing* ParentThing;
    bool IsThisTheft() const
    {
        CTCOwnedEntity* owned;
        if (ParentThing->GetInterface(&owned, 0x33)) {
            CBaseIntelligentPointer& owner = owned->POwner;
            CThing* thing = owner.GetPItem();
            if (thing && !(thing->m_stateFlags & 1)) {
                thing = owner.GetPItem();
                if (thing->HasTC(0x23) && thing->GetTC<CTCVillageMember>(0x23)->GetPVillage()) {
                    if (owner.GetPItem()->HasTC(0xbd) || owner.GetPItem()->HasTC(0x53)) {
                        thing = owner.GetPItem();
                        return !(thing->HasTC(0xd5) && thing->GetTC<CTCFreeUseMarker>(0xd5)->m_freeToUse);
                    }
                }
            }
        }
        return false;
    }
};

int main()
{
    int ok = 1;
    CTCVillage village;
    CTCVillageMember member; member.village = &village;
    CTCBase house, shop;
    CTCFreeUseMarker marker; marker.m_freeToUse = false;
    CThing owner; owner.Init(); owner.Add(0x23, &member); owner.Add(0x53, &shop); owner.Add(0xd5, &marker);
    CTCOwnedEntity owned; owned.POwner.m_pItem = &owner;
    CThing item; item.Init(); item.Add(0x33, &owned);
    CTCActionUseBase use; use.ParentThing = &item;
    if (!use.IsThisTheft()) ok = 0;                    // shop owner in a village: theft
    marker.m_freeToUse = true;
    if (use.IsThisTheft()) ok = 0;                     // marked free to use
    marker.m_freeToUse = false;
    owner.m_stateFlags = 1;
    if (use.IsThisTheft()) ok = 0;                     // dead owner
    owner.m_stateFlags = 0;
    member.village = 0;
    if (use.IsThisTheft()) ok = 0;                     // not in a village
    member.village = &village;
    CThing plain; plain.Init(); plain.Add(0x23, &member);
    owned.POwner.m_pItem = &plain;
    if (use.IsThisTheft()) ok = 0;                     // neither house nor shop
    CThing home; home.Init(); home.Add(0x23, &member); home.Add(0xbd, &house);
    owned.POwner.m_pItem = &home;
    if (!use.IsThisTheft()) ok = 0;                    // a house without a marker
    owned.POwner.m_pItem = 0;
    if (use.IsThisTheft()) ok = 0;                     // no owner
    CThing unowned; unowned.Init();
    use.ParentThing = &unowned;
    if (use.IsThisTheft()) ok = 0;                     // not an owned entity
    if (!ok) { std::printf("007eacb0_TEST FAIL\n"); return 1; }
    std::printf("007eacb0_TEST PASS\n");
    return 0;
}