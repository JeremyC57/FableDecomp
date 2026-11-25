// Leaf: ?OnItemAdded@CTCInventoryBase@NInventory@@MAEXJ_N0@Z
// NInventory::CTCInventoryBase::OnItemAdded(long itemDefIndex, bool unused, bool suppressTutorials) -- queues a "received item" entity event on the owner and, unless suppressed, gives the hero the tutorials the new item's kind triggers
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail 0x005BE028, 241 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("s", on).
// Callees: std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (two inline CThing::GetInterface lookups:
// the entity events component 0x42 and the hero component 0x29), operator new 0x00BFEA1A, CTCEntityEvents::AddEvent
// (NEntityEvents::CEventBase*) 0x008113C0, the item-kind tests 0x005D9830 (manifest "IsAugmentation") and 0x005D98B0 (same manifest
// label; a second sub-def test, named from use), GetInventoryCategory(long) 0x005D9B70 (all three __fastcall on the def
// index), CTCHero::GiveTutorialForCategory(ETutorialCategory, bool) 0x006C9FA0.
// Shape: the events component starts at 0 (`xor ebx,ebx`) and is set only when present: an inline QueryTC<T> whose
// `T* p = 0; if (test) p = <ternary end/hit pick>->component;` keeps p in ebx (so `this` is the one spilled, [ebp-8])
// and loads ParentThing once (an if-assign pick, a ternary-returning QueryTC or the out-parameter GetInterface put p in
// the key's slot or born after the test instead); the event is `new NEntityEvents::CEventReceivedItemInInventory(def)` (12 bytes, vtable 0x01253C8C) whose
// `__declspec(novtable)` NEntityEvents::CEventBase zeroes WorldFrame before the derived vtable store; the hero lookup key homes in the
// dead `suppressTutorials` slot; tutorials 3 (non-augmentation), 0x20 and the item's inventory category.
// Layout: ParentThing +0x04; NEntityEvents::CEventBase {vptr, WorldFrame +4}, ItemDefIndex +8 (Ego_r.pdb).
#include "fable_compiler.h"
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
class CTCBase;
#include "engine/ETCInterfaceType.h"                        // NF64: the shared enum (was a local empty enum; CTCInventoryItem.h includes it)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> FABLE_FORCEINLINE T* GetTC(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || wanted < it->first)
            it = map->end();
        return (T*)((void*)it->second);
    }
    template<class T> FABLE_FORCEINLINE T* QueryTC(long wanted)
    {
        T* p = 0;
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            p = (T*)((void*)((it == map->end() || wanted < it->first) ? map->end() : it)->second);
        }
        return p;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            *out = GetTC<T>(wanted);
            return true;
        }
        return false;
    }
};

namespace NEntityEvents {
class __declspec(novtable) CEventBase { public:
    virtual void v00();
    long WorldFrame;                                 // +0x04
    CEventBase() : WorldFrame(0) {}
};
}
namespace NEntityEvents {
struct CEventReceivedItemInInventory : NEntityEvents::CEventBase {  // vtable 0x01253C8C
    long ItemDefIndex;                               // +0x08
    CEventReceivedItemInInventory(long itemDefIndex) : ItemDefIndex(itemDefIndex) {}
    virtual void v00();
};
}
class CTCEntityEvents { public:                             // interface type 0x42
    void AddEvent(NEntityEvents::CEventBase* event);                // 0x008113C0
};
#define FABLE_RETAIL_TUTORIAL_CATEGORIES_PASS26   // NF315: the shared ETutorialCategory enum (Ego_d TPI 0xCE58) for the PDB parameter type
#include "engine/ETutorialCategory.h"
class CTCHero { public:                                     // interface type 0x29
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);   // 0x006C9FA0 (NF315: ?GiveTutorialForCategory@CTCHero@@QAE_NW4ETutorialCategory@@_N@Z, Ego_r + FableWin)
};
class CWideString;                     // NF285: the KINDS block also declares GetDescription (CWideString by value)
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS   // NF285: 0x005D9830 is the static ?IsExpression@CTCInventoryItem@@SI_NJ@Z (Ego_r + FableWin; was the free stand-in IsAugmentation_005d9830)
bool __fastcall IsTutorialSubDefItem(long itemDefIndex);     // 0x005D98B0
// NF64 (round 7, q10650): the free stand-in `GetInventoryCategory_005d9b70` is the static __fastcall CTCInventoryItem::GetTutorialCategory (Ego_r.pdb / FableWin.pdb; the
//   NF59 self row defines it), declared by engine/CTCInventoryItem.h.
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_GETTUTORIALCATEGORY_STATIC
#include "engine/CTCInventoryItem.h"

namespace NInventory {
class CTCInventoryBase { public:
    virtual void _v00(); virtual void _v01(); virtual void _v02(); virtual void _v03(); virtual void _v04(); virtual void _v05();
    virtual void _v06(); virtual void _v07(); virtual void _v08(); virtual void _v09(); virtual void _v10(); virtual void _v11();
    virtual void _v12(); virtual void _v13(); virtual void _v14(); virtual void _v15(); virtual void _v16(); virtual void _v17();
    virtual void _v18(); virtual void _v19(); virtual void _v20(); virtual void _v21(); virtual void _v22(); virtual void _v23();
    virtual void _v24(); virtual void _v25(); virtual void _v26(); virtual void _v27(); virtual void _v28(); virtual void _v29();
    virtual void _v30(); virtual void _v31(); virtual void _v32(); virtual void _v33(); virtual void _v34(); virtual void _v35();
    virtual void _v36(); virtual void _v37(); virtual void _v38(); virtual void _v39(); virtual void _v40(); virtual void _v41();
    virtual void _v42(); virtual void _v43(); virtual void _v44(); virtual void _v45(); virtual void _v46(); virtual void _v47();
    virtual void _v48(); virtual void _v49(); virtual void _v50(); virtual void _v51(); virtual void _v52(); virtual void _v53();
    virtual void _v54(); virtual void _v55(); virtual void _v56(); virtual void _v57(); virtual void _v58(); virtual void _v59();
    virtual void _v60(); virtual void _v61(); virtual void _v62(); virtual void _v63(); virtual void _v64(); virtual void _v65();
    virtual void _v66(); virtual void _v67(); virtual void _v68(); virtual void _v69(); virtual void _v70(); virtual void _v71();
    virtual void _v72(); virtual void _v73(); virtual void _v74(); virtual void _v75(); virtual void _v76(); virtual void _v77();
    virtual void _v78(); virtual void _v79(); virtual void _v80(); virtual void _v81(); virtual void _v82(); virtual void _v83();
    CThing* ParentThing;                             // +0x04
protected:
    virtual void OnItemAdded(long itemDefIndex, bool unused, bool suppressTutorials);   // slot 84: rtti-vtables CTCInventoryExperience slot 84; PDB MAE
public:
};

void CTCInventoryBase::OnItemAdded(long itemDefIndex, bool unused, bool suppressTutorials)
{
    CTCEntityEvents* events = ParentThing->QueryTC<CTCEntityEvents>(0x42);
    events->AddEvent(new NEntityEvents::CEventReceivedItemInInventory(itemDefIndex));
    if (!suppressTutorials) {
        CTCHero* hero;
        if (ParentThing->GetInterface(&hero, 0x29)) {
            if (!CTCInventoryItem::IsExpression(itemDefIndex))
                hero->GiveTutorialForCategory(TUTORIAL_CATEGORY_BASIC_OBJECTS, false);
            if (IsTutorialSubDefItem(itemDefIndex))
                hero->GiveTutorialForCategory(TUTORIAL_CATEGORY_TRADE_ITEM, false);
            ETutorialCategory category = CTCInventoryItem::GetTutorialCategory(itemDefIndex);   // NF315: the PDB return type (NF304 Q2)
            if (category)
                hero->GiveTutorialForCategory(category, false);
        }
    }
}
}