// CGameScriptInterface::IsEntityUnderScriptedControl(const CScriptThing& entity) const -- script call: whether a live entity's scripted-control component (interface 0x1f) is locked
// Leaf: ?IsEntityUnderScriptedControl@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x008960C0, 82 bytes, thiscall `ret 4`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 9 holds this body; slot 9 is
//   ?IsEntityUnderScriptedControl@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z (ghidra_out/cgsi_slots_aligned.tsv).
//   Interface 0x1f = CTCScriptedControl (its vtable 0x012658E4 slot 21 is 0x00719A40, which returns 0x1f). Getter:
//   FableWin.pdb ?IsLocked@CTCScriptedControl@@QBE_NXZ, inline (Locked +0x18, as the landed CTCScriptedControl rows).
// Body: t = entity.GetPThing() (slot 11); live t holding interface 0x1f (the sign bit of UsingComponents word 0, `jns`):
//   PeekTC (const LowerBound 0x004365B0) and return IsLocked().
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
class CTCScriptedControl {                              // interface 0x1f
public:
    enum { INTERFACE_TYPE = 0x1f };
    unsigned char _pad00[0x18];
    bool Locked;                                         // +0x18
    bool IsLocked() const { return Locked; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsEntityUnderScriptedControl(const CScriptThing& entity) const
{
    const CThing* t = entity.GetPThing();
    if (t && !t->IsDead())
    {
        const CTCScriptedControl* tc;
        if (t->PeekTC(&tc))
            return tc->IsLocked();
    }
    return false;
}