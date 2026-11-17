// CAIStateGroup_MinionHawkingFromStall::IsStillValid() -- still hawking while the shopkeeper has a shop, it is work
// Leaf: ?IsStillValid@CAIStateGroup_MinionHawkingFromStall@@UBE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// hours, and the live hawk target (+0x18) is either the creature itself or the shopkeeper's current thing.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094BDF0, length 173 bytes.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x90 = CTCShopKeeper, bit 16 of the bitset
// dword at +0x30; miss path reads the never-written out slot that shares the key word), CTCShopKeeper::GetShopTC
// @ 0x00798F50, CAIStateGroup_WorkBase::IsInWorkHours @ 0x0094BEF0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (four times), the shopkeeper's thing getter @ 0x00798D80 (manifest "_Cons_val<...>" is propagated and wrong).
// Shape: IS_THING_ALIVE inlined; the creature is a named local read before the third GetPItem (held in edi),
// then the shopkeeper's thing as a named local (held in edi) compared with a fresh GetPItem.
// No engine header for this class; the pointer member is +0x18.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
#include "fable_compiler.h"
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

// NF51 (round 7): the AI state-group base and its work-group layer come from the shared headers (were a local CAIStateGroupBase model
// and a derived-class IsInWorkHours stand-in for 0x0094BEF0): RTTI .?AVCAIStateGroup_MinionHawkingFromStall@@ derives
// CAIStateGroup_WorkBase<CAIStateGroupBase> (ghidra_out/rtti_bases.tsv), and the callee 0x0094BEF0 is that template's protected const
// ?StayInShop@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ (Ego_r.pdb 0x009702A0; engine/CAIStateGroup_WorkBase.h,
// declaration only), was `IsInWorkHours`; IsStillValid overrides the base's const slot 5 (Ego_r.pdb
// ?IsStillValid@CAIStateGroup_MinionHawkingFromStall@@UBE_NXZ).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_WorkBase.h"

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    FABLE_FORCEINLINE CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    FABLE_FORCEINLINE CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointerCtor& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CTCShop;
class CTCShopKeeper { public:
    CTCShop* GetShopTC();                       // 0x00798F50
    CThing* GetShopThing();                     // 0x00798D80
};

struct CAIStateGroup_MinionHawkingFromStall : CAIStateGroup_WorkBase<CAIStateGroupBase> {
    unsigned char _pad_0x14[0x4];               // +0x14
    CBaseIntelligentPointerCtor PHawkTarget;    // +0x18
    virtual bool IsStillValid() const;
};

bool CAIStateGroup_MinionHawkingFromStall::IsStillValid() const
{
    CTCShopKeeper* keeper;
    PBrain->PCreature->GetTC(TCI_SHOP_KEEPER, &keeper);
    if (keeper->GetShopTC() != 0 && StayInShop() && IS_THING_ALIVE(PHawkTarget)) {
        CThing* self = PBrain->PCreature;
        if (PHawkTarget.GetPItem() == self)
            return true;
        CThing* shopThing = keeper->GetShopThing();
        if (shopThing == PHawkTarget.GetPItem())
            return true;
    }
    return false;
}