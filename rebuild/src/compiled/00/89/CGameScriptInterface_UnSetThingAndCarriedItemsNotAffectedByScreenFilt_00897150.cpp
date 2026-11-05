// CGameScriptInterface::UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing& thing) const -- script call: clears the screen-filter bypass flag of the thing's carrying component (interface 0x46)
// Leaf: ?UnSetThingAndCarriedItemsNotAffectedByScreenFilter@CGameScriptInterface@@UBEXAAVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00897150, 64 bytes, thiscall `ret 4`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 678 holds this body; slot 678 is
//   ?UnSetThingAndCarriedItemsNotAffectedByScreenFilter@CGameScriptInterface@@UBEXAAVCScriptThing@@@Z (a non-const
//   reference). Interface 0x46 = CTCCarrying (vtable 0x012433F4 slot 21 = landed 0x004E74C5, returns 0x46).
// Body: t = thing.GetPThing() (slot 11) with no null or dead test; if t holds interface 0x46 (UsingComponents word 2 bit 6):
//   GetTC (non-const LowerBound 0x0040F020) and clear the byte at +0x1c. The inline that clears it is not named in the
//   PDB sets (FableWin.pdb has CTCCarrying::UnSetAllCarriedThingsAsBypassFilter() but its body is not proven to be this
//   store): descriptive member and inline.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        CInterfaceMap* map = &Components;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || T::INTERFACE_TYPE < it->first)
            it = map->end();
        *out = (T*)it->second;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || T::INTERFACE_TYPE < it->first)
            it = map->end();
        *out = (const T*)it->second;
        return true;
    }
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCCarrying {                                     // interface 0x46
public:
    enum { INTERFACE_TYPE = 0x46 };
    unsigned char _pad00[0x1c];
    bool BypassScreenFilter;                             // +0x1c (descriptive)
    void ClearBypassScreenFilter() { BypassScreenFilter = false; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing& thing) const
{
    CThing* t = thing.GetPThing();
    CTCCarrying* tc;
    if (t->GetTC(&tc))
        tc->ClearBypassScreenFilter();
}