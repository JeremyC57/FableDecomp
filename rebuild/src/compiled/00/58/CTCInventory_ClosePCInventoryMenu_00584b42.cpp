// CTCInventory::ClosePCInventoryMenu() -- asks the owner's in-game-menu component (interface type 0x12) to close its PC inventory menu
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup as `if (thing->HasTC(t)) thing->GetTC<T>(t)->Close()` on a named thing local with the ternary entry pick: the component loads straight into ecx for the virtual call
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00584B42, length 68 bytes; size-optimised TU with a frame pointer
// (`optimize("y",off)` + `("s",on)`, `leave`).
// Callees: std::bitset::test 0x00410DE0 on the thing's interface-presence bits (+0x20), CVectorMap::LowerBound
//          0x0040F020 on its sorted {type, component} map (+0x44), then virtual slot 31 (`call [eax+7Ch]`) on the
//          component found.
// Ego_r PDB: ?ClosePCInventoryMenu@CTCInventory@@QAEXXZ.
// The lookup is the thing's inlined GetInterface template (the landed out-of-line copies, e.g. 0x007BEA08, have
// the same bitset-then-lower-bound shape with a by-address key in the frame slot); a miss on the map yields
// the end entry, whose value is handed out regardless.
// Retail layout: CTCBase ParentThing at +0x04 (the PDB header layout of CTCInventory is wider), so `this` is
// modelled locally; the component and its slot are named by use.
// Shape: absent interface skips the call entirely (`je` to the epilogue); the found component is called with no
// null test. A size TU only inlines the lookup when it is `__forceinline`.
class CTCBase;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    bool HasTC(long wanted) { return m_present.test(wanted); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it == map->end() || wanted < it->first) ? map->end() : it)->second;
    }
};

class CTCInGameMenu { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ClosePCInventoryMenu();             // slot 31
};

class CTCInventory { public:
    virtual void v00();
    CThing* ParentThing;                             // +0x04
    void ClosePCInventoryMenu();
};

void CTCInventory::ClosePCInventoryMenu()
{
    CThing* thing = ParentThing;
    if (thing->HasTC(0x12))
        thing->GetTC<CTCInGameMenu>(0x12)->ClosePCInventoryMenu();
}