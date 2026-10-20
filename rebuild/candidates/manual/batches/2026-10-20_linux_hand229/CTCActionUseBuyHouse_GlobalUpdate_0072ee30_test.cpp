// Behaviour test for CTCActionUseBuyHouse::GlobalUpdate (0x0072EE30).
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
    template<class T> bool GetInterface(T** ppOut, long t)
    {
        if (m_presentBits[t >> 5] & (1UL << (t & 31))) {
            long type = t; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type);
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
struct CTCBuyableHouse : CTCBase { bool m_rentable; };
struct CMessageEvent { long m_answer; };
struct CMessageEventFilter_IsOfType { long Type; CMessageEventFilter_IsOfType(long t) : Type(t) {} };
static int g_removed = 0; static long g_filterType = 0;
struct CMessageEventManager {
    CMessageEvent* pending;
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& f) { g_filterType = f.Type; return pending; }
    void RemoveMessage(const CMessageEvent* m) { if (m == pending) { pending = 0; ++g_removed; } }
};
struct CWorld { CMessageEventManager* PMessageEventManager; };
static int g_confirmed = 0, g_rent = 0;
class CTCActionUseBase {
public:
    CThing* ParentThing;
    CWorld* m_world;
    CWorld* GetWorld() { return m_world; }
};
class CTCActionUseBuyHouse : public CTCActionUseBase {
public:
    bool WaitingForConfirmation;
    long HouseQuestion;
    void ReceivedConfirmation(CTCBuyableHouse*) { ++g_confirmed; }
    void OnActionUseRent(CTCBuyableHouse*) { ++g_rent; }
    void GlobalUpdate(long)
    {
        if (WaitingForConfirmation) {
            CMessageEventManager* messages = GetWorld()->PMessageEventManager;
            CMessageEvent* answer = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x10));
            if (answer) {
                CTCOwnedEntity* owned;
                if (ParentThing->GetInterface(&owned, 0x33)) {
                    CBaseIntelligentPointer& owner = owned->POwner;
                    CThing* thing = owner.GetPItem();
                    if (thing && !(thing->m_stateFlags & 1)) {
                        thing = owner.GetPItem();
                        if (thing->HasTC(0xd5)) {
                            CTCBuyableHouse* house = thing->GetTC<CTCBuyableHouse>(0xd5);
                            if (answer->m_answer == 1)
                                ReceivedConfirmation(house);
                            WaitingForConfirmation = false;
                            if (HouseQuestion == 1 && house->m_rentable)
                                OnActionUseRent(house);
                        }
                    }
                }
                messages->RemoveMessage(answer);
            }
        }
    }
};

int main()
{
    int ok = 1;
    CTCBuyableHouse buyable; buyable.m_rentable = true;
    CThing house; house.Init(); house.Add(0xd5, &buyable);
    CTCOwnedEntity owned; owned.POwner.m_pItem = &house;
    CThing sign; sign.Init(); sign.Add(0x33, &owned);
    CMessageEvent yes; yes.m_answer = 1;
    CMessageEventManager mgr; mgr.pending = 0;
    CWorld world; world.PMessageEventManager = &mgr;
    CTCActionUseBuyHouse use; use.ParentThing = &sign; use.m_world = &world; use.WaitingForConfirmation = true; use.HouseQuestion = 1;
    use.GlobalUpdate(0);                                   // no answer yet
    if (!use.WaitingForConfirmation || g_filterType != 0x10 || g_removed != 0) ok = 0;
    mgr.pending = &yes;
    use.GlobalUpdate(0);
    if (use.WaitingForConfirmation || g_confirmed != 1 || g_rent != 1 || g_removed != 1) ok = 0;
    CMessageEvent no; no.m_answer = 0;
    mgr.pending = &no; use.WaitingForConfirmation = true; use.HouseQuestion = 0;
    use.GlobalUpdate(0);
    if (use.WaitingForConfirmation || g_confirmed != 1 || g_rent != 1 || g_removed != 2) ok = 0;
    mgr.pending = &yes; use.WaitingForConfirmation = false;
    use.GlobalUpdate(0);                                   // not waiting: message left alone
    if (g_removed != 2 || mgr.pending != &yes) ok = 0;
    if (!ok) { std::printf("0072ee30_TEST FAIL\n"); return 1; }
    std::printf("0072ee30_TEST PASS\n");
    return 0;
}
