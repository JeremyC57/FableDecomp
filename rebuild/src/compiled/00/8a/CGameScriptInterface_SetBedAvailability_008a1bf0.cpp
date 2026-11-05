// CGameScriptInterface::SetBedAvailability(const CScriptThing& thing, bool value) -- if the script thing is valid and its thing has a CTCBed (interface 0x35), store value into its Empty flag
// Leaf: ?SetBedAvailability@CGameScriptInterface@@UAEXABVCScriptThing@@_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008a1bf0, 86 bytes, thiscall, `ret 8`.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 753; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order) names it ?SetBedAvailability@CGameScriptInterface@@UAEXABVCScriptThing@@_N@Z; CTCBed::GetInterfaceType() is 53 (0x35, landed); Ego_r.pdb CTCBed::Empty +0x0C.
// Callees: CScriptThing virtual slot 75 (+0x12C, IsValid) and slot 11 (+0x2C, GetThing), CVectorMap<ETCInterfaceType, CTCBase*,
//   CKeyPairCompareLess<...> >::LowerBound(const ETCInterfaceType&) 0x0040F020 (the inlined CThing::GetInterface: presence bit
//   test, lookup, end-or-larger-key -> end).
// Shape (family L083): the landed DisableFishingSpot 0x00895950; the GetInterface key is homed in the dead thing-argument slot.
// Header debt: engine/CScriptThing.h, CThing and the TC headers are flat layouts without vtables; modelled locally.
enum ETCInterfaceType { ETCInterfaceType_FIRST };
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x1C];
    unsigned long PresentInterfaces[9];                    // +0x20 (the bitset of present interface types)
    CInterfaceMap Interfaces;                              // +0x44
    template <class T> __forceinline bool GetInterface(T** out, ETCInterfaceType type)
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = type;
            CInterfaceMap* map = &Interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCBed {
public:
    unsigned char _pad00[0x0C];
    bool Empty;                                    // +0x0C
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::SetBedAvailability(const CScriptThing& thing, bool value)
{
    if (thing.IsAlive()) {
        CTCBed* component;
        if (thing.GetPThing()->GetInterface(&component, (ETCInterfaceType)0x35))
            component->Empty = value;
    }
}