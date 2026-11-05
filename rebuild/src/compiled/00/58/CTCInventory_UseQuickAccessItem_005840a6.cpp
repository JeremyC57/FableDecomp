// CTCInventory::UseQuickAccessItem(long) -- uses the item in a quick-access slot: the carried thing if it is that item, else the inventory item
// Leaf: UseQuickAccessItem
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed re-padded by-value maps +8.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005840A6, 177 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: std::bitset::test @ 0x00410DE0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020,
//          CTCCarrying::GetThingInPrimarySlot() @ 0x006A1440, IS_THING_ALIVE(CBaseObject* const&) @ 0x0040DCE0
//          (__fastcall), and this class's own virtuals OnItemSelectedInHand(CThing*, bool*) (slot 34, 0x0058400A) and
//          OnItemSelected(long, bool*) (slot 33, 0x00583E26) from vtable 0x01250DE4.
// Shape: the carrying component (interface 0x46) comes from the `__forceinline` GetTC helper with a zeroed out
// pointer (`and [ebp-4],0`, the key `push 46h ; pop ebx` reused for the compare), fetched before the slot test.
// The slot is read with the STLport `*(begin() + slot)` form (scaled index kept in edi, begin re-read). The
// thing in hand stays in ebx: IS_THING_ALIVE takes `CBaseObject* const&`, so the CThing* binds through a converted
// temporary homed in the dead `slot` argument slot (as in the landed CTCVillage::DetachFromDummyVillager 0x0065B2C4) and matched on
// its 16-bit def index (+0x8C). Each branch declares its own `handled` out-flag (both in that slot's high byte): one
// flag declared above the `if` overlaps the condition's temporary, which then takes the dead component word [ebp-4].
// Layout: retail CTCInventory quick-access vector<long> at +0x148 (PDB +0x168; engine/CTCInventory.h carries the
// 16-byte PDB containers, so `this` is a local model as in the landed IsQuickAccessItemUsable 0x005849EA).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing);   // 0x0040DCE0

class CTCCarrying { public:                                 // interface type 0x46
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};

struct CLongVector {                                 // std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() const { return _M_start; }
    const long& operator[](unsigned long n) const { return *(begin() + n); }
};

class CTCInventory { public:                                // vtable 0x01250DE4
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void OnItemSelected(long item, bool* handled);           // slot 33 (0x00583E26)
    virtual void OnItemSelectedInHand(CThing* thing, bool* handled); // slot 34 (0x0058400A)

    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x140];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168)

    void UseQuickAccessItem(long slot);
};

void CTCInventory::UseQuickAccessItem(long slot)
{
    CTCCarrying* carrying = 0;
    ParentThing->GetTC(TCI_CARRYING, &carrying);
    if (QuickAccessItems[slot] > 0) {
        CThing* inHand = carrying->GetThingInPrimarySlot();
        if (IS_THING_ALIVE(inHand) && inHand->GetDefGlobalIndex() == QuickAccessItems[slot]) {
            bool handled;
            OnItemSelectedInHand(inHand, &handled);
        }
        else {
            bool handled;
            OnItemSelected(QuickAccessItems[slot], &handled);
        }
    }
}