// NInventory::CTCInventoryBase::OnItemAdded(long itemDefIndex, bool unused, bool suppressTutorials) -- queues a "received item" entity event on the owner and, unless suppressed, gives the hero the tutorials the new item's kind triggers
// Retail 0x005BE028, 241 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (two inline CThing::GetInterface lookups:
// the entity events component 0x42 and the hero component 0x29), operator new 0x00BFEA1A, CTCEntityEvents::AddEvent
// (CEventBase*) 0x008113C0, the item-kind tests 0x005D9830 (manifest "IsAugmentation") and 0x005D98B0 (same manifest
// label; a second sub-def test, named from use), GetInventoryCategory(long) 0x005D9B70 (all three __fastcall on the def
// index), CTCHero::GiveTutorialForCategory(long, bool) 0x006C9FA0.
// Shape: the events component starts at 0 (`xor ebx,ebx`) and is set only when present: an inline QueryTC<T> whose
// `T* p = 0; if (test) p = <ternary end/hit pick>->component;` keeps p in ebx (so `this` is the one spilled, [ebp-8])
// and loads ParentThing once (an if-assign pick, a ternary-returning QueryTC or the out-parameter GetInterface put p in
// the key's slot or born after the test instead); the event is `new NEntityEvents::CEventReceivedItemInInventory(def)` (12 bytes, vtable 0x01253C8C) whose
// `__declspec(novtable)` CEventBase zeroes WorldFrame before the derived vtable store; the hero lookup key homes in the
// dead `suppressTutorials` slot; tutorials 3 (non-augmentation), 0x20 and the item's inventory category.
// Layout: ParentThing +0x04; CEventBase {vptr, WorldFrame +4}, ItemDefIndex +8 (Ego_r.pdb).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || wanted < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline T* QueryTC(long wanted)
    {
        T* p = 0;
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            p = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
        }
        return p;
    }
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            *out = GetTC<T>(wanted);
            return true;
        }
        return false;
    }
};

struct __declspec(novtable) CEventBase {
    virtual void v00();
    long WorldFrame;                                 // +0x04
    CEventBase() : WorldFrame(0) {}
};
namespace NEntityEvents {
struct CEventReceivedItemInInventory : CEventBase {  // vtable 0x01253C8C
    long ItemDefIndex;                               // +0x08
    CEventReceivedItemInInventory(long itemDefIndex) : ItemDefIndex(itemDefIndex) {}
    virtual void v00();
};
}
class CTCEntityEvents { public:                             // interface type 0x42
    void AddEvent(CEventBase* event);                // 0x008113C0
};
class CTCHero { public:                                     // interface type 0x29
    bool GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0
};
bool __fastcall IsAugmentation_005d9830(long itemDefIndex);           // 0x005D9830
bool __fastcall IsTutorialSubDefItem(long itemDefIndex);     // 0x005D98B0
long __fastcall GetInventoryCategory_005d9b70(long itemDefIndex);     // 0x005D9B70

namespace NInventory {
class CTCInventoryBase { public:
    void* __vftable;
    CThing* ParentThing;                             // +0x04
    void OnItemAdded(long itemDefIndex, bool unused, bool suppressTutorials);
};

void CTCInventoryBase::OnItemAdded(long itemDefIndex, bool unused, bool suppressTutorials)
{
    CTCEntityEvents* events = ParentThing->QueryTC<CTCEntityEvents>(0x42);
    events->AddEvent(new NEntityEvents::CEventReceivedItemInInventory(itemDefIndex));
    if (!suppressTutorials) {
        CTCHero* hero;
        if (ParentThing->GetInterface(&hero, 0x29)) {
            if (!IsAugmentation_005d9830(itemDefIndex))
                hero->GiveTutorialForCategory(3, false);
            if (IsTutorialSubDefItem(itemDefIndex))
                hero->GiveTutorialForCategory(0x20, false);
            long category = GetInventoryCategory_005d9b70(itemDefIndex);
            if (category)
                hero->GiveTutorialForCategory(category, false);
        }
    }
}
}
// ---- harness ----
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static bool g_present[0x100];
bool CPresenceBits::test(long bit) { return g_present[bit]; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
void CEventBase::v00() { }
void NEntityEvents::CEventReceivedItemInInventory::v00() { }
static int g_events = 0; static long g_lastItem = -1; static const void* g_eventsThis = 0;
void CTCEntityEvents::AddEvent(CEventBase* event)
{
    ++g_events; g_eventsThis = this;
    g_lastItem = ((NEntityEvents::CEventReceivedItemInInventory*)event)->ItemDefIndex;
    if (event->WorldFrame != 0) g_lastItem = -2;
    delete (NEntityEvents::CEventReceivedItemInInventory*)event;
}
static int g_tuts = 0; static long g_tutList[8];
bool CTCHero::GiveTutorialForCategory(long category, bool force) { if (g_tuts < 8) g_tutList[g_tuts] = category; ++g_tuts; return !force; }
static bool g_aug = false, g_sub = false; static long g_cat = 0;
bool __fastcall IsAugmentation_005d9830(long) { return g_aug; }
bool __fastcall IsTutorialSubDefItem(long) { return g_sub; }
long __fastcall GetInventoryCategory_005d9b70(long) { return g_cat; }
static CThing g_owner;
static CTCEntityEvents g_eventsTC;
static CTCHero g_hero;
static CInterfaceMap::SEntry g_entries[2];
int main()
{
    g_entries[0].type = 0x29; g_entries[0].component = &g_hero;
    g_entries[1].type = 0x42; g_entries[1].component = &g_eventsTC;
    g_owner.m_interfaces.first = g_entries; g_owner.m_interfaces.last = g_entries + 2;
    g_present[0x29] = g_present[0x42] = true;
    NInventory::CTCInventoryBase inv;
    inv.__vftable = 0; inv.ParentThing = &g_owner;
    inv.OnItemAdded(77, false, true);                // suppressed: event only
    CHECK(g_events == 1 && g_lastItem == 77 && g_eventsThis == &g_eventsTC && g_tuts == 0);
    g_aug = false; g_sub = true; g_cat = 9;
    inv.OnItemAdded(5, false, false);                // all three tutorials
    CHECK(g_events == 2 && g_lastItem == 5 && g_tuts == 3 && g_tutList[0] == 3 && g_tutList[1] == 0x20 && g_tutList[2] == 9);
    g_tuts = 0; g_aug = true; g_sub = false; g_cat = 0;
    inv.OnItemAdded(6, false, false);                // augmentation, no sub-def, no category
    CHECK(g_events == 3 && g_tuts == 0);
    g_present[0x29] = false; g_aug = false; g_cat = 4;
    inv.OnItemAdded(7, false, false);                // not a hero: no tutorials
    CHECK(g_events == 4 && g_lastItem == 7 && g_tuts == 0);
    if (fails == 0) printf("005be028_TEST PASS\n");
    return fails;
}