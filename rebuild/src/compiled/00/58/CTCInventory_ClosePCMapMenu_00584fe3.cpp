// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Leaf: ?ClosePCMapMenu@CTCInventory@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCInventory::ClosePCMapMenu() -- when the owner has a map-menu component (interface type 0x1B), closes it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00584FE3, length 74 bytes; size-optimised TU with a frame pointer
// (`optimize("y",off)` + `("s",on)`, `leave`).
// Callees: std::bitset::test 0x00410DE0 on the thing's interface-presence bits (+0x20), CVectorMap::LowerBound
//          0x0040F020 on its sorted {type, component} map (+0x44), then the component's close routine 0x005FAD30
//          (non-virtual: clears its words +0x214 and +0x188; manifest label "CWADFile::Close" is propagated;
//          this is its only caller).
// Ego_r PDB: ?ClosePCMapMenu@CTCInventory@@QAEXXZ.
// The lookup is the thing's inlined GetInterface template (the landed out-of-line copies, e.g. 0x007BEA08, have
// the same bitset-then-lower-bound shape with a by-address key in the frame slot).
// Retail layout: CTCBase ParentThing at +0x04 (the PDB header layout of CTCInventory is wider), so `this` is
// modelled locally; the map-menu class and its close routine are named by use.
// Shape: an absent interface skips to the epilogue; a present one is null-tested (`test eax,eax`) before the
// call. A size TU only inlines the lookup when it is `__forceinline`.
class CTCBase;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCMapMenu {
    void Close();                                    // 0x005FAD30
};

class CTCInventory { public:
    virtual void v00();
    CThing* ParentThing;                             // +0x04
    void ClosePCMapMenu();
};

void CTCInventory::ClosePCMapMenu()
{
    CTCMapMenu* mapMenu;
    if (ParentThing->GetTC(TCI_INVENTORY_MAP, &mapMenu) && mapMenu)
        mapMenu->Close();
}