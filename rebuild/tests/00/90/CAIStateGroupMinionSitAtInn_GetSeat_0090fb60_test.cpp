// Behaviour test for 0090fb60: body copied from src/0090fb60.cpp (CAIStateGroup_MinionSitAtInn::GetSeat), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x45];
    unsigned char m_flags91;
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
static int g_setPItem = 0; static CThing* g_lastSet = 0;
struct CBaseIntelligentPointerCtor {
    virtual void v00() {}
    CThing* m_pItem;
    CBaseIntelligentPointerCtor() : m_pItem(0) {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { ++g_setPItem; g_lastSet = item; m_pItem = item; }
    ~CBaseIntelligentPointerCtor() {}
};
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing() {}
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
static int g_freeSeat = 0; static CThing* g_seatFor = 0; static void* g_tavernThis = 0; static CThing* g_seat = 0;
class CTCTavern { public: CThing* GetFreeSeat(CThing* creature) { ++g_freeSeat; g_seatFor = creature; g_tavernThis = this; return g_seat; } };
class CAIBrain { public: unsigned char _pad[0x18]; CThing* PCreature; };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() {}
    CAIBrain* PBrain; long ScriptAIPriority; void* Name_PStringData; unsigned char Terminated; unsigned char Priority; unsigned char Enabled; unsigned char _pad_0x13;
};
struct CThingF : CThing {
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CAIStateGroup_MinionSitAtInn : CAIStateGroupBase {
    CIntelligentPointerThing PSeat;
    virtual void GetSeat(CThing* owner);
};
void CAIStateGroup_MinionSitAtInn::GetSeat(CThing* owner)
{
    CTCTavern* tavern = 0;
    if (owner->HasTC(0xab))
        tavern = ((CThingF*)owner)->FindTC<CTCTavern>(0xab);
    CThing* seat = tavern->GetFreeSeat(PBrain->PCreature);
    PSeat = seat;
}
int main()
{
    CThing inn; memset(&inn, 0, sizeof(inn)); CTCTavern tavern; CThing creature; CThing seat;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0xab, (CTCBase*)&tavern } };
    inn.m_interfaces.first = entries; inn.m_interfaces.last = entries + 2;
    inn.m_presentBits[0xab >> 5] = 1UL << (0xab & 31);     // an inn always carries its tavern component (a missing one would be a null call)
    CAIBrain brain; brain.PCreature = &creature; CAIStateGroup_MinionSitAtInn g; g.PBrain = &brain;
    g_seat = &seat; g.GetSeat(&inn);
    CHECK(g_freeSeat == 1 && g_seatFor == &creature && g_tavernThis == &tavern);
    CHECK(g_setPItem == 2 && g_lastSet == &seat && g.PSeat.m_pItem == &seat);   // through a converting temporary, then the assignment
    g_seat = 0; g.GetSeat(&inn); CHECK(g_freeSeat == 2 && g.PSeat.m_pItem == 0);
    if (fails == 0) printf("0090fb60_TEST PASS\n"); else printf("0090fb60_TEST FAIL\n");
    return fails;
}