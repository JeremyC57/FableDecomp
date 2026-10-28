// Behaviour test for 00947280: src/00947280.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CAIStateGroup_WanderAroundShops::IsRunnable()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_WanderAroundShops::IsRunnable() -- runnable in work hours when the idle scheduler (if any) prefers this group, a random shop can be picked and is alive, and a pretty thing (the shop itself when it is a shop component holder, else one found near it) is alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00947280, length 346 bytes (plain `ret`, virtual, bool).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 211 = bit 19 of the bitset dword at +0x38),
//          CTCIdleScheduler::IsPreferredIdle(CAIStateGroupBase*) @ 0x00801BE0, CAIStateGroup_WorkBase::IsInWorkHours
//          @ 0x0091A6F0, the shop picker @ 0x00946F90 (manifest "CAIStateGroup_WanderAroundShops_SelectRandomEnemy") and the
//          pretty-thing finder @ 0x00947110 (manifest "..._FindRandomNPC"), CBaseIntelligentPointer ctor 0x00A01B10 /
//          GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (vtable 0x01238C6C = CIntelligentPointer<CThing>).
// Shape: the scheduler lookup is the out-parameter GetInterface whose hit path alone calls IsPreferredIdle; both pointer
//          assignments from a raw CThing* go through the implicit CIntelligentPointer conversion temporary (ctor, SetPItem,
//          inline operator= naming the item: `CThing* item = other.GetPItem(); SetPItem(item);`, dtor); the same-type `PrettyThing = Shop` is the bare inline
//          operator=. IS_THING_ALIVE is the two-GetPItem inline test; the shop's type-144 test (bit 16 of +0x30) is a third
//          GetPItem. Separate `mov al,1` / `xor al,al` exits (nested success path).
// Layout: engine/CAIStateGroup_WanderAroundShops.h (Shop +0x14, PrettyThing +0x1c, NextFrameBuy +0x24) over the landed
//          CAIStateGroupBase model (the header flattens the vptr, so the base is modelled locally).
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIStateGroupBase;
class CTCIdleScheduler { public:                       // interface 211
    bool IsPreferredIdle(CAIStateGroupBase* group);   // 0x00801BE0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};
struct CAIStateGroup_WanderAroundShops : CAIStateGroupBase {
    CIntelligentPointerThing Shop;              // +0x14
    CIntelligentPointerThing PrettyThing;       // +0x1c
    long NextFrameBuy;                          // +0x24
    bool IsInWorkHours();                       // 0x0091A6F0
    CThing* SelectRandomShop();                 // 0x00946F90
    CThing* SelectPrettyThing(CThing* shop);    // 0x00947110
    virtual bool IsRunnable();
};

bool CAIStateGroup_WanderAroundShops::IsRunnable()
{
    CTCIdleScheduler* scheduler;
    if (PBrain->PCreature->GetInterface(&scheduler, 211)) {
        if (!scheduler->IsPreferredIdle(this))
            return false;
    }
    if (IsInWorkHours()) {
        Shop = SelectRandomShop();
        if (IS_THING_ALIVE(Shop)) {
            if (Shop.GetPItem()->HasTC(144))
                PrettyThing = Shop;
            else
                PrettyThing = SelectPrettyThing(Shop.GetPItem());
            if (IS_THING_ALIVE(PrettyThing))
                return true;
        }
    }
    return false;
}

// ---- stubs and driver ----
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerThing::v00() {}
CAIStateGroupBase::~CAIStateGroupBase() {}
static int g_lb = 0;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ ++g_lb; for (SEntry* it = first; it != last; ++it) if (!(it->type < *type)) return it; return last; }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
static bool g_preferred = true; static CAIStateGroupBase* g_asked = 0; static int g_idleCalls = 0;
bool CTCIdleScheduler::IsPreferredIdle(CAIStateGroupBase* group) { ++g_idleCalls; g_asked = group; return g_preferred; }
static bool g_work = true; static CThing* g_shop = 0; static CThing* g_pretty = 0; static CThing* g_prettyArg = 0;
bool CAIStateGroup_WanderAroundShops::IsInWorkHours() { return g_work; }
CThing* CAIStateGroup_WanderAroundShops::SelectRandomShop() { return g_shop; }
CThing* CAIStateGroup_WanderAroundShops::SelectPrettyThing(CThing* shop) { g_prettyArg = shop; return g_pretty; }
static void SetBit(CThing& t, long type) { t.m_presentBits[type >> 5] |= 1UL << (type & 31); }
int main()
{
    static CThing creature, shopA, shopB, pretty; static CTCIdleScheduler sched;
    static CInterfaceMap::SEntry entries[2];
    entries[0].type = 100; entries[0].component = 0; entries[1].type = 211; entries[1].component = (CTCBase*)&sched;
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    static CAIBrain brain; brain.PCreature = &creature;
    static unsigned char gStore[sizeof(CAIStateGroup_WanderAroundShops)];   // raw object: the members have no default ctor; IsRunnable is called non-virtually
    CAIStateGroup_WanderAroundShops& g = *(CAIStateGroup_WanderAroundShops*)gStore; g.PBrain = &brain;
    // no scheduler, out of work hours
    g_work = false; CHECK(g.CAIStateGroup_WanderAroundShops::IsRunnable() == false); CHECK(g_lb == 0);
    // in work hours, no shop
    g_work = true; g_shop = 0; CHECK(g.CAIStateGroup_WanderAroundShops::IsRunnable() == false); CHECK(g.Shop.m_pItem == 0);
    // dead shop
    shopA.m_flags91 = 1; g_shop = &shopA; CHECK(g.CAIStateGroup_WanderAroundShops::IsRunnable() == false); CHECK(g.Shop.m_pItem == &shopA);
    // live shop holding interface 144: the pretty thing is the shop itself
    shopA.m_flags91 = 0; SetBit(shopA, 144); g_prettyArg = 0;
    CHECK(g.CAIStateGroup_WanderAroundShops::IsRunnable() == true); CHECK(g.PrettyThing.m_pItem == &shopA); CHECK(g_prettyArg == 0);
    // live shop without it: the finder picks; a dead pick fails, a live pick succeeds
    g_shop = &shopB; pretty.m_flags91 = 1; g_pretty = &pretty;
    CHECK(g.CAIStateGroup_WanderAroundShops::IsRunnable() == false); CHECK(g_prettyArg == &shopB); CHECK(g.PrettyThing.m_pItem == &pretty);
    pretty.m_flags91 = 0; CHECK(g.CAIStateGroup_WanderAroundShops::IsRunnable() == true);
    g_pretty = 0; CHECK(g.CAIStateGroup_WanderAroundShops::IsRunnable() == false); CHECK(g.PrettyThing.m_pItem == 0);
    // scheduler present: consulted first; not preferred -> false without touching the shop
    SetBit(creature, 211); g_pretty = &pretty; g_preferred = false; g_shop = &shopA; g.Shop.m_pItem = 0;
    CHECK(g.CAIStateGroup_WanderAroundShops::IsRunnable() == false); CHECK(g_idleCalls == 1); CHECK(g_asked == &g); CHECK(g_lb == 1); CHECK(g.Shop.m_pItem == 0);
    g_preferred = true; CHECK(g.CAIStateGroup_WanderAroundShops::IsRunnable() == true); CHECK(g_idleCalls == 2); CHECK(g.Shop.m_pItem == &shopA);
    if (fails == 0) printf("00947280_TEST PASS\n"); else printf("00947280_TEST FAIL\n");
    return fails;
}