// Grade: DIFFER(236v241) -- register map: retail spills `this` to [ebp-8] and keeps the parent in esi / events in ebx / the type constant in edi (one ParentThing load); ours keeps this in edi and reloads ParentThing for GetTC. Tried: zero-preloaded GetInterface (243v241, events in memory), named parent local (same).
// NInventory::CTCInventoryBase::OnItemAdded(long itemDefIndex, bool unused, bool suppressTutorials) -- queues a "received item" entity event on the owner and, unless suppressed, gives the hero the tutorials the new item's kind triggers
// Retail 0x005BE028, 241 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (two inline CThing::GetInterface lookups:
// the entity events component 0x42 and the hero component 0x29), operator new 0x00BFEA1A, CTCEntityEvents::AddEvent
// (CEventBase*) 0x008113C0, the item-kind tests 0x005D9830 (manifest "IsAugmentation") and 0x005D98B0 (same manifest
// label; a second sub-def test, named from use), GetInventoryCategory(long) 0x005D9B70 (all three __fastcall on the def
// index), CTCHero::GiveTutorialForCategory(long, bool) 0x006C9FA0.
// Shape: the events component starts at 0 (`xor ebx,ebx`) and is set only when present (`T* p = 0; if (HasTC) p =
// GetTC`); the event is `new NEntityEvents::CEventReceivedItemInInventory(def)` (12 bytes, vtable 0x01253C8C) whose
// `__declspec(novtable)` CEventBase zeroes WorldFrame before the derived vtable store; the hero lookup key homes in the
// dead `suppressTutorials` slot; tutorials 3 (non-augmentation), 0x20 and the item's inventory category.
// Layout: ParentThing +0x04; CEventBase {vptr, WorldFrame +4}, ItemDefIndex +8 (Ego_r.pdb).
#pragma optimize("y", off)
#pragma optimize("s", on)

void* operator new(unsigned int size);             // 0x00BFEA1A
struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    bool HasTC(long wanted) { return m_present.test(wanted); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || wanted < it->type)
            it = map->last;
        return (T*)it->component;
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
struct CTCEntityEvents {                             // interface type 0x42
    void AddEvent(CEventBase* event);                // 0x008113C0
};
struct CTCHero {                                     // interface type 0x29
    bool GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0
};
bool __fastcall IsAugmentation(long itemDefIndex);           // 0x005D9830
bool __fastcall IsTutorialSubDefItem(long itemDefIndex);     // 0x005D98B0
long __fastcall GetInventoryCategory(long itemDefIndex);     // 0x005D9B70

namespace NInventory {
struct CTCInventoryBase {
    void* __vftable;
    CThing* ParentThing;                             // +0x04
    void OnItemAdded(long itemDefIndex, bool unused, bool suppressTutorials);
};

void CTCInventoryBase::OnItemAdded(long itemDefIndex, bool unused, bool suppressTutorials)
{
    CTCEntityEvents* events = 0;
    if (ParentThing->HasTC(0x42))
        events = ParentThing->GetTC<CTCEntityEvents>(0x42);
    events->AddEvent(new NEntityEvents::CEventReceivedItemInInventory(itemDefIndex));
    if (!suppressTutorials) {
        CTCHero* hero;
        if (ParentThing->GetInterface(&hero, 0x29)) {
            if (!IsAugmentation(itemDefIndex))
                hero->GiveTutorialForCategory(3, false);
            if (IsTutorialSubDefItem(itemDefIndex))
                hero->GiveTutorialForCategory(0x20, false);
            long category = GetInventoryCategory(itemDefIndex);
            if (category)
                hero->GiveTutorialForCategory(category, false);
        }
    }
}
}
